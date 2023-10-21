AC_PREREQ([2.71])
AC_INIT([Life Simulator], [0.20], [helix.graziani@hotmail.com])

AC_CONFIG_SRCDIR([include/school.hpp])
AC_CONFIG_AUX_DIR([tools])

AM_INIT_AUTOMAKE([subdir-objects])

AC_LANG(C++)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MKDIR_P

# Checks for libraries.
AC_CHECK_LIB([curl], [curl_easy_init])
AC_CHECK_LIB([m], [ceil])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strchr strstr])

AC_ARG_ENABLE([gui], AS_HELP_STRING([--disable-gui], [Disable building a GUI using raylib]))

AS_IF([test "x$enable_gui" != "xno"], [
  AC_CONFIG_COMMANDS([raylib], [cmake -S ${srcdir}/raylib -B raylib -DBUILD_EXAMPLES=OFF])
])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
