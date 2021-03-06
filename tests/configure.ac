AC_INIT([gestiong-tests],[0.1],[santi@programacionsocial.net],[gestiong-tests])

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AC_LANG_CPLUSPLUS
AC_PROG_CXX
AM_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])


AC_MSG_NOTICE( [gestiong-tests Custom configuration] )

AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
				  [enable debugging, default: no]),
  [case "${enableval}" in
			  yes)
				AM_CPPFLAGS="$AM_CPPFLAGS -D_GONG_DEBUG"
				AM_CXXFLAGS="$AM_CXXFLAGS -g -g3 -O0 -fPIC -DPIC"
                AC_SUBST(AM_CPPFLAGS)
                AC_SUBST(AM_CXXFLAGS)
				;;
			  no)
				AM_CXXFLAGS="$AM_CXXFLAGS -Wall -Werror -fPIC -DPIC"
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

GONGLIB_CPPFLAGS="$AM_CPPFLAGS -I../../gonglib $MYSQL_CPPFLAGS $POCO_CPPFLAGS $BOOST_CPPFLAGS"
GONGLIB_LIBS="-L../gonglib -lgonglib $MYSQL_LIBS $POCO_LIBS $BOOST_LDFLAGS $BOOST_REGEX_LIB"

AC_ARG_WITH( [gong-qt4],
AS_HELP_STRING([--with-gong-qt4], [Support for the gong qt4 module (default is YES)]),
[ac_cv_use_gong_qt4=$withval],
[ac_cv_use_gong_qt4=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_QT4], [ test "$ac_cv_use_gong_qt4" = yes ])
if test "$ac_cv_use_gong_qt4" = yes ; then

	GONG_CHECK_QT4

	AC_DEFINE( [HAVE_QT4MODULE], [1], [Define to 1 if you have the qt4 module.] )
	GONGQT4_CPPFLAGS="-I../../qt4 $QT_CXXFLAGS"
	GONGQT4_LIBS="-L../qt4 -lgongqt4 $QT_LDADD -lQt3Support"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGQT4_CPPFLAGS"
	GONGLIB_LIBS="$GONGQT4_LIBS $GONGLIB_LIBS"
else
	GONGQT4_CPPFLAGS=
	GONGQT4_LIBS=
fi
AC_SUBST(GONGQT4_CPPFLAGS)
AC_SUBST(GONGQT4_LIBS)

AC_ARG_WITH( [gong-dbapp],
AS_HELP_STRING([--with-gong-dbapp], [Support for the gong dbapp module (default is YES)]),
[ac_cv_use_gong_dbapp=$withval],
[ac_cv_use_gong_dbapp=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_DBAPP], [ test "$ac_cv_use_gong_dbapp" = yes ])
if test "$ac_cv_use_gong_dbapp" = yes ; then
	AC_DEFINE( [HAVE_DBAPPMODULE], [1], [Define to 1 if you have the dbapp module.] )
	GONGDBAPP_CPPFLAGS="-I../../dbapp"
	GONGDBAPP_LIBS="-L../dbapp -lgongdbapp"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGDBAPP_CPPFLAGS"
	GONGLIB_LIBS="$GONGDBAPP_LIBS $GONGLIB_LIBS"
else
	GONGDBAPP_CPPFLAGS=
	GONGDBAPP_LIBS=
fi
AC_SUBST(GONGDBAPP_CPPFLAGS)
AC_SUBST(GONGDBAPP_LIBS)

AC_SUBST(GONGLIB_CPPFLAGS)
AC_SUBST(GONGLIB_LIBS)
AM_CONDITIONAL( [COMPILE_EXPERIMENTAL], [ test ! false ] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_RTK], [ test ! false ] )

AC_CONFIG_FILES( Makefile gonglib/Makefile \
	qt4/Makefile dbapp/Makefile src/Makefile )
AC_OUTPUT
