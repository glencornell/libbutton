#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([libbutton], [0.0.1], [glen.cornell@gmail.com])
AC_CONFIG_SRCDIR([ptt.h])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([ptt-config.h])

# Initiaize automake
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Initialize libtool
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# Checks for libraries.
# Checks for libgpiod.
AC_ARG_WITH([gpiod], AS_HELP_STRING([--without-gpiod], [Build without libgpiod (default: enabled)]))
AS_IF([test "x$with_gpiod" != "xno"], [
  PKG_CHECK_MODULES([GPIOD], [libgpiod >= 1.6.3],[AC_DEFINE([HAVE_GPIOD], [1], [Define to 1 if you have libgpiod])], [])
])
AM_CONDITIONAL([HAVE_GPIOD],[test "x$with_gpiod" != "xno"])

# Checks for evdev support.
AC_ARG_ENABLE([evdev], AS_HELP_STRING([--disable-evdev], [Enable the evdev driver]))
AS_IF([test "x$enable_evdev" != "xno"], [AC_DEFINE([HAVE_EVDEV],[1],[Enable the evdev driver])], [])
AM_CONDITIONAL([HAVE_EVDEV],[test "x$enable_evdev" != "xno"])


# Checks for header files.
AC_CHECK_HEADERS([fcntl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([select strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
