#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libbutton], [0.0.1], [glen.cornell@gmail.com])
AC_CONFIG_SRCDIR([ptt.h])

# Initiaize automake
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
PKG_CHECK_MODULES([GPIOD], [libgpiod])
AX_PTHREAD

# Checks for header files.
AC_CHECK_HEADERS([netdb.h string.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ALLOCA

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
