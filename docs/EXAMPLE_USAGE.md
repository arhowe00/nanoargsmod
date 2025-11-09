# Example usage

In case it helps, here is how I incorporated `nanoargsmod` into my project. This
is the entire patch, notice where I added the library, how I added the
submodule, etc.:

```diff
commit f148cf9b00e7d22e1e61b47b037bee94ff1898a9
Author: Andrew Howe <arhowe00@gmail.com>
Date:   Sun Nov 9 13:45:28 2025 -0500

    Add `nanoargsmod` as command line parser

diff --git a/.gitmodules b/.gitmodules
index 800345d..9745a21 100644
--- a/.gitmodules
+++ b/.gitmodules
@@ -1,3 +1,6 @@
 [submodule "external/Catch2"]
        path = external/Catch2
        url = https://github.com/catchorg/Catch2.git
+[submodule "external/nanoargsmod"]
+       path = external/nanoargsmod
+       url = https://github.com/arhowe00/nanoargsmod.git
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 33d8fd8..9dcc267 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -11,6 +11,7 @@ set(CMAKE_CXX_SCAN_FOR_MODULES ON)
 enable_testing()

 add_subdirectory(external/Catch2)
+add_subdirectory(external/nanoargsmod/libnanoargsmod)
 add_subdirectory(libmenis)
 add_subdirectory(apps/aracne3)
 add_subdirectory(tests)
diff --git a/apps/aracne3/CMakeLists.txt b/apps/aracne3/CMakeLists.txt
index 53f4822..274a82f 100644
--- a/apps/aracne3/CMakeLists.txt
+++ b/apps/aracne3/CMakeLists.txt
@@ -1,4 +1,4 @@
 add_executable(aracne3 main.cpp)

-target_link_libraries(aracne3 PRIVATE menis)
+target_link_libraries(aracne3 PRIVATE menis nanoargsmod)
 target_compile_features(aracne3 PRIVATE cxx_std_23)
diff --git a/apps/aracne3/main.cpp b/apps/aracne3/main.cpp
index c87f9a5..6223d88 100644
--- a/apps/aracne3/main.cpp
+++ b/apps/aracne3/main.cpp
@@ -1,8 +1,19 @@
 #include <iostream>
-import menis.math;
+import nanoargsmod;
+
+int main(int argc, char** argv) {
+    nanoargsmod::CmdLineTool clt(argc, argv);
+
+    auto exp_mat = clt.get<std::string>("--exp_mat");
+    auto regulators = clt.get<std::string>("--regulators");
+    auto output = clt.get<std::string>("--output");
+
+    std::cout << "Parsed options:\n";
+    std::cout << "  --exp_mat: " << (exp_mat.has_value() ? exp_mat.value() : "not provided")
+              << "\n";
+    std::cout << "  --regulators: "
+              << (regulators.has_value() ? regulators.value() : "not provided") << "\n";
+    std::cout << "  --output: " << (output.has_value() ? output.value() : "not provided") << "\n";

-int main() {
-    int result = menis::add(5, 7);
-    std::cout << "5 + 7 = " << result << std::endl;
     return 0;
 }
diff --git a/external/nanoargsmod b/external/nanoargsmod
new file mode 160000
index 0000000..0ce3f3f
--- /dev/null
+++ b/external/nanoargsmod
@@ -0,0 +1 @@
+Subproject commit 0ce3f3fa8d221df420ef5f86e099f0fb6f555940
```
