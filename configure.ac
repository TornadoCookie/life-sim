AC_PREREQ([2.71])
AC_INIT([Life Simulator], [0.10], [helix.graziani@hotmail.com])

AC_CONFIG_SRCDIR([include/school.hpp])
AC_CONFIG_AUX_DIR([tools])

AM_INIT_AUTOMAKE([subdir-objects])

AC_LANG(C++)

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MKDIR_P

# Checks for libraries.
AC_CHECK_LIB([curl], [curl_easy_init])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strchr strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
