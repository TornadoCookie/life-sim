AC_PREREQ([2.71])
AC_INIT([Life Simulator], [0.10], [helix.graziani@hotmail.com])

AC_CONFIG_SRCDIR([include/school.hpp])
AC_CONFIG_AUX_DIR([tools])

AM_INIT_AUTOMAKE([subdir-objects])

AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([curl/curl.h], [], [AC_MSG_ERROR([curl/curl.h not found.])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
