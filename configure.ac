AC_INIT(volume-controller, 0.1)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/maple/main.cpp])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE
AM_PROG_LIBTOOL
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_HEADER_STDC
AC_LANG_CPLUSPLUS

AC_ARG_WITH([maple],
	AS_HELP_STRING([--with-maple=LOC], [Pass the location of maple (assumed to be ~/libmaple)]),
	[have_maple=$withval], [have_maple=$HOME/libmaple])

AC_CHECK_FILE([$have_maple/build/libmaple.a],
	[
		MAPLE_DIR="$have_maple"
		MAPLE_LDADD="-L$have_maple/build/ -L$have_maple/support/ld -lmaple"
		MAPLE_FLAGS="-I$have_maple/libmaple -I$have_maple/libmaple/usb -I$have_maple/libmaple/usb/usb_lib"
		MAPLE_CFLAGS="$MAPLE_FLAGS -std=gnu99"
		MAPLE_CXXFLAGS="$MAPLE_FLAGS -I$have_maple/wirish -I$have_maple/wirish/comm -I$have_maple/wirish/boards -fno-rtti -fno-exceptions"
		AC_SUBST(MAPLE_DIR)
		AC_SUBST(MAPLE_LDADD)
		AC_SUBST(MAPLE_CFLAGS)
		AC_SUBST(MAPLE_CXXFLAGS)
	],
	[AC_MSG_ERROR([Cannot find maple in $have_maple])])


AC_CONFIG_FILES([Makefile src/Makefile src/cli/Makefile src/gtk/Makefile src/maple/Makefile])
AC_OUTPUT
