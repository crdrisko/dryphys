# Building and Installing DryPhys

## Installation

Using [CMake](https://cmake.org), the following commands can be run to install the repository:

```bash
git clone https://github.com/crdrisko/dryphys.git
mkdir build && cd build
cmake ../dryphys/.
make && [sudo] make install
```

By default, the project is installed in the `/usr/local` filesystem but can be changed with the `CMAKE_INSTALL_PREFIX` option.

## Testing

With each library in the project, I have attempted to provide ~100% unit test coverage for all functions. These tests can be found in the `root/tests` directory but do not get built by default. To turn on the testing features of the repository, add either the `phys_build_all` or `phys_build_tests` option to the CMake instructions, depending on which tests and libraries you want to build. More information about the specific options can be found in the [`CMakeLists.txt`](../CMakeLists.txt) file.

```bash
## Same steps as before ... ##
cmake ../dryphys/. -Dphys_build_all=ON
make && [sudo] make install

## Run the unit tests ##
ctest --output-on-failure
```

[GoogleTest](https://github.com/google/googletest), which is used as the unit test framework, will be installed as an external CMake project if the repository can not be found.
