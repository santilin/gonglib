AC_INIT( [Capel], [0.2] )

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AM_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
AX_CXX_COMPILE_STDCXX([11])

AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
				  [enable debugging, default: no]),
  [case "${enableval}" in
			  yes)
				AM_CPPFLAGS="$AM_CPPFLAGS -D_GONG_DEBUG"
				AM_CXXFLAGS="$AM_CXXFLAGS -g -g3 -O0"
                AC_SUBST(AM_CPPFLAGS)
                AC_SUBST(AM_CXXFLAGS)
				;;
			  no)
				AM_CXXFLAGS="$AM_CXXFLAGS -Wall -Werror"
                AC_SUBST(AM_CXXFLAGS)
				;;
			  *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
  esac],
  [debug=false])

AC_ARG_WITH( [poco],
	AS_HELP_STRING([--with-poco], [Use the poco library]),
	[ac_cv_use_poco=$withval],
	[ac_cv_use_poco=yes] )
AM_CONDITIONAL( [COMPILE_WITH_POCOLIB], [ test "$ac_cv_use_poco" = yes ])
if test "$ac_cv_use_poco" = yes ; then
        GONG_CHECK_POCO
	if test "x$POCO_CPPFLAGS" = x ; then
		AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de Poco. Si las has instalado en un lugar no estandard, usa la opción --with-poco=PFX. Para instalar las bibliotecas de MySQL: (Mandriva: libpoco-devel) (Debian/K/Ubuntu: libpoco-dev) ]])
	fi
fi

AC_ARG_WITH( [mysql], 
	AS_HELP_STRING([--with-mysql], [Support for input from MySQL (default is YES)]), 
	[ac_cv_use_mysql=$withval],
	[ac_cv_use_mysql=yes] )
AM_CONDITIONAL( [COMPILE_WITH_MYSQLLIB], [ test "$ac_cv_use_mysql" = yes ])
if test "$ac_cv_use_mysql" = yes ; then
        GONG_CHECK_MYSQL
	if test "x$MYSQL_CPPFLAGS" = x ; then
		AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de MySQL. Si las has instalado en un lugar no estandard, usa la opción --with-mysql-prefix=PFX. Para instalar las bibliotecas de MySQL: (Mandriva: libmysql-devel) (Debian/K/Ubuntu: libmysqlclient15-dev) ]])
	fi
fi

AC_ARG_WITH( [boost],
            AS_HELP_STRING([--with-boost], [Support for the Boost library (default is YES)]),
            [ac_cv_use_boost=$withval],
            [ac_cv_use_boost=yes] )
if test "$ac_cv_use_boost" = yes ; then
		AX_BOOST_BASE(1.0)
		AX_BOOST_REGEX
fi

GONGLIB_CPPFLAGS="$AM_CXXFLAGS -I../../gonglib $MYSQL_CPPFLAGS $POCO_CPPFLAGS $BOOST_CPPFLAGS"
GONGLIB_LIBS="-L../gonglib -lgonglib $MYSQL_LIBS $POCO_LIBS $BOOST_LDFLAGS $BOOST_REGEX_LIB"

if test -f ../src/ventadirecta.cpp ; then
	AC_DEFINE( [HAS_VENTADIRECTA], [1], [VentaDirecta addon] )
	AC_MSG_NOTICE( [Compilando con el addon ventadirecta...] )
else
	AC_MSG_NOTICE( [../src/ventadirecta.cpp no existe ] ) 
fi

AM_CONDITIONAL( [ADDON_VENTADIRECTA], [ test -f ../src/ventadirecta ])

AC_SUBST(GONGLIB_CPPFLAGS)
AC_SUBST(GONGLIB_LIBS)

AC_CONFIG_FILES( Makefile gonglib/Makefile \
	src/Makefile )
AC_OUTPUT

