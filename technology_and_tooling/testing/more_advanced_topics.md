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


## Designing testable code

We will be developing our inflammation analysis software into a different, object-orientated structure. This will allow us to add flexibility to the program as well as increasing the testability. Ideally we would like our functions that analyse the data to be agnostic to the way that data is stored and loaded. For example having our inflammation data stored in a database rather then CSV files may well be useful, especially if we add data from further studies or from other sources and would like to link together and structure our data in a more formalised way. We should not have to rewrite our tests for the analysis functions to reflect this change.

Increasing the ease of writing tests can result in increased test coverage, and thereby reduce the chance that future changes made to the codebase will introduce regressions. In fact, writing testable code often also results in a cleaner and more modular structure that adheres to best practices. Before we dive into refactoring our code and our tests, here is a summary of some things to think about when writing your software that will make it more testable:

### Separation of concerns

It is good practice to organise code into modular function or classes that each have a single, well-defined responsibility. By doing this, not only will it be more readable, but also it will be more straightforward to isolate and test individual components of your system. 

### Avoid duplication

Extract common functionality into reusable functions and classes that can be tested thoroughly in isolation. This will also help to reduce the complexity of your code.

### Programming paradigms

Another technique, which can result in more testable software, is to use pure functions that have no side effects, this is because the outputs depend on the inputs alone. In this case, it can be ensured that the results are deterministic. For more information, see the [functional programming paradigm](https://train.oxrse.uk/material/HPCu/software_architecture_and_design/functional), pages in our training material.

### Test-driven development

Improving the testability of your code can also be achieved by creating tests before actually writing the classes and functions. This software development methodology is called Test-Driven Development (TDD). TDD ensures that the design for testability is in mind from the onset and typically involves a process of adding one test at time. This newest test will initially fail since the functionality has not yet been implemented. The code is then written that allows this test to pass and the process is repeated, ensuring that requirements are thought about before diving in and starting to implement algorithms.

### Minimise dependencies using dependency injection

A way to reduce the degree of coupling between a function being tested by a unit test and any dependencies, is to use *dependency injection*. This involves passing an object or function to our routines rather than creating such objects internally. 


## Refactoring our code and our tests
Firstly, we are going to change our procedural inflammation project into an object-orientated version, let's start by creating classes to represent the different types of data in our study. This was already investigated in the [object-orientated programming](https://train.oxrse.uk/material/HPCu/software_architecture_and_design/object_orientated) part of the course, where we created different subclasses of the `Person` class and added them to a `Trial` object. We use similar concepts, but will doing things slightly differently here.

We would ideally like to have models that represent individual patients and their associated data. It is going to be up to you to write them!

::::challenge{id=patient-class title="Creating a `Patient` class."}

 Write a class `Patient`. For now, the only attributes a `Patient` has is an `id` and a list of numbers containing their inflammation scores (flare-ups per day) as recorded in a row of one of the CSV files. We would also like to add some useful methods to the `Patient` class that will return the mean, max and min of the data for that patient. Call these `data_mean`, `data_max` and `data_min`.

:::solution
~~~python
import numpy as np

class Patient:
    def __init__(self, id, data):
        self.id = id
        self.data = data

    def data_mean(self):
        """Calculate the mean of patient's inflammation data."""
        return np.mean(self.data)

    def data_max(self):
        """Calculate the max of patient's inflammation data."""
        return np.max(self.data)

    def data_min(self):
        """Calculate the min of patient's inflammation data."""
        return np.min(self.data)

~~~

:::
::::

Now we have a class that represents a patient in the study, we can also create a class representing a trial (each of the 12 CSV files represents a separate trial). A trial has an `id` of its own and the attribute `data`, which holds a 2D numpy array from one CSV file (note, this is different to how we set up the `Trial` object in the object-orientated programming section).

::::challenge{id=trial-class title="Creating a `Trial` class."}

 Write a class `Trial` that represents a trial. For now, the only attributes a `Trial` has are an `id` and `data`, which is a 2D numpy array with the data from one CSV file. The data from the CSV should be read in by calling a method `load_csv` which can be called from the class constructor (`__init__`). You can also add all the functions from our `models.py` file to this class: `daily_mean` and `daily_max`, `daily_min` and `patient_normalise`, they will need to be modified slightly to work as methods of the `Trial` class.

:::solution
~~~python
class Trial:
    def __init__(self, filename, id):
        self.data = self.load_csv(filename)
        self.id = id

    @staticmethod
    def load_csv(filename):
        """Load a Numpy array from a CSV

        :param filename: Filename of CSV to load
        """
        return np.loadtxt(fname=filename, delimiter=',')

    def daily_mean(self):
        """Calculate the daily mean of a 2d inflammation data array."""
        return np.mean(self.data, axis=0)

    def daily_max(self):
        """Calculate the daily max of a 2d inflammation data array."""
        return np.max(self.data, axis=0)

    def daily_min(self):
        """Calculate the daily min of a 2d inflammation data array."""
        return np.min(self.data, axis=0)

    def patient_normalise(self):
        """
        Normalise patient data from a 2D inflammation data array.

        NaN values are ignored, and normalised to 0.

        Negative values are rounded to 0.
        """
        if np.any(self.data < 0):
            raise ValueError('Inflammation values should not be negative')
        if not isinstance(self.data, np.ndarray):
            raise TypeError('data input should be ndarray')
        if len(self.data.shape) != 2:
            raise ValueError('inflammation array should be 2-dimensional')
        max_data = np.nanmax(self.data, axis=1)
        with np.errstate(invalid='ignore', divide='ignore'):
            normalised = self.data / max_data[:, np.newaxis]
        normalised[np.isnan(normalised)] = 0
        normalised[normalised < 0] = 0
        return normalised

~~~

:::
::::

Now we can create `Trial` objects, with associated `data` attributes, but how can we create `Patient` objects? We could do that by creating them in the standard way:

~~~python
filename = "inflammation-01.csv"
data = np.loadtxt(fname=filename, delimiter=',')
row = data[0, :] # The first row of the 2D data array
patient_0 = Patient(0, row) # Create a Patient with id 0

~~~

Alternatively we could create a `Person` using a method in the `Trial` class, since all the required data is already there:

~~~python
class Trial:
    def __init__(self, filename, id):
        self.data = self.load_csv(filename)
        self.id = id

    def get_patient(self, row):
        """Get a Patient object by data row. The id of the object is the 
        same as the row number."""
        return Patient(row, self.data[row, :])

    ...

filename = "inflammation-01.csv"
trail_group_01 = Trial(filename, "Group01")
patient_0 = trail_group_01.get_patient(0) # Create a Patient with id 0
~~~


We should now adjust and extend our existing tests from the previous lesson in order to fit with these changes. 

::::challenge{id=test-patient title="Testing the `Patient` class."}

 Write some tests for the `Patient` class that cover the functions `data_mean`, `data_max` and `data_min` as well as a test that checks that the attributes of the class are created correctly. You do not need to write extensive parametrised tests at this stage, this is more an exercise to practice testing class methods as opposed to standard procedural functions.

:::solution
~~~python
import pytest
from inflammation.models import Patient

def test_patient_data_mean():
    patient = Patient(id=1, data=[1, 2, 3, 4, 5])
    assert patient.data_mean() == 3.0

def test_patient_data_max():
    patient = Patient(id=1, data=[1, 2, 3, 4, 5])
    assert patient.data_max() == 5

def test_patient_data_min():
    patient = Patient(id=1, data=[1, 2, 3, 4, 5])
    assert patient.data_min() == 1

def test_patient_attributes():
    patient = Patient(id=2, data=[10, 20, 30, 40, 50])
    assert patient.id == 2
    assert patient.data == [10, 20, 30, 40, 50]
~~~

:::
::::

In the exercise above, we found ourselves having to create the same or similar `Patient` objects multiple times. To prevent this repetition, we could encapsulate these tests in their own class like this:

~~~python
import pytest
from inflammation.models import Patient

class TestPatient:
    def setup_class(self):
        self.patient1 = Patient(id=1, data=[1, 2, 3, 4, 5])
        self.patient2 = Patient(id=2, data=[10, 20, 30, 40, 50])

    def test_patient_data_mean(self):
        assert self.patient1.data_mean() == 3.0

    def test_patient_data_max(self):
        assert self.patient1.data_max() == 5

    def test_patient_data_min(self):
        assert self.patient1.data_min() == 1

    def test_patient_attributes(self):
        assert self.patient2.id == 2
        assert self.patient2.data == [10, 20, 30, 40, 50]
~~~

Writing tests in this manner helps to organise similar tests into groups and also allows sharing of data between tests. The `pytest` library will run the `setup_class` method before running all of the tests in the class, if you wish to run the setup before each individual test, the `setup_method` method can be used. Similarly, if you want to perform any actions after running the test methods in the class, `teardown_class` and `teardown_method` methods exist. These can be useful for cleaning up, for example if any files were created on your system or an connections were opened. For more information you can [view the documentation here](https://docs.pytest.org/en/latest/how-to/xunit_setup.html).

:::callout
## Fixtures

As an alternative to encapsulating test methods in a class and using `setup` and `teardown` methods, we can use *fixtures*. Fixtures are defined by using the `@pytest.fixture` decorator on a function. Then this function will become available to be passed as an argument to your tests and used within them. If there is a cleanup part to the code, then the fixture function should be written to include a `yield` statement rather than a `return` statement. Anything up to the `yield` statement is setup code, and anything after the statement will be run post-testing in order to clean up.

Here is how we can write our tests for the `Person` class using fixtures instead of a `setup_class` method:

~~~python
import pytest
from inflammation.models import Patient

@pytest.fixture()
def patient_1():
    return Patient(id=1, data=[1, 2, 3, 4, 5])

@pytest.fixture()
def patient_2():
    return Patient(id=2, data=[10, 20, 30, 40, 50])

def test_patient_data_mean(patient_1):
    assert patient_1.data_mean() == 3.0

def test_patient_data_max(patient_1):
    assert patient_1.data_max() == 5

def test_patient_data_min(patient_1):
    assert patient_1.data_min() == 1

def test_patient_attributes(patient_2):
    assert patient_2.id == 2
    assert patient_2.data == [10, 20, 30, 40, 50]
~~~

We could also encapsulate our tests in a class as well as using fixtures if we wished in order to keep them organised. In this case, the fixtures are available to be used within any classes we create in the file:

~~~python
import pytest
from inflammation.models_oo import Patient

@pytest.fixture()
def patient_1():
    return Patient(id=1, data=[1, 2, 3, 4, 5])

@pytest.fixture()
def patient_2():
    return Patient(id=2, data=[10, 20, 30, 40, 50])

class TestPatient:
    
    def test_patient_data_mean(self, patient_1):
        assert patient_1.data_mean() == 3.0

    def test_patient_data_max(self, patient_1):
        assert patient_1.data_max() == 5

    def test_patient_data_min(self, patient_1):
        assert patient_1.data_min() == 1

    def test_patient_attributes(self, patient_2):
        assert patient_2.id == 2
        assert patient_2.data == [10, 20, 30, 40, 50]
~~~

By default, fixtures will be created when first requested by a test and will be destroyed at the end of the test. We can change this behaviour by defining the *scope* of the fixture. If we want to use the decorator `@pytest.fixture(scope="session")` for example, the fixture will only be destroyed at the end of the entire test session. Modifying this behaviour is especially useful if the fixture is expensive to create (such as a large file) and we do not need to recreate it for each test.

:::

Next we can adapt our tests from the previous lesson that test the analysis functions that are now methods in the `Trial` class. 

::::challenge{id=test-trial title="Testing the `Trial` class."}

 Write some tests for the `Trial` class and the associated methods. You can adapt the tests that you wrote in your `test_models.py` file from the previous lesson. You can use fixtures to help with creating instances of the class for testing. 

:::solution

Here is the solution for the first three of the tests, the others should have been refactored in a similar fashion.

~~~python
@pytest.fixture()
def trial_instance():
    return Trial("test_data.csv", 1)


class TestTrial:
    def test_daily_mean_zeros(self, trial_instance):
        """Test that mean function works for an array of zeros."""
        trial_instance.data = np.array([[0, 0],
                            [0, 0],
                            [0, 0]])
        test_result = np.array([0, 0])

        # Need to use Numpy testing functions to compare arrays
        npt.assert_array_equal(trial_instance.daily_mean(), test_result)


    def test_daily_mean_integers(self, trial_instance):
        """Test that mean function works for an array of positive integers."""

        trial_instance.data = np.array([[1, 2],
                            [3, 4],
                            [5, 6]])
        test_result = np.array([3, 4])

        # Need to use Numpy testing functions to compare arrays
        npt.assert_array_equal(trial_instance.daily_mean(), test_result)


    @pytest.mark.parametrize(
        "test, expected",
        [
            ([ [0, 0, 0], [0, 0, 0], [0, 0, 0] ], [0, 0, 0]),
            ([ [4, 2, 5], [1, 6, 2], [4, 1, 9] ], [4, 6, 9]),
            ([ [4, -2, 5], [1, -6, 2], [-4, -1, 9] ], [4, -1, 9]),
        ])
    def test_daily_max(self, test, expected, trial_instance):
        """Test max function works for zeroes, positive integers, mix of positive/negative integers."""
        trial_instance.data = np.array(test)
        npt.assert_array_equal(trial_instance.daily_max(), np.array(expected))

    ...
~~~

:::
::::

In our tests for the `Trial` class, we have to initialise the class using a CSV file in order to create an instance, even if we do not use the data in our tests.  How can we simplify this? One thing that can be changed is the `__init__` method, if we just needed the data as an argument, rather than the path to a CSV file, that would make testing easier. After this change, a separate method is going to be needed to allow creating a `Trial` from a CSV filepath, this can be achieved using a class method. Here is the first section of our adjusted object code:

~~~python
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

    @staticmethod
    def load_csv(filename):
        """Load a Numpy array from a CSV

        :param filename: Filename of CSV to load
        """
        return np.loadtxt(fname=filename, delimiter=',')

    ...
~~~

Now, a `Trial` object can be instantiated in two ways:

~~~python
import numpy as np
from inflammation.models import Trial

filename = "inflammation-01.csv"
data = np.loadtxt(fname=filename, delimiter=',')

trial_group_01 = Trial(data, "Group01")
trial_group_02 = Trial.from_csv("inflammation-02.csv", "Group02")
~~~

For our tests, we no longer need a CSV file in order to ensure that the statistical methods from the class give the expected results and we can replace our `trial_instance` fixture:

~~~python
@pytest.fixture()
def trial_instance():
    return Trial(np.array([[0, 0],[0, 0]]), 1)
~~~

Alternatively, we can create objects within test methods, if we prefer to do things that way:

~~~python
class TestTrial:
    def test_daily_mean_zeros(self):
        """Test that mean function works for an array of zeros."""
        trial_instance = Trial(np.array([[0, 0],[0, 0],[0, 0]]), "Test")
        test_result = np.array([0, 0])

        # Need to use Numpy testing functions to compare arrays
        npt.assert_array_equal(trial_instance.daily_mean(), test_result)

    ...
~~~

### Using a database rather than CSV files

Our alterations to the `Trial` class to make it easier to test have also paved the way to easily adding more methods that allow objects to be created from alternative data sources, such as a database. In order to achieve this, we are going to need to create separate functionality to query a database and return some data from it. Since we are writing new functions, we are also going to need to test them! We will now focus on ensuring this database functionality is tested before returning to our inflammation study to incorporate it.

In the following example, we have a function `query_database` that utilises a connection to a [SQLite](https://www.sqlite.org/) database. In a similar fashion to how a CSV file was needed for a `Trial` object, this function is going to be difficult to test without connecting to the `example.db` database. The contents of our file, named `sqlite_example.py` are shown here:

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

If we refactor the function to inject the database connection dependency, we can easily replace that connection during testing with one that is connected to a test database. This also means we can test the two distinct tasks, connecting to the database and querying the database, separately. Additionally, we have the option to replace the connection with a fake (*mocked*) object, meaning that we do not have to connect to an actual database at all in order to test the function. Information on mocking will be given later in this lesson.

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

Here is an example of some tests for these functions, these can be created in a new file named `test_sqlite.py` within the `/tests` directory. If you would like to learn more about the Structured Query Language (SQL) expressions in this example that are used to interact with the database see the [SQL Zoo](https://sqlzoo.net/wiki/SQL_Tutorial) site:

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

### More about Fixtures

In the example below, we can use a fixture named `setup_database` to create our test database, add data and also remove the database file once the tests have finished running. As a result, our `test_query_database` function can be simplified and if we want to use the test database in other tests, we simply need to add `setup_database` as an argument to those tests.

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
    conn.commit()
    yield conn  # Provide the fixture value
    # Teardown database connection
    cur.execute("DROP TABLE Animals")
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
    conn.commit()
    yield conn
    cur.execute("DROP TABLE Animals")

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
    conn.commit()
    yield conn
    cur.execute("DROP TABLE Animals")

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

For more details on what you can do with fixtures, please refer to the [pytest fixtures documentation](https://docs.pytest.org/en/7.1.x/how-to/fixtures.html).

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

We have now covered a number of topics, designing testable code, using fixtures and mocking. These should help you to ensure you write reliable and maintainable software. Happy testing!
