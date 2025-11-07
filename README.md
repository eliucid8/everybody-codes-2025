# Everybody Codes 2025
## Author: Eric Liu

to build:
```
# Debug build
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug

# Build debug version (with -g -O0)
cmake --build build-debug -j
```

```
# Release build (optimized, no debug symbols)
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release

# Build release version (with -O3 -DNDEBUG)
cmake --build build-release -j

```
