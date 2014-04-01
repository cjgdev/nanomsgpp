Design of libnanomsgpp
======================

libnanomsgpp tries to follow the C++ conventions and best practices that make
code safer and more robust. Several themes underpin libnanomsgpp which are worth
recounting here for developer reference.

RAII
----

The first theme is that of RAII. This idiom ties resource management to the 
lifetime of an object, allowing automatic allocation and reclamation. RAII leads
to code that is exception-safe, and prevents resource leaks.

C++ Interoperability with Standard Libraries
--------------------------------------------

The next theme is that of interoperability, particularly with the standard
library (STL). Wherever possible, interoperability with the STL is provided as
a convenience, allowing the developer to use standard algorithms and routines
with libnanomsgpp.

C++11
-----

The next theme is that of C++11. As of the time of writing, every commercial
compiler has at least some support for C++11. libnanomsgpp uses C++11 to write
cleaner code that is faster, and more robust than C++03. The latest language
features are used to help developers use libnanomsgpp in The Right Way, and make
the compiler do the hard work of detecting usage errors.

Performance
-----------

The next theme is that of performance. libnanomsg strives to strike a balance
between being usable in a way that is useful and intuitive, while achieving
the performance demanded by developers.

Clean Code
----------

The next theme is that of cleanliness. libnanomsg strives to provide code that
is simple to read and use, and adhere to sound C++ design principles, const
correctness, and principles that promote clean design (SOLID).

Openness
--------

The next theme is that of openness. libnanomsgpp is intended to be used for any
purpose intended by the developer, whether commercial or open source. However,
it is worth mentioning that the use of libnanomsgpp does imply the use of
nanomsg, so you must accept both the license agreement of libnanomsgpp and that
of nanomsg.

Testing
-------

The final theme is that of correctness. libnanomsg comes with a full suite of
unit tests that cover the entire codebase. These tests are built and run 
automatically by Travis CI.