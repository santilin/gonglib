dnl <<<<<MODULE_INFO
dnl MODULE qt4 GONG_CHECK_QT4
dnl MODULE rtk GONG_CHECK_RTK
dnl TYPE Module gonglib .
dnl >>>>>MODULE_INFO

AC_INIT([GongLib],[0.1],[scapel@users.sourceforge.net],[gonglib],[http://www.programacionsocial.net/gonglib])

dnl <<<<<GONGLIB_MODULE_INIT
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_INIT_AUTOMAKE

AC_MSG_NOTICE( [gonglib Custom configuration] )

AC_ARG_WITH( [mysql],
	AS_HELP_STRING([--with-mysql], [Support for input from MySQL (default is YES)]),
	[ac_cv_use_mysql=$withval],
	[ac_cv_use_mysql=yes] )
if test "$ac_cv_use_mysql" = yes ; then
        GONG_CHECK_MYSQL
	if test "x$MYSQL_CPPFLAGS" = x ; then
		AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de MySQL. Si las has instalado en un lugar no estandard, usa la opción --with-mysql-prefix=PFX. Para instalar las bibliotecas de MySQL: (Mandriva: libmysql-devel) (Debian/K/Ubuntu: libmysqlclient15-dev) ]])
	fi
fi

AC_ARG_WITH( [LIBXML2],
			AS_HELP_STRING([--with-libxml2], [Support for XML input and output (default is YES)]),
			[ac_cv_use_libxml2=$withval],
			[ac_cv_use_libxml2=yes] )
if test "$ac_cv_use_libxml2" = yes ; then
	AC_DEFINE( [HAVE_LIBXML2], [], [Define to 1 if you have the Xml2 library.] )
	m4_ifdef([AM_PATH_XML2], [AM_PATH_XML2([2.5.0])], [] )
	if test "x$XML_CPPFLAGS" = x ; then
		AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de XML2 (versión 2.5.0 o superior). Si las has instalado en un lugar no estandard, usa la opción --with-xml-prefix=PFX. Para instalar XML2 (Mandriva: libxml2-devel) (Debian/K/Ubuntu: libxml2-dev) ]])
	fi
fi

AC_ARG_WITH( [boost],
            AS_HELP_STRING([--with-boost], [Support for the Boost library (default is YES)]),
            [ac_cv_use_boost=$withval],
            [ac_cv_use_boost=yes] )
if test "$ac_cv_use_boost" = yes ; then
		AX_BOOST_BASE(1.0)
		AC_DEFINE( [HAVE_boost], [], [Define to 1 if you have the Boost library.] )
fi

AC_ARG_ENABLE(debug,
  AS_HELP_STRING([--enable-debug],
				  [enable debugging, default: no]),
  [case "${enableval}" in
			  yes)
				AM_CPPFLAGS="$AM_CPPFLAGS -Wall -D_GONG_DEBUG"
				AM_CXXFLAGS="$AM_CXXFLAGS -g -g3 -O0"
				SAVE_CXXFLAGS=$CXXFLAGS
				CXXFLAGS=`echo $SAVE_CXXFLAGS | sed -e "s/-O2//" -e "s/-g //"`
				SAVE_CFLAGS=$CFLAGS
				CFLAGS=`echo $SAVE_CFLAGS | sed -e "s/-O2//" -e "s/-g //"`
               AC_SUBST(AM_CPPFLAGS)
               AC_SUBST(AM_CXXFLAGS)
				;;
			  no)
				AM_CPPFLAGS="$AM_CPPFLAGS -Werror -O2"
				SAVE_CXXFLAGS=$CXXFLAGS
				CXXFLAGS=`echo $SAVE_CXXFLAGS | sed -e "s/-O2//" -e "s/-g //"`
				SAVE_CFLAGS=$CFLAGS
				CFLAGS=`echo $SAVE_CFLAGS | sed -e "s/-O2//" -e "s/-g //"`
				CXXFLAGS="$CXXFLAGS -O2"
				CFLAGS="$CFLAGS -O2"
               AC_SUBST(AM_CPPFLAGS)
               AC_SUBST(AM_CXXFLAGS)
				;;
			  *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
  esac],
  [debug=false])

dnl >>>>>GONGLIB_MODULE_INIT

dnl <<<<<GONGLIB_MODULE_GONGLIB
GONGLIB_CPPFLAGS="-I./gonglib $XML_CPPFLAGS $MYSQL_CPPFLAGS"
GONGLIB_LIBS="-L../../gonglib -lgonglib -lboost_system $MYSQL_LIBS $BDB_LDADD $XML_LIBS"
dnl >>>>>GONGLIB_MODULE_GONGLIB


dnl <<<<<GONGLIB_MODULE__MODULES
AC_ARG_WITH( [GONG_QT4],
AS_HELP_STRING([--with-gong-qt4], [Support for the gong qt4 module (default is YES)]),
[ac_cv_use_gong_qt4=$withval],
[ac_cv_use_gong_qt4=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_QT4], [ test "$ac_cv_use_gong_qt4" = yes ])
if test "$ac_cv_use_gong_qt4" = yes ; then

	GONG_CHECK_QT4

	AC_DEFINE( [HAVE_QT4MODULE], [1], [Define to 1 if you have the Qt4 module.] )
	GONGQT4_CPPFLAGS="-I./gong-qt4 $QT_CXXFLAGS"
	GONGQT4_LIBS="-L../../gong-qt4 -lgongqt4 $QT_LDADD -lQt3Support"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGQT4_CPPFLAGS"
	GONGLIB_LIBS="$GONGQT4_LIBS $GONGLIB_LIBS"
else
	GONGQT4_CPPFLAGS=
	GONGQT4_LIBS=
fi
AC_SUBST(GONGQT4_CPPFLAGS)
AC_SUBST(GONGQT4_LIBS)

AC_ARG_WITH( [GONG_RTK],
AS_HELP_STRING([--with-gong-rtk], [Support for the gong rtk module (default is YES)]),
[ac_cv_use_gong_rtk=$withval],
[ac_cv_use_gong_rtk=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_RTK], [ test "$ac_cv_use_gong_rtk" = yes ])
if test "$ac_cv_use_gong_rtk" = yes ; then

	GONG_CHECK_RTK

	AC_DEFINE( [HAVE_RTKMODULE], [1], [Define to 1 if you have the Rtk module.] )
	GONGRTK_CPPFLAGS="-I./gong-rtk"
	GONGRTK_LIBS="-L../../gong-rtk -lgongrtk -ljpeg $BDB_LDADD"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGRTK_CPPFLAGS"
	GONGLIB_LIBS="$GONGRTK_LIBS $GONGLIB_LIBS"
else
	GONGRTK_CPPFLAGS=
	GONGRTK_LIBS=
fi
AC_SUBST(GONGRTK_CPPFLAGS)
AC_SUBST(GONGRTK_LIBS)
dnl >>>>>GONGLIB_MODULE__MODULES

AC_SUBST(GONGLIB_CPPFLAGS)
AC_SUBST(GONGLIB_LIBS)

AC_SUBST([GONGLIB_SO_VERSION], [1:0:0])
AC_SUBST([GONGLIB_API_VERSION], [1.0])
# Override the template file name of the generated .pc file, so that there
# is no need to rename the template file when the API version changes.
AC_CONFIG_FILES( Makefile gonglib-${GONGLIB_API_VERSION}.pc:gonglib.pc.in )
AC_OUTPUT
