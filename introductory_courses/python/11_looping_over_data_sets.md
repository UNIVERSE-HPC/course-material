---
name: Looping Over Data Sets
dependsOn: [introductory_courses.python.10_conditionals]
tags: [python]
learningOutcomes:
  - Batch processing files with loops.
  - Using glob and pathlib modules to work with files and paths.
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/assets/img/TheCarpentries.svg
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
    # extract <regionfrom the filename, expected to be in the format 'data/gapminder_gdp_<region>.csv'.
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
