#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([et-edit], [0.5.0-alpha], [mki1967@gmail.com], [etedit], ['https://github.com/mki1967/et-edit'])
# AC_INIT([et-edit], [0.5.0-alpha], [mki1967@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/transformation_functions.c])
AC_CONFIG_HEADERS([config.h])
# AM_CONFIG_HEADERS([config.h])

# Checks for programs.
# AC_PROG_CXX  # I think I do not need C++ ...
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lGL':
AC_CHECK_LIB([GL], [glPushMatrix])
# FIXME: Replace `main' with a function in `-lX11':
AC_CHECK_LIB([X11], [XRaiseWindow])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [sqrt])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([sqrt])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
