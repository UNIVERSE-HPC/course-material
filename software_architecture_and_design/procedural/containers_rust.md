---
name: Containers
dependsOn: [
    software_architecture_and_design.procedural.variables_rust,
]
tags: [rust]
---

*Container* types are those that can hold other objects, and Rust supports a number of 
different [containers](https://doc.rust-lang.org/std/collections/index.html) we can use to hold 
data of differing types in a multitude of ways. 

## Vectors

One of the most fundamental data structures in any language is the array, used
to hold many values at once in a contiguous region of memory. Rust has a
built-in array type `[T; N]` for every element type `T` and non-negative
compile-time constant size `N`, and a variable-sized, heap-allocated
array type, `Vec<T>`. Vectors are the most commonly used collection type in Rust.


Fixed-size arrays are allocated on the stack, and are not resizable.

```rust
let a = [1, 2, 3]; // a: [i32; 3]
```

Vectors are allocated on the heap, and are resizable.

```rust
let mut v = vec![1, 2, 3]; // v: Vec<i32>
v.push(4);
println!("v = {:?}", v);
```

Here we see the `vec!` macro, which is used to initialise a vector. The `!`
indicates that this is a macro invocation, rather than a function call. The
macro is used to create a vector of any size, and initialise it with the values
provided. The type of the vector is inferred from the values provided.

We also demonstrate the `{:?}` format specifier, which is used to print a value
using the `Debug` trait. This is a trait that is implemented for many types in
the standard library, and is used to provide a human-readable representation of
a value suitable for debugging purposes (as opposed to the `Display` trait, used
by the `{}` format specifier, which is used to provide a human-readable
representation of a value suitable for display to the user).


### Creating and Extracting Things from Vectors

Vectors can be created in a number of ways. We can create an empty vector, and
then push values onto it:

```rust
let mut v = Vec::new();
v.push(1);
```

Or we can create a vector with some initial values:

```rust
let v = vec![1, 2, 3];
```

Or a vector of a given size, with each element initialised to the same value:

```rust
let v = vec![0; 10]; // ten zeroes
```

Individual elements of a vector can be accessed using the `[]` operator:

```rust
let v = vec![1, 2, 3, 4, 5];
println!("The third element of v is {}", v[2]);
```

Note that unlike other languages such as C++, Rust performs bounds checking at runtime, and
will panic if you try to access an element that is out of bounds.

```rust
let v = vec![1, 2, 3, 4, 5];
println!("The 100th element of v is {}", v[99]);
```

You can get the length of a vector using the `len` method:

```rust
let v = vec![1, 2, 3, 4, 5];
println!("The length of v is {}", v.len());
```

For more methods on vectors, see the [documentation](https://doc.rust-lang.org/std/vec/struct.Vec.html).

## Loop or iterate over a Vector

A vector can be iterated over using a `for` loop

```rust
let v = vec![1, 2, 3, 4, 5];
for i in &v { // i: &i32
    println!("{}", i);
}
```

This will print out each element of the vector in turn. Note that we use `&v`
because we want `i` to be an immutable reference to each element of the vector.
If we wanted `i` to take ownership of each element of the vector, we would use
`v` instead of `&v`. Since the loop takes ownership of the vector, the vector is
no longer valid after the completion of the loop.

```rust
let v = vec![1, 2, 3, 4, 5];
for i in v { // i: i32
    println!("{}", i);
}
// v is no longer valid here
```

If we want to mutate each element of the vector in-place, we can use `&mut`.
This will allow us to modify the elements of the vector `v`, and the vector will
still be valid after the loop.

```rust
let mut v = vec![1, 2, 3, 4, 5];
for i in &mut v { // i: &mut i32
    *i += 50;
}
```

::::challenge{id=vector_sum title="Vector Inclusive Scan" }

Write code to calculate the inclusive scan of a vector of integers. The inclusive scan is a new
vector where each element is the sum of all the elements up to and including that element in the
original vector. For example, the inclusive scan of `[1, 2, 3, 4, 5]` is `[1, 3, 6, 10, 15]`.

Write two versions of this code, one which creates a new vector and one which modifies the original
vector in-place.

:::solution
```rust
let v = vec![1, 2, 3, 4, 5];
let mut scan = Vec::new();
let mut sum = 0;
for x in &v {
  sum += x;
  scan.push(sum);
}

println!("scan = {:?}", scan);

let mut v = vec![1, 2, 3, 4, 5];
let mut sum = 0;
for x in &mut v {
  sum += *x;
  *x = sum;
}
```
:::
::::


::::challenge{id=dot_product title="Dot Product" }

Write code to calculate the scalar (dot) product of two `Vec<f64>` variables.

:::solution
```rust
let x = vec![1.0, 2.0, 3.0];
let y = vec![1.0, 2.0, 3.0];

assert_eq!(x.len(), y.len());
let mut dot = 0.0;
for i in 0..x.len() {
  dot += x[i] * y[i];
}
println!("dot = {}", dot);
```
:::
::::

## Slices

A *slice* is a reference to a contiguous region of a vector. Slices are
represented as `&[T]`, where `T` is the type of the elements in the slice. For
example, we can take a slice of the first three elements of a vector:

```rust
let v = vec![1, 2, 3, 4, 5];
let first_three = &v[0..3];
```


::::challenge{id=inclusive_scan_slice title="Inclusive Scan with Slices" }

Modify your code from the previous inclusive scan challenge so that the inclusive scan is performed over the first `n` values of the original vector, using a slice.


:::solution
```rust
let v = vec![1, 2, 3, 4, 5];
let n = 3;
let mut scan = Vec::new();
let mut sum = 0;
for x in &v[0..n] {
  sum += x;
  scan.push(sum);
}
```
:::
::::


### Deleting Values, big-O notation and std::list

Deleting elements from the end of a vector is simple and fast and can be done using the 
`pop_back` function, which takes constant, or $\mathcal{O}(1)$ time using big-O notation. This 
means that the time taken is a constant or fixed amount of time independent of the size 
of the vector. Deleting elements from the *start* or *middle* of the vector is more 
difficult. An `Vec` in Rust is an implementation of an *array* data structure, and 
therefore the values contained occupy a *contiguous* section of memory, the start of 
which is also the start of the vector. When deleting an element from the start or 
middle, the remainder of the vector must be shifted down to maintain the contiguous 
nature of the vector and the alignment of the first element to the start of the 
allocated memory. Therefore deleting elmements from the start or middle of a vector 
takes an amount of time that scales linearly with the size of the vector $n$, or 
$\mathcal{O}(n)$ time.

For example, if we want to delete an element from the middle of a vector while
preserving the order of the elements, we can do the 
following:

```rust
let mut x = vec![1, 2, 3, 4];
let delete_this = 1; // an index to "2"
for i in delete_this..x.len() - 1 {
  x[i] = x[i + 1];
}
x.pop();
```

Notice that this requires a loop through all the elements of the vector, hence the time 
taken is $\mathcal{O}(n)$. The output of this program will show us the vector with a '2' 
removed:

~~~
[1, 3, 4, ]
~~~

If we don't care about the order of the elements, we can swap the element we want to
delete with the last element of the vector, and then pop the last element off the
vector. This is much faster, since it doesn't require a loop through the vector:

```rust
let mut x = vec![1, 2, 3, 4];
let delete_this = 1; // an index to "2"
x[delete_this] = x[x.len() - 1];
x.pop();
```


## Move semantics for containers

Unlike C++, Rust moves owned values into containers, rather than copying them.
If you do want a copy, you can explicitly use the `clone` method.

For example, say we wanted to create a vector of strings from a couple of String variables:

```rust
let first = String::from("Hello");
let second = String::from("World"); 
let v = vec![first, second];
// first and second are no longer valid here
```

This will move the values of `first` and `second` into the vector `v`. If we
wanted to keep the values of `first` and `second` after this, we would need to
clone them:

```rust
let first = String::from("Hello");
let second = String::from("World");
let v = vec![first.clone(), second.clone()];
// first and second are still valid here
```

## The borrow checker and containers

The borrow checker is a key part of Rust's safety guarantees. Recall the rule that it enforces:

> At any given time, you can have either one mutable reference or any number of immutable references.

This rule applies to containers as well. If you've used a C++ `std::vector` before, you might have run into the following problem:

```cpp
std::vector<int> v = {1};
auto first = v[0];
v.push_back(2);
v.push_back(3);
v.push_back(4);
std::cout << first << std::endl;
```

It is not clear what the output of this program will be. The `push_back` method
calls could re-allocate the memory for the vector, invalidating the `first`
variable. In practice, this will probably print `1`, but it could print `2`, or
crash, or do something else entirely. This is an example of *undefined
behaviour*, and is a common source of bugs in C++ programs.

In Rust, the borrow checker will prevent this from happening. Consider the
following Rust code:

```rust
let mut v = vec![1];
let first = &v[0];
v.push(2);
v.push(3);
v.push(4);
println!("{}", first);
```

This will fail to compile, with the following error:

```text
error[E0502]: cannot borrow `v` as mutable because it is also borrowed as immutable
 --> src/main.rs:6:5
  |
5 |     let first = &v[0];
  |                  - immutable borrow occurs here
6 |     v.push(2);
  |     ^^^^^^^^^ mutable borrow occurs here
...
9 |     println!("{}", first);
  |                    ----- immutable borrow later used here
```

The borrow checker has detected that we are trying to mutate `v` while we still
have an immutable reference to it. This is not allowed, as we have both a mutable
and an immutable reference to the same value at the same time.


## Other containers in Rust

Rust has a number of other container types, the [documentation](https://doc.rust-lang.org/std/collections/index.html) gives a list:
- Sequences: Vec, VecDeque, LinkedList
- Maps: HashMap, BTreeMap
- Sets: HashSet, BTreeSet
- Misc: BinaryHeap

The [documentation](https://doc.rust-lang.org/std/collections/index.html) gives a good overview of the different containers, their strengths and weaknesses, and when to use them.

As well as `Vec`, the other most commonly used container is `HashMap`, which is
similar to the C++ `std::map` container, or the Python `dict`.

Here is an example of using a `HashMap`:

```rust
use std::collections::HashMap;

let mut map = HashMap::new();
map.insert("Liverpool", 467995);
map.insert("Edinburgh", 448850);
map.insert("Manchester", 430818);

map.insert("Oxford", 137343);

for (key, value) in &map {
    println!("{}: {}", key, value);
}

println!("The population of Liverpool is {}", map["Liverpool"]);
```

## Tuples

A tuple is a fixed-size container of values that can be of *different* types. It
is most useful for holding a collection of useful variables, or returning
multiple values from a function.

```rust
let fruit = (3.14, 2, "Cherry");
```

Values can be obtained from a tuple via *destructuring*.

```rust
let (weight, number, name) = fruit;
```

Note that we used a tuple and destructuring in the previous example of a `HashMap`:

```rust
// key and value are destructured from the tuple
for (key, value) in &map {
    println!("{}: {}", key, value);
}
```

::::challenge{id=data_analysis_containers title="Analysing the IRIS dataset"}

Download the iris dataset hosted by the UCI Machine Learning Repository [here](https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data). This is a CSV file with the following columns:

1. sepal length in cm
2. sepal width in cm
3. petal length in cm
4. petal width in cm
5. class: 
   -- Iris Setosa
   -- Iris Versicolour
   -- Iris Virginica)_)

Your goal is to provide minimum and maximum bounds of sepal length for each
class of Iris. Below is an example code for reading in the dataset and printing
it to the screen. Modify this code to initially store the dataset in one or
more `Vec`'s. Subsequently, calculate the minimum/maximum sepal length
using your data vector(s) and store the result in a `HashMap` which maps
class name to min/max bounds.

To read in the csv file, we will use the `csv` crate. Add the following to your `Cargo.toml` file:

```toml
[dependencies]
csv = "1.2"
```

And the following to your `main.rs` file:

```rust
use std::error::Error;

fn main() -> Result<(), Box<dyn Error>> {
    let mut reader = csv::Reader::from_path("iris.data")?;
    for result in reader.records() {
        let record = result?;
        let sepal_len = record.get(0).unwrap().parse::<f64>()?;
        let iris_class = record.get(4).unwrap();
        println!("sepal_len = {} iris_class = {}", sepal_len, iris_class);
    }
    Ok(())
}
```

:::callout
Note that we are using the `?` operator to propagate errors from the csv crate `from_path` function, and the `parse` function to the return value of the `main` function. Helpfully, the text of the error is printed to the screen when the program is run and an error occurs.
:::

:::callout
You might run into an issue with the borrow checker using the example code above. Notice how we get a reference to a `str` for the `iris_class` variable. Think about the lifetime of this reference and how it relates to the `Vec` container you are constructing. You might need to use the `to_owned` or `to_string` methods to fix this.
:::

:::callout
Note you can take the minimum of two f64 values using the [`f64::min`](https://doc.rust-lang.org/std/primitive.f64.html#method.min) function, and the maximum using the [`f64::max`](https://doc.rust-lang.org/std/primitive.f64.html#method.max) function.
:::

:::solution
```rust
use std::error::Error;
use std::collections::HashMap;

fn main() -> Result<(), Box<dyn Error>> {
    let mut reader = csv::Reader::from_path("iris.data")?;
    let mut dataset = Vec::new();
    for result in reader.records() {
        let record = result?;
        let sepal_len = record.get(0).unwrap().parse::<f64>()?;
        let iris_class = record.get(4).unwrap().to_owned();
        dataset.push((iris_class, sepal_len));
    }

    let mut stats = HashMap::new();
    for (iris_class, sepal_len) in dataset {
        if stats.contains_key(&iris_class) {
            let (min, max) = stats.get_mut(&iris_class).unwrap();
            *min = f64::min(*min, sepal_len);
            *max = f64::max(*max, sepal_len);
        } else {
            stats.insert(iris_class, (sepal_len, sepal_len));
        }
    }

    for (iris_class, (min, max)) in stats {
        println!("{} = ({}, {})", iris_class, min, max);
    }

    Ok(())
}
```
:::
::::