# lci/configure.ac

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])

AC_INIT([lci],
        [1.0.0],
        [rubicks@users.noreply.github.com],
        [lci],
        [https://github.com/rubicks/lci])

AC_CONFIG_MACRO_DIR([m4])
AC_LANG_PUSH([C])
AM_INIT_AUTOMAKE([subdir-objects])
LT_PREREQ([2.4])
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])



# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_LN_S


# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_LONG_LONG_INT

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strchr strstr strtol])
AC_CHECK_LIB([m],[floor])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
