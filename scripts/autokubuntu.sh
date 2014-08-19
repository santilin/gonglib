#!/bin/bash

# probar apt-get -q -y -o APT::Install-Recommends=true -o APT::Get::AutomaticRemove=true install task-spanish-kde-desktop
# probar unburden-home-dir

LATEXFONTS="fonts-lyx"


VERSION=3
DISTRO=$(cat /etc/*release | grep "^ID=" | sed -e "s/^ID=//")
# /etc/debian_version
DISTRO_VER=$(cat /etc/*release | grep "^VERSION_ID=" | sed -e "s/^VERSION_ID=//" | tr -d \")
KDE=$(ps ax | grep "kdeinit" | grep Running)
if test "x$KDE" = "x"; then
   KDE="0"
else
   KDE="1"
fi
GNOME="0"
PING_TEST_IP="208.67.222.222"
NTPDATE_SERVER="pool.ntp.org"
HAY_INTERNET=0
LANGUAGE="es"
DATETIME=`date --rfc-3339=seconds | sed -e "s/[-]//g" -e "s/ /_/" -e "s/:[0-9]*+.*$//" -e "s/://"`
WIRELESS_FIRMWARE="atmel-firmware firmware-bnx2 firmware-bnx2x firmware-ipw2x00 firmware-iwlwifi firmware-qlogic firmware-qlogic firmware-ralink libertas-firmware linux-wlan-ng-firmware zd1211-firmware b43-fwcutter"
OPENOFFICE_COMMON="libreoffice libreoffice-l10n-$LANGUAGE libreoffice-help-$LANGUAGE libreoffice-thesaurus-$LANGUAGE libreoffice-hypenation-$LANGUAGE mythes-$LANGUAGE myspell-$LANGUAGE"
FREEFONTS="ttf-bitstream-vera ttf-breip ttf-dejavu ttf-freefont ttf-liberation ttf-opensymbol ttf-unifont fonts-linuxlibertine"
MSFONTS="ttf-mscorefonts-installer"

instala_programa() {
	echo "Instalando $1"
	shift
	apt-get install -y -q $@
	echo 
	echo "Pulsa una tecla para continuar..."
	read nada
}

aptgetupdate() {
	case $DISTRO in
	debian*)
		if grep "^deb cdrom" /etc/apt/sources.list; then
			if ! grep "^deb http" /etc/apt/sources.list; then
				echo "Tienes un debian instalado desde CDROM pero sin repositorios. ¿Quieres usar los repositorios de Internet?"
				read siono
				case $siono in
				s|S|si|Si|sí|Sí)
					cat > /etc/apt/sources.list <<EOF
deb http://ftp.gva.es/mirror/debian/ stable main contrib non-free
deb-src http://ftp.gva.es/mirror/debian/ stable main contrib non-free

#testing-updates, previously known as 'volatile'
deb http://ftp.gva.es/mirror/debian/ stable-updates main
deb-src http://ftp.gva.es/mirror/debian/ stable-updates main


deb http://security.debian.org/ stable/updates main contrib non-free
deb-src http://security.debian.org/ stable/updates main contrib non-free
EOF
					;;
				*)
					return
					;;
				esac
			fi
		fi
		;;
	esac
	apt-get update
	apt-get upgrade -y
}

instala_todo_comun() {
	instala_programas_necesarios
	instala_programas_impresora
	language_espanol
}

instala_multimedia_libre() {
	echo "Para ver DVD comerciales"
	apt-get install -y libdvdread4
  	/usr/share/doc/libdvdread4/install-css.sh
	instala_programa "Multimedia básico" vorbis-tools mplayer transcode vlc
}

pregunta_si() {
	echo $1
	read siono
	case $siono in
		s|S|si|Si|sí|Sí)
			return 1
			;;
		*)
			return 0
			;;
	esac
	;;
}

completar_kde() {
	if test "x$KDE" = "x0"; then
		if ! pregunta_si("No parece que estés usando KDE. ¿Quieres continuar?"); then
			exit 0
		fi
	fi
}	

completar_lxde() {
	if test "x$LXDE" = "x0"; then
		if ! pregunta_si("No parece que estés usando LXDE. ¿Quieres continuar?"); then
			exit 0
		fi
	fi
}	


instala_multimedia_prop() {
	instala_multimedia_libre
	instala_programa "Codecs mp3" lame lame-extras gstreamer0.8-lame w32codecs
	if test "x$KDE" = "x1"; then
		instala_programa kubuntu_restricted_extras
	fi
	if test "x$GNOME" = "x1"; then
		instala_programa ubuntu_restricted_extras
	fi
}

instala_todo_libre() {
	instala_multimedia_libre
	if test "x$KDE" = "x1"; then
		instala_multimedia_libre_kde
	fi
	if test "x$GNOME" = "x1"; then
		instala_multimedia_libre_gnome
	fi
}

instala_multimedia_libre_kde() {
	instala_programa "Multimedia para KDE" amarok
	instala_programa "k3b" k3b dvd+rw-tools libk3b3-extracodecs
}

instala_multimedia_libre_gnome() {
	instala_programa "Multimedia para GNOME" vlc
}

borra_programas_huerfanos() {
	instala_programa "Deborphan" deborphan
	echo "Estos paquetes parece que están huérfanos"
	deborphan --guess-all > /tmp/deborphan.list
	cat /tmp/deborphan.list
	echo "Quieres eliminarlos?"
	read ANSwER
	if test "x$ANSWER" = "xy"; then
	    packages=$(cat /tmp/deborphan.list)
	    apt-get apt-get remove --purge $packages
	fi
}

borra_bluetooh() {
	echo "Eliminar bluetooh"
	apt-get remove --purge gnome-bluetooth bluez
	sed '/exit 0/i \
rfkill block bluetooh' /etc/rc.local
}

borra_programas_kde() {
	echo "Borrar programas de KDE"
	apt-get remove --purge kdewallpaper
}

borra_modem() {
	echo "Eliminando paquetes del modem"
	apt-get remove --purge kppp modemmanager
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
	echo "Espera..."
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

instala_fuentes_windows_kk() {
	instala_programa "Fuentes compatibles windows kk" $MSFONTS
}

instala_fuentes_libres() {
	instala_programa "Fuentes libres" $FREEFONTS
}

instala_libreoffice() {
	instala_programa "LibreOffice" $OPENOFFICE_COMMON
        if test "x$KDE" = "x1"; then
		instala_programa "LibreOffice KDE" libreoffice-kde libreoffice-style-oxygen
        fi
        if test "x$GNOME" = "x1"; then
		instala_programa "LibreOffice KDE" libreoffice-gtk
        fi
}

instala_thunderbird() {
	instala_programa "Thunderbird" thunderbird thunderbird-locale-$LANGUAGE enigmail
}

instala_gestiong() {
	case $DISTRO in 
	debian*)
		case $DISTRO_VER in
			7) instala_programa aptitude install build-essential libtool autoconf libpoco-dev libqt4-dev libboost-dev libxml2-dev libmysqlclient-dev libdb-dev libjpeg-dev libpng-dev libboost-regex-dev
			;;
		esac
		;;
	*)
		case $DISTRO_VER in 
		12* ) instala_programa "GestiONG" build-essential libstdc++6-4.4-dbg libtool autoconf libqt4-dev libboost-all-dev libmysqlclient-dev libdb5.1-dev qt4-dev-tools libjpeg-dev libpng-dev libpoco-dev
			;;
		* ) instala_programa "GestiONG" build-essential libstdc++6-4.4-dbg libtool autoconf automake libx11-dev libqt4-dev libboost-all-dev libmysqlclient-dev libdb5.1-dev qt4-dev-tools libjpeg-dev libpng-dev libpoco-dev
			;;
		esac
		;;
	esac
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

actualiza_todo() {
	apt-get update
	apt-get safe-upgrade -y -q
}

menu_welcome() {
	while [ 1 ]; do
		my_dialog --cancel-label "Salir" \
			--backtitle "Distro: $DISTRO, Version: $DISTRO_VER" \
			--title "=== MENÚ PRINCIPAL ===" \
			--menu "Elige la sección" 20 75 9 \
			1 "Sistema: actualizar software, reloj, teclado, etc." \
			2 "Escritorio: idioma, tipos de letra, cambiar de escritorio, etc." \
			3 "Software: Instalar programas adicionales" \
			4 "Hardware: Instalar impresoras, discos externos, wifi, etc." \
			5 "Limpieza" \
			9 "Ver si hay una nueva versión de este programa"
		case `cat /tmp/menuoption.txt` in
		1 )	menu_sistema ;;
		2 )	menu_escritorio ;;
		3 )	menu_software ;;
		4 )	menu_hardware ;;
		5 ) 	menu_limpieza ;;
		9 ) 	check_new_version ;;
		* )	break ;;
		esac
	done
}

menu_sistema() {
	while [ 1 ]; do
		BACKTITLE=`uname -a`
		my_dialog --cancel-label "Volver" \
			--title "=== SISTEMA ===" \
			--menu "Elige la sección" 20 75 5 \
			1 "Actualizar el sistema" \
			2 "Poner el reloj en hora" \
			3 "Compartir los archivos de este ordenador" 
		case `cat /tmp/menuoption.txt` in
		1 )     clear; aptgetupdate ;;
		2 )		clear; reloj_hora ;;
# http://www.spencerstirling.com/computergeek/NFS_samba.html
		3 ) 	clear; instala_programa "NFS" nfs-kernel-server nfs-common portmap kdenetwork-filesharing;;
		* )	break ;;
		esac
	done
}


menu_escritorio() {
	while [ 1 ]; do
		my_dialog --title "=== MENU ESCRITORIO ===" \
			--menu "Elige la operación sobre el escritorio" 20 75 9 \
			1 "Completar KDE" \
			2 "Completar GNOME y MATE" \
			3 "Completar LXDE" \
			4 "Poner programas en español" \
			5 "Instalar fuentes compatibles con Windows kk" \
			6 "Instalar fuentes propietarias de Windows kk"
		case `cat /tmp/menuoption.txt` in
		1 )     clear; completar_kde;
		2 )     clear; completar_gnome;
		3 )     clear; completar_lxde;
		4 ) 	clear; language_espanol ;;
		5 )     clear; instala_fuentes_libres ;;
		6 )     clear; instala_fuentes_windows_kk ;;
		* )	break ;;
		esac
	done
}


menu_software() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== INSTALACIÓN DE PROGRAMAS ===" \
			--menu "Elige los programas a instalar" 20 75 9 \
			1 "Multimedia: Grabar/reproducir CD y DVD, video, música, etc." \
			2 "Gráficos: Photoshop, etc." \
			3 "Completar Open Office" \
			4 "Thunderbird para leer mi correo (outlook)" \
			5 "Programas para administradoras del sistema" \
			6 "Programas propietarios: Skype, Spotify, etc." \
			7 "GestiONG" \
			9 "Básico"
		case `cat /tmp/menuoption.txt` in
		1 ) 	menu_multimedia ;;
		2 )		menu_graficos ;;
		3 ) 	clear; instala_libreoffice ;;
		4 ) 	clear; instala_thunderbird ;;
		5 )		clear; instala_programa "Administración del sistema" rsync mc nmap openssh-server ;;
		6 ) 	menu_software_propietarios ;;
		7 )		instala_gestiong ;;
		9 )	instala_todo_comun ;;
		* )		break ;;
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

menu_hardware() {
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
			3 "Codecs propietarios mp3 y otros compatibles con windows" 
		case `cat /tmp/menuoption.txt` in
		1 )	clear; instala_multimedia_libre_kde ;;
		2 )	clear; instala_multimedia_libre_gnome ;; 
		3 ) clear; instala_multimedia_prop ;;
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


menu_software_propietarios() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== MENÚ INSTALAR PROGRAMAS PROPIETARIOS ===" \
			--menu "Elige el programa a instalar" 20 75 2 \
			1 "Skype"  \
			2 "Acrobar Reader para PDF"
		case `cat /tmp/menuoption.txt` in
		1 )		instala_skype ;;
		2 ) 	instala_acrobar_reader ;;
		* )	break ;;
		esac
	done
}


menu_limpieza() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== LIMPIEZA ===" \
			--menu "Elige la sección" 20 75 5 \
			1 "Liberar espacio en disco" \
			2 "Borrar programas huérfanos" \
			3 "Eliminar bluetooh" \
			4 "Eliminar modem" \
			5 "Eliminar programas de KDE" 
		case `cat /tmp/menuoption.txt` in
		1 ) 	clear; liberar_espacio_disco ;;
		2 ) 	clear; borra_programas_huerfanos ;;
		3 )	clear; borra_bluetooh ;;
		4 ) 	clear; borra_modem ;;
		5 )	clear; borra_programas_kde ;;
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
			2 "Arreglar problema al iniciar KDE o GNOME"
		case `cat /tmp/menuoption.txt` in
		1 ) 	clear; anadir_usuaria ;;
		2 ) 	clear; arreglar_permisos_usuaria ;;
		* )	break ;;
		esac
	done
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
	apt-get autoremove -y
	apt-get autoclean -y -q

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
esac
init
check_dialog
menu_welcome
echo
echo "¡Hasta la vista!"
echo
