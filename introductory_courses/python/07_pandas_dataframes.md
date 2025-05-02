---
name: Pandas Dataframes
dependsOn: [introductory_courses.python.06_analyzing_and_visualizing_data]
tags: [python]
learningOutcomes:
  - Reading and writing data.
  - Using Pandas library to inspect, summarize, and subset tabular data.
attribution:
  - citation: >
      "Programming with Python" course by the Carpentries
    url: https://swcarpentry.github.io/python-novice-inflammation/
    image: https://carpentries.org/assets/img/TheCarpentries.svg
    license: CC-BY-4.0
---

:::callout
For this lesson, you will need to download and unzip [uniqplus_python_data.zip](uniqplus_python_data.zip).
:::

## Use the Pandas library to do statistics on tabular data

- Pandas is a widely-used Python library for statistics, particularly on tabular data.
- Borrows many features from R's dataframes.
  - A 2-dimensional table whose columns have names
    and potentially have different data types.
- Load it with `import pandas as pd`. The alias pd is commonly used for Pandas.
- Read a Comma Separated Values (CSV) data file with `pd.read_csv`.
  - Argument is the name of the file to be read.
  - Assign result to a variable to store the data that was read.

```python
import pandas as pd

data = pd.read_csv('data/gapminder_gdp_oceania.csv')
print(data)
```

```text
       country  gdpPercap_1952  gdpPercap_1957  gdpPercap_1962  \
0    Australia     10039.59564     10949.64959     12217.22686
1  New Zealand     10556.57566     12247.39532     13175.67800

   gdpPercap_1967  gdpPercap_1972  gdpPercap_1977  gdpPercap_1982  \
0     14526.12465     16788.62948     18334.19751     19477.00928
1     14463.91893     16046.03728     16233.71770     17632.41040

   gdpPercap_1987  gdpPercap_1992  gdpPercap_1997  gdpPercap_2002  \
0     21888.88903     23424.76683     26997.93657     30687.75473
1     19007.19129     18363.32494     21050.41377     23189.80135

   gdpPercap_2007
0     34435.36744
1     25185.00911
```

- The columns in a dataframe are the observed variables, and the rows are the observations.
- Pandas uses backslash `\` to show wrapped lines when output is too wide to fit the screen.

:::callout

## File Not Found

Our lessons store their data files in a `data` sub-directory,
which is why the path to the file is `data/gapminder_gdp_oceania.csv`.
If you forget to include `data/`,
or if you include it but your copy of the file is somewhere else,
you will get a runtime error
that ends with a line like this:

```text
FileNotFoundError: [Errno 2] No such file or directory: 'data/gapminder_gdp_oceania.csv'
```

:::

## Use `index_col` to specify that a column's values should be used as row headings

- Row headings are numbers (0 and 1 in this case).
- Really want to index by country.
- Pass the name of the column to `read_csv` as its `index_col` parameter to do this.

```python
data = pd.read_csv('data/gapminder_gdp_oceania.csv', index_col='country')
print(data)
```

```text
             gdpPercap_1952  gdpPercap_1957  gdpPercap_1962  gdpPercap_1967  \
country
Australia       10039.59564     10949.64959     12217.22686     14526.12465
New Zealand     10556.57566     12247.39532     13175.67800     14463.91893

             gdpPercap_1972  gdpPercap_1977  gdpPercap_1982  gdpPercap_1987  \
country
Australia       16788.62948     18334.19751     19477.00928     21888.88903
New Zealand     16046.03728     16233.71770     17632.41040     19007.19129

             gdpPercap_1992  gdpPercap_1997  gdpPercap_2002  gdpPercap_2007
country
Australia       23424.76683     26997.93657     30687.75473     34435.36744
New Zealand     18363.32494     21050.41377     23189.80135     25185.00911
```

## Use the `DataFrame.info()` method to find out more about a dataframe

```python
data.info()
```

```text
<class 'pandas.core.frame.DataFrame'>
Index: 2 entries, Australia to New Zealand
Data columns (total 12 columns):
gdpPercap_1952    2 non-null float64
gdpPercap_1957    2 non-null float64
gdpPercap_1962    2 non-null float64
gdpPercap_1967    2 non-null float64
gdpPercap_1972    2 non-null float64
gdpPercap_1977    2 non-null float64
gdpPercap_1982    2 non-null float64
gdpPercap_1987    2 non-null float64
gdpPercap_1992    2 non-null float64
gdpPercap_1997    2 non-null float64
gdpPercap_2002    2 non-null float64
gdpPercap_2007    2 non-null float64
dtypes: float64(12)
memory usage: 208.0+ bytes
```

- This is a `DataFrame`
- Two rows named `'Australia'` and `'New Zealand'`
- Twelve columns, each of which has two actual 64-bit floating point values.
  - We will talk later about null values, which are used to represent missing observations.
- Uses 208 bytes of memory.

## The `DataFrame.columns` variable stores information about the dataframe's columns

- Note that this is data, _not_ a method. (It doesn't have parentheses.)
  - Like `math.pi`.
  - So do not use `()` to try to call it.
- Called a _member variable_, or just _member_.

```python
print(data.columns)
```

```text
Index(['gdpPercap_1952', 'gdpPercap_1957', 'gdpPercap_1962', 'gdpPercap_1967',
       'gdpPercap_1972', 'gdpPercap_1977', 'gdpPercap_1982', 'gdpPercap_1987',
       'gdpPercap_1992', 'gdpPercap_1997', 'gdpPercap_2002', 'gdpPercap_2007'],
      dtype='object')
```

## Use `DataFrame.T` to transpose a dataframe

- Sometimes want to treat columns as rows and vice versa.
- Transpose (written `.T`) doesn't copy the data, just changes the program's view of it.
- Like `columns`, it is a member variable.

```python
print(data.T)
```

```text
country           Australia  New Zealand
gdpPercap_1952  10039.59564  10556.57566
gdpPercap_1957  10949.64959  12247.39532
gdpPercap_1962  12217.22686  13175.67800
gdpPercap_1967  14526.12465  14463.91893
gdpPercap_1972  16788.62948  16046.03728
gdpPercap_1977  18334.19751  16233.71770
gdpPercap_1982  19477.00928  17632.41040
gdpPercap_1987  21888.88903  19007.19129
gdpPercap_1992  23424.76683  18363.32494
gdpPercap_1997  26997.93657  21050.41377
gdpPercap_2002  30687.75473  23189.80135
gdpPercap_2007  34435.36744  25185.00911
```

## Use `DataFrame.describe()` to get summary statistics about data

`DataFrame.describe()` gets the summary statistics of only the columns that have numerical data.
All other columns are ignored, unless you use the argument `include='all'`.

```python
print(data.describe())
```

```text
       gdpPercap_1952  gdpPercap_1957  gdpPercap_1962  gdpPercap_1967  \
count        2.000000        2.000000        2.000000        2.000000
mean     10298.085650    11598.522455    12696.452430    14495.021790
std        365.560078      917.644806      677.727301       43.986086
min      10039.595640    10949.649590    12217.226860    14463.918930
25%      10168.840645    11274.086022    12456.839645    14479.470360
50%      10298.085650    11598.522455    12696.452430    14495.021790
75%      10427.330655    11922.958888    12936.065215    14510.573220
max      10556.575660    12247.395320    13175.678000    14526.124650

       gdpPercap_1972  gdpPercap_1977  gdpPercap_1982  gdpPercap_1987  \
count         2.00000        2.000000        2.000000        2.000000
mean      16417.33338    17283.957605    18554.709840    20448.040160
std         525.09198     1485.263517     1304.328377     2037.668013
min       16046.03728    16233.717700    17632.410400    19007.191290
25%       16231.68533    16758.837652    18093.560120    19727.615725
50%       16417.33338    17283.957605    18554.709840    20448.040160
75%       16602.98143    17809.077557    19015.859560    21168.464595
max       16788.62948    18334.197510    19477.009280    21888.889030

       gdpPercap_1992  gdpPercap_1997  gdpPercap_2002  gdpPercap_2007
count        2.000000        2.000000        2.000000        2.000000
mean     20894.045885    24024.175170    26938.778040    29810.188275
std       3578.979883     4205.533703     5301.853680     6540.991104
min      18363.324940    21050.413770    23189.801350    25185.009110
25%      19628.685413    22537.294470    25064.289695    27497.598692
50%      20894.045885    24024.175170    26938.778040    29810.188275
75%      22159.406358    25511.055870    28813.266385    32122.777857
max      23424.766830    26997.936570    30687.754730    34435.367440
```

- Not particularly useful with just two records,
  but very helpful when there are thousands.

::::challenge{id="reading_other_data" title="Reading Other Data"}

Read the data in `gapminder_gdp_americas.csv`
(which should be in the same directory as `gapminder_gdp_oceania.csv`)
into a variable called `americas`
and display its summary statistics.

:::solution
To read in a CSV, we use `pd.read_csv` and pass the filename `'data/gapminder_gdp_americas.csv'` to it.
We also once again pass the column name `'country'` to the parameter `index_col` in order to index by country.
The summary statistics can be displayed with the `DataFrame.describe()` method.

```python
americas = pd.read_csv('data/gapminder_gdp_americas.csv', index_col='country')
americas.describe()
```

:::
::::

::::challenge{id="inspecting_data" title="Inspecting Data"}

After reading the data for the Americas,
use `help(americas.head)` and `help(americas.tail)`
to find out what `DataFrame.head` and `DataFrame.tail` do.

1. What method call will display the first three rows of this data?
2. What method call will display the last three columns of this data?
   (Hint: you may need to change your view of the data.)

:::solution

1. We can check out the first five rows of `americas` by executing `americas.head()`
   (allowing us to view the head of the DataFrame). We can specify the number of rows we wish
   to see by specifying the parameter `n` in our call
   to `americas.head()`. To view the first three rows, execute:

   ```python
   americas.head(n=3)
   ```

   ```text
             continent  gdpPercap_1952  gdpPercap_1957  gdpPercap_1962  \
   country
   Argentina  Americas     5911.315053     6856.856212     7133.166023
   Bolivia    Americas     2677.326347     2127.686326     2180.972546
   Brazil     Americas     2108.944355     2487.365989     3336.585802

              gdpPercap_1967  gdpPercap_1972  gdpPercap_1977  gdpPercap_1982  \
   country
   Argentina     8052.953021     9443.038526    10079.026740     8997.897412
   Bolivia       2586.886053     2980.331339     3548.097832     3156.510452
   Brazil        3429.864357     4985.711467     6660.118654     7030.835878

              gdpPercap_1987  gdpPercap_1992  gdpPercap_1997  gdpPercap_2002  \
   country
   Argentina     9139.671389     9308.418710    10967.281950     8797.640716
   Bolivia       2753.691490     2961.699694     3326.143191     3413.262690
   Brazil        7807.095818     6950.283021     7957.980824     8131.212843

              gdpPercap_2007
   country
   Argentina    12779.379640
   Bolivia       3822.137084
   Brazil        9065.800825
   ```

2. To check out the last three rows of `americas`, we would use the command,
   `americas.tail(n=3)`, analogous to `head()` used above. However, here we want to look at
   the last three columns so we need to change our view and then use `tail()`. To do so, we
   create a new DataFrame in which rows and columns are switched:

   ```python
   americas_flipped = americas.T
   ```

   We can then view the last three columns of `americas` by viewing the last three rows
   of `americas_flipped`:

   ```python
   americas_flipped.tail(n=3)
   ```

   ```text
   country        Argentina  Bolivia   Brazil   Canada    Chile Colombia  \
   gdpPercap_1997   10967.3  3326.14  7957.98  28954.9  10118.1  6117.36
   gdpPercap_2002   8797.64  3413.26  8131.21    33329  10778.8  5755.26
   gdpPercap_2007   12779.4  3822.14   9065.8  36319.2  13171.6  7006.58

   country        Costa Rica     Cuba Dominican Republic  Ecuador    ...     \
   gdpPercap_1997    6677.05  5431.99             3614.1  7429.46    ...
   gdpPercap_2002    7723.45  6340.65            4563.81  5773.04    ...
   gdpPercap_2007    9645.06   8948.1            6025.37  6873.26    ...

   country          Mexico Nicaragua   Panama Paraguay     Peru Puerto Rico  \
   gdpPercap_1997   9767.3   2253.02  7113.69   4247.4  5838.35     16999.4
   gdpPercap_2002  10742.4   2474.55  7356.03  3783.67  5909.02     18855.6
   gdpPercap_2007  11977.6   2749.32  9809.19  4172.84  7408.91     19328.7

   country        Trinidad and Tobago United States  Uruguay Venezuela
   gdpPercap_1997             8792.57       35767.4  9230.24   10165.5
   gdpPercap_2002             11460.6       39097.1     7727   8605.05
   gdpPercap_2007             18008.5       42951.7  10611.5   11415.8
   ```

   This shows the data that we want, but we may prefer to display three columns instead of three rows,
   so we can flip it back:

   ```python
   americas_flipped.tail(n=3).T
   ```

   **Note:** we could have done the above in a single line of code by 'chaining' the commands:

   ```python
   americas.T.tail(n=3).T
   ```

:::
::::

::::challenge{id="reading_files_in_other_directories" title="Reading Files in Other Directories"}

Imagine the data for your current project is stored in a file called `microbes.csv`,
which is located in a folder called `field_data`.
You are doing analysis in a notebook called `analysis.ipynb`
in a sibling folder called `thesis`:

```text
your_home_directory
+-- field_data/
|   +-- microbes.csv
+-- thesis/
    +-- analysis.ipynb
```

What value(s) should you pass to `read_csv` to read `microbes.csv` in `analysis.ipynb`?

:::solution
We need to specify the path to the file of interest in the call to `pd.read_csv`. We first need to 'jump' out of
the folder `thesis` using '../' and then into the folder `field_data` using 'field_data/'. Then we can specify the filename `microbes.csv.
The result is as follows:

```python
data_microbes = pd.read_csv('../field_data/microbes.csv')
```

:::
::::

::::challenge{id="writing_data" title="Writing Data"}

As well as the `read_csv` function for reading data from a file,
Pandas provides a `to_csv` function to write dataframes to files.
Applying what you've learned about reading from files,
write one of your dataframes to a file called `processed.csv`.
You can use `help` to get information on how to use `to_csv`.
:::solution
In order to write the DataFrame `americas` to a file called `processed.csv`, execute the following command:

```python
americas.to_csv('processed.csv')
```

For help on `to_csv`, you could execute, for example:

```python
help(americas.to_csv)
```

Note that `help(to_csv)` throws an error! This is a subtlety and is due to the fact that `to_csv` is NOT a function in
and of itself and the actual call is `americas.to_csv`.
:::
::::

## Note about Pandas DataFrames/Series

A pands [Dataframe](https://pandas.pydata.org/pandas-docs/stable/generated/pandas.DataFrame.html) is a collection of [Series](https://pandas.pydata.org/pandas-docs/stable/generated/pandas.Series.html);
The DataFrame is the way Pandas represents a table, and Series is the data-structure
Pandas use to represent a column.

Pandas is built on top of the [Numpy](http://www.numpy.org/) library, which in practice means that
most of the methods defined for Numpy Arrays apply to Pandas Series/DataFrames.

What makes Pandas so attractive is the powerful interface to access individual records
of the table, proper handling of missing values, and relational-databases operations
between DataFrames.

## Selecting values

To access a value at the position `[i,j]` of a DataFrame, we have two options, depending on
what is the meaning of `i` in use.
Remember that a DataFrame provides an _index_ as a way to identify the rows of the table;
a row, then, has a _position_ inside the table as well as a _label_, which
uniquely identifies its _entry_ in the DataFrame.

## Use `DataFrame.iloc[..., ...]` to select values by their (entry) position

- Can specify location by numerical index analogously to 2D version of character selection in strings.

```python
import pandas as pd
data = pd.read_csv('data/gapminder_gdp_europe.csv', index_col='country')
print(data.iloc[0, 0])
```

```text
1601.056136
```

## Use `DataFrame.loc[..., ...]` to select values by their (entry) label

- Can specify location by row name analogously to 2D version of dictionary keys.

```python
print(data.loc["Albania", "gdpPercap_1952"])
```

```text
1601.056136
```

## Use `:` on its own to mean all columns or all rows

- Just like Python's usual slicing notation.

```python
print(data.loc["Albania", :])
```

```text
gdpPercap_1952    1601.056136
gdpPercap_1957    1942.284244
gdpPercap_1962    2312.888958
gdpPercap_1967    2760.196931
gdpPercap_1972    3313.422188
gdpPercap_1977    3533.003910
gdpPercap_1982    3630.880722
gdpPercap_1987    3738.932735
gdpPercap_1992    2497.437901
gdpPercap_1997    3193.054604
gdpPercap_2002    4604.211737
gdpPercap_2007    5937.029526
Name: Albania, dtype: float64
```

- Would get the same result printing `data.loc["Albania"]` (without a second index).

```python
print(data.loc[:, "gdpPercap_1952"])
```

```text
country
Albania                    1601.056136
Austria                    6137.076492
Belgium                    8343.105127
⋮ ⋮ ⋮
Switzerland               14734.232750
Turkey                     1969.100980
United Kingdom             9979.508487
Name: gdpPercap_1952, dtype: float64
```

- Would get the same result printing `data["gdpPercap_1952"]`
- Also get the same result printing `data.gdpPercap_1952` (not recommended, because easily confused with `.` notation for methods)

## Select multiple columns or rows using `DataFrame.loc` and a named slice

```python
print(data.loc['Italy':'Poland', 'gdpPercap_1962':'gdpPercap_1972'])
```

```text
             gdpPercap_1962  gdpPercap_1967  gdpPercap_1972
country
Italy           8243.582340    10022.401310    12269.273780
Montenegro      4649.593785     5907.850937     7778.414017
Netherlands    12790.849560    15363.251360    18794.745670
Norway         13450.401510    16361.876470    18965.055510
Poland          5338.752143     6557.152776     8006.506993
```

In the above code, we discover that **slicing using `loc` is inclusive at both
ends**, which differs from **slicing using `iloc`**, where slicing indicates
everything up to but not including the final index.

## Result of slicing can be used in further operations

- Usually don't just print a slice.
- All the statistical operators that work on entire dataframes
  work the same way on slices.
- E.g., calculate max of a slice.

```python
print(data.loc['Italy':'Poland', 'gdpPercap_1962':'gdpPercap_1972'].max())
```

```text
gdpPercap_1962    13450.40151
gdpPercap_1967    16361.87647
gdpPercap_1972    18965.05551
dtype: float64
```

```python
print(data.loc['Italy':'Poland', 'gdpPercap_1962':'gdpPercap_1972'].min())
```

```text
gdpPercap_1962    4649.593785
gdpPercap_1967    5907.850937
gdpPercap_1972    7778.414017
dtype: float64
```

## Use comparisons to select data based on value

- Comparison is applied element by element.
- Returns a similarly-shaped dataframe of `True` and `False`.

```python
# Use a subset of data to keep output readable.
subset = data.loc['Italy':'Poland', 'gdpPercap_1962':'gdpPercap_1972']
print('Subset of data:\n', subset)

# Which values were greater than 10000 ?
print('\nWhere are values large?\n', subset > 10000)
```

```text
Subset of data:
             gdpPercap_1962  gdpPercap_1967  gdpPercap_1972
country
Italy           8243.582340    10022.401310    12269.273780
Montenegro      4649.593785     5907.850937     7778.414017
Netherlands    12790.849560    15363.251360    18794.745670
Norway         13450.401510    16361.876470    18965.055510
Poland          5338.752143     6557.152776     8006.506993

Where are values large?
            gdpPercap_1962 gdpPercap_1967 gdpPercap_1972
country
Italy                False           True           True
Montenegro           False          False          False
Netherlands           True           True           True
Norway                True           True           True
Poland               False          False          False
```

## Select values or NaN using a Boolean mask

- A frame full of Booleans is sometimes called a _mask_ because of how it can be used.

```python
mask = subset > 10000
print(subset[mask])
```

```text
             gdpPercap_1962  gdpPercap_1967  gdpPercap_1972
country
Italy                   NaN     10022.40131     12269.27378
Montenegro              NaN             NaN             NaN
Netherlands     12790.84956     15363.25136     18794.74567
Norway          13450.40151     16361.87647     18965.05551
Poland                  NaN             NaN             NaN
```

- Get the value where the mask is true, and NaN (Not a Number) where it is false.
- Useful because NaNs are ignored by operations like max, min, average, etc.

```python
print(subset[subset > 10000].describe())
```

```text
       gdpPercap_1962  gdpPercap_1967  gdpPercap_1972
count        2.000000        3.000000        3.000000
mean     13120.625535    13915.843047    16676.358320
std        466.373656     3408.589070     3817.597015
min      12790.849560    10022.401310    12269.273780
25%      12955.737547    12692.826335    15532.009725
50%      13120.625535    15363.251360    18794.745670
75%      13285.513523    15862.563915    18879.900590
max      13450.401510    16361.876470    18965.055510
```

## Group By: split-apply-combine

Pandas vectorizing methods and grouping operations are features that provide users
much flexibility to analyse their data.

For instance, let's say we want to have a clearer view on how the European countries
split themselves according to their GDP.

1. We may have a glance by splitting the countries in two groups during the years surveyed,
   those who presented a GDP _higher_ than the European average and those with a _lower_ GDP.
2. We then estimate a _wealthy score_ based on the historical (from 1962 to 2007) values,
   where we account how many times a country has participated in the groups of _lower_ or _higher_ GDP

```python
mask_higher = data > data.mean()
wealth_score = mask_higher.aggregate('sum', axis=1) / len(data.columns)
wealth_score
```

```text
country
Albania                   0.000000
Austria                   1.000000
Belgium                   1.000000
Bosnia and Herzegovina    0.000000
Bulgaria                  0.000000
Croatia                   0.000000
Czech Republic            0.500000
Denmark                   1.000000
Finland                   1.000000
France                    1.000000
Germany                   1.000000
Greece                    0.333333
Hungary                   0.000000
Iceland                   1.000000
Ireland                   0.333333
Italy                     0.500000
Montenegro                0.000000
Netherlands               1.000000
Norway                    1.000000
Poland                    0.000000
Portugal                  0.000000
Romania                   0.000000
Serbia                    0.000000
Slovak Republic           0.000000
Slovenia                  0.333333
Spain                     0.333333
Sweden                    1.000000
Switzerland               1.000000
Turkey                    0.000000
United Kingdom            1.000000
dtype: float64
```

Finally, for each group in the `wealth_score` table, we sum their (financial) contribution
across the years surveyed using chained methods:

```python
data.groupby(wealth_score).sum()
```

```text
          gdpPercap_1952  gdpPercap_1957  gdpPercap_1962  gdpPercap_1967  \
0.000000    36916.854200    46110.918793    56850.065437    71324.848786
0.333333    16790.046878    20942.456800    25744.935321    33567.667670
0.500000    11807.544405    14505.000150    18380.449470    21421.846200
1.000000   104317.277560   127332.008735   149989.154201   178000.350040

          gdpPercap_1972  gdpPercap_1977  gdpPercap_1982  gdpPercap_1987  \
0.000000    88569.346898   104459.358438   113553.768507   119649.599409
0.333333    45277.839976    53860.456750    59679.634020    64436.912960
0.500000    25377.727380    29056.145370    31914.712050    35517.678220
1.000000   215162.343140   241143.412730   263388.781960   296825.131210

          gdpPercap_1992  gdpPercap_1997  gdpPercap_2002  gdpPercap_2007
0.000000    92380.047256   103772.937598   118590.929863   149577.357928
0.333333    67918.093220    80876.051580   102086.795210   122803.729520
0.500000    36310.666080    40723.538700    45564.308390    51403.028210
1.000000   315238.235970   346930.926170   385109.939210   427850.333420
```

::::challenge{id="selection_of_individual_values" title="Selection of Individual Values"}

Assume Pandas has been imported into your notebook
and the Gapminder GDP data for Europe has been loaded:

```python
import pandas as pd

df = pd.read_csv('data/gapminder_gdp_europe.csv', index_col='country')
```

Write an expression to find the Per Capita GDP of Serbia in 2007.
:::solution
The selection can be done by using the labels for both the row ("Serbia") and the column ("gdpPercap_2007"):

```python
print(df.loc['Serbia', 'gdpPercap_2007'])
```

The output is

```text
9786.534714
```

:::
::::

::::challenge{id="extent_of_slicing" title="Extent of Slicing"}

1. Do the two statements below produce the same output?
2. Based on this,
   what rule governs what is included (or not) in numerical slices and named slices in Pandas?

```python
print(df.iloc[0:2, 0:2])
print(df.loc['Albania':'Belgium', 'gdpPercap_1952':'gdpPercap_1962'])
```

:::solution
No, they do not produce the same output! The output of the first statement is:

```text
        gdpPercap_1952  gdpPercap_1957
country
Albania     1601.056136     1942.284244
Austria     6137.076492     8842.598030
```

The second statement gives:

```text
        gdpPercap_1952  gdpPercap_1957  gdpPercap_1962
country
Albania     1601.056136     1942.284244     2312.888958
Austria     6137.076492     8842.598030    10750.721110
Belgium     8343.105127     9714.960623    10991.206760
```

Clearly, the second statement produces an additional column and an additional row compared to the first statement.  
What conclusion can we draw? We see that a numerical slice, 0:2, _omits_ the final index (i.e. index 2)
in the range provided,
while a named slice, 'gdpPercap*1952':'gdpPercap_1962', \_includes* the final element.
:::
::::

::::challenge{id="reconstructing_data" title="Reconstructing Data"}

Explain what each line in the following short program does:
what is in `first`, `second`, etc.?

```python
first = pd.read_csv('data/gapminder_all.csv', index_col='country')
second = first[first['continent'] == 'Americas']
third = second.drop('Puerto Rico')
fourth = third.drop('continent', axis=1)
fourth.to_csv('result.csv')
```

:::solution
Let's go through this piece of code line by line.

```python
first = pd.read_csv('data/gapminder_all.csv', index_col='country')
```

This line loads the dataset containing the GDP data from all countries into a dataframe called
`first`. The `index_col='country'` parameter selects which column to use as the
row labels in the dataframe.

```python
second = first[first['continent'] == 'Americas']
```

This line makes a selection: only those rows of `first` for which the 'continent' column matches
'Americas' are extracted. Notice how the Boolean expression inside the brackets,
`first['continent'] == 'Americas'`, is used to select only those rows where the expression is true.
Try printing this expression! Can you print also its individual True/False elements?
(hint: first assign the expression to a variable)

```python
third = second.drop('Puerto Rico')
```

As the syntax suggests, this line drops the row from `second` where the label is 'Puerto Rico'. The
resulting dataframe `third` has one row less than the original dataframe `second`.

```python
fourth = third.drop('continent', axis=1)
```

Again we apply the drop function, but in this case we are dropping not a row but a whole column.
To accomplish this, we need to specify also the `axis` parameter (we want to drop the second column
which has index 1).

```python
fourth.to_csv('result.csv')
```

The final step is to write the data that we have been working on to a csv file. Pandas makes this easy
with the `to_csv()` function. The only required argument to the function is the filename. Note that the
file will be written in the directory from which you started the Jupyter or Python session.
:::
::::

::::challenge{id="Selecting Indices" title="Selecting Indices"}

Explain in simple terms what `idxmin` and `idxmax` do in the short program below.
When would you use these methods?

```python
data = pd.read_csv('data/gapminder_gdp_europe.csv', index_col='country')
print(data.idxmin())
print(data.idxmax())
```

:::solution
For each column in `data`, `idxmin` will return the index value corresponding to each column's minimum;
`idxmax` will do accordingly the same for each column's maximum value.

You can use these functions whenever you want to get the row index of the minimum/maximum value and not the actual minimum/maximum value.
:::
::::

::::challenge{id="practice_with_selection" title="Practice with Selection"}

Assume Pandas has been imported and the Gapminder GDP data for Europe has been loaded.
Write an expression to select each of the following:

1. GDP per capita for all countries in 1982.
2. GDP per capita for Denmark for all years.
3. GDP per capita for all countries for years _after_ 1985.
4. GDP per capita for each country in 2007 as a multiple of
   GDP per capita for that country in 1952.

:::solution
1:

```python
data['gdpPercap_1982']
```

2:

```python
data.loc['Denmark',:]
```

3:

```python
data.loc[:,'gdpPercap_1985':]
```

Pandas is smart enough to recognize the number at the end of the column label and does not give you an error, although no column named `gdpPercap_1985` actually exists. This is useful if new columns are added to the CSV file later.

4:

```python
data['gdpPercap_2007']/data['gdpPercap_1952']
```

:::
::::

::::challenge{id="exploring_avilable_methods" title="Exploring available methods using the `dir()` function"}

Python includes a `dir()` function that can be used to display all of the available methods (functions) that are built into a data object. In Episode 4, we used some methods with a string. But we can see many more are available by using `dir()`:

```python
my_string = 'Hello world!'   # creation of a string object
dir(my_string)
```

This command returns:

```text
['__add__',
...
'__subclasshook__',
'capitalize',
'casefold',
'center',
...
'upper',
'zfill']
```

You can use `help()` or <kbd>Shift</kbd>+<kbd>Tab</kbdto get more information about what these methods do.

Assume Pandas has been imported and the Gapminder GDP data for Europe has been loaded as `data`. Then, use `dir()`
to find the function that prints out the median per-capita GDP across all European countries for each year that information is available.

:::solution
Among many choices, `dir()` lists the `median()` function as a possibility. Thus,

```python
data.median()
```

:::
::::

::::challenge{id="interpretation" title="Interpretation"}

Poland's borders have been stable since 1945, but changed several times in the years before then.
How would you handle this if you were creating a table of GDP per capita for Poland for the entire twentieth century?
::::
