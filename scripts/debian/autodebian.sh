#!/bin/bash

VERSION=3
HOST="Kubuntu 12.04"
KDE="1"
GNOME="0"
PING_TEST_IP="208.67.222.222"
NTPDATE_SERVER="pool.ntp.org"
HAY_INTERNET=0
LENGUAJE="es"
DATETIME=`date --rfc-3339=seconds | sed -e "s/[-]//g" -e "s/ /_/" -e "s/:[0-9]*+.*$//" -e "s/://"`
WIRELESS_FIRMWARE="atmel-firmware firmware-bnx2 firmware-bnx2x firmware-ipw2x00 firmware-iwlwifi firmware-qlogic firmware-qlogic firmware-ralink libertas-firmware linux-wlan-ng-firmware zd1211-firmware b43-fwcutter"
OPENOFFICE_COMMON="openoffice.org openoffice.org-l10n-$LANGUAGE openoffice.org-help-$LANGUAGE openoffice.org-thesaurus-$LANGUAGE openoffice.org-hypenation-$LANGUAGE myspell-$LENGUAJE openjdk-6-jre"
APT_MULTIMEDIA_BASICO="vorbis-tools mplayer transcode"
FREEFONTS="ttf-bitstream-vera ttf-breip ttf-dejavu ttf-freefont ttf-liberation ttf-opensymbol ttf-unifont"
MSFONST="ttf-mscorefonts-installer"

instala_programa() {
	echo "Instalando $1"
	shift
	apt-get install -y -q $@
}

instala_todo_comun() {
	instala_programas_necesarios
	instala_programas_impresora
	instala_openoffice
	instala_firefox
	instala_thunderbird
	language_espanol
}

instala_prop_imprescindible() {
	instala_todo_comun
	instala_programa "Kubuntu restriced extras" kubuntu-restricted-extras
}

instala_todo_libre_kde() {
	instala_todo_comun
	instala_programa "Multimedia para KDE" amarok vlc
	instala_programa "Codecs mp3" lame lame-extras gstreamer0.8-lame w32codecs
	instala_programa "k3b" k3b dvd+rw-tools libk3b3-extracodecs
	language_espanol
}

instala_todo_libre_gnome() {
	instala_todo_comun
	instala_programa "Multimedia para GNOME" vlc
	language_espanol
}

borra_programas_innecesarios() {
	instala_programa "Deborphan" deborphan
	echo "Eliminando paquetes huérfanos"
	deborphan --guess-all | grep -v "\-dev" | xargs apt-get remove --purge
	echo "Eliminando paquetes tontos"
	apt-get remove --purge popularity-contest ubuntuone*
	echo "Eliminando paquetes del modem"
	apt-get remove --purge kppp
	echo "Eliminar bluetooh"
	apt-get remove --purge gnome-bluetooth bluez
	sed '/exit 0/i \
rfkill block bluetooh' /etc/rc.local
}

instala_programas_necesarios() {
	instala_programa "Programas necesarios" bzip2 dialog
}

instala_programas_impresora() {
	instala_programa "Impresoras" smbclient
}

instala_impresora_hp() {
# Se necesita python-gobject para hp-sendfax: https://answers.launchpad.net/hplip/+question/30741
	instala_programa "Impresoras HP" hplip hplip-gui python-gobject hp-ppd hplip-ppds
	echo "Para configurar tu impresora, ve al menú principal 'K', elige 'Sistema' y luego 'HPLIP ToolBox - Printer Toolbox'"
	echo "Pulsa una tecla para continuar"
	read
}

detectar_wifi() {
	clear;
	lspci | grep [Ww]ireless
	dmesg | grep [Ff]irmware
        dmesg | grep [Ww]ireless
}


instala_hp_1020() {
	instala_programas_impresora
	apt-get uninstall -y foo2zjs
	instala_programa "HP" hannah-foo2zjs
	cd tmp
	wget -Ofoo2zjs.tar.gz http://foo2zjs.rkkda.com/foo2zjs.tar.gz
	tar -zxvf
	tar zxf foo2zjs.tar.gz
  	cd foo2zjs
  	make
  	make install install-hotplug cups
	hannah-foo2zjs
        # cat /usr/share/foo2zjs/firmware/sihp1020.dl > /dev/usb/lp0
}

instala_skype() {
	# http://www.debianadmin.com/how-to-install-skype-20-in-debian-lenny.html
	SKYPE_PACKAGE="skype-debian_2.1.0.47-1_i386.deb"
	instala_programa "Skype" libasound2 libc6 libgcc1 libqt4-core libqt4-gui libsigc++-2.0-0c2a libstdc++6 libx11-6
	rm $SKYPE_PACKAGE
	wget http://skype.com/go/getskype-linux-beta-deb -O$SKYPE_PACKAGE
	dpkg -i --force-architecture $SKYPE_PACKAGE
	rm $SKYPE_PACKAGE
	if [ $? ]; then
		echo "Mal"
	else
		echo "Bien"
	fi
	read
}

instala_acrobat_reader() {
	ACROBAT_READER_PACKAGE="AdbeRdr9.2-1_i386linux_enu.deb"
	rm $ACROBAT_READER_PACKAGE
	wget ftp://ftp.adobe.com/pub/adobe/reader/unix/9.x/9.2/enu/$ACROBAT_READER_PACKAGE -O$ACROBAT_READER_PACKAGE
	dpkg -i $ACROBAT_READER_PACKAGE
}


reloj_hora() {
	which_ntpdate=`which ntpdate`
	if [ ! -e "$which_ntpdate" -o ! -x "$which_ntpdate" ]; then
		instala_programa "Programa para sincronizar la hora" ntpdate
	fi
	ntpdate $NTPDATE_SERVER
	read
}

language_espanol() {
	echo $(check-language-support -l es)
	instala_programa "Escritorio en español" $(check-language-support -l es)
}

instala_openoffice_kde() {
	instala_openoffice
	instala_programa "OpenOffice" -t lenny-backports $OPENOFFICE_COMMON openoffice.org-kde
}

instala_openoffice() {
	instala_programa "OpenOffice" -t lenny-backports $OPENOFFICE_COMMON openoffice.org-gtk
}


instala_thunderbird() {
	instala_programa "Thunderbird" thunderbird thunderbird-$LENGUAJE-$LENGUAJE thunderbird-$LENGUAJE-$LENGUAJE enigmail
}

instala_multimedia_basico() {
	instala_programa "Multimedia básico" $APT_MULTIMEDIA_BASICO
}

my_dialog() {
	if test "x$HAY_DIALOG" == "xtrue"; then
		dialog "$@" 2> /tmp/menuoption.txt
	else
		clear
		skipping=0
		skipping_menu=0
		volver=0
		for linea in "$@"; do
			case "$linea" in
				"--title") ;;
				"--cancel-label") skipping=1 ;;
				"--menu") skipping_menu=1 ;;
				[0-9]*) if [[ $skipping_menu > 0 && $skipping_menu < 4 ]]; then
									skipping_menu=$(($skipping_menu+1))
								else
									skipping_menu=0
									if [[ $volver == 0 ]]; then
										echo "0) Volver"
										volver=1
									fi
									echo -n "$linea) "
								fi
								;;
				* ) if [[ $skipping > 0 ]]; then
							skipping=0
						else
							if [[ $skipping_menu == 0 ]]; then
								echo $linea
							fi
						fi ;;
			esac
		done
		read TECLA
		echo $TECLA > /tmp/menuoption.txt
	fi
}

repositorios_libres() {
	apt-get safe-upgrade -y -q
	cp /etc/apt/sources.list /etc/apt/sources.list.debroot.$DATETIME
	cat >/etc/apt/sources.list <<EOF
deb http://ftp.fr.debian.org/debian lenny main contrib

deb http://security.debian.org lenny/updates main contrib
deb-src http://security.debian.org lenny/updates main contrib

deb http://volatile.debian.org/debian-volatile lenny/volatile main contrib
deb-src http://volatile.debian.org/debian-volatile lenny/volatile main contrib

deb http://www.backports.org/debian lenny-backports main contrib
EOF
	import_gpg_keys
	apt-get update
	apt-get dist-upgrade -y -q -o Aptitude::CmdLine::Assume-Yes=true
	apt-get safe-upgrade -y -q -t lenny-backports -o Aptitude::CmdLine::Assume-Yes=true
	instala_programas_necesarios
}

repositorios_no_libres() {
	apt-get safe-upgrade -y -q
	cp /etc/apt/sources.list /etc/apt/sources.list.autodebian.$DATETIME
	cat >/etc/apt/sources.list <<EOF
deb http://ftp.fr.debian.org/debian lenny main contrib non-free

deb http://security.debian.org lenny/updates main contrib non-free
deb-src http://security.debian.org lenny/updates main contrib non-free

deb http://volatile.debian.org/debian-volatile lenny/volatile main contrib
deb-src http://volatile.debian.org/debian-volatile lenny/volatile main contrib

deb http://www.backports.org/debian lenny-backports main contrib non-free

# multimeda plugins
deb http://debian-multimedia.org/ lenny main
EOF
	import_gpg_keys
	apt-get update
	apt-get dist-upgrade -y -q -o Aptitude::CmdLine::Assume-Yes=true
	apt-get safe-upgrade -y -q -t lenny-backports -o Aptitude::CmdLine::Assume-Yes=true
	instala_programas_necesarios
}

import_gpg_keys() {
	if apt-key list | grep 1F41B907 ; then
		echo "Clave debian-multimedia ya existe"
        else
		gpg --keyserver keys.gnupg.net --recv-keys 1F41B907 ; gpg --export 1F41B907 | apt-key add -
	fi
	if apt-key list | grep 16BA136C ; then
		echo "Clave Backports ya existe"
        else
		gpg --keyserver hkp://subkeys.pgp.net --recv-keys 16BA136C ; gpg --export 16BA136C | apt-key add -
	fi
}

repositorios_cdrom() {
	apt-cdrom
	apt-get update
	apt-get dist-upgrade -y -q
}

actualiza_todo() {
	apt-get update
	apt-get safe-upgrade -y -q
}

menu_welcome() {
	while [ 1 ]; do
		my_dialog --cancel-label "Salir" \
			--title "=== MENÚ PRINCIPAL ===" \
			--menu "Elige la sección" 20 75 9 \
			1 "Instalar lo básico" \
			2 "Instalar programas" \
			3 "Instalar impresoras, discos externos, wifi, etc." \
			4 "Configurar: reloj, teclado, idioma, liberar espacio, etc." \
			5 "Usuarias" \
			9 "Ver si hay una nueva versión de este programa"
		case `cat /tmp/menuoption.txt` in
		1 )		menu_todo ;;
		2 )		menu_programas ;;
		3 )		menu_perifericos ;;
		4 )		menu_configuracion ;;
 		5 ) 	menu_usuarias ;;
		9 ) 	check_new_version ;;
		* )	break ;;
		esac
	done
}

menu_todo() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== INSTALAR TODO LO NECESARIO ===" \
			--menu "Elige la sección" 20 75 2 \
			1 "Programas propietarios imprescindibles: Fuentes windows, codecs mp3, etc. " \
			2 "Escritorio GNOME" \
			3 "Escritorio KDE"
		case `cat /tmp/menuoption.txt` in
		1 ) instala_prop_imprescindible ;;
		2 ) instala_todo_libre_gnome ;;
		3 ) instala_todo_libre_kde ;;
		* ) break;
		esac
	done
}

menu_gruposprogramas() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== GRUPOS DE PROGRAMAS ===" \
			--menu "Elige la sección" 20 75 6 \
			1 "Sólo software libre" \
			2 "Software libre + musica + video + hardware no libre" \
			3 "Software privativo" \
			8 "Importar claves GPG" \
			9 "CDROM de debian"
		case `cat /tmp/menuoption.txt` in
		1 )		clear; repositorios_libres ;;
		2 ) 	clear; repositorios_no_libres ;;
		3 ) 	clear; instalar_windows ;;
		8 )     clear; import_gpg_keys ;;
		9 )		clear; repositorios_cdrom ;;
		* )	break ;;
		esac
	done
}

menu_programas() {
	while [ 1 ]; do
		my_dialog --title "=== INSTALACIÓN DE PROGRAMAS ===" \
			--menu "Elige los programas a instalar" 20 75 9 \
			1 "Open Office para KDE" \
			2 "Open Office para GNOME" \
			4 "Thunderbird para leer mi correo (outlook)" \
			5 "Gráficos: Photoshop, etc." \
			6 "Multimedia: Grabar/reproducir CD y DVD, video, música, etc." \
			7 "Programas para administradoras del sistema" \
			8 "Programas propietarios: Skype, Acrobat Reader, etc." \
			9 "GestiONG"
		case `cat /tmp/menuoption.txt` in
		1 )		clear; instala_openoffice_kde ;;
		2 ) 	clear; instala_openoffice ;;
		4 ) 	clear; instala_thunderbird ;;
		5 )		menu_graficos ;;
		6 ) 	menu_multimedia ;;
		7 )		clear; instala_programa "Administración del sistema" rsync mc nmap openssh-server ;;
		8 ) 	menu_programas_propietarios ;;
		* )	break ;;
		esac
	done
}


menu_impresoras() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ IMPRESORAS ===" \
			--menu "Elige el grupo de impresoras a instalar" 20 75 3 \
			1 "HP LaserJet 2600,1600,1020,1018,1005,1000:Minolta 2xx0,Samsumg CLX-3160,CLP-600,CLP-300:XeroxPhaser 6115,6110"  \
			2 "Otra impresora HP" \
			9 "Cualquier otra"
		case `cat /tmp/menuoption.txt` in
		1 )	clear; instala_hp_1020 ;;
		2 ) clear; instala_impresora_hp ;;
		9 ) clear; instala_programas_impresora ;;
		* )	break ;;
		esac
	done
}

menu_perifericos() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ PERIFÉRICOS ===" \
			--menu "Elige el periférico a instalar" 20 75 3 \
			1 "Instalar una impresora" \
			2 "No puedo escribir en mi disco externo que está preparado para Windows" \
			3 "Quiero que funcione mi tarjeta wifi (Inalámbrica)"
		case `cat /tmp/menuoption.txt` in
		1 )	menu_impresoras ;;
		2 )	instala_programa "Sistema de archivos ntfs" ntfs-3g ;;
		3 )	menu_internet_inalambrico ;;
		* )	break ;;
		esac
	done
}

menu_multimedia() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ MULTIMEDIA ===" \
			--menu "Elige los programas a instalar" 20 75 6 \
			1 "Reproductores libres de audio y video para KDE" \
			2 "Reproductores libres de audio y video para GNOME" \
			3 "Codecs mp3 y otros compatibles con windows" \
			5 "Grabación de CD y DVD para KDE" \
			6 "Grabación de CD y DVD para GNOME"
		case `cat /tmp/menuoption.txt` in
		1 )	clear; instala_multimedia_basico; instala_programa "Multimedia para KDE" amarok kaffeine ;;
		2 )	clear; instala_multimedia_basico; instala_programa "Multimedia para GNOME" vlc ;;
		3 ) 	clear; instala_programa "Codecs mp3" lame lame-extras gstreamer0.8-lame w32codecs ;;
		5 ) 	clear; instala_programa "k3b" k3b dvd+rw-tools libk3b3-extracodecs;;
		6 ) 	clear; instala_programa "Graba CD y DVD" dvd+rw-tools ;;
		* )	break ;;
		esac
	done
}

menu_configuracion() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== CONFIGURACIÓN ===" \
			--menu "Elige la sección" 20 75 5 \
			1 "Poner el escritorio en español" \
			2 "Poner el reloj en hora" \
			3 "Liberar espacio en disco" \
			4 "Compartir los archivos de este ordenador" \
			5 "Instalar más fuentes de letra: Arial, etc."
		case `cat /tmp/menuoption.txt` in
		1 ) 	clear; language_espanol ;;
		2 )	clear; reloj_hora ;;
		3 ) 	clear; liberar_espacio_disco ;;
# http://www.spencerstirling.com/computergeek/NFS_samba.html
		4 ) 	clear; instala_programa "NFS" nfs-kernel-server nfs-common portmap kdenetwork-filesharing;;
                5 )     clear; instala_programa "Fuentes" $FREEFONTS $MSFONTS ;;
		* )	break ;;
		esac
	done
}

menu_graficos() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ PROGRAMAS GRÁFICOS ===" \
			--menu "Elige el programa a instalar" 20 75 2 \
			1 "Gimp -- Retoque fotográfico" \
			2 "InkScape -- Dibujo vectorial"
		case `cat /tmp/menuoption.txt` in
		1 )	instala_programa Gimp gimp ;;
		2 ) instala_programa InkScape inkscape ;;
		* )	break ;;
		esac
	done
}

menu_internet_inalambrico() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== INTERNET INALÁMBRICO ===" \
			--menu "Elige la sección" 20 75 3 \
			1 "Detectar mi tarjeta wifi" \
			2 "Instalar firmwares no libres"
		case `cat /tmp/menuoption.txt` in
		1 ) clear; detectar_wifi; read ;;
		2 ) clear; instala_programa "Firmwares no libres" $WIRELESS_FIRMWARE ;;
		* )	break ;;
		esac
	done
}


menu_programas_propietarios() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ INSTALAR PROGRAMAS PROPIETARIOS ===" \
			--menu "Elige el programa a instalar" 20 75 2 \
			1 "Skype"  \
			2 "Acrobar Reader para PDF"
		case `cat /tmp/menuoption.txt` in
		1 )	instala_skype ;;
		2 ) 	instala_acrobar_reader ;;
		* )	break ;;
		esac
	done
}

menu_usuarias() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== USUARIAS ===" \
			--menu "Elige la sección" 20 75 4 \
			1 "Añadir una usuaria" \
			2 "Personalizar el escritorio KDE" \
			3 "Personalizar el escritorio GNOME" \
			4 "Arreglar problema al iniciar KDE o GNOME"
		case `cat /tmp/menuoption.txt` in
		1 ) 	clear; anadir_usuaria ;;
		2 ) 	clear; personalizar_escritorio KDE ;;
		3 ) 	clear; personalizar_escritorio GNOME ;;
		4 ) 	clear; arreglar_permisos_usuaria ;;
		* )	break ;;
		esac
	done
}



# tar --exclude=.. -czvf ../usuaria.tar.gz .*
personalizar_escritorio() {
	clear
	echo -n "Introduce el nombre de la usuaria: "
	read USUARIA
	if [ "x$USUARIA" != "x" ]; then
		FILENAME=usuaria-$1.tar.gz
		rm /tmp/$FILENAME
		wget http://www.gestiong.org/$FILENAME -O/tmp/$FILENAME
		cd /home/$USUARIA
		tar -zxvf /tmp/$FILENAME
		# Solo ficheros ./.*
		find . -printf "%p\n" | grep "\.\/\." |
			while IFS= read -r file
			do
				if grep alba "$file"; then
					sed -e s/alba/$USUARIA/g -i $file
				fi
			done
		chown -R $USUARIA:$USUARIA /home/$USUARIA
		rm /tmp/files.lst
		rm /tmp/$FILENAME
	fi
}


anadir_usuaria() {
	clear
	echo -n "Introduce el nombre de la usuaria: "
	read USUARIA
	if [ "x$USUARIA" != "x" ]; then
		adduser $USUARIA
		adduser $USUARIA dialout
		adduser $USUARIA cdrom
		adduser $USUARIA floppy
		adduser $USUARIA audio
		adduser $USUARIA video
		adduser $USUARIA plugdev
		adduser $USUARIA powerdev
		adduser $USUARIA netdev
	fi
}

arreglar_permisos_usuaria() {
	clear
	echo -n "Introduce el nombre de la usuaria: "
	read USUARIA
	if [ "x$USUARIA" != "x" ]; then
		chown -R $USUARIA:$USUARIA /home/$USUARIA
	fi
}

liberar_espacio_disco() {
  clear
  df -h >> /tmp/espacioendisco.txt

  apt-get clean -y -q
  apt-get autoclean -y -q
  borra_programas_innecesarios

  clear
  echo "Espacio antes de liberar"
  cat /tmp/espacioendisco.txt
  rm /tmp/espacioendisco.txt
  echo
  echo "Espacio despues de liberar"
  df -h
  read
}


internet_fuerza_bruta() {
	/etc/init.d/dbus restart
}


check_internet() {
	local command_ping="ping -n -q -c 3 $PING_TEST_IP"
	echo "Comprobando si hay conexión a INTERNET..."
	$command_ping
	local result=$?
	if [ $result != 0 ]; then
		wget http://www.google.com -Ogoogleindex.html.deleteme
		result=$?
		rm googleindex.html.deleteme
	fi
	while true; do
		if [ $result != 0 ]; then
			cat <<EOF

HORROR: No estás conectada a Internet.

¿Qué quieres hacer?
1) Probar otra vez
2) Intentar conectarme, ya lo he solucionado
3) Configurar la conexión
4) Probar por la fuerza bruta
5) Continuar con el programa sin Internet
9) Terminar este programa

EOF
			read respuesta
			case $respuesta in
			1)	;;

			2) 	dhclient3 ;;

			3)	clear; cat <<EOF
Atención, voy a abrir una ventana para configurar las tarjetas de red que tengas disponibles.

Maximiza la ventana y luego pulsa el botón "Administrator mode" (o "Modo Administrador") y a continuación
teclea la contraseña de root.

Pulsa una tecla para continuar...
EOF
				read
				su -c "kcmshell kcm_knetworkconfmodule" $LOGNAME ;
				exit ;;
			4)	clear; internet_fuerza_bruta ;;
			5)	HAY_INTERNET=0; return 2 ;;

			9)	echo "Hasta la vista"; exit ;;
			esac
			$command_ping
			result=$?
			if [ $result != 0 ]; then
				wget http://www.google.com -Ogoogleindex.html.deleteme
				result=$?
				rm googleindex.html.deleteme
			fi
		else
			HAY_INTERNET=1
			return 0
		fi
	done
}

init() {
	check_internet
	HAY_INTERNET=$?
	if [ $HAY_INTERNET -ne 0 ]; then
		echo "Parece que no tienes una conexión a Internet"
		echo "¿Quieres continuar? (S/N)"
		read answer
		if [ "$answer" != "s" -a "$answer" != "S" ]; then
			echo "Programa terminado."
			exit
		fi
	fi
}

check_dialog() {
# 	dialog_path=`which dialog`
# 	if [ ! -e "$dialog_path" -o ! -x "$dialog_path"  ] ; then
# 		apt-get install dialog
# 	fi
	dialog_path=`which dialog`
	if [ ! -x "$dialog_path" ] ; then
		if [ $HAY_INTERNET -ne 0 ]; then
			apt-get install dialog
		fi
	fi
	if [ ! -x "$dialog_path" ] ; then
		HAY_DIALOG=false
	else
		HAY_DIALOG=true
	fi
}

check_root() {
	if [[ $EUID -ne 0 ]]; then
		echo "Este programa solo puede ser utilizado por el usuario root." 1>&2
		echo "Escriba 'su' y la contraseña de root y vuelva a intentarlo" 1>&2
		exit 1
	fi
}

instalar_windows() {
	echo "Instala windows ;)"
	exit 1
}


check_new_version() {
	NEWFILE=/tmp/new_version_autodebian.sh
	rm $NEWFILE
	wget www.gestiong.org/autodebian.sh -O$NEWFILE
	NEWVERSION=`grep "^VERSION=" $NEWFILE | cut -d= -f2 | sed -e "s/\"//g"`
	if (( $NEWVERSION > $VERSION )); then
		echo mv $NEWFILE $0
		clear
		echo "Se ha instalado la nueva versión $NEWVERSION. Vuelve a ejecutar $0"
		rm $NEWFILE
		exit
	else
		clear
		echo "No hay una nueva versión de este programa: v$VERSION"
		rm $NEWFILE
		read
	fi
}


check_root
case "$1" in
	version)
		echo $0 v$VERSION
		exit
		;;
	kde)
		KDE="1"
		GNOME="0"
		;;
	gnome)
		KDE="0"
		GNOME="1"
		;;
esac
init
check_dialog
menu_welcome
echo
echo "¡Hasta la vista!"
echo
