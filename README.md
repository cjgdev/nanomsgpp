libnanomsgpp - C++ bindings for nanomsg
=======================================

`libnanomsgpp` is a portable, pure C++ wrapper around nanomsg.

`libnanomsgpp` is licensed under the MIT license. Please see
the [separate license](COPYING) for more information.

* Website: [libnanomsgpp.github.com](http://libnanomsgpp.github.com)
* StackOverflow Tag: [libnanomsgpp](http://stackoverflow.com/questions/tagged/libnanomsgpp)
* Issues: [GitHub Issues](https://github.com/libnanomsgpp/libnanomsgpp/issues) (Right here!)

What It Can Do
==============

Building libnanomsgpp - Using CMake
===================================

`libnanomsgpp` builds cleanly on most platforms without any external dependencies.

Compiler and linker options
---------------------------

CMake lets you specify a few variables to control the behavior of the
compiler and linker. These flags are rarely used but can be useful for
64-bit to 32-bit cross-compilation.

- `CMAKE_C_FLAGS`: Set your own compiler flags
- `CMAKE_FIND_ROOT_PATH`: Override the search path for libraries

License
=======

`libnanomsgpp` is licensed under the MIT license.