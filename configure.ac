#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rogomatic], [r2.0.3], [ant@anthive.com])

AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_LN_S
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([curses], [initscr], , AC_MSG_ERROR([Cannot find ncurses]))
AC_CHECK_LIB([m], [sqrt], , AC_MSG_ERROR([Cannot find sqrt]))

# Checks for header files.
AC_CHECK_HEADERS([curses.h math.h fcntl.h stdio.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset setenv sqrt])

AC_CONFIG_FILES([Makefile src/Makefile src/install.h])
AC_OUTPUT
