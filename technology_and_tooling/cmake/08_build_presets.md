---
name: Build presets
dependsOn: [technology_and_tooling.cmake.07_options_and_warnings]
tags: [cpp]
attribution:
  - citation: >
      "Modern CMake" course developed by Fergus Cooper and the Oxford Research
      Software Engineering group
    url: https://www.rse.ox.ac.uk/
    image: ./img/2024_oxrse_square.svg
    license: CC-BY-4.0
---

## Why presets?

So far we have configured our builds by passing options on the command line:

```bash
cmake -S . -B build_dir -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

That works, but it is long, easy to mistype, and hard to share. If a colleague,
a continuous integration server, and your IDE all need to build the project the
same way, they each have to reproduce this line exactly.

_Presets_ solve this. A `CMakePresets.json` file at the root of your project
records named build configurations, so everyone configures with a short,
identical command.

## `CMakePresets.json`

Presets live in a JSON file next to your top-level `CMakeLists.txt`:

```json
{
  "version": 5,
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Default (Ninja, Debug)",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build_dir",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "release",
      "inherits": "default",
      "displayName": "Release",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Release" }
    }
  ],
  "buildPresets": [
    { "name": "default", "configurePreset": "default" },
    { "name": "release", "configurePreset": "release" }
  ],
  "testPresets": [
    { "name": "default", "configurePreset": "default", "output": { "outputOnFailure": true } }
  ]
}
```

The `"version"` field selects the schema, and each schema version needs a
minimum CMake to read it. Version 5 matches our `3.24` baseline. Newer versions
add more features (for example _workflow presets_, which chain configure, build
and test, arrived in version 6 / CMake 3.25).

Note how `release` uses `"inherits": "default"` so it only has to state what
differs. Presets are composable in this way, which keeps them tidy. Here that
also means `release` inherits `binaryDir`, so both presets configure into the
same `build_dir`, and switching between them reconfigures in place. If you would
rather keep them side by side, give each preset its own directory, for instance
`"binaryDir": "${sourceDir}/build_dir/${presetName}"`.

## Using presets

List what is available, then configure, build and test by name:

```bash
cmake --list-presets
```

```output
Available configure presets:

  "default" - Default (Ninja, Debug)
  "release" - Release
```

```bash
cmake --preset release
cmake --build --preset release
ctest --preset default
```

`cmake --preset release` reads the generator, build directory, and cache
variables from the file, so the long command line is gone. The exact same
command works on any machine that has the project checked out.

:::callout
Presets you want to keep to yourself (a local compiler path, a scratch build
directory) go in a `CMakeUserPresets.json` alongside `CMakePresets.json`. It has
the same format, is not meant to be committed, and is a good candidate for your
`.gitignore`.
:::

::::challenge{id=add-a-preset title="Add a preset"}

Add a `CMakePresets.json` to Checkpoint 5 with two configure presets: a `debug`
preset using Ninja, and a `release` preset that inherits from it and sets
`CMAKE_BUILD_TYPE` to `Release`. Configure and build the release preset with
`cmake --preset release` and `cmake --build --preset release`.

:::solution

```json
{
  "version": 5,
  "configurePresets": [
    {
      "name": "debug",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build_dir",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Debug" }
    },
    {
      "name": "release",
      "inherits": "debug",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Release" }
    }
  ],
  "buildPresets": [
    { "name": "release", "configurePreset": "release" }
  ]
}
```

Checkpoints 6 and 7 each ship a `CMakePresets.json`, so you can compare.

:::

::::
