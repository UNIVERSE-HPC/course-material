---
name: Using Mocks in Tests
id: mocking
dependsOn: [
  technology_and_tooling.testing.testable_code_fixtures
]
tags: [pytest]
---

## Why use mocking?

Sometimes we may not want to use "real" objects or functions in our tests, such as those that write to a production database, those that read data from external services or simply those parts that take a long time to run. The technique of mocking allows these objects and functions to be replaced with ones that simulate the same behaviour for the purpose of testing. Doing this allows us to create different scenarios whilst isolating the test code and ensuring that the tests are run in an environment that is independent of external factors. Mocking also allows us to check if a specific function is called, how many times it was called and if the arguments passed to the call were correct.

### Using the `unittest.mock` library

Let us continue with previous example from the  [testable code and fixtures section](testable_code_fixtures), where we were testing functions to connect to and query a SQLite database. We will be testing the functionality of code in the `sqlite_example.py` file that we created previously. This time, instead of actually connecting to the database, we can mock an object to replace it with one that we can control and monitor. We will need to import a library in order to create our mocks. Rather than `pytest`, another library `unittest`, which is the testing library that comes as standard with Python, will be used. We can use the `unittest.mock.Mock` class to create a mock object. As a simple example, we can replace our `query_database` function with this `Mock` object. Then we are able to replace the value returned from `query_database` with whatever we want. Here is the contents of a new file `test_mocks.py`. 

~~~python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database
from unittest.mock import Mock

@pytest.fixture(scope="session")
def database_fn_fixture(tmp_path_factory):
    """
    Uses tmp_path_factory to create a filename in a temp directory
    """
    yield tmp_path_factory.mktemp("data") / "test.db"


def test_query_database_mock(database_fn_fixture):
    """Mock the query_database function to show the principle
    """
    sql = "SELECT * FROM Animals"
    conn = connect_to_database(database_fn_fixture)
    query_database = Mock()
    query_database.return_value = ("Jerry", "Mouse", 1)
    assert query_database(sql, connection=conn) == ("Jerry", "Mouse", 1)
    query_database.assert_called_once_with(sql, connection=conn)

~~~

In the example above, we do not require a database connection, a database file, or to perform any query on a database at all, since we have replaced the entire `query_database` function. The test is not especially useful, however, since we are now simply testing that the `Mock` object returns the value that we asked it to return. Note that we also test that the function was called with the correct arguments (although in this case we could call `query_database` with any arguments we liked since it is actually an `Mock` object).

:::callout{variant="note"}
## The difference between `Mock` and `MagicMock`

In the examples in this lesson, we will use the `Mock` object from the `unittest` library. When looking elsewhere for information you may find examples that use the `MagicMock` object. The difference between the two is that `MagicMock` objects have default implementations of Python "magic" methods. These are also sometimes referred to as *dunder methods* (double underscore methods), officially however, they are known as [*special methods*](https://docs.python.org/3/reference/datamodel.html#specialnames). Since we will not be relying on any of these methods for our examples, we will stick with the more simple object that does not risk bringing any unexpected behaviour to our mocks.
:::

For a more useful (and interesting) example, we could mock the `sqlite3` connection itself. Once we have done this, we will also need to add the cursor that is associated with the connection to the mocked connection and add a return value for the `cursor.fetchall()` method that we call. The example below shows how we might do this:

~~~python
import pytest
import sqlite3
from sqlite_example import query_database
from unittest.mock import Mock

def test_query_db_mocked_connection():
    """Mock the database connection and cursor to ensure the correct methods
    are called within the query_database function
    """
    sqlite3.connect = Mock()
    # create a fake cursor and define the data fetchall() returns
    mock_cursor = sqlite3.connect.return_value.cursor.return_value
    mock_cursor.fetchall.return_value = [("Jerry", "Mouse", 1)]
    # make our fake connection
    conn = sqlite3.connect("my_non_existent_file")
    sql = "SELECT * FROM Animals"
    # test what query_database does with our fake connection
    result = query_database(sql, connection=conn)
    assert result[0] == ("Jerry", "Mouse", 1)
    # check that query_database passes our SQL string to cursor.execute()
    mock_cursor.execute.assert_called_once_with(sql)
    # check that fetchall() was called
    mock_cursor.fetchall.assert_called_once()
    # check that query_database closes the connection
    conn.close.assert_called_once()

~~~

#### Patching functions

If we add the test above to our `test_mocks.py` file and run `python -m pytest tests/test_mocks.py ` we find that the tests pass. If we run this file along with the `test_sqlite.py` file that we created in the [previous lesson](testable_code_fixtures), however, we may find that we start to get test failures with errors similar to this:

~~~bash
 def test_connect_to_db_type(database_fn_fixture):
        """
        Test that connect_to_db function returns sqlite3.Connection
        """
        conn = connect_to_database(database_fn_fixture)
>       assert isinstance(conn, sqlite3.Connection)
E       AssertionError: assert False
E        +  where False = isinstance(<Mock name='mock()' id='4430639376'>, <class 'sqlite3.Connection'>)
E        +    where <class 'sqlite3.Connection'> = sqlite3.Connection

tests/test_sqlite.py:34: AssertionError

 def test_connect_to_db_name(database_fn_fixture):
        """
        Test that connect_to_db function connects to correct DB file
        """
        conn = connect_to_database(database_fn_fixture)
        cur = conn.cursor()
        # List current databases https://www.sqlite.org/pragma.html#pragma_database_list
        cur.execute("PRAGMA database_list;")
        # Unpack the three parameters returned
>       db_index, db_type, db_filepath = cur.fetchone()
E       TypeError: cannot unpack non-iterable Mock object

tests/test_sqlite.py:68: TypeError
~~~

We can see that in the first case, the test is failing because `assert isinstance(conn, sqlite3.Connection)` is actually receiving a `Mock` object instead of an `sqlite3.Connection` object. In the second case, a `Mock` object is received instead of the `tuple` we would expect from the `cursor.fetchone()` function, so we get an error when trying to unpack it.

It appears that our mocked `sqlite.connection` has created issues in other test functions where we did not intend to use it. To overcome this behaviour, we will need to use a *patch* which will only affect the scope of the function. There are two ways of using a patch, a *context manager* or a *function decorator*. 

::::challenge{id=patch-context-manager title="Using the `with patch` context manager."}

Rewrite `test_query_db_mocked_connection` to use a context manager. You can view the [unittest documentation](https://docs.python.org/3/library/unittest.mock.html#unittest.mock.patch) as a guide.

:::solution
Below is an example of using a context manager to patch the test. When we use a patch we are actually receiving a `Mock` object that behaves in exactly the same way as in the previous examples. First we `import patch` from the `unitest.mock` library, then we create a patch called `mock_connection` which only exists within the context of the `with` statement. After this statement, the context will be cleaned up automatically. 

~~~python
import pytest
import sqlite3
from sqlite_example import query_database
from unittest.mock import patch

def test_query_db_mocked_connection():
    """Mock the database connection and cursor to ensure the correct methods
    are called within the query_database function
    """
    with patch('sqlite3.connect') as mock_connection:
        mock_cursor = mock_connection.return_value.cursor.return_value
        mock_cursor.fetchall.return_value = [("Jerry", "Mouse", 1)]
        # make our fake connection
        conn = mock_connection("my_non_existent_file")
        sql = "SELECT * FROM Animals"
        # test what query_database does with our fake connection
        result = query_database(sql, connection=conn)
        assert result[0] == ("Jerry", "Mouse", 1)
        # check that query_database passes our SQL string to cursor.execute()
        mock_cursor.execute.assert_called_once_with(sql)
        # check that fetchall() was called
        mock_cursor.fetchall.assert_called_once()
        # check that query_database closes the connection
        conn.close.assert_called_once()
~~~

:::
::::

Now lets look at using a function decorator.

::::challenge{id=patch-decorator title="Using the `@patch` decorator"}

Rewrite `test_query_db_mocked_connection` to use a function decorator instead of a context manager. You can view the [unittest documentation](https://docs.python.org/3/library/unittest.mock.html#unittest.mock.patch) as a guide.

:::solution
~~~python
@patch('sqlite3.connect')
def test_query_db_mocked_connection(mock_connection):
    mock_cursor = mock_connection.return_value.cursor.return_value
    mock_cursor.fetchall.return_value = [("Jerry", "Mouse", 1)]
    # make our fake connection
    conn = mock_connection("my_non_existent_file")
    sql = "SELECT * FROM Animals"
    # test what query_database does with our fake connection
    result = query_database(sql, connection=conn)
    assert result[0] == ("Jerry", "Mouse", 1)
    # check that query_database passes our SQL string to cursor.execute()
    mock_cursor.execute.assert_called_once_with(sql)
    # check that fetchall() was called
    mock_cursor.fetchall.assert_called_once()
    # check that query_database closes the connection
    conn.close.assert_called_once()
~~~

:::
::::

### Mocking using other libraries

#### Monkeypatching in pytest

As an alternative to using the `unitest.mock` library, its possible to use a version of mocking from within `pytest`, termed *monkeypatching*. A built-in fixture called `monkeypatch` allows modifying attributes, functions or classes within the scope of the test function. Some example methods are:

- `monkeypatch.setattr()` - used to set an attribute to a new value or replace it with a new function
- `monkeypatch.delattr()` - used to delete an attribute
- `monkeypatch.setitem()` - used to modify a dictionary item
- `monkeypatch.delitem()` - used to delete an item from a dictionary

It makes sense to use both the pytest `monkeypatch` fixture and `unittest.mock` together, otherwise we will have to write our own mock objects from scratch; the `monkeypatch` fixture does not provide mock objects, nor does it allow us to check how our functions interacted with mock objects.

::::challenge{id=mock-monkeypatch title="Using `monkeypatch` and `Mock`"}

Rewrite `test_query_db_mocked_connection` to use the pytest `monkeypatch` fixture alongside `unittest.mock`. You can view the [pytest monkeypatch documentation](https://docs.pytest.org/en/7.1.x/how-to/monkeypatch.html) if needed.

:::solution
~~~python
import pytest
import sqlite3
from sqlite_example import query_database
from unittest.mock import patch, Mock

def test_query_db_mocked_connection(monkeypatch):
    """Mock the database connection and cursor to ensure the correct methods
    are called within the query_database function
    """
    # Create a mock sqlite3.Connection object
    mock_conn = Mock()

    # Create a mock sqlite3.Cursor object
    mock_cursor = Mock()
    mock_cursor.fetchall.return_value = [("Jerry", "Mouse", 1)]
    mock_conn.cursor.return_value = mock_cursor

    # Monkeypatch the sqlite3.connect function with a mock
    def mock_connect(*args, **kwargs):
        return mock_conn

    monkeypatch.setattr("sqlite3.connect", mock_connect)
    conn = sqlite3.connect("my_non_existent_file")
    # Call the function and assert the expected behavior
    sql = "SELECT * FROM Animals"
    result = query_database(sql, connection=conn)
    assert result[0] == ("Jerry", "Mouse", 1)
    # check that query_database passes our SQL string to cursor.execute()
    mock_cursor.execute.assert_called_once_with(sql)
    # check that fetchall() was called
    mock_cursor.fetchall.assert_called_once()
    # check that query_database closes the connection
    conn.close.assert_called_once()
    
~~~

:::
::::

:::callout
#### Using the `mocker` fixture from `pytest-mock`

An alternative to using the `unitest.mock` library is to install `pytest-mock` alongside `pytest`. This wil give you access to a fixture called `mocker`. This fixture provides access to `unittest.patch` functionalities as well as mocks. There is no need to `import unittest` and no `monkeypatch` functions are required. For more information see the [pytest-mock documentation](https://pytest-mock.readthedocs.io/en/latest/index.html).

:::

Well done for making it this far, mocking is often a confusing subject due to the many ways in which it can be done and the abstract nature of temporarily replacing parts of the thing you are testing.  After this introduction, you can now solidify your learning by practicing the techniques here on your own code whilst using the documentation as a reference.

## Putting it all together - adding a database as a data source

Finally, we can come back to our `Trial` object and integrate the functions to connect to and query an SQLite database. We have provided a file `inflammation_data.db` that contains all of the data from the 12 csv files in one table called `data`. 

To get this file, if on WSL or Linux (e.g. Ubuntu or the Ubuntu VM), then do:

~~~bash
$ wget https://train.oxrse.uk/material/HPCu/software_architecture_and_design/procedural/inflammation/inflammation_data.db
~~~

Or, if on a Mac, do:

~~~bash
$ curl -O https://train.oxrse.uk/material/HPCu/software_architecture_and_design/procedural/inflammation/inflammation_data.db
~~~

Save the file into the `inflammation/data` directory of your project. 

The `data` table has 43 columns, `patient_id`, `trial_id`, `filename` and `day01` to `day40` that record the number of inflammation flare-ups for these days. The `patient_id` field is in the form of `pxx` where patient 1 is `p01`, for `trial_id` the format is `txx` where trial 1 is `t01`. Now we can add a new method `from_database` to our class:

~~~python
import numpy as np
from sqlite_example import connect_to_database, query_database

class Trial:
    def __init__(self, data, id):
        self.data = data
        self.id = id

    @classmethod
    def from_csv(cls, filename, id):
        """
        Class method to create a Trial instance from data in a CSV file.

        Parameters:
        filename (str): The file path of the CSV file to read.
        id (str): The id to assign to the Trial instance.

        Returns:
        Trial: A Trial instance with the data and id from the CSV file.
        """
        data = cls.load_csv(filename)
        return cls(data, id)
    
    @classmethod
    def from_database(cls, db_filepath, trial_id):
        """
        Class method to create a Trial instance from data in a SQLite database.

        Parameters:
        db_filepath (str): The file path of the SQLite database to connect to.
        trial_id (str): The trial_id to query the database for.

        Returns:
        Trial: A Trial instance with the data and id from the database.
        """
        query = f'SELECT * FROM data WHERE trial_id = "{trial_id}"'
        connection = connect_to_database(db_filepath)
        data = query_database(query, connection)
        if not data:
            raise ValueError("No data found for trial_id")
        # Convert the list of tuples to a numpy array and skip the first two columns
        if np.shape(data)[0] == 1:  # If only one row is returned, convert to 2D array
            data = np.array([data[0][3:]]).astype(float)
        else:
            data = np.array(data)[:, 3:].astype(float)
        return cls(data, trial_id)

    @staticmethod
    def load_csv(filename):
        """Load a Numpy array from a CSV

        :param filename: Filename of CSV to load
        """
        return np.loadtxt(fname=filename, delimiter=',')

    ...
~~~

Using our new method, an instance of the `Trial` class can now be created in the following way:

~~~python
from inflammation.models import Trial

trial_group01 = Trial.from_database("inflammation_data.db", "t01")
~~~

Our existing tests for the statistical methods from the `Trial` object do not need to be altered even if the underlying data storage has changed, as long as the data is loaded into a numpy array of the same format as we had previously.

::::challenge{id=test_db_load title="Testing loading from a database."}

Write some more tests for the `Trial` class. These should check that the data loaded via the `from_database` class method is stored in the `data` attribute in the same format as when using either the `from_csv` class method or when using the normal constructor (`Trial(data, id)`). Feel free to use mocking and/or fixtures as appropriate.

:::solution
Here we give some example tests using the `mocker` fixture from `pytest-mock` as well as a real test database. Your tests do not need to be identical to these ones. At this stage, you should know that testing the functionality can be done in in a number of ways!

~~~python
import numpy as np
import pytest
from inflammation.models import Trial
from sql_example import connect_to_database, query_database

@pytest.fixture(scope="session")
def database_fn_fixture(tmp_path_factory):
    """
    Uses tmp_path_factory to create a filename in a temp directory
    """
    yield tmp_path_factory.mktemp("data") / "test.db"

@pytest.fixture(scope="session")
def database_connection(database_fn_fixture):
    """
    Create database connection
    """
    conn = sqlite3.connect(database_fn_fixture)
    yield conn
    conn.close()
    Path.unlink(database_fn_fixture)

@pytest.fixture(scope="session")
def setup_database(database_connection):
    """
    Populate data in database
    """
    conn = database_connection
    cur = conn.cursor()
    cur.execute('''
        CREATE TABLE data (
            patient_id TEXT,
            trial_id TEXT,
            filename TEXT,
            value1 INTEGER,
            value2 INTEGER
        )
    ''')
    cur.execute('''
        INSERT INTO data VALUES
        ('p01', 't02', 'filename3', 4, 5),
        ('p05', 't02', 'filename3', 2, 1),
        ('p05', 't03', 'filename4', 3, 2)
    ''')
    conn.commit()
    yield conn 
    cur.execute("DROP TABLE data")

def test_trial_from_database(database_fn_fixture, setup_database):
    # Create a Trial object
    trial = Trial.from_database(database_fn_fixture, "t02")
    assert isinstance(trial.data, np.ndarray)
    # Check that the data attribute is correct (the first three columns should be skipped)
    npt.assert_array_equal(trial.data, np.array([[4, 5], [2, 1]]))
    assert trial.id == "t02"

def test_trial_from_database_no_data(database_fn_fixture, setup_database):
    # Try to create a Trial object with a trial_id that doesn't exist in the database
    with pytest.raises(ValueError, match='No data found for trial_id'):
        Trial.from_database(database_fn_fixture, 't01')

def test_trial_from_database_one_row(database_fn_fixture, setup_database):
    # Try to create a Trial object with a trial_id that only has one row in the database
    trial = Trial.from_database(database_fn_fixture, 't03')
    assert isinstance(trial.data, np.ndarray)
    # Check that the data attribute is correct (the first three columns should be skipped)
    np.testing.assert_array_equal(trial.data, np.array([[3., 2.]]))
    assert trial.id == 't03'

def test_trial_from_mock_database(mocker):
    # Create a mock sqlite3.Connection object
    mock_conn = mocker.Mock(spec=sqlite3.Connection)
    # Create a mock sqlite3.Cursor object
    mock_cursor = mocker.Mock(spec=sqlite3.Cursor)
    mock_cursor.fetchall.return_value = [('p01', 't02', 'filename3', 4, 5),
                                         ('p05', 't02', 'filename3', 2, 1)]
    mock_conn.cursor.return_value = mock_cursor
    # Replace the sqlite3.connect function with a mock
    mocker.patch('sqlite3.connect', return_value=mock_conn)
    # Create a Trial object
    trial = Trial.from_database('test_db.db', 1)
    assert isinstance(trial.data, np.ndarray)
    # Check that the data attribute is correct (the first three columns should be skipped)
    npt.assert_array_equal(trial.data, np.array([[4, 5], [2, 1]]))
    assert trial.id == 1

~~~

:::
::::

When combined with the previous course, we have now covered a number of more advanced topics: Designing testable code, using fixtures and mocking. These should help you to ensure you write reliable and maintainable software. Happy testing!

:::callout{variant="keypoints"}
- Mocking allows objects and functions to be **replaced** during testing with fake versions that **simulate attributes and behaviours**
- Examples of mocked classes and methods are those that **write to a production database**, those that **read data from external services** or simply **those parts that take a long time to run**
- Mocking allows checking if a **specific function is called, how many times it was called** and **if the arguments passed to the call were correct**
- Mocking is available through the `unitest.mock` library, the `monkeypatch` fixture in `pytest` and the `mocker` fixture in `pytest-mock`
- Using a **context manager** or a **function decorator** to **patch** a method ensures that a `unittest.Mock` object will only affect the scope of the test function
- Mocking can be used alongside **fixtures** and **writing testable code** to **isolate components** for effective testing  
:::
