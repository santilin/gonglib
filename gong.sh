#!/bin/bash
GONGLIB_VERSION=0.1

if [[ $EUID -eq 0 ]]; then
  echo "Este script no se puede ejecutar como root" 1>&2
  exit 1
fi

# si gong.sh es un enlace simbólico, leerlo
ISLINK=`readlink $0`
if test "x$ISLINK" == "x"; then
	GONGDIR=$(dirname $0)
else
	SCRIPTDIR=$(dirname $(readlink $0) )
	GONGDIR="$(dirname $0)/$SCRIPTDIR"
fi
GONGDIR=$(cd $GONGDIR; pwd)
GONGMODULESDIR=$GONGDIR
CAPEL="$GONGDIR/capel/Debug/src/capel"

COMANDO=$1
shift

# check_in_project PROJECT
# -----------------------
# Comprueba si estamos dentro del directorio $PROYECT.
check_in_project ()
{
	PROYECTO=$1
	if test "x$PROYECTO" == "x"; then
		echo "Se necesita el nombre del proyecto"
		exit 1
	fi
	CURDIR=`basename $PWD`
	if test $PROYECTO==$CURDIR; then
		return 1
	else
		return 0
	fi
}

do_cleanup()
{
	PROYECTO=$1
	shift
	if test "x$PROYECTO" == "x"; then
		echo "Se necesita el nombre del proyecto"
		exit 1
	fi
	_PWD=`pwd`
	_CURDIR=`basename $_PWD`
	if test $PROYECTO == $_CURDIR; then
		# Ficheros generados por make -f Makefile.csv y configure
		rm Makefile.in aclocal.m4 compile config.guess config.h.in config.log config.sub configure depcomp install-sh ltmain.sh missing src/Makefile.in
		rm -rf autom4te.cache
		rm m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4
		find . -iname "Makefile.in" -o -iname "Makefile" | xargs rm -fv

		find . -iname "Debug" -a -type d | xargs rm -rf
		find . -iname "Release" -a -type d | xargs rm -rf
		find . -iname "bin" -a -type d | xargs rm -rf
		find . -iname ".libs" -a -type d | xargs rm -rf
		find . -iname ".deps" -a -type d | xargs rm -rf
		find . -iname ".kdev4" -a -type d | xargs rm -rf

		rm -rf debianpackage
		rm -rf .anjuta .anjuta_sym_db.db TODO.tasks
		find . -iname "*.kdevses" -o -iname "*.kdevelop.pcs" | xargs rm -fv

		rm po/POTFILES tags

		find . -iname "*~" -o -iname "*#" -o -iname "*.tmp" -o -iname "*.tag" -o -iname "*.o" -o -iname "*.a" \
			-o -iname "*.bak" -o -iname "\.#*" -o -iname "*.gmo" -o -iname "*.lo" -o -iname "*la" | xargs rm -fv
		find . -iname "moc_*.cpp" | xargs rm -fv
		echo "Proyecto $PROYECTO limpiado"
	else
		echo "Tienes que estar en el directorio $PROYECTO para limpiarlo"
		exit 1
	fi
}

do_capel()
{
	FILENAME=$1
	shift
	if test "x$FILENAME" == "x"; then
		echo "Introduzca el nombre del archivo o del directorio sobre el que actuar"
		return 1;
	fi
	FILES=
	if test -d $FILENAME; then
		# añadir -L al find para que entre en los enlaces simbólicos
		FILES=$(find $FILENAME -name "*.cpp" -o -name "*.rtk" | xargs grep -l MODULE_INFO)
	else
		if test -f $FILENAME; then
			cd $(dirname $FILENAME)
			FILES=$(basename $FILENAME)
		fi
	fi
	if test "x$FILES" == "x"; then
		echo "No hay ficheros sobre los que actuar"
		return 1
	else
		for file_cpp in $FILES; do
			echo "Ejecutando $CAPEL $file_cpp"
			$CAPEL $file_cpp
		done
	fi
}

do_init()
{
	cd $GONGDIR/capel
	do_cleanup capel
	make -f Makefile.cvs || exit $?
	./configure || exit $?
	make
	cd -
}

create_links()
{
	PWD_SAVE=`pwd`
	echo $PWD_SAVE
	rm acinclude.m4
	ln -s $GONGDIR/acinclude.m4 .
	rmret=$(rm m4)
	if [[ ! -z "$rmret" ]]; then
		exit
	fi
	ln -s $GONGDIR/m4
	rm gonglib
	ln -s $GONGDIR/mod/gonglib gonglib
	mkdir share 2>/dev/null
	cd share
	rm gestiong
	ln -s $GONGDIR/share/ gestiong
	cd ..
	MODULES=$(grep -i "dnl module " configure.ac | cut -d " " -f 3)
	for mod in $MODULES; do
		m=`echo $mod | tr '[:upper:]' '[:lower:]'`
		rm $m
		case $m in
			tpv|deposit|editorial|ventasweb|ofertas)
				echo "Creating link $m as $GONGDIR/../gestiong-priv/mod/$m"
				ln -s $GONGDIR/../gestiong-priv/mod/$m $m
				;;
		    *) 
				echo "Creating link $m as $GONGDIR/mod/$m"
				ln -s $GONGDIR/mod/$m $m
				;;
		esac
	done
	cd $PWD_SAVE
}

update_subdirs()
{
	MODULES=$(grep -i "dnl module" configure.ac | cut -d " " -f 3)
	SUBDIRS="gonglib"
	AC_CONFIG_FILES="Makefile gonglib/Makefile"
	for mod in $MODULES; do
		m=`echo $mod | tr '[:upper:]' '[:lower:]'`
		SUBDIRS="$SUBDIRS $m"
		AC_CONFIG_FILES="$AC_CONFIG_FILES $m/Makefile"
	done
	echo "Updating AC_CONFIG_FILES in configure.ac"
	sed -e "syAC_CONFIG_FILES\s*(.*yAC_CONFIG_FILES( $AC_CONFIG_FILES \\\\y" -i configure.ac
	echo "Updating SUBDIRS in Makefile.am"
	sed -e "s/SUBDIRS\s*=.*/SUBDIRS=$SUBDIRS \\\\/" -i Makefile.am
}

case $COMANDO in

init)
	do_init
;;

create_app)
	PROYECTO=$1
	LOWER_PROYECTO=`echo $PROYECTO | tr '[:upper:]' '[:lower:]'`
	shift
	if test "x$PROYECTO" == "x"; then
		echo "Falta el nombre del proyecto"
		exit 1
	fi
	MODULE=$2
	if test "x$MODULE" == "x"; then
		echo "Faltan los módulos a incluir. Estos son los módulos disponibles:"
		find $GONGDIR/mod -maxdepth 1 -a -type d -printf "%f\n"| sort
		exit 1;
	fi
	if check_in_project $PROYECTO ; then :
	else
		echo "** Creando directorio: $PROYECTO"
		mkdir $LOWER_PROYECTO
		cd $LOWER_PROYECTO
	fi
	if test -f configure.ac; then
		echo "Ya existe el fichero configure.ac, no se hace nada"
	else
		echo "** Creando: configure.ac"
		echo "dnl <<<<<MODULE_INFO" > configure.ac
		echo "dnl MODULE rtk GONG_CHECK_RTK" >> configure.ac
		echo "dnl MODULE qt4 GONG_CHECK_QT4" >> configure.ac
		echo "dnl MODULE dbapp" >> configure.ac
		for MODULE in $*; do
			echo "dnl MODULE $MODULE" >> configure.ac
		done
		echo "dnl TYPE Project $PROYECTO" >> configure.ac
		echo "dnl >>>>>MODULE_INFO" >> configure.ac
		echo "** Ejecutando: capel configure.ac"
		if $CAPEL configure.ac; then
			echo "        src/Makefile )" >> configure.ac
			echo "        src" >> Makefile.am
			if $CAPEL src/${LOWER_PROYECTO}module.cpp; then
				echo "El proyecto $PROYECTO ha sido creado."
				echo "Ejecute $GONGDIR/gong.sh init_project en el directorio base del proyecto $PROYECTO."
			fi
		fi
	fi
;;

create_module)
	MODULE=$1
	shift
	if test "x$MODULE" == "x"; then
		echo "Falta el nombre del módulo"
		exit 1
	fi
	LOWER_MODULE=`echo $MODULE | tr '[:upper:]' '[:lower:]'`
	UPPER_MODULE=`echo $MODULE | tr '[:lower:]' '[:upper:]'`
	if check_in_project $LOWER_MODULE ; then :
	else
		echo "Creando directorio $LOWER_MODULE"
		mkdir $LOWER_MODULE
		cd $LOWER_MODULE
	fi
	if test -f Makefile.am; then
		echo "Makefile.am ya existe"
	else
		echo "Creando Makefile.am"
		echo "INCLUDES = @GONGLIB_CPPFLAGS@" > Makefile.am
		echo "AM_CPPFLAGS = @AM_CPPFLAGS@" >> Makefile.am
		echo "AM_CXXFLAGS = @AM_CXXFLAGS@" >> Makefile.am
		echo >> Makefile.am
		echo "lib_LTLIBRARIES = libgong$LOWER_MODULE.la" >> Makefile.am
		echo "libgong${LOWER_MODULE}_ladir = \$(includedir)/gonglib" >> Makefile.am
		echo "libgong${LOWER_MODULE}_la_SOURCES = ${LOWER_MODULE}module.cpp" >> Makefile.am
		echo "libgong${LOWER_MODULE}_la_HEADERS = ${LOWER_MODULE}module.h" >> Makefile.am
		echo "nodist_libgong${LOWER_MODULE}_la_SOURCES = moc_${LOWER_MODULE}module.cpp" >> Makefile.am
		echo >> Makefile.am
		echo "moc_%.cpp: %.h" >> Makefile.am
		echo -e '\tmoc $< -o \$@' >> Makefile.am
		echo >> Makefile.am

		echo "Creando mastertable.h"
		echo "#ifndef ${UPPER_MODULE}MASTERTABLE_H" >> ${LOWER_MODULE}mastertable.h
		echo "#define ${UPPER_MODULE}MASTERTABLE_H" >> ${LOWER_MODULE}mastertable.h
		echo "" >> ${LOWER_MODULE}mastertable.h
		echo "namespace gong {" >> ${LOWER_MODULE}mastertable.h
		echo "namespace ${LOWER_MODULE} {" >> ${LOWER_MODULE}mastertable.h
		echo "" >> ${LOWER_MODULE}mastertable.h
		echo "} // namespace ${LOWER_MODULE}" >> ${LOWER_MODULE}mastertable.h
		echo "} // namespace gong" >> ${LOWER_MODULE}mastertable.h
		echo "" >> ${LOWER_MODULE}mastertable.h
		echo "#endif // ${UPPER_MODULE}MASTERTABLE_H" >> ${LOWER_MODULE}mastertable.h

		echo "/*<<<<<MODULE_INFO*/" > ${LOWER_MODULE}module.cpp
		echo "// COPYLEFT Module ${LOWER_MODULE}" >> ${LOWER_MODULE}module.cpp
		echo "// TYPE GongModule ${LOWER_MODULE}::${MODULE}Module" >> ${LOWER_MODULE}module.cpp
		echo "/*>>>>>MODULE_INFO*/" >> ${LOWER_MODULE}module.cpp

		echo "Añada registros al módulo con ./gong.sh add_record ${LOWER_MODULE} RecordName ${LOWER_MODULE}"
		echo "Ejecute ./gong.sh capel ${LOWER_MODULE} para generar código para el módulo"
		echo "Añada dnl // Module ${LOWER_MODULE} a la extrusión MODULE_INFO del configure.ac principal de la aplicación para usar este módulo"
	fi
;;


add_record)
	NAMESPACE=$1
	shift
	if test "x$NAMESPACE" == "x"; then
		echo "uso: gong.sh add_record namespace recname [targetdir]"
		exit 1
	fi
	RECNAME=$1
	shift
	if test "x$RECNAME" == "x"; then
		echo "uso: gong.sh add_record namespace recname [targetdir]"
		exit 1
	fi
	LOWER_RECNAME=`echo $RECNAME | tr '[:upper:]' '[:lower:]'`
	TARGETDIR=$1
	shift
	if test "x$TARGETDIR" == "x"; then
		TARGETDIR=$NAMESPACE
	fi
	if test -d "$TARGETDIR/src"; then
		TARGETDIR="$TARGETDIR/src"
	fi
	if test -d $TARGETDIR; then
		FRMCPP="$TARGETDIR/${NAMESPACE}frmedit$RECNAME.cpp"
		if test -f $FRMCPP; then
			echo "El formulario $FRMCPP ya existe"
			exit 1
		fi
		GONGMODULE="$TARGETDIR/${NAMESPACE}module.cpp"
		if test -f $GONGMODULE; then
			sed -e "s%^// TYPE GongModule%// RECORD $RECNAME FrmEditRecMaster\\n&%" -i $GONGMODULE
		else
			echo "No existe el módulo $GONGMODULE para este proyecto"
			exit 1
		fi
		MAKEF="$TARGETDIR/Makefile.am"
		if test -f $MAKEF; then
			sed -e "s/^lib.*${NAMESPACE}.*_SOURCES.*[\\\\]*$/&\\n \\t${NAMESPACE}frmedit$LOWER_RECNAME.cpp \\\\/" \
				-e "s/^nodist_.*SOURCES.*[\\\\]*$/&\\n \\tmoc_${NAMESPACE}frmedit$LOWER_RECNAME.cpp \\\\/" \
				-e "s/^lib.*${NAMESPACE}.*_HEADERS.*[\\\\]*$/&\\n \\t${NAMESPACE}frmedit$LOWER_RECNAME.h \\\\/" -i $MAKEF
			sed -e "s/^lib.*${NAMESPACE}.*_SOURCES.*[\\\\]*$/&\\n \\t${NAMESPACE}rec$LOWER_RECNAME.cpp \\\\/" \
				-e "s/^lib.*${NAMESPACE}.*_HEADERS.*[\\\\]*$/&\\n \\t${NAMESPACE}rec$LOWER_RECNAME.h \\\\/" -i $MAKEF
		else
			echo "No existe $MAKEF"
			exit 1
		fi
		echo "Revise la extrusión MODULE_INFO de $GONGMODULE y ejecute gong.sh capel $GONGMODULE para generar el formulario"
	else
		echo "No existe el directorio de destino '$TARGETDIR'"
		exit 1
	fi
;;

add_fielddef)
	NAMESPACE=$1
	shift
	if test "x$NAMESPACE" == "x"; then
		echo "uso: gong.sh add_fielddef namespace fldname [targetdir]"
		exit 1
	fi
	FLDNAME=$1
	shift
	if test "x$FLDNAME" == "x"; then
		echo "uso: gong.sh add_fieldef namespace recname [targetdir]"
		exit 1
	fi
	TARGETDIR=$1
	shift
	if test "x$TARGETDIR" == "x"; then
		TARGETDIR=$NAMESPACE
	fi
	if test -d "$TARGETDIR/src"; then
		TARGETDIR="$TARGETDIR/src"
	fi
	if test -d $TARGETDIR; then
		FLDDEFCPP="$TARGETDIR/${NAMESPACE}fld$FLDNAME.cpp"
		if test -f $FLDDEFCPP; then
			echo "La definición de campo $FLDDEFCPP ya existe"
			exit 1
		fi
		echo "/*<<<<<MODULE_INFO*/" > $FLDDEFCPP
		echo "// COPYLEFT dbFieldDefinition que representa $FLDNAME" >> $FLDDEFCPP
		echo "// TYPE dbFieldDefinition $NAMESPACE::Fld$FLDNAME" >> $FLDDEFCPP
		echo "/*>>>>>MODULE_INFO*/" >> $FLDDEFCPP

		MAKEF="$TARGETDIR/Makefile.am"
		if test -f $MAKEF; then
			sed -e "s/^.*${NAMESPACE}.*_SOURCES.*[\\\\]*$/&\\n \\t${NAMESPACE}fld$FLDNAME.cpp \\\\/" \
				-e "s/^.*${NAMESPACE}.*_HEADERS.*[\\\\]*$/&\\n \\t${NAMESPACE}fld$FLDNAME.h \\\\/" -i $MAKEF
		else
			echo "No existe $MAKEF"
			exit 1
		fi
		echo "Revise la extrusión MODULE_INFO de $FLDDEFCPP y ejecute gong.sh capel $TARGETDIR/$FLDDEFCPP para generar el formulario"
	else
		echo "No existe el directorio de destino '$TARGETDIR'"
		exit 1
	fi
;;


capel)
	do_capel $@
;;

cleanup)
	do_cleanup $1
;;

# Elimino las llamadas a capel porque ya están en el svn los cambios
init_project)
	PWD_SAVE=`pwd`
	CURDIR=`basename $PWD`
	if [ -f configure.ac ]; then
		do_capel configure.ac
 		create_links
		cd $PWD_SAVE
		update_subdirs $CURDIR
		banner $CURDIR
		make -f Makefile.cvs || exit $?
		if test "x$1" == "xDebug"; then
			banner "Debug"
			mkdir Debug
			cd Debug
			autoreconf -i --force .. || exit $?
			../configure --enable-debug --prefix=$PWD/.. CFLAGS= CXXFLAGS= || exit $?
		else
			banner "Release"
			mkdir Release
			cd Release
			autoreconf -i --force ..
			../configure --disable-debug --prefix $PWD/.. CFLAGS= CXXFLAGS= || exit $?
		fi
		make || exit $?
		cd ..
	else
		echo "Missing configure.ac in $CURDIR"
	fi
;;

build_gonglib)
	PWD_SAVE=`pwd`
	CURDIR=`basename $PWD`
	$GONGLIB_VERSION = $1
	if [ -f configure.ac ]; then
		banner $CURDIR-$GONGLIB_VERSION
		mkdir gonglib-$GONGLIB_VERSION
		cd gonglib-$GONGLIB_VERSION
		cp ../Makefile.cvs .
		cp ../configure.ac .
		cp ../Makefile.am .
		cp ../gonglib.pc.in .
		cp ../acinclude.m4 .
		mkdir m4
		touch INSTALL NEWS README AUTHORS ChangeLog COPYING
		echo "libgonglib_0_1_la_SOURCES = \\" >> Makefile.am
		find ../gonglib -name "*.cpp" -printf "../gonglib/%f \\\\\n" >> Makefile.am
		find ../gong-qt4 -name "*.cpp" -printf "../gong-qt4/%f \\\\\n" >> Makefile.am
		find ../gong-rtk -name "*.cpp" -printf "../gong-rtk/%f \\\\\n" >> Makefile.am
		echo "" >> Makefile.am
		echo "libgonglib_0_1_la_HEADERS = \\" >> Makefile.am
		find ../gonglib -name "*.h" -printf "../gonglib/%f \\\\\n" >> Makefile.am
		find ../gong-qt4 -name "*.h" -printf "../gong-qt4/%f \\\\\n" >> Makefile.am
		find ../gong-rtk -name "*.h" -printf "../gong-rtk/%f \\\\\n" >> Makefile.am
		make -f Makefile.cvs || exit $?
		if test "x$1" == "x"; then
			autoreconf -i --force || exit $?
			./configure --enable-debug --prefix=$PWD/.. || exit $?
		else
			banner "Release"
			autoreconf -i --force
			./configure --prefix $PWD/.. || exit $?
		fi
		make || exit $?
		cd ..
	else
		echo "Missing configure.ac in $CURDIR"
	fi
;;


update_project)
	PWD_SAVE=`pwd`
	CURDIR=`basename $PWD`
	cd $GONGDIR
	echo "Updating gonglib project..."
	svn update || exit
	# Then, this project
	cd $PWD_SAVE
	echo "Updating $CURDIR project..."
	svn update # Do not exit on failure here
;;

make_tar)
	PROJECTNAME=$1
	shift
	if test "x$PROJECTNAME" == "x"; then
		echo "uso: gong.sh make_tar [project_name]"
		exit 1
	fi
	VERSION=$(grep "AC_INIT" configure.ac | grep -o "[0-9]\.[0-9]")
# 	cd  ~/devel/tmp
# 	rm -rf $PROJECTNAME-$VERSION
# 	mkdir $PROJECTNAME-$VERSION
# 	cd $PROJECTNAME-$VERSION
# 	MODULES=$(grep -i "dnl module " configure.ac | cut -d " " -f 3)
# 	svn export svn://santilinx/gestiong/gonglib gonglib
# 	svn export svn://santilinx/gestiong/m4 m4
# 	svn export svn://santilinx/gestiong/share share
# 	for m in $MODULES; do
# 		mod=`echo $m | tr '[:upper:]' '[:lower:]'`
# 		svn export svn://santilinx/gestiong/mod/$mod/ $mod
# 	done
# 	if test "x$PROJECTNAME" == "xgestiong"; then
# 		svn export svn://santilinx/gestiong/gestiong . --force
# 	else
# 		svn export svn://santilinx/gongapps/$PROJECTNAME . --force
# 	fi
	if test -d Release; then
		cd Release
	else
		cd Debug
	fi
	if make ; then
		make dist
	fi
# 		DISTFILE=$(ls $PROJECTNAME*.tar.gz)
# 		rm -rf /tmp/$PROJECTNAME*
# 		cp $DISTFILE /tmp
# 		cd /tmp
# 		tar -zxvf $DISTFILE
# 		DISTDIR=$(find /tmp \( ! -name tmp -prune \) -type d -a -name "$PROJECTNAME*")
# 		cd $DISTDIR
# 		rm -rf autom4te.cache
# 		sed -e s%-I../../%-I../% -i configure.ac
# 		make -f Makefile.cvs
# 		rm acinclude.m4
# 		cp $GONGDIR/acinclude.m4 .
# 		cd ..
# 		echo "Working on $(pwd)"
# 		tar -zcvf ${PROJECTNAME}_$VERSION.orig.tar.gz $DISTDIR
# 		echo "Tar file created in $(pwd)/${PROJECTNAME}_$VERSION.orig.tar.gz"

	;;

*)
	echo "Use: "
	echo "gong.sh init"
	echo "gong.sh init_project [nodebug]"
	echo "gong.sh update_project"
	echo "gong.sh create_app AppName"
	echo "gong.sh create_module ModuleName"
	echo "gong.sh add_record namespace RecordName [targetdir]"
	echo "gong.sh add_fielddef namespace RecordName [targetdir]"
	echo "gong.sh capel filename"
	echo "gong.sh cleanup projectname|modulename"
	echo "gong.sh build_gonglib version"
	echo "gong.sh make_tar projectname"
;;

esac
exit 0 # ok

# limpiar qt3
# sudo apt-get remove qt3-dev-tools libqt3-mt-dev qt3-designer
# sudo apt-get install libjpeg8-dev
# sudo apt-get autoremove

