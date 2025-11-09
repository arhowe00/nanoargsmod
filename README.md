# `import nanoargsmod;`

nanoargsmod is an extremely lightweight \>C++20 module that allows your CLI app
to receive values in flags. It allows flags that provide values (e.g., `--flag1
mystring`, `-n 343`, `--alpha 0.05`) and lone boolean flags (`--verbose`).

## Example

To use it, you construct the object with `argc` and `argv` and then query the
flags through that object.

```cpp
nanoargsmod::CmdLineTool clt(argc, argv);     // from main
auto name = clt.get<std::string>("--name");   // returns std::optional<std::string>
bool verbose = clt.flag("--verbose");         // returns true if --verbose was present
auto missing = clt.get<int>("-m");            // this will be std::nullopt. Your app must decide to tell the user and crash

// ... use the options in your code ...
```

## Using in Your Project

One easy way is to use git submodules with cmake. Assuming your modules are in
`external/`:

```bash
git submodule add https://github.com/arhowe00/nanoargsmod.git extenrnal/nanoargsmod
git submodule init --update --recursive
```

Add to your `CMakeLists.txt`:

```cmake
add_subdirectory(external/nanoargsmod/libnanoargsmod)
target_link_libraries(your_app PRIVATE nanoargsmod)
```

In your C++ file with main(), `import nanoargsmod;` should work at the top.

```cpp
import nanoargsmod;

int main(int argc, char** argv) {
    nanoargsmod::CmdLineTool clt(argc, argv);
}
```

## Limitations

This module implements boilerplate. This is not a sophisticated command line
parser that will work with ambitious projects like clang. This is a simple,
no-frills parser that gives command line executable builders the ability to
receive values on the command line without writing a file that implements a
parser themselves.
