# For using custom m4 macros
ACLOCAL_AMFLAGS = -I m4

# Compiler build flags
AM_CPPFLAGS = -I${top_srcdir}/src ${NANOMSG_CFLAGS}

# Build rules for nanomsgpp library
pkginclude_HEADERS = \
	src/nanomsgpp/nanomsgpp.hpp

lib_LTLIBRARIES = \
	src/nanomsgpp/libnanomsgpp.la

src_nanomsgpp_libnanomsgpp_la_SOURCES = \
	src/nanomsgpp/device.hpp        \
	src/nanomsgpp/device.cpp        \
	src/nanomsgpp/exception.hpp     \
	src/nanomsgpp/exception.cpp     \
	src/nanomsgpp/message.hpp       \
	src/nanomsgpp/message.cpp       \
	src/nanomsgpp/socket.hpp        \
	src/nanomsgpp/socket.cpp        \
	src/nanomsgpp/socket_option.hpp \
	src/nanomsgpp/socket_option.cpp \
	src/nanomsgpp/socket_type.hpp   \
	src/nanomsgpp/socket_type.cpp
src_nanomsgpp_libnanomsgpp_la_LDFLAGS = -version-info 0:0:0
src_nanomsgpp_libnanomsgpp_la_LIBADD = $(NANOMSG_LIBS)

# Build rules for pkgconfig
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = nanomsgpp.pc

# Build rules for client program
bin_PROGRAMS = \
	src/client/nanomsgpp

src_client_nanomsgpp_SOURCES = \
	src/client/options.hpp \
	src/client/options.cpp \
	src/client/nanomsgpp.cpp
src_client_nanomsgpp_CPPFLAGS = -I${top_srcdir}/src ${NANOMSG_CFLAGS} $(BOOST_CPPFLAGS)
src_client_nanomsgpp_LDFLAGS = $(BOOST_LDFLAGS)
src_client_nanomsgpp_LDADD = $(top_builddir)/src/nanomsgpp/libnanomsgpp.la $(NANOMSG_LIBS) $(BOOST_PROGRAM_OPTIONS_LIB) $(LDADD)

# Build rules for tests.
# TESTS_ENVIRONMENT: Set environment variables for the test run
# TESTS: Define programs run automatically by "make check"
# UNIT_TESTS: Define programs run automatically by "make check-unit-tests"
# check_PROGRAMS: Define programs built by "make check" but not necessarily run
TESTS_ENVIRONMENT=
TESTS=
UNIT_TESTS=
check_PROGRAMS=

# Define unit tests with gtest
UNIT_TESTS += test/nanomsgpp_test
check_PROGRAMS += test/nanomsgpp_test
test_nanomsgpp_test_SOURCES = \
	test/nanomsgpp_test.cpp \
	test/device_test.cpp    \
	test/message_test.cpp   \
	test/socket_test.cpp
test_nanomsgpp_test_CFLAGS = -I$(top_srcdir)/src $(NANOMSG_CFLAGS)
test_nanomsgpp_test_LDADD = $(top_builddir)/src/nanomsgpp/libnanomsgpp.la $(LDADD)

TESTS += $(UNIT_TESTS)

TESTS_ENVIRONMENT_WITH_VALGRIND="libtool --mode=execute valgrind --leak-check=full"

.PHONY: check-with-valgrind
check-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

.PHONY: check-unit-tests
check-unit-tests:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS)

.PHONY: check-unit-tests-with-valgrind
check-unit-tests-with-valgrind:
	$(MAKE) $(AM_MAKEFLAGS) check TESTS=$(UNIT_TESTS) TESTS_ENVIRONMENT=$(TESTS_ENVIRONMENT_WITH_VALGRIND)

# Nonstandard package files for distribution
EXTRA_DIST = \
	README.md \
	LICENSE \
	autogen.sh \
	nanomsgpp.pc

include Makefile.am.coverage
