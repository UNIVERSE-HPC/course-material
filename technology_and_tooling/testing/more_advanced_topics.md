---
name: More Advanced Topics
id: more_advanced_topics
dependsOn: [
  technology_and_tooling.testing.diagnosing_issues
]
tags: [pytest, fixtures, mocking]
---

## Introduction

Having completed the previous sections of the course, we now know:
- how to write tests
- how to debug problems with our code 
- how to code in a defensive manner in order to prevent invalid inputs from causing problems.

Up to this point, our examples have focused on unit testing of simple functions that formed part of larger program to analyse inflammation data from patients in a drug trial. If we were to develop this program further, a larger number of tests would need to written and organised in order to cater for the increased complexity of the codebase.  In the future, we may also want to integrate our program with other entities that are external to the application itself, such as a database, a web-based service or even analysis components that are written in another language such as C++.

In this section of the course, we will cover a number of topics that will help us to deal with testing software that is more complex whilst ensuring that it functions as we would expect:

- Designing testable code
- Fixtures
- Mocking

We will be moving away from our inflammation analysis example for now and will focus on testing some functions that interact with a database. In the future, having our inflammation data stored in a database may well be useful, however, especially if we add data from further studies or from other sources and would like to link together and structure our data in a more formalised way.

## Designing testable code

It is worth taking time to think about how your code will be structured, not only for readability that will allow others (and perhaps yourself, when viewing it several months later!) to understand what the code does and how it does it, but also to make testing more straightforward. Increasing the ease of writing tests can result in increased test coverage, and thereby reduce the chance that future changes made to the codebase will introduce regressions. In fact, writing testable code often also results in a cleaner and more modular structure that adheres to best practices.

### Test-driven development

In one software development methodology, Test-Driven Development (TDD), tests are actually written before the code which ensures that the design for testability is in mind from the onset. TDD typically involves a process of adding one test at time. This newest test will initially fail since the functionality has not yet been implemented. The code is then written that allows this test to pass and the process is repeated, ensuring that requirements are thought about before diving in and starting to implement algorithms.

### Programming paradigms

Another technique, which can lead to more testable software, is to use pure functions that have no side effects, this is because the outputs depend on the inputs alone. In this case, it can be ensured that the results are deterministic. For more information, see the [functional programming paradigm](https://train.oxrse.uk/material/HPCu/software_architecture_and_design/functional), pages in our training material.

### Dependency injection

A way to reduce the degree of coupling between a function being tested by a unit test and any dependencies, is to use *dependency injection*. This involves passing an object or function to our routines rather than creating such objects internally. In the following example, we have a function `query_database` that utilises a connection to a [SQLite](https://www.sqlite.org/) database. It is going to be difficult to test this function without connecting to the `example.db` database. The contents of our file, named `sqlite_example.py` are shown here:

~~~python
# Original code: Function that performs a database query
import sqlite3

def query_database(sql):
    # connection - a live communication channel between the app and the database
    conn = sqlite3.connect('example.db')
    # cursor - used to traverse and manipulate results returned by a query
    cursor = conn.cursor()
    # we pass a string named 'sql' that contains our SQL query
    cursor.execute(sql)
    # fetchall - returns a list of tuples containing all rows of our result
    result = cursor.fetchall()
    conn.close()
    return result

~~~

If we refactor the function to inject the database connection dependency, we can easily replace that connection during testing with one that is connected to a test database. Alternatively we could replace it with a fake (*mocked*) object that represents the connection, meaning that we do not have to connect to an actual database at all in order to test the function. Information on mocking will be given later in this lesson.

~~~python
# Rewritten code: Performs a database query with dependency injection
import sqlite3

def connect_to_database(filename):
  return sqlite3.connect(filename)

def query_database(sql, connection=None):
    if connection is None:
        raise TypeError("No database connection given.")
    cursor = connection.cursor()
    cursor.execute(sql)
    result = cursor.fetchall()
    connection.close()
    return result
~~~

Here is an example of some tests for these functions, these can be created in a new file named `test_sqlite.py` within a `/tests` directory. If you would like to learn more about the Structured Query Language (SQL) expressions in this example that are used to interact with the database see the [SQL Zoo](https://sqlzoo.net/wiki/SQL_Tutorial) site:

~~~python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database

def test_connect_to_db_type():
    """
    Test that connect_to_db function returns sqlite3.Connection
    """
    conn = connect_to_database('test.db')
    assert isinstance(conn, sqlite3.Connection)
    conn.close()

def test_connect_to_db_name():
    """
    Test that connect_to_db function connects to correct DB file
    """
    conn = connect_to_database('test.db')
    cur = conn.cursor()
    # List current databases https://www.sqlite.org/pragma.html#pragma_database_list
    cur.execute('PRAGMA database_list;')
    # Unpack the three parameters returned 
    db_index, db_type, db_filepath = cur.fetchone()
    # Extract just the filename from the full filepath
    db_filename = Path(db_filepath).name 
    assert db_filename == 'test.db'
    conn.close()

def test_query_database():
    """
    Test that query_database retrieves the correct data
    """
    # if the database already exists, delete it
    if Path("test.db").exists():
        Path.unlink("test.db")
    # Create a new test database and enter some data
    conn = sqlite3.connect("test.db")
    cur = conn.cursor()
    cur.execute("CREATE TABLE Animals(Name, Species, Age)")
    cur.execute("INSERT INTO Animals VALUES ('Bugs', 'Rabbit', 6)")
    # Use query_database to retrieve data
    sql = "SELECT * FROM Animals"
    result = query_database(sql, connection=conn)
    # Result returned is a list (cursor.fetchall)
    assert isinstance(result, list)
    # There should just be one record
    assert len(result) == 1
    # That record should be the data we added
    assert result[0] == ("Bugs", "Rabbit", 6)

def test_query_database_without_connection():
    """
    Test the `query_database` function without a provided connection
    """
    sql = 'SELECT * FROM Animals'
    # ensure that we get a TypeError
    with pytest.raises(TypeError):
        query_database(sql)

~~~

As you can see, we can test the `connect_to_database` and `query_database` functions separately. The tests are becoming complex, however, especially the one for `query_database`. Next we can look at how fixtures can help us to reduce this complexity, especially when we want to reuse resources such as a test database.

## Fixtures

When writing your tests, you will often find that different tests benefit from the same or similar setup of objects, variables or even connections to allow creation of certain scenarios. After testing, there may also be *teardown* functions or procedures that need to be run in order to clean up files that have be generated or to close database connections that have been opened. This is where fixtures come to the rescue. 

Fixtures are created by using the `@pytest.fixture` decorator on a function which allows this function to be passed as an argument to your tests and used within them. If there is a cleanup part to the code, then the fixture function should be written using the `yield` statement rather than a `return` statement. Anything up to the `yield` statement is setup code, and anything after the statement will be run post-testing in order to clean up.

In the example below, we can use a fixture named `setup_database` to create our test database, add data and also remove the database file once the tests have finished running. As a result, our `test_query_database` function can be simplified and if we want to use the test database in an other tests, we simply need to add `setup_database` as an argument to those tests.

~~~python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database

@pytest.fixture
def setup_database():
    """
    Setup database connection and populate data
    """
    conn = sqlite3.connect("test.db")
    cur = conn.cursor()
    cur.execute("CREATE TABLE Animals(Name, Species, Age)")
    cur.execute("INSERT INTO Animals VALUES ('Bugs', 'Rabbit', 6)")
    yield conn  # Provide the fixture value
    # Teardown database connection
    conn.close()
    Path.unlink("test.db")

def test_query_database(setup_database):
    """
    Test that query_database retrieves the correct data
    """
    conn = setup_database
    sql = "SELECT * FROM Animals"
    result = query_database(sql, connection=conn)
    # Result returned is a list (cursor.fetchall)
    assert isinstance(result, list)
    # There should just be one record
    assert len(result) == 1
    # That record should be the data we added
    assert result[0] == ("Bugs", "Rabbit", 6)

~~~

:::callout
## Discussion Point: Should We Use Multiple `assert` statements in One Test Function?

According to the book, The Art of Unit Testing by Roy Osherove, a unit test, by definition, should test a *unit of work*. What this means exactly is itself a point for discussion, but generally it refers to actions that take place between an entry point (e.g. a declaration fo a function) and an exit point (e.g. the output of a function). It is also often said that each test should fail for only one reason alone.

Does using multiple `assert` statements in one test contravene these guidelines?

Given that, unlike some other testing frameworks, `pytest` will output an error showing which of the `assert` statements in the test failed and why, does this change the situation?

Are there any disadvantages to enforcing a rule of one `assert` per test?
:::

::::challenge{id=nested-fixtures title="Fixtures that use fixtures"}

The `setup_database` fixture does several things including initiating the connection as well as creating and populating the database table. In order to separate out these functionalities, split this fixture into two, with one fixture `database_connection` for providing the database connection and another`setup_database` that uses the first fixture and then populates the database. You can view the [pytest fixtures documentation](https://docs.pytest.org/en/7.1.x/how-to/fixtures.html) as a guide.

:::solution
~~~python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database

@pytest.fixture
def database_connection():
    """
    Create database connection
    """
    db_filename = "test.db"
    conn = sqlite3.connect(db_filename)
    yield conn
    conn.close()
    Path.unlink(db_filename)

@pytest.fixture
def setup_database(database_connection):
    """
    Populate data in database
    """
    conn = database_connection
    cur = conn.cursor()
    cur.execute("CREATE TABLE Animals(Name, Species, Age)")
    cur.execute("INSERT INTO Animals VALUES ('Bugs', 'Rabbit', 6)")
    yield conn 

def test_query_database(setup_database):
    """
    Test that query_database retrieves the correct data
    """
    conn = setup_database
    sql = "SELECT * FROM Animals"
    result = query_database(sql, connection=conn)
    # Result returned is a list (cursor.fetchall)
    assert isinstance(result, list)
    # There should just be one record
    assert len(result) == 1
    # That record should be the data we added
    assert result[0] == ("Bugs", "Rabbit", 6)
~~~

:::
::::

By default, fixtures will be created when first requested by a test and will be destroyed at the end of the test. We can change this behaviour by defining the *scope* of the fixture. If we want to use the decorator `@pytest.fixture(scope="session")` for example, the fixture will only be destroyed at the end of the entire test session. Modifying this behaviour is especially useful if the fixture is expensive to create (such as a large file) and we do not need to recreate it for each test. 

As well as writing our own fixtures, we can use those that are [predefined/(built-in)](https://docs.pytest.org/en/latest/reference/fixtures.html). For example we may want to use a temporary directory for our files during testing, rather than creating files in the directory that we are working from (this is what currently happens when we run our database tests). The built-in fixture `temp_path_factory` allows us to to do this. We can refactor our code to add an extra fixture that uses feature and then it can be used by all the tests that we have written as well as by the `setup_database` fixture. The contents of our `test_sqlite.py` is now:

~~~python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database


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
    cur.execute("CREATE TABLE Animals(Name, Species, Age)")
    cur.execute("INSERT INTO Animals VALUES ('Bugs', 'Rabbit', 6)")
    yield conn 

def test_connect_to_db_type(database_fn_fixture):
    """
    Test that connect_to_db function returns sqlite3.Connection
    """
    conn = connect_to_database(database_fn_fixture)
    assert isinstance(conn, sqlite3.Connection)
    conn.close()

def test_connect_to_db_name(database_fn_fixture):
    """
    Test that connect_to_db function connects to correct DB file
    """
    conn = connect_to_database(database_fn_fixture)
    cur = conn.cursor()
    # List current databases https://www.sqlite.org/pragma.html#pragma_database_list
    cur.execute("PRAGMA database_list;")
    # Unpack the three parameters returned
    db_index, db_type, db_filepath = cur.fetchone()
    # Test that the database filename is the same as the one from the fixture
    assert Path(db_filepath).name == Path(database_fn_fixture).name
    conn.close()

def test_query_database(setup_database):
    """
    Test that query_database retrieves the correct data
    """
    conn = setup_database
    sql = "SELECT * FROM Animals"
    result = query_database(sql, connection=conn)
    # Result returned is a list (cursor.fetchall)
    assert isinstance(result, list)
    # There should just be one record
    assert len(result) == 1
    # That record should be the data we added
    assert result[0] == ("Bugs", "Rabbit", 6)

def test_query_database_without_connection():
    """
    Test the `query_database` function without a provided connection
    """
    sql = 'SELECT * FROM Animals'
    # ensure that we get a TypeError
    with pytest.raises(TypeError):
        query_database(sql)

~~~

Congratulations, you now know about fixtures. For more details on what you can do please refer to the [pytest fixtures documentation](https://docs.pytest.org/en/7.1.x/how-to/fixtures.html).

## Mocking

Sometimes we may not want to use "real" objects or functions in our tests, such as those that write to a production database, those that read data from external services or simply those parts that take a long time to run. The technique of mocking allows these objects and functions to be replaced with ones that simulate the same behaviour for the purpose of testing. Doing this allows us to create different scenarios whilst isolating the test code and ensuring that the tests are run in an environment that is independent of external factors. Mocking also allows us to check if a specific function is called, how many times it was called and if the arguments passed to the call were correct.

### Using the `unittest.mock` library

Let us continue with our example where we are testing functions to connect to and query a SQLite database. Instead of actually connecting to the database, we can mock an object to replace it with one that we can control and monitor. We will need to import a library in order to create our mocks. Rather than `pytest`, another library `unittest`, which is the testing library that comes as standard with Python, will be used. We can use the `unittest.mock.Mock` class to create a mock object. As a simple example, we can replace our `query_database` function with this `Mock` object. Then we are able to replace the value returned from `query_database` with whatever we want. Here is the contents of a new file `test_mocks.py`. 

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

:::callout
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

If we add the test above to our `test_mocks.py` file and run `python -m pytest tests/test_mocks.py ` we find that the tests pass. If we run this file along with the `test_sqlite.py` file that we created earlier, however, we may find that we start to get test failures with errors similar to this:

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

::::challenge{id=patch-decorator title="Using the `patch` decorator"}

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

Here is an example of how we could replace `unitest.mock` with `monkeypatch` in our `test_query_db_mocked_connection()` test:

~~~python
import pytest
import sqlite3
from sqlite_example import query_database

class MockSQLiteConnection:
    
    def __init__(self, filename):
        # Define any attributes or methods needed for your mock cursor
        self.filename = filename

    def cursor(self):
        # Implement a method to return a mock cursor
        return MockSQLiteCursor()
    
    def close(self):
        # Implement a method to close connection
        pass

class MockSQLiteCursor:

    def execute(self, query):
        # Implement a method to simulate executing a query
        pass

    def fetchall(self):
        # Implement a method to simulate fetching a result
        return [("Jerry", "Mouse", 1)]


def test_query_db_mocked_connection(monkeypatch):
    # a function that returns our mock connection
    def mock_connection(*args, **kwargs):
        return MockSQLiteConnection(*args)
    # replace the sqlite3 connect() method with our function
    monkeypatch.setattr("sqlite3.connect", mock_connection)
    # make our fake connection
    conn = sqlite3.connect("my_non_existent_file")
    sql = "SELECT * FROM Animals"
    # test what query_database does with our fake connection
    result = query_database(sql, connection=conn)
    assert result[0] == ("Jerry", "Mouse", 1)

~~~

As can be seen, if we do not use the `unittest.mock` library at all, we have to write our own  `MockSQLiteConnection` and `MockSQLiteCursor` objects. Also we can no longer use the functions that allow us to check how our functions interacted with the mock objects such as `assert_called_once_with()`. In reality, it would make more sense to use both the pytest `monkeypatch` fixture and `unittest.mock`.

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

#### Using the `mocker` fixture from `pytest-mock`

Another alternative to using the `unitest.mock` library is to install `pytest-mock` alongside `pytest`. This wil give you access to a fixture called `mocker` which provides access to the `unittest.patch` functionalities through this fixture, which means that mocking will automatically be reversed after the test is run. There is no need to `import unittest` and no `monkeypatch` functions are required. Here is how our test would look if we were to use this library:

~~~python
import pytest
import sqlite3
from sqlite_example import query_database

def test_query_db_mocked_connection_mocker(mocker):
    """Mock the database connection and cursor to ensure the correct methods are called within the query_database function"""
    # Create a mock sqlite3.Connection object
    mock_conn = mocker.Mock(spec=sqlite3.Connection)

    # Create a mock sqlite3.Cursor object
    mock_cursor = mocker.Mock(spec=sqlite3.Cursor)
    mock_cursor.fetchall.return_value = [("Jerry", "Mouse", 1)]
    mock_conn.cursor.return_value = mock_cursor

    # Replace the sqlite3.connect function with a mock
    mocker.patch('sqlite3.connect', return_value=mock_conn)

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

Well done for making it this far, mocking is often a confusing subject due to the many ways in which it can be done and the abstract nature of temporarily replacing parts of the thing you are testing.  After this introduction, you can now solidify your learning by practicing the techniques here on your own code whilst using the documentation as a reference.
