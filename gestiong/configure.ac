dnl <<<<<MODULE_INFO
dnl MODULE rtk GONG_CHECK_RTK
dnl MODULE qt4 GONG_CHECK_QT4
dnl MODULE dbapp
dnl MODULE Contactos
dnl MODULE Empresa
dnl MODULE Contab
dnl MODULE Pagos
dnl MODULE Factu
dnl MODULE GastosTipo
dnl MODULE Socias
dnl MODULE SinPapeles
dnl MODULE RecColor
dnl TYPE Project gestiong
dnl >>>>>MODULE_INFO

AC_INIT([GestiONG],[0.5.1],[gestiong@programacionsocial.net],[gestiong])

dnl <<<<<GONGLIB_MODULE_INIT
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AM_PROG_LIBTOOL
AM_INIT_AUTOMAKE

AC_MSG_NOTICE( [gestiong Custom configuration] )

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
AX_LIB_SQLITE3

AC_ARG_WITH( [boost],
            AS_HELP_STRING([--with-boost], [Support for the Boost library (default is YES)]),
            [ac_cv_use_boost=$withval],
            [ac_cv_use_boost=yes] )
if test "$ac_cv_use_boost" = yes ; then
		AX_BOOST_BASE(1.0)
		AC_DEFINE( [HAVE_boost], [], [Define to 1 if you have the Boost library.] )
fi

dnl >>>>>GONGLIB_MODULE_INIT

dnl <<<<<GONGLIB_MODULE_GONGLIB
GONGLIB_CPPFLAGS="-I../../gonglib $MYSQL_CPPFLAGS $SQLITE3_CFLAGS $POCO_CPPFLAGS $BOOST_CPPFLAGS "
GONGLIB_LIBS="-L../gonglib -lgonglib $MYSQL_LIBS $SQLITE3_LDFLAGS $POCO_LIBS $BOOST_LDFLAGS "
dnl >>>>>GONGLIB_MODULE_GONGLIB


dnl <<<<<GONGLIB_MODULE__MODULES
AC_ARG_WITH( [gong-rtk],
AS_HELP_STRING([--with-gong-rtk], [Support for the gong rtk module (default is YES)]), 
[ac_cv_use_gong_rtk=$withval],
[ac_cv_use_gong_rtk=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_RTK], [ test "$ac_cv_use_gong_rtk" = yes ])
if test "$ac_cv_use_gong_rtk" = yes ; then

	GONG_CHECK_RTK

	AC_DEFINE( [HAVE_RTKMODULE], [1], [Define to 1 if you have the rtk module.] )
	GONGRTK_CPPFLAGS="-I../../rtk"
	GONGRTK_LIBS="-L../rtk -lgongrtk -ljpeg -lpng $BDB_LDADD"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGRTK_CPPFLAGS"
	GONGLIB_LIBS="$GONGRTK_LIBS $GONGLIB_LIBS"
else
	GONGRTK_CPPFLAGS=
	GONGRTK_LIBS=
fi
AC_SUBST(GONGRTK_CPPFLAGS)
AC_SUBST(GONGRTK_LIBS)

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

AC_ARG_WITH( [gong-contactos],
AS_HELP_STRING([--with-gong-contactos], [Support for the gong contactos module (default is YES)]), 
[ac_cv_use_gong_contactos=$withval],
[ac_cv_use_gong_contactos=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_CONTACTOS], [ test "$ac_cv_use_gong_contactos" = yes ])
if test "$ac_cv_use_gong_contactos" = yes ; then
	AC_DEFINE( [HAVE_CONTACTOSMODULE], [1], [Define to 1 if you have the Contactos module.] )
	GONGCONTACTOS_CPPFLAGS="-I../../contactos"
	GONGCONTACTOS_LIBS="-L../contactos -lgongcontactos"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGCONTACTOS_CPPFLAGS"
	GONGLIB_LIBS="$GONGCONTACTOS_LIBS $GONGLIB_LIBS"
else
	GONGCONTACTOS_CPPFLAGS=
	GONGCONTACTOS_LIBS=
fi
AC_SUBST(GONGCONTACTOS_CPPFLAGS)
AC_SUBST(GONGCONTACTOS_LIBS)

AC_ARG_WITH( [gong-empresa],
AS_HELP_STRING([--with-gong-empresa], [Support for the gong empresa module (default is YES)]), 
[ac_cv_use_gong_empresa=$withval],
[ac_cv_use_gong_empresa=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_EMPRESA], [ test "$ac_cv_use_gong_empresa" = yes ])
if test "$ac_cv_use_gong_empresa" = yes ; then
	AC_DEFINE( [HAVE_EMPRESAMODULE], [1], [Define to 1 if you have the Empresa module.] )
	GONGEMPRESA_CPPFLAGS="-I../../empresa"
	GONGEMPRESA_LIBS="-L../empresa -lgongempresa"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGEMPRESA_CPPFLAGS"
	GONGLIB_LIBS="$GONGEMPRESA_LIBS $GONGLIB_LIBS"
else
	GONGEMPRESA_CPPFLAGS=
	GONGEMPRESA_LIBS=
fi
AC_SUBST(GONGEMPRESA_CPPFLAGS)
AC_SUBST(GONGEMPRESA_LIBS)

AC_ARG_WITH( [gong-contab],
AS_HELP_STRING([--with-gong-contab], [Support for the gong contab module (default is YES)]), 
[ac_cv_use_gong_contab=$withval],
[ac_cv_use_gong_contab=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_CONTAB], [ test "$ac_cv_use_gong_contab" = yes ])
if test "$ac_cv_use_gong_contab" = yes ; then
	AC_DEFINE( [HAVE_CONTABMODULE], [1], [Define to 1 if you have the Contab module.] )
	GONGCONTAB_CPPFLAGS="-I../../contab"
	GONGCONTAB_LIBS="-L../contab -lgongcontab"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGCONTAB_CPPFLAGS"
	GONGLIB_LIBS="$GONGCONTAB_LIBS $GONGLIB_LIBS"
else
	GONGCONTAB_CPPFLAGS=
	GONGCONTAB_LIBS=
fi
AC_SUBST(GONGCONTAB_CPPFLAGS)
AC_SUBST(GONGCONTAB_LIBS)

AC_ARG_WITH( [gong-pagos],
AS_HELP_STRING([--with-gong-pagos], [Support for the gong pagos module (default is YES)]), 
[ac_cv_use_gong_pagos=$withval],
[ac_cv_use_gong_pagos=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_PAGOS], [ test "$ac_cv_use_gong_pagos" = yes ])
if test "$ac_cv_use_gong_pagos" = yes ; then
	AC_DEFINE( [HAVE_PAGOSMODULE], [1], [Define to 1 if you have the Pagos module.] )
	GONGPAGOS_CPPFLAGS="-I../../pagos"
	GONGPAGOS_LIBS="-L../pagos -lgongpagos"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGPAGOS_CPPFLAGS"
	GONGLIB_LIBS="$GONGPAGOS_LIBS $GONGLIB_LIBS"
else
	GONGPAGOS_CPPFLAGS=
	GONGPAGOS_LIBS=
fi
AC_SUBST(GONGPAGOS_CPPFLAGS)
AC_SUBST(GONGPAGOS_LIBS)

AC_ARG_WITH( [gong-factu],
AS_HELP_STRING([--with-gong-factu], [Support for the gong factu module (default is YES)]), 
[ac_cv_use_gong_factu=$withval],
[ac_cv_use_gong_factu=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_FACTU], [ test "$ac_cv_use_gong_factu" = yes ])
if test "$ac_cv_use_gong_factu" = yes ; then
	AC_DEFINE( [HAVE_FACTUMODULE], [1], [Define to 1 if you have the Factu module.] )
	GONGFACTU_CPPFLAGS="-I../../factu"
	GONGFACTU_LIBS="-L../factu -lgongfactu"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGFACTU_CPPFLAGS"
	GONGLIB_LIBS="$GONGFACTU_LIBS $GONGLIB_LIBS"
else
	GONGFACTU_CPPFLAGS=
	GONGFACTU_LIBS=
fi
AC_SUBST(GONGFACTU_CPPFLAGS)
AC_SUBST(GONGFACTU_LIBS)

AC_ARG_WITH( [gong-gastostipo],
AS_HELP_STRING([--with-gong-gastostipo], [Support for the gong gastostipo module (default is YES)]), 
[ac_cv_use_gong_gastostipo=$withval],
[ac_cv_use_gong_gastostipo=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_GASTOSTIPO], [ test "$ac_cv_use_gong_gastostipo" = yes ])
if test "$ac_cv_use_gong_gastostipo" = yes ; then
	AC_DEFINE( [HAVE_GASTOSTIPOMODULE], [1], [Define to 1 if you have the GastosTipo module.] )
	GONGGASTOSTIPO_CPPFLAGS="-I../../gastostipo"
	GONGGASTOSTIPO_LIBS="-L../gastostipo -lgonggastostipo"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGGASTOSTIPO_CPPFLAGS"
	GONGLIB_LIBS="$GONGGASTOSTIPO_LIBS $GONGLIB_LIBS"
else
	GONGGASTOSTIPO_CPPFLAGS=
	GONGGASTOSTIPO_LIBS=
fi
AC_SUBST(GONGGASTOSTIPO_CPPFLAGS)
AC_SUBST(GONGGASTOSTIPO_LIBS)

AC_ARG_WITH( [gong-socias],
AS_HELP_STRING([--with-gong-socias], [Support for the gong socias module (default is YES)]), 
[ac_cv_use_gong_socias=$withval],
[ac_cv_use_gong_socias=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_SOCIAS], [ test "$ac_cv_use_gong_socias" = yes ])
if test "$ac_cv_use_gong_socias" = yes ; then
	AC_DEFINE( [HAVE_SOCIASMODULE], [1], [Define to 1 if you have the Socias module.] )
	GONGSOCIAS_CPPFLAGS="-I../../socias"
	GONGSOCIAS_LIBS="-L../socias -lgongsocias"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGSOCIAS_CPPFLAGS"
	GONGLIB_LIBS="$GONGSOCIAS_LIBS $GONGLIB_LIBS"
else
	GONGSOCIAS_CPPFLAGS=
	GONGSOCIAS_LIBS=
fi
AC_SUBST(GONGSOCIAS_CPPFLAGS)
AC_SUBST(GONGSOCIAS_LIBS)

AC_ARG_WITH( [gong-sinpapeles],
AS_HELP_STRING([--with-gong-sinpapeles], [Support for the gong sinpapeles module (default is YES)]), 
[ac_cv_use_gong_sinpapeles=$withval],
[ac_cv_use_gong_sinpapeles=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_SINPAPELES], [ test "$ac_cv_use_gong_sinpapeles" = yes ])
if test "$ac_cv_use_gong_sinpapeles" = yes ; then
	AC_DEFINE( [HAVE_SINPAPELESMODULE], [1], [Define to 1 if you have the SinPapeles module.] )
	GONGSINPAPELES_CPPFLAGS="-I../../sinpapeles"
	GONGSINPAPELES_LIBS="-L../sinpapeles -lgongsinpapeles"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGSINPAPELES_CPPFLAGS"
	GONGLIB_LIBS="$GONGSINPAPELES_LIBS $GONGLIB_LIBS"
else
	GONGSINPAPELES_CPPFLAGS=
	GONGSINPAPELES_LIBS=
fi
AC_SUBST(GONGSINPAPELES_CPPFLAGS)
AC_SUBST(GONGSINPAPELES_LIBS)

AC_ARG_WITH( [gong-reccolor],
AS_HELP_STRING([--with-gong-reccolor], [Support for the gong reccolor module (default is YES)]), 
[ac_cv_use_gong_reccolor=$withval],
[ac_cv_use_gong_reccolor=yes] )
AM_CONDITIONAL( [COMPILE_WITH_GONG_RECCOLOR], [ test "$ac_cv_use_gong_reccolor" = yes ])
if test "$ac_cv_use_gong_reccolor" = yes ; then
	AC_DEFINE( [HAVE_RECCOLORMODULE], [1], [Define to 1 if you have the RecColor module.] )
	GONGRECCOLOR_CPPFLAGS="-I../../reccolor"
	GONGRECCOLOR_LIBS="-L../reccolor -lgongreccolor"
	GONGLIB_CPPFLAGS="$GONGLIB_CPPFLAGS $GONGRECCOLOR_CPPFLAGS"
	GONGLIB_LIBS="$GONGRECCOLOR_LIBS $GONGLIB_LIBS"
else
	GONGRECCOLOR_CPPFLAGS=
	GONGRECCOLOR_LIBS=
fi
AC_SUBST(GONGRECCOLOR_CPPFLAGS)
AC_SUBST(GONGRECCOLOR_LIBS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_CPPFLAGS)
dnl >>>>>GONGLIB_MODULE__MODULES

AC_SUBST(GONGLIB_CPPFLAGS)
AC_SUBST(GONGLIB_LIBS)
AM_CONDITIONAL( [COMPILE_EXPERIMENTAL], [ test 0 ])

AC_CONFIG_FILES( Makefile gonglib/Makefile rtk/Makefile qt4/Makefile dbapp/Makefile contactos/Makefile empresa/Makefile contab/Makefile pagos/Makefile factu/Makefile gastostipo/Makefile socias/Makefile sinpapeles/Makefile reccolor/Makefile \
	src/Makefile )
AC_OUTPUT
