---
name: Testable Code and Fixtures
id: testable_code_fixtures
dependsOn: [technology_and_tooling.testing.diagnosing_issues]
tags: [pytest]
learningOutcomes:
  - Understand how to design and write testable code.
  - Understand how grouping tests into classes allows re-use of objects.
  - Be aware of `pytest`'s built-in methods for setup and teardown of objects in tests.
  - Understand the benefit and use of fixtures in test suites.
  - Be able to define fixtures for use in `pytest` test suites.
attribution:
  - citation: This course material was developed as part of UNIVERSE-HPC, which is funded through the SPF ExCALIBUR programme under grant number EP/W035731/1
    url: https://www.universe-hpc.ac.uk
    image: https://www.universe-hpc.ac.uk/assets/images/universe-hpc.png
    license: CC-BY-4.0
---

## Introduction

Having completed the previous sections of the course, we now know:

- how to write tests
- how to debug problems with our code
- how to code in a defensive manner in order to prevent invalid inputs from causing problems.

Up to this point, our examples have focused on unit testing of simple functions that formed part of larger program to analyse inflammation data from patients in a drug trial. If we were to develop this program further, a larger number of tests would need to written and organised in order to cater for the increased complexity of the codebase. In the future, we may also want to integrate our program with other entities that are external to the application itself, such as a database, a web-based service or even analysis components that are written in another language such as C++.

In this section of the course, we will cover two topics that will help us to deal with testing software that is more complex whilst ensuring that it functions as we would expect:

- Designing testable code
- Fixtures

## Designing testable code

We will be developing our inflammation analysis software into a different, object-orientated structure. This will allow us to add flexibility to the program as well as increasing the testability. Ideally we would like our functions that analyse the data to be agnostic to the way that data is stored and loaded. For example having our inflammation data stored in a database rather then CSV files may well be useful, especially if we add data from further studies or from other sources and would like to link together and structure our data in a more formalised way. We should not have to rewrite our tests for the analysis functions to reflect this change.

Increasing the ease of writing tests can result in increased test coverage, and thereby reduce the chance that future changes made to the codebase will introduce regressions. In fact, writing testable code often also results in a cleaner and more modular structure that adheres to best practices. Before we dive into refactoring our code and our tests, here is a summary of some things to think about when writing your software that will make it more testable:

### Separation of concerns

It is good practice to organise code into modular function or classes that each have a single, well-defined responsibility. By doing this, not only will it be more readable, but also it will be more straightforward to isolate and test individual components of your system. Another way to ensure separate concerns is to use _dependency injection_. This involves passing an object or function to our routines rather than creating such objects internally.

### Avoid duplication

Extract common functionality into reusable functions and classes that can be tested thoroughly in isolation. This will also help to reduce the complexity of your code.

### Programming paradigms

Using pure functions that have no side effects, can result in more testable software, this is because the outputs depend on the inputs alone. In this case, it can be ensured that the results are deterministic. For more information, see the [functional programming paradigm](/software_architecture_and_design/functional), pages in our training material.

### Test-driven development

Test-driven development (TDD) is a software development approach that consists of short development cycles, where tests are written before actually writing the classes and functions. The tests are run and initially fail, then the minimum amount of code is written in order to make the tests pass. TDD ensures that the design for testability is in mind from the onset and that requirements are thought about before diving in and starting to implement algorithms.

## Refactoring our code and our tests

We are going to refactor our code to incorporate some of the ideas above in order to make it more testable. Firstly, we are going to change our procedural inflammation project into an object-orientated version, let's start by creating classes to represent the different types of data in our study. This was already investigated in the [object-orientated programming](/software_architecture_and_design/object_orientated) part of the course, where we created different subclasses of the `Person` class and added them to a `Trial` object. We use similar concepts, but will doing things slightly differently here.

We would ideally like to have models that represent individual patients and their associated data. It is going to be up to you to write them!

::::challenge{id=patient-class title="Creating a `Patient` class."}

Write a class `Patient`. For now, the only attributes a `Patient` has is an `id` and a list of numbers containing their inflammation scores (flare-ups per day) as recorded in a row of one of the CSV files. We would also like to add some useful methods to the `Patient` class that will return the mean, max and min of the data for that patient. Call these `data_mean`, `data_max` and `data_min`.

:::solution

```python
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

```

:::
::::

Now we have a class that represents a patient in the study, we can also create a class representing a trial (each of the 12 CSV files represents a separate trial). A trial has an `id` of its own and the attribute `data`, which holds a 2D numpy array from one CSV file (note, this is different to how we set up the `Trial` object in the object-orientated programming section).

::::challenge{id=trial-class title="Creating a `Trial` class."}

Write a class `Trial` that represents a trial. For now, the only attributes a `Trial` has are an `id` and `data`, which is a 2D numpy array with the data from one CSV file. The data from the CSV should be read in by calling a method `load_csv` which can be called from the class constructor (`__init__`). You can also add all the functions from our `models.py` file to this class: `daily_mean` and `daily_max`, `daily_min` and `patient_normalise`, they will need to be modified slightly to work as methods of the `Trial` class.

:::solution

```python
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

```

:::
::::

Now we can create `Trial` objects, with associated `data` attributes, but how can we create `Patient` objects? We could do that by creating them in the standard way:

```python
filename = "inflammation-01.csv"
data = np.loadtxt(fname=filename, delimiter=',')
row = data[0, :] # The first row of the 2D data array
patient_0 = Patient(0, row) # Create a Patient with id 0

```

Alternatively we could create a `Person` using a method in the `Trial` class, since all the required data is already there:

```python
filename = "inflammation-01.csv"
trail_group_01 = Trial(filename, "Group01")
patient_0 = trail_group_01.get_patient(0) # Create a Patient with id 0
```

::::challenge{id=gat-patient title="Get a Patient from a Trial."}

Add a method `get_patient` to the `Trial` class that returns an instance of a `Patient`.

:::solution

```python
class Trial:
    def __init__(self, filename, id):
        self.data = self.load_csv(filename)
        self.id = id

    def get_patient(self, row):
        """Get a Patient object by data row. The id of the object is the
        same as the row number."""
        return Patient(row, self.data[row, :])

    ...
```

:::
::::

We should now adjust and extend our existing tests from the previous lesson in order to fit with these changes.

::::challenge{id=test-patient title="Testing the `Patient` class."}

Write some tests for the `Patient` class that cover the functions `data_mean`, `data_max` and `data_min` as well as a test that checks that the attributes of the class are created correctly. You do not need to write extensive parametrised tests at this stage, this is more an exercise to practice testing class methods as opposed to standard procedural functions.

:::solution

```python
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
```

:::
::::

In the exercise above, we found ourselves having to create the same or similar `Patient` objects multiple times.
To prevent this repetition, we could encapsulate these tests in their own class, `TestPatient`. Writing tests in this manner helps to organise similar tests into groups and also allows sharing of data between tests.
The `pytest` library defines methods that you can add to your class, such as `setup_class` which will be run before running all of the tests in that class or `setup_method` that will be run before each test within the class.
This method can be used for creating data or opening files, for example. An additional method called `teardown_class` could be also be added, if needed, and `pytest` will run this method after the tests in the class have completed.
Alternatively `teardown_method` will run after each test. These methods can be useful for cleaning up in cases where files were created on your system or an connections were opened.
For more information you can [view the documentation here](https://docs.pytest.org/en/latest/how-to/xunit_setup.html).

::::challenge{id=test-patient-class title="Grouping tests in a class."}

Encapsulate the tests for the `Patient` class inside a class named `TestPatient`. Include a method `setup_class` where the two `Patient` objects (with `id` of 1 and 2) will be created rather than creating an object within each test.

:::solution

```python
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

```

:::
::::

## Fixtures

As an alternative to encapsulating test methods in a class and using `setup` and `teardown` methods, we can use _fixtures_.
Fixtures are defined by using the `@pytest.fixture` decorator on a function.
This function will then become available to be passed as an argument to your tests and used within them.

Here is how we can write our tests for the `Patient` class using fixtures instead of a `setup_class` method:

```python
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
```

By default, fixtures will be created when first requested by a test and will be destroyed at the end of the test. We can change this behaviour by defining the _scope_ of the fixture. If we want to use the decorator `@pytest.fixture(scope="session")` for example, the fixture will only be destroyed at the end of the entire test session. Modifying this behaviour is especially useful if the fixture is expensive to create (such as a large file) and we do not need to recreate it for each test.

Next we can adapt our tests from the previous lesson that test the analysis functions that are now methods in the `Trial` class.

::::challenge{id=test-trial title="Testing the `Trial` class."}

Write some tests for the `Trial` class and the associated methods. You can adapt the tests that you wrote in your `test_models.py` file from the previous lesson. You can use fixtures to help with creating instances of the class for testing.

:::solution

Here is the solution for the first three of the tests, the others should have been refactored in a similar fashion.

```python
import numpy.testing as npt

@pytest.fixture()
def trial_instance():
    return Trial("test_data.csv", 1)


class TestTrial:
    def test_daily_mean_zeros(self, trial_instance):
        """Test that mean function works for an array of zeros."""
        trial_instance.data = np.array([
            [0, 0],
            [0, 0],
            [0, 0]])
        test_result = np.array([0, 0])

        # Need to use Numpy testing functions to compare arrays
        npt.assert_array_equal(trial_instance.daily_mean(), test_result)


    def test_daily_mean_integers(self, trial_instance):
        """Test that mean function works for an array of positive integers."""

        trial_instance.data = np.array([
            [1, 2],
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
```

:::
::::

In our tests for the `Trial` class, we have to initialise the class using a CSV file in order to create an instance, even if we do not use that particular data in our tests. How can we simplify this? One thing that can be changed is the `__init__` method, if we just needed the data as an argument, rather than the path to a CSV file, that would make testing easier. After this change, a separate method is going to be needed to allow creating a `Trial` from a CSV filepath, this can be achieved using a class method.

::::challenge{id=load-from-csv title="Refactor the `Trial` class."}

As described above, refactor the `__init__` method of the `Trial` class to take `data` and `id` as arguments. Then create a class method called `from_csv` that accepts a filepath to a CSV file and returns an instance of the `Trial` class with the data loaded.

:::solution
Here is the first section of our adjusted object code:

```python
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

        Parameters:
        filename (str). Filename of CSV to load
        """
        return np.loadtxt(fname=filename, delimiter=',')

    ...
```

:::
::::

Now, a `Trial` object can be created in two ways:

```python
import numpy as np
from inflammation.models import Trial

filename = "inflammation-01.csv"
data = np.loadtxt(fname=filename, delimiter=',')

trial_group_01 = Trial(data, "Group01")
trial_group_02 = Trial.from_csv("inflammation-02.csv", "Group02")
```

For our tests, we no longer need a CSV file in order to ensure that the statistical methods from the class give the expected results and we can replace our `trial_instance` fixture:

```python
@pytest.fixture()
def trial_instance():
    return Trial(np.array([[0, 0],[0, 0]]), 1)
```

Alternatively, we can create objects within test methods, if we prefer to do things that way:

```python
class TestTrial:
    def test_daily_mean_zeros(self):
        """Test that mean function works for an array of zeros."""
        trial_instance = Trial(np.array([[0, 0],[0, 0],[0, 0]]), "Test")
        test_result = np.array([0, 0])

        # Need to use Numpy testing functions to compare arrays
        npt.assert_array_equal(trial_instance.daily_mean(), test_result)

    ...
```

### Using a database rather than CSV files

Our alterations to the `Trial` class to make it easier to test have also paved the way to adding more methods that allow objects to be created from alternative data sources, such as a database. In order to achieve this, we are going to need to create separate functionality to query a database and return some data from it. And since we are writing new functions, we are also going to need to test them! We will now focus on ensuring this database functionality is tested before returning to our inflammation study to incorporate it.

In the following example, we have a function `query_database` that utilises a connection to a [SQLite](https://www.sqlite.org/) database. In a similar fashion to how a CSV file was needed for a `Trial` object, this function is going to be difficult to test without connecting to the `example.db` database. The contents of our file, named `sqlite_example.py` are shown here. You can create the file alongside the rest of the inflammation code in your working directory. You may have to install the `sqlite3` library to your python environment in order to use it.

```python
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

```

If we refactor the function to inject the database connection dependency, we can then easily replace that connection during testing with one that is connected to a test database. This also means we can test the two distinct tasks, connecting to the database and querying the database, separately. Additionally, we have the option to replace the connection with a fake (_mocked_) object, meaning that we do not have to connect to an actual database at all in order to test the function.

::::challenge{id=dependency-inject title="Using dependency injection."}

Create a separate function `connect_to_database` that returns the database connection. Refactor `query_database` to accept the database connection as a named argument. Programming defensively, raise an error if no connection is given.

:::solution

```python
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

```

:::
::::

Now let write some tests for these functions, these can be created in a new file named `test_sqlite.py` within the `/tests` directory. Here are some initial tests that check `connect_to_database` returns a connection of the correct type that refers to correct database file as well as checking that `query_database` returns the correct data. If you would like to learn more about the Structured Query Language (SQL) expressions in this example that are used to interact with the database see the [SQL Zoo](https://sqlzoo.net/wiki/SQL_Tutorial) site.

```python
import pytest
import sqlite3
from pathlib import Path
from sqlite_example import connect_to_database, query_database

def test_connect_to_db_type():
    """
    Test that connect_to_database function returns sqlite3.Connection
    """
    conn = connect_to_database('test.db')
    assert isinstance(conn, sqlite3.Connection)
    conn.close()

def test_connect_to_db_name():
    """
    Test that connect_to_database function connects to correct DB file
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

```

As you can see, the tests are becoming complex, especially the one for `query_database`. Next we can look at how fixtures can help us to reduce this complexity, especially when we want to reuse resources such as a test database.

### More about Fixtures

Our `test_query_database` function can be simplified by separating the process of creating the database and of populating it with data from the test itself.
We can create a fixture to do this which can then be passed to the `test_query_database` function.
The fixture can also be responsible for removing the database after the tests have run.

In the example below, we can use a fixture named `setup_database` to create our test database, add data and also remove the database file once the tests have finished running.
As a result, our `test_query_database` function can be simplified and, if we want to use the test database in other tests, we simply need to add `setup_database` as an argument to those tests.

#### Using `yield` instead of `return`

If the fixture needs to perform some cleanup after the test has run then the fixture function should use a `yield` statement rather than a `return` statement.
Anything up to the `yield` statement is setup code, and anything after will be run post-testing in order to clean up (teardown code).

::::challenge{id=database-fixture title="Adding a fixture to setup the database."}

Add a fixture named `setup_database` to create our test database, add data and also remove the database file once the tests have finished running. Pass the fixture as an argument to `test_query_database`.

:::solution

```python
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

```

:::
::::

:::callout{variant="discussion"}

### Using multiple `assert` statements

According to the book, The Art of Unit Testing by Roy Osherove, a unit test, by definition, should test a _unit of work_.
What this means exactly is itself a point for discussion, but generally it refers to actions that take place between an entry point (e.g. a declaration fo a function) and an exit point (e.g. the output of a function).
It is also often said that each test should fail for only one reason alone.

Does using multiple `assert` statements in one test contravene these guidelines?

Given that, unlike some other testing frameworks, `pytest` will output an error showing which of the `assert` statements in the test failed and why, does this change the situation?

Are there any disadvantages to enforcing a rule of one `assert` per test?
:::

::::challenge{id=nested-fixtures title="Fixtures that use fixtures."}

The `setup_database` fixture does several things including initiating the connection as well as creating and populating the database table. In order to separate out these functionalities, split this fixture into two, with one fixture `database_connection` for providing the database connection and another`setup_database` that uses the first fixture and then populates the database. You can view the [pytest fixtures documentation](https://docs.pytest.org/en/7.1.x/how-to/fixtures.html) as a guide.

:::solution

```python
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
```

:::
::::

#### Using Built-in Fixtures

As well as writing our own fixtures, we can use those that are [predefined/(built-in)](https://docs.pytest.org/en/latest/reference/fixtures.html).
For example we may want to use a temporary directory for our files during testing, rather than creating files in the directory that we are working from (this is what currently happens when we run our database tests).
The built-in fixture `temp_path_factory` allows us to to do this. We can refactor our code to add an extra fixture that uses this feature and then it can be used by all the tests that we have written as well as by the `setup_database` fixture.

::::challenge{id=builtin-fixtures title="Using built-in fixtures."}

Add another fixture, `database_filename`, that uses the built-in `temp_path_factory` fixture to create a temporary directory for storing our `test.db` database file. This fixture can then be passed into the `database_connection` fixture.

:::solution
The contents of our `test_sqlite.py` is now:

```python
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

```

:::
::::

For more details on what you can do with fixtures, please refer to the [pytest fixtures documentation](https://docs.pytest.org/en/7.1.x/how-to/fixtures.html).

#### Next steps

Now we know about testable code and fixtures. Before we add the functionality to create a `Trial` object using data stored in a database, we will look at how to mock objects for testing. This is covered in the [next lesson](./mocking).

:::callout{variant="keypoints"}

- **Separation of concerns** using methods such as **dependency injection** can make it easier to isolate and test components of your code
- Refactoring code to make it **more testable** can make it **less complex** and **more extensible**
- Tests can be grouped into classes in order to organise them
- **Fixtures** allow **setup** and **teardown** of objects and data that are going to be reused in tests
- There are a set of **built-in fixtures** available in `pytest` that can help you create temporary directories or access logging or other outputs during testing

:::
