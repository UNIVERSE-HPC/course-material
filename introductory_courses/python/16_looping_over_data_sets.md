---
name: Looping Over Data Sets
dependsOn: [introductory_courses.python.15_pandas_dataframes]
tags: [python]
learningOutcomes:
  - Batch processing files with loops.
  - Using glob and pathlib modules to work with files and paths.
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/carpentries-logo.svg
    license: CC-BY-4.0
---

## Use a `for` loop to process files given a list of their names

- A filename is a character string.
- And lists can contain character strings.

```python
import pandas as pd
for filename in ['data/gapminder_gdp_africa.csv', 'data/gapminder_gdp_asia.csv']:
    data = pd.read_csv(filename, index_col='country')
    print(filename, data.min())
```

```text
data/gapminder_gdp_africa.csv gdpPercap_1952    298.846212
gdpPercap_1957    335.997115
gdpPercap_1962    355.203227
gdpPercap_1967    412.977514
⋮ ⋮ ⋮
gdpPercap_1997    312.188423
gdpPercap_2002    241.165877
gdpPercap_2007    277.551859
dtype: float64
data/gapminder_gdp_asia.csv gdpPercap_1952    331
gdpPercap_1957    350
gdpPercap_1962    388
gdpPercap_1967    349
⋮ ⋮ ⋮
gdpPercap_1997    415
gdpPercap_2002    611
gdpPercap_2007    944
dtype: float64
```

## Use [`glob.glob`](https://docs.python.org/3/library/glob.html#glob.glob) to find sets of files whose names match a pattern

- In Unix, the term "globbing" means "matching a set of files with a pattern".
- The most common patterns are:
  - `*` meaning "match zero or more characters"
  - `?` meaning "match exactly one character"
- Python's standard library contains the [`glob`](https://docs.python.org/3/library/glob.html) module to provide pattern matching functionality
- The [`glob`](https://docs.python.org/3/library/glob.html) module contains a function also called `glob` to match file patterns
- E.g., `glob.glob('*.txt')` matches all files in the current directory
  whose names end with `.txt`.
- Result is a (possibly empty) list of character strings.

```python
import glob
print('all csv files in data directory:', glob.glob('data/*.csv'))
```

```text
all csv files in data directory: ['data/gapminder_all.csv', 'data/gapminder_gdp_africa.csv', \
'data/gapminder_gdp_americas.csv', 'data/gapminder_gdp_asia.csv', 'data/gapminder_gdp_europe.csv', \
'data/gapminder_gdp_oceania.csv']
```

```python
print('all PDB files:', glob.glob('*.pdb'))
```

```text
all PDB files: []
```

## Use `glob` and `for` to process batches of files

- Helps a lot if the files are named and stored systematically and consistently
  so that simple patterns will find the right data.

```python
for filename in glob.glob('data/gapminder_*.csv'):
    data = pd.read_csv(filename)
    print(filename, data['gdpPercap_1952'].min())
```

```text
data/gapminder_all.csv 298.8462121
data/gapminder_gdp_africa.csv 298.8462121
data/gapminder_gdp_americas.csv 1397.717137
data/gapminder_gdp_asia.csv 331.0
data/gapminder_gdp_europe.csv 973.5331948
data/gapminder_gdp_oceania.csv 10039.59564
```

- This includes all data, as well as per-region data.
- Use a more specific pattern in the exercises to exclude the whole data set.
- But note that the minimum of the entire data set is also the minimum of one of the data sets,
  which is a nice check on correctness.

::::challenge{id="determining_matches" title="Determining Matches"}

Which of these files is _not_ matched by the expression `glob.glob('data/*as*.csv')`?

1. `data/gapminder_gdp_africa.csv`
2. `data/gapminder_gdp_americas.csv`
3. `data/gapminder_gdp_asia.csv`

:::solution

1 is not matched by the glob.
:::
::::

::::challenge{id="minimum_file_size" title="Minimum File Size"}

Modify this program so that it prints the number of records in
the file that has the fewest records.

```python nolint
import glob
import pandas as pd
fewest = ____
for filename in glob.glob('data/*.csv'):
    dataframe = pd.____(filename)
    fewest = min(____, dataframe.shape[0])
print('smallest file has', fewest, 'records')
```

Note that the [`DataFrame.shape()` method](https://pandas.pydata.org/pandas-docs/stable/reference/api/pandas.DataFrame.shape.html)
returns a tuple with the number of rows and columns of the data frame.

:::solution

```python
import glob
import pandas as pd
fewest = float('Inf')
for filename in glob.glob('data/*.csv'):
    dataframe = pd.read_csv(filename)
    fewest = min(fewest, dataframe.shape[0])
print('smallest file has', fewest, 'records')
```

:::
::::

::::challenge{id="comparing_data" title="Comparing Data"}

Write a program that reads in the regional data sets
and plots the average GDP per capita for each region over time
in a single chart.
:::solution
This solution builds a useful legend by using the [string `split` method](https://docs.python.org/3/library/stdtypes.html#str.split) to
extract the `region` from the path 'data/gapminder_gdp_a_specific_region.csv'.

```python
import glob
import pandas as pd
import matplotlib.pyplot as plt
fig, ax = plt.subplots(1,1)
for filename in glob.glob('data/gapminder_gdp*.csv'):
    dataframe = pd.read_csv(filename)
    # extract <region> from the filename, expected to be in the format 'data/gapminder_gdp_<region>.csv'.
    # we will split the string using the split method and `_` as our separator,
    # retrieve the last string in the list that split returns (`<region>.csv`),
    # and then remove the `.csv` extension from that string.
    region = filename.split('_')[-1][:-4]
    dataframe.mean().plot(ax=ax, label=region)
plt.legend()
plt.show()
```

:::
::::

:::callout

## Dealing with File Paths

The [`pathlib` module](https://docs.python.org/3/library/pathlib.html) provides useful abstractions for file and path manipulation like
returning the name of a file without the file extension. This is very useful when looping over files and
directories. In the example below, we create a `Path` object and inspect its attributes.

```python
from pathlib import Path

p = Path("data/gapminder_gdp_africa.csv")
print(p.parent), print(p.stem), print(p.suffix)
```

```text
data
gapminder_gdp_africa
.csv
```

**Hint:** It is possible to check all available attributes and methods on the `Path` object with the `dir()`
function!
:::

## Functions make data analysis reusable

Now that you can write [functions](09_writing_functions) and load data with [pandas](15_pandas_dataframes),
you can combine them: wrap the steps of an analysis in a function, then call it for each file or column.

:::callout

## Using Functions With Conditionals in Pandas

Functions will often contain conditionals. Here is a short example that
will indicate which quartile the argument is in based on hand-coded values
for the quartile cut points.

```python
def calculate_life_quartile(exp):
    if exp < 58.41:
        # This observation is in the first quartile
        return 1
    elif exp >= 58.41 and exp < 67.05:
        # This observation is in the second quartile
        return 2
    elif exp >= 67.05 and exp < 71.70:
        # This observation is in the third quartile
        return 3
    elif exp >= 71.70:
        # This observation is in the fourth quartile
        return 4
    else:
        # This observation has bad data
        return None

calculate_life_quartile(62.5)
```

```text
2
```

That function would typically be used within a `for` loop, but Pandas has
a different, more efficient way of doing the same thing, and that is by
_applying_ a function to a dataframe or a portion of a dataframe. Here
is an example, using the definition above.

```python
import pandas as pd

data = pd.read_csv('data/gapminder_all.csv')
data['life_qrtl'] = data['lifeExp_1952'].apply(calculate_life_quartile)
```

There is a lot in that second line, so let's take it piece by piece.
On the right side of the `=` we start with `data['lifeExp_1952']`, which is the
column in the dataframe called `data` labeled `lifeExp_1952`. We use the
`apply()` to do what it says, apply the `calculate_life_quartile` to the
value of this column for every row in the dataframe.
:::

::::challenge{id="encapsulation" title="Encapsulation"}

Fill in the blanks to create a function that takes a single filename as an argument,
loads the data in the file named by the argument,
and returns the minimum value in that data.

```python nolint
import pandas as pd

def min_in_data(____):
    data = ____
    return ____
```

:::solution

```python
import pandas as pd

def min_in_data(filename):
    data = pd.read_csv(filename)
    return data.min()
```

:::
::::

::::challenge{id="encapsulating_data_analysis" title="Encapsulating Data Analysis"}

Assume that the following code has been executed:

```python
import pandas as pd

df = pd.read_csv('data/gapminder_gdp_asia.csv', index_col=0)
japan = df.loc['Japan']
```

1. Complete the statements below to obtain the average GDP for Japan
   across the years reported for the 1980s.

   ```python nolint
   year = 1983
   gdp_decade = 'gdpPercap_' + str(year // ____)
   avg = (japan.loc[gdp_decade + ___] + japan.loc[gdp_decade + ___]) / 2
   ```

2. Abstract the code above into a single function.

   ```python nolint
   def avg_gdp_in_decade(country, continent, year):
       df = pd.read_csv('data/gapminder_gdp_'+___+'.csv',delimiter=',',index_col=0)
       ____
       ____
       ____
       return avg
   ```

3. How would you generalize this function
   if you did not know beforehand which specific years occurred as columns in the data?
   For instance, what if we also had data from years ending in 1 and 9 for each decade?
   (Hint: use the columns to filter out the ones that correspond to the decade,
   instead of enumerating them in the code.)

:::solution

1. The average GDP for Japan across the years reported for the 1980s is computed with:

   ```python
   year = 1983
   gdp_decade = 'gdpPercap_' + str(year // 10)
   avg = (japan.loc[gdp_decade + '2'] + japan.loc[gdp_decade + '7']) / 2
   ```

2. That code as a function is:

   ```python test
   def avg_gdp_in_decade(country, continent, year):
       df = pd.read_csv('data/gapminder_gdp_' + continent + '.csv', index_col=0)
       c = df.loc[country]
       gdp_decade = 'gdpPercap_' + str(year // 10)
       avg = (c.loc[gdp_decade + '2'] + c.loc[gdp_decade + '7'])/2
       return avg
   ```

3. To obtain the average for the relevant years, we need to loop over them:

   ```python
   def avg_gdp_in_decade(country, continent, year):
       df = pd.read_csv('data/gapminder_gdp_' + continent + '.csv', index_col=0)
       c = df.loc[country]
       gdp_decade = 'gdpPercap_' + str(year // 10)
       total = 0.0
       num_years = 0
       for yr_header in c.index: # c's index contains reported years
           if yr_header.startswith(gdp_decade):
               total = total + c.loc[yr_header]
               num_years = num_years + 1
       return total/num_years
   ```

The function can now be called by:

```python
avg_gdp_in_decade('Japan','asia',1983)
```

```text
20880.023800000003
```

:::
::::
