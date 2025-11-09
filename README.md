# nanoargsmod

Simple command line argument parser using C++23 modules.

## Installation

Add as a git submodule:

```bash
git submodule add https://github.com/yourusername/nanoargsmod.git external/nanoargsmod
git submodule update --init --recursive
```

In your `CMakeLists.txt`:

```cmake
add_subdirectory(external/nanoargsmod/libnanoargsmod)
target_link_libraries(your_executable PRIVATE nanoargsmod)
```

## Usage

```cpp
import nanoargsmod;

int main(int argc, char** argv) {
    nanoargsmod::Args args(argc, argv);

    // Required arguments (throws if missing)
    auto input = args.get("input");

    // Optional with defaults
    auto output = args.get_or("output", "out.txt");
    int threads = args.get_int("threads", 4);
    double threshold = args.get_double("threshold", 0.05);

    // Check presence
    if (args.has("verbose")) { /* ... */ }

    // Positional arguments
    for (const auto& file : args.positional()) { /* ... */ }
}
```

Supports: `-f value`, `--flag value`, `--flag=value`, positional args, `--` separator.

## Building

```bash
git submodule update --init --recursive
cmake --preset deb
cmake --build build-debug
```
