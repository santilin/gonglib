AC_DEFUN([GONG_CHECK_POCO], [
	AC_MSG_CHECKING([for libpoco...])
	POCO_CPPFLAGS="-I/usr/include"
	POCO_LIBS="-L/usr/lib -lPocoNet -lPocoXML -lPocoFoundation"
	AC_MSG_RESULT( $POCO_CPPFLAGS $POCO_LIBS )
	AC_DEFINE( [HAVE_POCOLIB], [1], [Define to 1 if you have the Poco library.] )

dnl	ac_save_CPPFLAGS=$CPPFLAGS
dnl	ac_save_LIBS=$LIBS
dnl	CPPFLAGS="$CPPFLAGS $POCO_CPPFLAGS"
dnl	LIBS="$LIBS $POCO_LIBS"
dnl
dnl        AC_TRY_COMPILE([
dnl                #include <Poco/Net/ServerSocket.h>
dnl                #include <Poco/Net/StreamSocket.h>
dnl                #include <Poco/Timestamp.h>
dnl                #include <Poco/DateTime.h>
dnl                #include <Poco/DateTimeParser.h>
dnl                #include <Poco/DateTimeFormatter.h>
dnl                #include <Poco/Net/NetException.h>
dnl                ],[ echo *** The test compilation failed],
dnl                have_poco=yes, have_poco=no)
dnl        if test "$have_poco" = yes; then
dnl			AC_MSG_RESULT(yes)
dnl			AC_DEFINE( [HAVE_POCOLIB], [], [Define to 1 if you have the Poco library.] )
dnl        else
dnl			AC_MSG_RESULT(no)
dnl                 echo
dnl                 echo "ERROR: Poco development libraries are required. Install and try again (see wiki:prerequisites)."
dnl                 echo
dnl                 exit 1
dnl				CPPFLAGS="$ac_save_CPPFLAGS"
dnl				LIBS="$ac_save_LIBS"
dnl     		POCO_CPPFLAGS=""
dnl     		POCO_LIBS=""
dnl        fi
dnl
  AC_SUBST(POCO_CPPFLAGS)
  AC_SUBST(POCO_LIBS)
])




# Check for Qt compiler flags, linker flags, and binary packages
AC_DEFUN([GONG_CHECK_QT5],
[
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PATH_X])

AC_MSG_CHECKING([QTDIR])
AC_ARG_WITH([qtdir], [  --with-qtdir=DIR        Qt installation directory [default=$QTDIR]], QTDIR=$withval)
# Check that QTDIR is defined or that --with-qtdir given
if test x"$QTDIR" = x ; then
	QTDIR=`qmake -query QT_INSTALL_PREFIX`
fi
if test x"$QTDIR" = x ; then
	# some usual Qt-locations
	QT_SEARCH="/usr /usr/lib/qt /usr/X11R6 /usr/local/Trolltech/Qt-4.0.0 /usr/local/Trolltech/Qt-4.0.1 /usr/local/Trolltech/Qt-4.1.0 /usr/local/Trolltech/Qt-4.2.0 /usr/local/Trolltech/Qt-4.2.1 /usr/local/Trolltech/Qt-4.2.2"
	for i in $QT_SEARCH ; do
		QT_INCLUDE_SEARCH="include/qt5 include"
		for j in $QT_INCLUDE_SEARCH ; do
				if test -f $i/$j/Qt/qglobal.h -a x$QTDIR = x ; then
				QTDIR=$i
				QT_INCLUDES=$i/$j
			fi
		done
	done
else
	QT_INCLUDES=`qmake -query QT_INSTALL_HEADERS`
fi
if test x"$QTDIR" = x ; then
	AC_MSG_ERROR([*** QTDIR must be defined, or --with-qtdir option given])
fi
AC_MSG_RESULT([$QTDIR])

AC_MSG_CHECKING([Qt includes])
# Check where includes are located
if test x"$QT_INCLUDES" = x ; then
	AC_MSG_ERROR([*** could not find Qt-includes! Make sure you have the Qt-devel-files installed!])
fi
AC_MSG_RESULT([$QT_INCLUDES])


if test -z "$QTHOSTDIR" ; then
	if test -n "$QTDIR" ; then
		QTHOSTDIR="$QTDIR"
	else
		QTHOSTDIR=/usr
	fi
fi

# Check that moc is in path
AC_CHECK_PROG(MOC, moc-qt5, $QTHOSTDIR/bin/moc-qt5,,$QTHOSTDIR/bin/)
if test x$MOC = x ; then
	AC_CHECK_PROG(MOC, moc, $QTHOSTDIR/bin/moc,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
        	AC_MSG_ERROR([*** not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

# Check that uic is in path
AC_CHECK_PROG(UIC, uic-qt5, $QTHOSTDIR/bin/uic-qt5,,$QTHOSTDIR/bin/)
if test x$UIC = x ; then
	AC_CHECK_PROG(UIC, uic, $QTHOSTDIR/bin/uic,,$QTHOSTDIR/bin/)
	if test x$UIC = x ; then
        	AC_MSG_ERROR([*** not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

# lupdate is the Qt translation-update utility.
AC_CHECK_PROG(LUPDATE, lupdate-qt5, $QTHOSTDIR/bin/lupdate-qt5,,$QTHOSTDIR/bin/)
if test x$LUPDATE = x ; then
	AC_CHECK_PROG(LUPDATE, lupdate, $QTHOSTDIR/bin/lupdate,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
	        AC_MSG_WARN([*** not found! It's not needed just for compiling but should be part of a proper Qt-devel-tools-installation!])
	fi
fi

# lrelease is the Qt translation-release utility.
AC_CHECK_PROG(LRELEASE, lrelease-qt5, $QTHOSTDIR/bin/lrelease-qt5,,$QTHOSTDIR/bin/)
if test x$LRELEASE = x ; then
	AC_CHECK_PROG(LRELEASE, lrelease, $QTHOSTDIR/bin/lrelease,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
	        AC_MSG_WARN([*** not found! It's not needed just for compiling but should be part of a proper Qt-devel-tools-installation!])
	fi
fi

# Calculate Qt include path
QT_CXXFLAGS="-DQT3_SUPPORT -I$QT_INCLUDES -I$QT_INCLUDES/Qt -I$QT_INCLUDES/QtGui -I$QT_INCLUDES/Qt3Support -I$QT_INCLUDES/QtCore"

# On unix, figure out if we're doing a static or dynamic link
# AC_MSG_CHECKING([if Qt is static])
# AC_MSG_RESULT([$QT_IS_STATIC])

QT_LIBS=`qmake -query QT_INSTALL_LIBS`
QT_LIBS="-L$QT_LIBS"

QT_TRANSLATIONS=`qmake -query QT_INSTALL_TRANSLATIONS`

QT_CXXFLAGS="$QT_CXXFLAGS -D_REENTRANT -DQT_NO_DEBUG -DQT_THREAD_SUPPORT"

QT_LDADD="$QT_LIBS -lX11 -lm -ldl -lQtSvg -lQtGui -lQtCore"

AC_MSG_CHECKING([QT_CXXFLAGS])
AC_MSG_RESULT([$QT_CXXFLAGS])
AC_MSG_CHECKING([QT_LDADD])
AC_MSG_RESULT([$QT_LDADD])

AC_SUBST(QT_CXXFLAGS)
AC_SUBST(QT_LDADD)
AC_SUBST(QT_TRANSLATIONS)

])


AC_DEFUN([GONG_CHECK_RTK],
[
	AC_ARG_WITH( [boost-spirit],
		        AS_HELP_STRING([--with-boost-spirit], [Support for formulae with Boost::Spirit (default is YES)]),
		        [ac_cv_use_boost_spirit=$withval],
		        [ac_cv_use_boost_spirit=yes] )
	AM_CONDITIONAL( [COMPILE_WITH_BOOST_SPIRIT], [ test "$ac_cv_use_boot_spirit" = yes ])
	if test "$ac_cv_use_boost_spirit" = yes ; then
		AX_BOOST_BASE(1.0)
		AC_DEFINE( [HAVE_BOOST_SPIRIT], [], [Define to 1 if you have the Boost::Spirit library.] )
	fi
	AC_ARG_WITH( [bdb],
		        AS_HELP_STRING([--with-bdb], [Support for the Berkeley Database (default is NO)]),
		        [ac_cv_use_boost_bdb=$withval],
		        [ac_cv_use_boost_bdb=no] )
	AM_CONDITIONAL( [COMPILE_WITH_BDB], [ test "$ac_cv_use_bdb" = yes ])
	if test "$ac_cv_use_boost_bdb" = yes ; then
			GONG_CHECK_BDB
			AC_DEFINE( [HAVE_BDB], [], [Define to 1 if you have the Berkeley Database library.] )
	fi
])


# Check for Qt3 compiler flags, linker flags, and binary packages
#
# Copyright (c) 2002, Geoffrey Wossum
# All rights reserved.
#
# http://autoqt.sourceforge.net
#
#   This macro calls:
#
#     AC_SUBST(QT3_CXXFLAGS) & AC_SUBST(QT3_LDADD) & AC_SUBST(QT3_DIR)
#
#   And sets:
#
#     HAVE_QT3
#     QT3_WIN32

AC_DEFUN([GONG_CHECK_QT3],
[
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PATH_X])

AC_MSG_CHECKING([QT3_DIR])
AC_ARG_WITH([qt3dir],
	AS_HELP_STRING([--with-qt3dir=DIR], [Qt installation directory [default=$QT3_DIR]]),
		QT3_DIR=$withval)

QT_SEARCH="$QT3_DIR /usr/qt/3 /usr/lib/qt3 /usr/lib/qt/3 /usr/local/qt3 /usr/include/qt3 /usr/lib/qt /usr/local/qt"
for i in $QT_SEARCH; do
	if test -f $i/include/qglobal.h; then
			QT3_DIR=$i ;
			QTINCLUDE=$i/include
			break
		fi
	if test -f $i/qglobal.h ; then
			QT3_DIR=$i ;
			QTINCLUDE=$i
			break
fi
done

dnl debian has libs and includes in diferent QT3_DIR
if test x"$QT3_DIR" = x ; then
    QT_SEARCH=`echo $QT_SEARCH | sed -e 's/lib/include/'`
    for i in $QT_SEARCH; do
        if test -f $i/qglobal.h -a x$QT3_DIR = x; then
                QT3_DIR=$i;
                QTINCLUDE=$i
        fi
        break
    done
fi
if test x"$QTINCLUDE" = x ; then
	AC_MSG_ERROR([[No se han detectado las cabeceras de desarrollo de la biblioteca Qt3. Si las has instalado en un lugar no estandard, usa la opción --with-qt3dir=[DIR] o define \$QT3_DIR. Para instalar las Qt3: (Mandriva: libqt3-devel) (Debian/K/Ubuntu: libqt3-mt-dev) ]])
fi
AC_MSG_RESULT([$QT3_DIR])

# Figure out which version of Qt we are using
AC_MSG_CHECKING([Qt version])
QT_VER=`grep 'define.*QT_VERSION_STR\W' $QTINCLUDE/qglobal.h | perl -p -e 's/\D//g'`
case "${QT_VER}" in
    2*)
        QT_MAJOR="2"
    ;;
    3*)
        QT_MAJOR="3"
    ;;
    *)
        AC_MSG_ERROR([*** Don't know how to handle this Qt major version: $QT_VER])
    ;;
esac
AC_MSG_RESULT([$QT_VER ($QT_MAJOR)])

# qembed is the Qt data embedding utility.
# It is located in $QT3_DIR/tools/qembed, and must be compiled and installed
# manually, we'll let it slide if it isn't present
AC_CHECK_PROG(QEMBED, qembed, qembed)

# Calculate Qt include path
QT3_CXXFLAGS="-I$QTINCLUDE"

QT_IS_EMBEDDED="no"
# On unix, figure out if we're doing a static or dynamic link
case "${host}" in
    *)
        QTLIBDIR=`echo $QT3_DIR | sed -e 's/include/lib/'`
        QTLIBDIRS="$QTLIBDIR/lib $QTLIBDIR /usr/local/lib /usr/lib"
        for qt_libdir in $QTLIBDIRS; do
                lsqtlibdir=`ls $qt_libdir/libqt.* 2> /dev/null`
                if test "x$lsqtlibdir" != x; then
                        QTLIBDIR=$qt_libdir
                        break
                fi
                lsqtlibdir=`ls $qt_libdir/libqt-* 2> /dev/null`
                if test "x$lsqtlibdir" != x; then
                        QTLIBDIR=$qt_libdir
                        break
                fi
        done

        QT_IS_STATIC=`ls $QTLIBDIR/libqt*.a 2> /dev/null`
        if test "x$QT_IS_STATIC" = x; then
				QT_IS_STATIC="no"
        else
				QT_IS_STATIC="yes"
        fi

        if test x$QT_IS_STATIC = xno ; then
                QT_IS_DYNAMIC=`ls $QTLIBDIR/libqt*.so 2> /dev/null`
                if test "x$QT_IS_DYNAMIC" = x;  then
                        AC_MSG_ERROR([*** Couldn't find any Qt libraries in $QTLIBDIR])
                fi
        fi

        if test "x`ls $QTLIBDIR/libqt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqt"
            QT_IS_MT="no"
        elif test "x`ls $QTLIBDIR/libqt-mt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqt-mt"
            QT_IS_MT="yes"
        elif test "x`ls $QTLIBDIR/libqte.* 2> /dev/null`" != x ; then
            QT_LIB="-lqte"
            QT_IS_MT="no"
            QT_IS_EMBEDDED="yes"
        elif test "x`ls $QTLIBDIR/libqte-mt.* 2> /dev/null`" != x ; then
            QT_LIB="-lqte-mt"
            QT_IS_MT="yes"
            QT_IS_EMBEDDED="yes"
        fi
        ;;
esac
AC_MSG_CHECKING([if Qt is static])
AC_MSG_RESULT([$QT_IS_STATIC])
AC_MSG_CHECKING([if Qt is multithreaded])
AC_MSG_RESULT([$QT_IS_MT])
AC_MSG_CHECKING([if Qt is embedded])
AC_MSG_RESULT([$QT_IS_EMBEDDED])

case "${host}" in
    *irix*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lX11 -lm -lSM -lICE"
        fi
        ;;

    *linux*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes && test $QT_IS_EMBEDDED = no; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lX11 -lm -lSM -lICE -ldl -ljpeg"
        fi
        ;;


    *osf*)
        # Digital Unix (aka DGUX aka Tru64)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lX11 -lm -lSM -lICE"
        fi
        ;;

    *solaris*)
        QT_LIBS="$QT_LIB"
        if test $QT_IS_STATIC = yes ; then
            QT_LIBS="$QT_LIBS -L$x_libraries -lX11 -lm -lSM -lICE -lresolv -lsocket -lnsl"
        fi
        ;;

esac


if test x"$QT_IS_EMBEDDED" = "xyes" ; then
        QT3_CXXFLAGS="-DQWS $QT3_CXXFLAGS"
fi

if test x"$QT_IS_MT" = "xyes" ; then
        QT3_CXXFLAGS="$QT3_CXXFLAGS -D_REENTRANT -DQT_THREAD_SUPPORT"
fi

QT3_LDADD="-L$QTLIBDIR $QT_LIBS"

if test x$QT_IS_STATIC = xyes ; then
    OLDLIBS="$LIBS"
    LIBS="$QT3_LDADD"
    AC_CHECK_LIB(Xft, XftFontOpen, QT3_LDADD="$QT3_LDADD -lXft")
    LIBS="$OLDLIBS"
fi

AC_MSG_CHECKING([QT3_CXXFLAGS])
AC_MSG_RESULT([$QT3_CXXFLAGS])
AC_MSG_CHECKING([QT3_LDADD])
AC_MSG_RESULT([$QT3_LDADD])

if test x"$QT3_CXXFLAGS" != x ; then
	AC_DEFINE( [HAVE_QT3], [1], [Define to 1 if you have the Qt3 development library.] )
fi

AC_SUBST(QT3_CXXFLAGS)
AC_SUBST(QT3_LDADD)
AC_SUBST(QT3_DIR)

])



#
#
# Check for Berkeley db v4 compiler and linker flags
#
#

AC_DEFUN([GONG_CHECK_BDB], [
	AC_MSG_CHECKING([Berkeley DB v4])
	AC_ARG_WITH([bdbdir],
		AS_HELP_STRING([--with-bdbdir=DIR], [Berkeley DB 4 installation directory [default=$BDBDIR]]),
			BDBDIR=$withval)

# Look for libraries
BDB_SEARCH="$BDBDIR/lib $BDB /usr/lib /usr/local/lib /usr/lib/i386-linux-gnu /usr/lib/x86_64-linux-gnu"
for bdb_libdir in $BDB_SEARCH; do
        if test "x`ls $bdb_libdir/libdb-5* 2>/dev/null`" != "x"; then
                BDBLIBDIR=$bdb_libdir;
                break
         else
           if test "x`ls $bdb_libdir/libdb-4* 2>/dev/null`" != "x"; then
                BDBLIBDIR=$bdb_libdir;
                break
         fi
        fi
done
if test x"$BDBLIBDIR" = "x" ; then
	AC_MSG_ERROR([[No se han detectado las bibliotecas de desarrollo de la Berkeley DB 4.5 o superior. Si las has instalado en un lugar no estandard, usa la opción --with-bdbdir=DIR. Para instalar la Berkeley DB 4: (Debian/K/Ubuntu: libdb4.5-devel) (Mandriva: libdb4.5-devel) ]])
fi

if test "x`ls $BDBLIBDIR/libdb.* 2> /dev/null`" != x ; then
        BDB_LIB="-ldb"
fi

BDB_LDADD="-L$BDBLIBDIR $BDB_LIB"

# Look for includes
BDB_SEARCH="$BDBDIR/include $BDBDIR /usr/include /usr/local/include"
for incdir in $BDB_SEARCH; do
        if test -f $incdir/db4/db.h;  then
                BDBINCDIR=$incdir/db4;
                break
        fi
done
if test x"$BDBINCDIR" = "x" ; then
	for incdir in $BDB_SEARCH; do
        	if test -f $incdir/db.h;  then
                	BDBINCDIR=$incdir;
                	break
        	fi
	done
fi
if test x"$BDBINCDIR" = "x" ; then
	AC_MSG_ERROR([[No se han detectado las cabeceras de las bibliotecas de desarrollo de la Berkeley DB 4.5 o superior. Si las has instalado en un lugar no estandard, usa la opción --with-bdbdir=DIR. Para instalar la Berkeley DB 4: (Debian/K/Ubuntu: libdb4.5-devel) (Mandriva: libdb4.5-devel) ]])
fi

BDB_CPPFLAGS="-I$BDBINCDIR"
AC_MSG_RESULT([$BDB_CPPFLAGS $BDB_LDADD])

AC_SUBST(BDB_CPPFLAGS)
AC_SUBST(BDB_LDADD)

])


#
# GONG_CHECK_MYSQL([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for MYSQL, AC_SUBST(MYSQL_CPPFLAGS and MYSQL_LIBS) and AC_DEFINE(HAVE_MYSQL)
# Santiago Capel 2005-09-25

AC_DEFUN([GONG_CHECK_MYSQL], [
	AC_ARG_WITH([mysql-prefix],
            [  --with-mysql-prefix=PFX   Prefix where libmysqlclient is installed (optional)],
            mysql_config_prefix="$withval", mysql_config_prefix="")
	AC_ARG_WITH([mysql-exec-prefix],
            [  --with-mysql-exec-prefix=PFX Exec prefix where mysql_config is installed (optional)],
            mysql_config_exec_prefix="$withval", mysql_config_exec_prefix="")
	AC_DEFINE( [HAVE_MYSQL], [], [Define to 1 if you have the MySQL client library.] )

  if test x$mysql_config_exec_prefix != x ; then
     mysql_config_args="$mysql_config_args"
     if test x${MYSQL_CONFIG+set} != xset ; then
        MYSQL_CONFIG=$mysql_config_exec_prefix/mysql_config
     fi
  fi
  if test x$mysql_config_prefix != x ; then
     mysql_config_args="$mysql_config_args --prefix=$mysql_config_prefix"
     if test x${MYSQL_CONFIG+set} != xset ; then
        MYSQL_CONFIG=$mysql_config_prefix/mysql_config
     fi
  fi

  AC_PATH_PROG(MYSQL_CONFIG, mysql_config, no)
  min_mysql_version=ifelse([$1], ,3.2.0,[$1])
  AC_MSG_CHECKING(for libmysql - version >= $min_mysql_version)
  no_MYSQL=""
  if test "$MYSQL_CONFIG" = "no" ; then
    no_MYSQL=yes
  else
    MYSQL_CPPFLAGS=`$MYSQL_CONFIG $mysql_config_args --include`
    MYSQL_LIBS=`$MYSQL_CONFIG $mysql_config_args --libs`
    mysql_config_major_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    mysql_config_minor_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    mysql_config_micro_version=`$MYSQL_CONFIG $mysql_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  fi
  if test "x$no_MYSQL" = x ; then
     AC_MSG_RESULT(yes (version $mysql_config_major_version.$mysql_config_minor_version.$mysql_config_micro_version))
     ifelse([$2], , :, [$2])
  else
     AC_MSG_RESULT(no)
     if test "$MYSQL_CONFIG" = "no" ; then
       echo "*** The mysql-config script installed by MYSQL could not be found"
       echo "*** If libMYSQL was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the MYSQL_CONFIG environment variable to the"
       echo "*** full path to mysql-config."
     else
       if test -f conf.mysqltest ; then
        :
       else
          echo "*** Could not run libMYSQL test program, checking why..."
          CPPFLAGS="$CPPFLAGS $MYSQL_CPPFLAGS"
          LIBS="$LIBS $MYSQL_LIBS"
          AC_TRY_LINK([
#include <mysql/mysql.h>
#include <stdio.h>
],      [ LIBMYSQL_TEST_VERSION; return 0;],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding LIBMYSQL or finding the wrong"
          echo "*** version of LIBMYSQL. If it is not finding LIBMYSQL, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
          echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means LIBMYSQL was incorrectly installed"
          echo "*** or that you have moved LIBMYSQL since it was installed. In the latter case, you"
          echo "*** may want to edit the mysql-config script: $MYSQL_CONFIG" ])
          CPPFLAGS="$ac_save_CPPFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi

     MYSQL_CPPFLAGS=""
     MYSQL_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(MYSQL_CPPFLAGS)
  AC_SUBST(MYSQL_LIBS)
  rm -f conf.mysqltest
])

#
#
#
#
# Check posgresql dirs
# Taken from asterisk
#
#
#
#

AC_DEFUN([GONG_CHECK_POSTGRESQL], [

AC_ARG_WITH([pq],
	AC_HELP_STRING([--with-pq=PATH],[use PostgreSQL files in PATH]) )
case ${withval} in
     n|no)
     USE_PQ="no"
     ;;
     y|ye|yes)
     PQ_MANDATORY="yes"
	 USE_PQ="yes"
     ;;
     *)
     PQ_DIR="${withval}"
     PQ_MANDATORY="yes"
     ;;
esac

PG_CONFIG=No
if test "${USE_PQ}" != "no"; then
   if test "x${PQ_DIR}" != "x"; then
      AC_PATH_TOOL([PG_CONFIG], [pg_config], No, [${PQ_DIR}/bin])
      if test x"${PG_CONFIG}" = xNo; then
         echo "***"
         echo "*** pg_config was not found in the path you specified:"
         echo "*** ${PQ_DIR}/bin"
         echo "*** Either correct the installation, or run configure"
         echo "*** including --without-pq"
         exit 1
      fi
   else
      AC_PATH_TOOL([PG_CONFIG], [pg_config], No)
   fi
fi

if test x"${PG_CONFIG}" != xNo; then
   PQ_libdir=`pg_config --libdir`
   PQ_includedir=`pg_config --includedir`

	AC_CHECK_LIB([pq], [PQexec], [ac_cv_lib_pq_PQexec = "yes"] )

   if test "${ac_cv_lib_pq_PQexec}" = "yes"; then
      PQ_LDADD="-L${PQ_libdir} -lpq -lz"
      PQ_CPPFLAGS="-I${PQ_includedir}"
	  AC_DEFINE( [HAVE_PGSQL], [1], [Define to 1 if you have the PostGreSQL client library.] )
   elif test ! -z "${PQ_MANDATORY}";
   then
      echo "***"
      echo "*** The PostgreSQL installation on this system appears to be broken."
      echo "*** Either correct the installation, or run configure"
      echo "*** including --without-pq"
      exit 1
   fi
fi

AC_SUBST(PQ_CPPFLAGS)
AC_SUBST(PQ_LDADD)

])

##### http://autoconf-archive.cryp.to/ax_boost_base.html
#
# SYNOPSIS
#
#   AX_BOOST_BASE([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the Boost C++ libraries of a particular version (or newer)
#
#   If no path to the installed boost library is given the macro
#   searchs under /usr, /usr/local, /opt and /opt/local and evaluates
#   the $BOOST_ROOT environment variable. Further documentation is
#   available at <http://randspringer.de/boost/index.html>.
#
#   This macro calls:
#
#     AC_SUBST(BOOST_CPPFLAGS) / AC_SUBST(BOOST_LDFLAGS)
#
#   And sets:
#
#     HAVE_BOOST
#
# LAST MODIFICATION
#
#   2007-07-28
#
# COPYLEFT
#
#   Copyright (c) 2007 Thomas Porschberg <thomas@randspringer.de>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_BOOST_BASE],
[
AC_ARG_WITH([boost],
	AS_HELP_STRING([--with-boost@<:@=DIR@:>@], [use boost (default is yes) - it is possible to specify the root directory for boost (optional)]),
	[
    if test "$withval" = "no"; then
		want_boost="no"
    elif test "$withval" = "yes"; then
        want_boost="yes"
        ac_boost_path=""
    else
	    want_boost="yes"
        ac_boost_path="$withval"
	fi
    ],
    [want_boost="yes"])

if test "x$want_boost" = "xyes"; then
	boost_lib_version_req=ifelse([$1], ,1.20.0,$1)
	boost_lib_version_req_shorten=`expr $boost_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
	boost_lib_version_req_major=`expr $boost_lib_version_req : '\([[0-9]]*\)'`
	boost_lib_version_req_minor=`expr $boost_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
	boost_lib_version_req_sub_minor=`expr $boost_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
	if test "x$boost_lib_version_req_sub_minor" = "x" ; then
		boost_lib_version_req_sub_minor="0"
    	fi
	WANT_BOOST_VERSION=`expr $boost_lib_version_req_major \* 100000 \+  $boost_lib_version_req_minor \* 100 \+ $boost_lib_version_req_sub_minor`
	AC_MSG_CHECKING(for boostlib >= $boost_lib_version_req)
	succeeded=no

	dnl first we check the system location for boost libraries
	dnl this location ist chosen if boost libraries are installed with the --layout=system option
	dnl or if you install boost with RPM
	if test "$ac_boost_path" != ""; then
		BOOST_LDFLAGS="-L$ac_boost_path/lib"
		BOOST_CPPFLAGS="-I$ac_boost_path/include"
	else
		for ac_boost_path_tmp in /usr /usr/local /opt /opt/local ; do
			if test -d "$ac_boost_path_tmp/include/boost" && test -r "$ac_boost_path_tmp/include/boost"; then
				BOOST_LDFLAGS="-L$ac_boost_path_tmp/lib"
				BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
				break;
			fi
		done
	fi

	CPPFLAGS_SAVED="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
	export CPPFLAGS

	LDFLAGS_SAVED="$LDFLAGS"
	LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
	export LDFLAGS

	AC_LANG_PUSH(C++)
     	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
	@%:@include <boost/version.hpp>
	]], [[
	#if BOOST_VERSION >= $WANT_BOOST_VERSION
	// Everything is okay
	#else
	#  error Boost version is too old
	#endif
	]])],[
        AC_MSG_RESULT(yes)
	succeeded=yes
	found_system=yes
       	],[
       	])
	AC_LANG_POP([C++])



	dnl if we found no boost with system layout we search for boost libraries
	dnl built and installed without the --layout=system option or for a staged(not installed) version
	if test "x$succeeded" != "xyes"; then
		_version=0
		if test "$ac_boost_path" != ""; then
               		BOOST_LDFLAGS="-L$ac_boost_path/lib"
			if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
				for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
					_version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
					V_CHECK=`expr $_version_tmp \> $_version`
					if test "$V_CHECK" = "1" ; then
						_version=$_version_tmp
					fi
					VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
					BOOST_CPPFLAGS="-I$ac_boost_path/include/boost-$VERSION_UNDERSCORE"
				done
			fi
		else
			for ac_boost_path in /usr /usr/local /opt /opt/local ; do
				if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
					for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
						_version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
						V_CHECK=`expr $_version_tmp \> $_version`
						if test "$V_CHECK" = "1" ; then
							_version=$_version_tmp
	               					best_path=$ac_boost_path
						fi
					done
				fi
			done

			VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
			BOOST_CPPFLAGS="-I$best_path/include/boost-$VERSION_UNDERSCORE"
			BOOST_LDFLAGS="-L$best_path/lib"

	    		if test "x$BOOST_ROOT" != "x"; then
				if test -d "$BOOST_ROOT" && test -r "$BOOST_ROOT" && test -d "$BOOST_ROOT/stage/lib" && test -r "$BOOST_ROOT/stage/lib"; then
					version_dir=`expr //$BOOST_ROOT : '.*/\(.*\)'`
					stage_version=`echo $version_dir | sed 's/boost_//' | sed 's/_/./g'`
			        	stage_version_shorten=`expr $stage_version : '\([[0-9]]*\.[[0-9]]*\)'`
					V_CHECK=`expr $stage_version_shorten \>\= $_version`
				        if test "$V_CHECK" = "1" ; then
						AC_MSG_NOTICE(We will use a staged boost library from $BOOST_ROOT)
						BOOST_CPPFLAGS="-I$BOOST_ROOT"
						BOOST_LDFLAGS="-L$BOOST_ROOT/stage/lib"
					fi
				fi
	    		fi
		fi

		CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
		export CPPFLAGS
		LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
		export LDFLAGS

		AC_LANG_PUSH(C++)
	     	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
		@%:@include <boost/version.hpp>
		]], [[
		#if BOOST_VERSION >= $WANT_BOOST_VERSION
		// Everything is okay
		#else
		#  error Boost version is too old
		#endif
		]])],[
        	AC_MSG_RESULT(yes)
		succeeded=yes
		found_system=yes
       		],[
	       	])
		AC_LANG_POP([C++])
	fi

	if test "$succeeded" != "yes" ; then
		if test "$_version" = "0" ; then
			AC_MSG_ERROR([[No se han detectado las bibliotecas de boost (version $boost_lib_version_req_shorten o superior). Utiliza --without-boost para no inclirlas. Si las has instalado en un lugar no estandard, usa la opción --with-boost[=DIR]. Si estás segura de que tienes boost instalada, comprueba la versión mirando en <boost/version.hpp>. Mira en http://randspringer.de/boost para documentación extra.  Para instalar boost: (Debian/K/Ubuntu: libboost-dev) (Mandriva: libboost1-dev)  ]])
			AC_MSG_ERROR([[We could not detect the boost libraries (version $boost_lib_version_req_shorten or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
		else
			AC_MSG_NOTICE([Your boost libraries seems to old (version $_version).])
		fi
	else
		AC_SUBST(BOOST_CPPFLAGS)
		AC_SUBST(BOOST_LDFLAGS)
		AC_DEFINE(HAVE_BOOST,,[define if the Boost library is available])
	fi

        CPPFLAGS="$CPPFLAGS_SAVED"
       	LDFLAGS="$LDFLAGS_SAVED"
fi

])



# Check for Qt compiler flags, linker flags, and binary packages
AC_DEFUN([GONG_CHECK_QT4],
[
AC_REQUIRE([AC_PROG_CXX])
AC_REQUIRE([AC_PATH_X])

AC_MSG_CHECKING([QTDIR])
AC_ARG_WITH([qtdir], [  --with-qtdir=DIR        Qt installation directory [default=$QTDIR]], QTDIR=$withval)
# Check that QTDIR is defined or that --with-qtdir given
if test x"$QTDIR" = x ; then
	QTDIR=`qmake -query QT_INSTALL_PREFIX`
fi
if test x"$QTDIR" = x ; then
	# some usual Qt-locations
	QT_SEARCH="/usr /usr/lib/qt /usr/X11R6 /usr/local/Trolltech/Qt-4.0.0 /usr/local/Trolltech/Qt-4.0.1 /usr/local/Trolltech/Qt-4.1.0 /usr/local/Trolltech/Qt-4.2.0 /usr/local/Trolltech/Qt-4.2.1 /usr/local/Trolltech/Qt-4.2.2"
	for i in $QT_SEARCH ; do
		QT_INCLUDE_SEARCH="include/qt4 include"
		for j in $QT_INCLUDE_SEARCH ; do
				if test -f $i/$j/Qt/qglobal.h -a x$QTDIR = x ; then
				QTDIR=$i
				QT_INCLUDES=$i/$j
			fi
		done
	done
else
	QT_INCLUDES=`qmake -query QT_INSTALL_HEADERS`
fi
if test x"$QTDIR" = x ; then
	AC_MSG_ERROR([*** QTDIR must be defined, or --with-qtdir option given])
fi
AC_MSG_RESULT([$QTDIR])

AC_MSG_CHECKING([Qt includes])
# Check where includes are located
if test x"$QT_INCLUDES" = x ; then
	AC_MSG_ERROR([*** could not find Qt-includes! Make sure you have the Qt-devel-files installed!])
fi
AC_MSG_RESULT([$QT_INCLUDES])


if test -z "$QTHOSTDIR" ; then
	if test -n "$QTDIR" ; then
		QTHOSTDIR="$QTDIR"
	else
		QTHOSTDIR=/usr
	fi
fi

# Check that moc is in path
AC_CHECK_PROG(MOC, moc-qt4, $QTHOSTDIR/bin/moc-qt4,,$QTHOSTDIR/bin/)
if test x$MOC = x ; then
	AC_CHECK_PROG(MOC, moc, $QTHOSTDIR/bin/moc,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
        	AC_MSG_ERROR([*** not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

# Check that uic is in path
AC_CHECK_PROG(UIC, uic-qt4, $QTHOSTDIR/bin/uic-qt4,,$QTHOSTDIR/bin/)
if test x$UIC = x ; then
	AC_CHECK_PROG(UIC, uic, $QTHOSTDIR/bin/uic,,$QTHOSTDIR/bin/)
	if test x$UIC = x ; then
        	AC_MSG_ERROR([*** not found! Make sure you have Qt-devel-tools installed!])
	fi
fi

# lupdate is the Qt translation-update utility.
AC_CHECK_PROG(LUPDATE, lupdate-qt4, $QTHOSTDIR/bin/lupdate-qt4,,$QTHOSTDIR/bin/)
if test x$LUPDATE = x ; then
	AC_CHECK_PROG(LUPDATE, lupdate, $QTHOSTDIR/bin/lupdate,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
	        AC_MSG_WARN([*** not found! It's not needed just for compiling but should be part of a proper Qt-devel-tools-installation!])
	fi
fi

# lrelease is the Qt translation-release utility.
AC_CHECK_PROG(LRELEASE, lrelease-qt4, $QTHOSTDIR/bin/lrelease-qt4,,$QTHOSTDIR/bin/)
if test x$LRELEASE = x ; then
	AC_CHECK_PROG(LRELEASE, lrelease, $QTHOSTDIR/bin/lrelease,,$QTHOSTDIR/bin/)
	if test x$MOC = x ; then
	        AC_MSG_WARN([*** not found! It's not needed just for compiling but should be part of a proper Qt-devel-tools-installation!])
	fi
fi

# Calculate Qt include path
QT_CXXFLAGS="-DQT3_SUPPORT -I$QT_INCLUDES -I$QT_INCLUDES/Qt -I$QT_INCLUDES/QtGui -I$QT_INCLUDES/Qt3Support -I$QT_INCLUDES/QtCore"

# On unix, figure out if we're doing a static or dynamic link
# AC_MSG_CHECKING([if Qt is static])
# AC_MSG_RESULT([$QT_IS_STATIC])

QT_LIBS=`qmake -query QT_INSTALL_LIBS`
QT_LIBS="-L$QT_LIBS"

QT_TRANSLATIONS=`qmake -query QT_INSTALL_TRANSLATIONS`

QT_CXXFLAGS="$QT_CXXFLAGS -D_REENTRANT -DQT_NO_DEBUG -DQT_THREAD_SUPPORT"

QT_LDADD="$QT_LIBS -lX11 -lm -ldl -lQtSvg -lQtGui -lQtCore"

AC_MSG_CHECKING([QT_CXXFLAGS])
AC_MSG_RESULT([$QT_CXXFLAGS])
AC_MSG_CHECKING([QT_LDADD])
AC_MSG_RESULT([$QT_LDADD])

AC_SUBST(QT_CXXFLAGS)
AC_SUBST(QT_LDADD)
AC_SUBST(QT_TRANSLATIONS)

])


# ===========================================================================
#      http://www.gnu.org/software/autoconf-archive/ax_lib_sqlite3.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_LIB_SQLITE3([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the SQLite 3 library of a particular version (or newer)
#
#   This macro takes only one optional argument, required version of SQLite
#   3 library. If required version is not passed, 3.0.0 is used in the test
#   of existance of SQLite 3.
#
#   If no intallation prefix to the installed SQLite library is given the
#   macro searches under /usr, /usr/local, and /opt.
#
#   This macro calls:
#
#     AC_SUBST(SQLITE3_CFLAGS)
#     AC_SUBST(SQLITE3_LDFLAGS)
#     AC_SUBST(SQLITE3_VERSION)
#
#   And sets:
#
#     HAVE_SQLITE3
#
# LICENSE
#
#   Copyright (c) 2008 Mateusz Loskot <mateusz@loskot.net>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

AC_DEFUN([AX_LIB_SQLITE3],
[
    AC_ARG_WITH([sqlite3],
        AS_HELP_STRING(
            [--with-sqlite3=@<:@ARG@:>@],
            [use SQLite 3 library @<:@default=no@:>@, optionally specify the prefix for sqlite3 library]
        ),
        [
        if test "$withval" = "no"; then
            WANT_SQLITE3="no"
        elif test "$withval" = "yes"; then
            WANT_SQLITE3="yes"
            ac_sqlite3_path=""
        else
            WANT_SQLITE3="yes"
            ac_sqlite3_path="$withval"
        fi
        ],
        [WANT_SQLITE3="yes"]
    )

    SQLITE3_CFLAGS=""
    SQLITE3_LDFLAGS=""
    SQLITE3_VERSION=""

    if test "x$WANT_SQLITE3" = "xyes"; then

        ac_sqlite3_header="sqlite3.h"

        sqlite3_version_req=ifelse([$1], [], [3.0.0], [$1])
        sqlite3_version_req_shorten=`expr $sqlite3_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
        sqlite3_version_req_major=`expr $sqlite3_version_req : '\([[0-9]]*\)'`
        sqlite3_version_req_minor=`expr $sqlite3_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
        sqlite3_version_req_micro=`expr $sqlite3_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
        if test "x$sqlite3_version_req_micro" = "x" ; then
            sqlite3_version_req_micro="0"
        fi

        sqlite3_version_req_number=`expr $sqlite3_version_req_major \* 1000000 \
                                   \+ $sqlite3_version_req_minor \* 1000 \
                                   \+ $sqlite3_version_req_micro`

        AC_MSG_CHECKING([for SQLite3 library >= $sqlite3_version_req])

        if test "$ac_sqlite3_path" != ""; then
            ac_sqlite3_ldflags="-L$ac_sqlite3_path/lib"
            ac_sqlite3_cppflags="-I$ac_sqlite3_path/include"
        else
            for ac_sqlite3_path_tmp in /usr /usr/local /opt ; do
                if test -f "$ac_sqlite3_path_tmp/include/$ac_sqlite3_header" \
                    && test -r "$ac_sqlite3_path_tmp/include/$ac_sqlite3_header"; then
                    ac_sqlite3_path=$ac_sqlite3_path_tmp
                    ac_sqlite3_cppflags="-I$ac_sqlite3_path_tmp/include"
                    ac_sqlite3_ldflags="-L$ac_sqlite3_path_tmp/lib"
                    break;
                fi
            done
        fi

        ac_sqlite3_ldflags="$ac_sqlite3_ldflags -lsqlite3"

        saved_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS $ac_sqlite3_cppflags"

        AC_LANG_PUSH(C)
        AC_COMPILE_IFELSE(
            [
            AC_LANG_PROGRAM([[@%:@include <sqlite3.h>]],
                [[
#if (SQLITE_VERSION_NUMBER >= $sqlite3_version_req_number)
/* Everything is okay */
#else
#  error SQLite version is too old
#endif
                ]]
            )
            ],
            [
            AC_MSG_RESULT([yes])
            success="yes"
            ],
            [
            AC_MSG_RESULT([not found])
            success="no"
            ]
        )
        AC_LANG_POP(C)

        CPPFLAGS="$saved_CPPFLAGS"

        if test "$success" = "yes"; then

            SQLITE3_CFLAGS="$ac_sqlite3_cppflags"
            SQLITE3_LDFLAGS="$ac_sqlite3_ldflags"

            ac_sqlite3_header_path="$ac_sqlite3_path/include/$ac_sqlite3_header"

            dnl Retrieve SQLite release version
            if test "x$ac_sqlite3_header_path" != "x"; then
                ac_sqlite3_version=`cat $ac_sqlite3_header_path \
                    | grep '#define.*SQLITE_VERSION.*\"' | sed -e 's/.* "//' \
                        | sed -e 's/"//'`
                if test $ac_sqlite3_version != ""; then
                    SQLITE3_VERSION=$ac_sqlite3_version
                else
                    AC_MSG_WARN([Cannot find SQLITE_VERSION macro in sqlite3.h header to retrieve SQLite version!])
                fi
            fi

            AC_SUBST(SQLITE3_CFLAGS)
            AC_SUBST(SQLITE3_LDFLAGS)
            AC_SUBST(SQLITE3_VERSION)
            AC_DEFINE([HAVE_SQLITE3], [], [Have the SQLITE3 library])
        fi
    fi
])
