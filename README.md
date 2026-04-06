# Bazel C/C++ Starter

This workspace uses Bazel with bzlmod enabled via `MODULE.bazel`.

## Layout

- `tools/cc`: macros for C23 and C++20 targets
- `00-tooling-hello/c`: example C package
- `00-tooling-hello/cpp`: example C++ package
- `BUILD.bazel`: root aggregation target so `//:all` builds the starter packages

## Commands

Build all starter packages:

```sh
bazel build //:all
```

Run them:

```sh
bazel run //00-tooling-hello/c:hello_c
bazel run //00-tooling-hello/cpp:hello_cpp
```
