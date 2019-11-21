# External Libraries
External libraries can be added in this directory, and added to the build with custom `CMakeLists.txt` files.

Libraries should be added in their own directory, with its own `CMakeLists.txt`:
```
/lib/<library name>/CMakeLists.txt      <--- Cmake configurations for library
/lib/<library name>/<library code>/     <--- Library source (ie submodule/repository)
```
