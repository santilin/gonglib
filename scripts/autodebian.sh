#!/bin/bash

# probar apt-get -q -y -o APT::Install-Recommends=true -o APT::Get::AutomaticRemove=true install task-spanish-kde-desktop
# probar /etc/issue

# Añadir update-grub


#instalación de un plugin para plasma
# git clone https://github.com/faho/kwin-tiling.git
# cd kwin-tiling/
# plasmapkg –type kwinscript -i .

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
LXDE=$(ps ax | grep lxsession | grep LXDE)
if test "x$LXDE" = "x"; then
	LXDE="0"
else
	LXDE="1"
fi 
PING_TEST_IP="208.67.222.222"
NTPDATE_SERVER="pool.ntp.org"
HAY_INTERNET=0
LANGUAGE="es"
DATETIME=`date --rfc-3339=seconds | sed -e "s/[-]//g" -e "s/ /_/" -e "s/:[0-9]*+.*$//" -e "s/://"`
WIRELESS_FIRMWARE="atmel-firmware firmware-bnx2 firmware-bnx2x firmware-ipw2x00 firmware-iwlwifi firmware-qlogic firmware-qlogic firmware-ralink libertas-firmware linux-wlan-ng-firmware zd1211-firmware b43-fwcutter"
LIBREOFFICE_COMMON="libreoffice libreoffice-l10n-$LANGUAGE libreoffice-help-$LANGUAGE myspell-$LANGUAGE libreoffice-java-common libreoffice-pdfimport libreoffice-style-tango tango-icon-theme libreoffice-writer2xhtml"
LIBREOFFICE_DEBIAN="hyphen-es mythes-es libreoffice-grammarcheck-es"
LIBREOFFICE_OTHER="libreoffice-thesaurus-$LANGUAGE libreoffice-hypenation-$LANGUAGE libreoffice-style-human"


FREEFONTS="ttf-bitstream-vera fonts-breip ttf-dejavu ttf-freefont ttf-liberation ttf-opensymbol ttf-unifont fonts-linuxlibertine fonts-isabella"
MSFONTS="ttf-mscorefonts-installer"

pulsa_tecla() 
{
	echo "Pulsa una tecla para continuar..."
	read
}


pregunta_si() {
	echo $1
	read siono
	case $siono in
		s|S|si|Si|sí|Sí)
			return 0
			;;
		*)
			return 1
			;;
	esac
}


instala_programa() {
	echo "Instalando $1"
	shift
	apt-get install -y -q $@
    echo
}

aptgetupdate() {
	case $DISTRO in
	debian*)
		if grep "^deb cdrom" /etc/apt/sources.list; then
			if ! grep "^deb http" /etc/apt/sources.list; then
				if pregunta_si "Tienes un debian instalado desde CDROM pero sin repositorios. ¿Quieres usar los repositorios de Internet?"; then
					cat > /etc/apt/sources.list <<EOF
deb http://ftp.gva.es/mirror/debian/ stable main contrib non-free
deb-src http://ftp.gva.es/mirror/debian/ stable main contrib non-free

#testing-updates, previously known as 'volatile'
deb http://ftp.gva.es/mirror/debian/ stable-updates main
deb-src http://ftp.gva.es/mirror/debian/ stable-updates main


deb http://security.debian.org/ stable/updates main contrib non-free
deb-src http://security.debian.org/ stable/updates main contrib non-free
EOF
				else
					return
				fi
			fi
		fi
		;;
	esac
	apt-get update
	apt-get upgrade -y
}

instala_multimedia_libre() {
	instala_programa "Multimedia básico" vorbis-tools mplayer transcode vlc
	if [ ! -b /dev/cdrom ]; then
		echo "/dev/cdrom existe";
	else 
		cd /dev
		ln -s sr0 cdrom
	fi
        if [ ! -b /dev/dvd ]; then
                echo "/dev/dvd existe";
        else 
                cd /dev
                ln -s sr0 dvd
        fi
		
	echo "Para ver DVD comerciales"
	apt-get install -y libdvdread4 
	case $DISTRO in
	debian*)
		wget http://download.videolan.org/debian/stable/libdvdcss2_1.2.13-0_i386.deb -O /tmp/libdvdcss.deb
		dpkg -i /tmp/libdvdcss.deb && rm /tmp/libdvdcss.deb
		;;
	*)
	  	/usr/share/doc/libdvdread4/install-css.sh
		;;
	esac
}

completar_escritorio() {
	instala_programa "Programas necesarios" bzip2 dialog
	instala_programas_impresora
	instala_multimedia_libre
	instala_fuentes_libres
}

completar_kde() {
	if test "x$KDE" = "x0"; then
		if ! pregunta_si "No parece que estés usando KDE. ¿Quieres continuar?"; then
			return
		fi
	fi
	instala_multimedia_libre_kde
    instala_programa "para KDE" kdenetwork-filesharing k4dirstat

}	

compartir_ficheros() {
# http://www.spencerstirling.com/computergeek/NFS_samba.html
    instala_programa "NFS" nfs-kernel-server nfs-common portmap
}

completar_lxde() {
	if test "x$LXDE" = "x0"; then
		if ! pregunta_si "No parece que estés usando LXDE. ¿Quieres continuar?"; then
			return
		fi
	fi
	completar_escritorio
	instala_programa "CD, pdf, etc." xfburn epdfview clementine gthumb
	if pregunta_si "¿Quieres instalar el control de batería?"; then
		apt-get install fdpowermon
	fi
	if pregunta_si "¿Quieres instalar el control de volumen?"; then
		instala_programa "Para compilar" build-essential libasound2-dev libglib2.0-dev libgtk-3-dev git libnotify-dev autoconf intltool
		cd /tmp
		git clone https://github.com/Maato/volumeicon.git
		cd volumeicon
		bash autogen.sh
		./configure --enable-notify
		make
		if make install; then
			if grep volumeicon /etc/xdg/lxsession/autostart; then
				echo "Ya estaba añadido a autoarranque"
			else
				echo "@volumeicon" >> /etc/xdg/lxsession/autostart
				ech "Instalado"
			fi
		else	
			echo "Error instalando el control de volumen"
		fi
	fi
}	


completar_gnome() {
	if test "x$GNOME" = "x0"; then
		if ! pregunta_si "No parece que estés usando GNOME. ¿Quieres continuar?"; then
			return
		fi
	fi
	completar_escritorio
	instala_multimedia_libre_gnome	
}

instala_multimedia_prop() {
	instala_multimedia_libre
	case $DISTRO in
	debian*)
		instala_programa "Codecs mp3" lame gstreamer0.10-plugins-ugly
		;;
	*)
		instala_programa "Codecs mp3" lame lame-extras gstreamer0.8-lame w32codecs
		;;
	esac
	if test "x$KDE" = "x1"; then
		instala_programa kubuntu_restricted_extras
	fi
	if test "x$GNOME" = "x1"; then
		instala_programa ubuntu_restricted_extras
	fi
}

instala_multimedia_libre_kde() {
	instala_programa "Multimedia para KDE" amarok
	instala_programa "k3b" k3b dvd+rw-tools
}

instala_multimedia_libre_gnome() {
	instala_programa "Multimedia para GNOME" vlc
}

borra_programas_huerfanos() {
	instala_programa "Deborphan" deborphan
	echo "Estos paquetes parece que están huérfanos"
	deborphan --guess-all > /tmp/deborphan.list
	cat /tmp/deborphan.list
    if pregunta_si "Quieres eliminarlos?"; then
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

instala_programas_impresora() {
	instala_programa "Impresoras" smbclient cups cups-browsed printer-driver-hpcups system-config-printer-udev
}

instala_impresora_hp() {
# Se necesita python-gobject para hp-sendfax: https://answers.launchpad.net/hplip/+question/30741
	instala_programa "Impresoras HP" hplip hplip-gui python-gobject hp-ppd hplip-ppds
	echo "Para configurar tu impresora, ve al menú principal 'K', elige 'Sistema' y luego 'HPLIP ToolBox - Printer Toolbox'"
	pulsa_tecla
}

detectar_wifi() {
	clear;
	lspci | grep [Ww]ireless
	dmesg | grep [Ff]irmware
        dmesg | grep [Ww]ireless
}

instala_firmware() {
	FIRMWARE=firmware.tar.gz
	if ! -f $FIRMWARE; then
		FIRMWARE=$(find . -name firmware.tar.gz)
	fi
	if ! -f $FIRMWARE; then
		echo "No se ha encontrado el archivo firmware.tar.gz. Por favor, cópialo en $(pwd)"
		exit 1
	fi
    cd /lib
    tar -zxvf $FIRMWARE .
    pausa
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
	case $DISTRO in
	debian*)
	instala_programa "Necesario para skype" libqt4-dbus libqt4-network libqt4-xml libqtwebkit4 libxss1 libasound2-plugins
        wget -O /tmp/skype-install.deb http://www.skype.com/go/getskype-linux-deb
        dpkg -i /tmp/skype-install.deb || return
        rm skype-install.deb
        pausa
        ;;
    *)
        firefox http://www.skype.com/intl/en/get-skype/on-your-computer/linux/
        ;;
    esac
}

instala_flash_player() {
	case $DISTRO in
	debian*)
		instala_programa "Flash plugin" flashplugin-nonfree
        ;;
    *)
	echo "No sé como instalarlo en tu sistema"
        ;;
    esac
    pausa
}

reloj_hora() {
	which_ntpdate=`which ntpdate`
	if [ ! -e "$which_ntpdate" -o ! -x "$which_ntpdate" ]; then
		instala_programa "Programa para sincronizar la hora" ntpdate
	fi
	ntpdate $NTPDATE_SERVER
	pulsa_tecla
}

language_espanol() {
	case $DISTRO in 
	ubuntu*)
		echo $(check-language-support -l es)
		instala_programa "Escritorio en español" $(check-language-support -l es)
		;;
	debian*)
		echo "Lo siento, tienes que hacerlo a mano"
		read
	esac
}

instala_fuentes_windows_kk() {
	instala_programa "Fuentes compatibles windows kk" $MSFONTS
}

instala_fuentes_libres() {
	instala_programa "Fuentes libres" $FREEFONTS
}

instala_libreoffice() {
	case $DISTRO in
	debian*)
		instala_programa "LibreOffice" $LIBREOFFICE_COMMON $LIBREOFFICE_DEBIAN
		;;
	*)
		instala_programa "LibreOffice" $LIBREOFFICE_COMMON $LIBREOFFICE_OTHER
		;;
	esac
        if test "x$KDE" = "x1"; then
		instala_programa "LibreOffice KDE" libreoffice-kde libreoffice-style-oxygen
        fi
        if test "x$GNOME" = "x1"; then
		instala_programa "LibreOffice KDE" libreoffice-gtk
        fi
}

instala_thunderbird() {
	case $DISTRO in
	debian*)
		instala_programa "Thunderbird" icedove icedove-l10n-$LANGUAGE* enigmail
		;;
	*)
		instala_programa "Thunderbird" thunderbird thunderbird-locale-$LANGUAGE* enigmail
		;;
	esac
}

instala_gestiong() {
	case $DISTRO in 
	debian*)
		case $DISTRO_VER in
			7|8) instala_programa "GestiONG" build-essential libtool autoconf libpoco-dev libqt4-dev libboost-dev libxml2-dev libmysqlclient-dev libdb-dev libjpeg-dev libpng-dev libboost-regex-dev gawk libsqlite3-dev
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


borra_dev_doc() {
	# borrar los paquetes -dev
	apt-get autoremove --purge $(dpkg -l "*-dev$*" | grep ii | grep -v libqt4-dev | grep -v python-dbus-dev | awk '{print $2}')
	# borrar los paquetes -doc
	apt-get autoremove --purge $(dpkg -l "*-doc$*" | grep ii | awk '{print $2}')
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
			--menu "Elige la sección" 20 75 5 \
			1 "Sistema: actualizar software, idioma, reloj, etc." \
			2 "Escritorio" \
			3 "Software libre" \
			4 "Software no libre" \
			5 "Hardware: impresoras, discos externos, wifi, etc." 
		case `cat /tmp/menuoption.txt` in
		1 )		menu_sistema ;;
		2 )		menu_escritorio ;;
		3 )		menu_software_libre ;;
		4 )		menu_software_no_libre ;;
		5 )		menu_hardware ;;
		* )	break ;;
		esac
	done
}

menu_sistema() {
	while [ 1 ]; do
		BACKTITLE=`uname -a`
		my_dialog --cancel-label "Volver" \
			--title "=== SISTEMA ===" \
			--menu "Elige la sección" 20 75 6 \
			1 "Reparar el sistema de paquetes" \
			2 "Actualizar el sistema" \
			3 "Descargar versiones en español de los programas instalados" \
			4 "Poner el reloj en hora" \
			5 "Compartir los archivos de este ordenador" \
			6 "Limpieza" \
			9 "Ver si hay una nueva versión de este programa"
		case `cat /tmp/menuoption.txt` in
		1 )     clear; apt-get -f install; dpkg --configure -a; pulsa_tecla ;;
		2 )     clear; aptgetupdate ;;
		3 ) 	clear; language_espanol ;;
		4 )		clear; reloj_hora ;;
# http://www.spencerstirling.com/computergeek/NFS_samba.html
		5 ) 	clear; 
				instala_programa "NFS" nfs-kernel-server nfs-common portmap
				if test "x$KDE" = "x1"; then
					instala_programa "GUI de kde para compartir archivos" kdenetwork-filesharing
				fi 
				;;
		6 ) 	menu_limpieza ;;
		9 ) 	check_new_version ;;
		* )		break ;;
		esac
	done
}


menu_escritorio() {
	while [ 1 ]; do
		my_dialog --title "=== MENU ESCRITORIO ===" \
			--menu "Elige la operación sobre el escritorio" 20 75 3 \
			1 "Completar KDE" \
			2 "Completar GNOME y MATE" \
			3 "Completar LXDE"
		case `cat /tmp/menuoption.txt` in
		1 )     clear; completar_kde;;
		2 )     clear; completar_gnome;;
		3 )     clear; completar_lxde;;
		* )	break ;;
		esac
	done
}


menu_software_libre() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== INSTALACIÓN DE SOFTWARE LIBRE ===" \
			--menu "Elige los programas a instalar" 20 75 8 \
			1 "LibreOffice completo" \
			2 "Gráficos: Photoshop, etc." \
			3 "Thunderbird para leer mi correo (outlook)" \
			4 "Programas para administradoras del sistema" \
			5 "GestiONG" 
		case `cat /tmp/menuoption.txt` in
		1 ) 	clear; instala_libreoffice ;;
		2 )		menu_graficos ;;
		3 ) 	clear; instala_thunderbird ;;
		4 )		clear; instala_programa "Administración del sistema" rsync mc nmap openssh-server ;;
		5 )		instala_gestiong ;;
		* )		break ;;
		esac
	done
}

menu_software_no_libre() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== INSTALACIÓN DE PROGRAMAS ===" \
			--menu "Elige los programas a instalar" 20 75 9 \
			1 "Instalar fuentes propietarias de Windows kk" \
			2 "Codecs propietarios mp3 y otros compatibles con Windows kk" \
			3 "Programas propietarios: Flash, Skype, Spotify, etc." 
		case `cat /tmp/menuoption.txt` in
		1 )     clear; instala_fuentes_windows_kk ;;
		2 ) 	clear; instala_multimedia_prop ;;
		3 ) 	menu_software_propietarios ;;
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
			3 "Quiero que funcione mi tarjeta wifi (Inalámbrica)" \
			4 "Instalar firmware"
		case `cat /tmp/menuoption.txt` in
		1 )	menu_impresoras ;;
		2 )	instala_programa "Sistema de archivos ntfs" ntfs-3g ;;
		3 )	menu_internet_inalambrico ;;
		4 ) instala_firmware ;;
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
			--menu "Elige el programa a instalar" 20 75 3 \
			1 "Skype"  \
			2 "Acrobar Reader para PDF" \
			3 "Flash plugin"
		case `cat /tmp/menuoption.txt` in
		1 )	instala_skype ;;
		2 ) 	instala_acrobar_reader ;;
		3 )     instala_flash ;;
		* )	break ;;
		esac
	done
}


menu_limpieza() {
	while [ 1 ]; do
		my_dialog --cancel-label "Volver" \
			--title "=== LIMPIEZA ===" \
			--menu "Elige la sección" 20 75 7 \
			1 "Liberar espacio en mi carpeta personal" \
            2 "Liberar espacio del sistema" \
			3 "Borrar programas de desarrollo y de documentación" \
			4 "Eliminar bluetooh" \
			5 "Eliminar modem" \
			6 "Eliminar programas de KDE" \
			7 "Borrar programas huérfanos" 
		case `cat /tmp/menuoption.txt` in
        1 )     clear; liberar_espacio_home; pulsa_tecla ;;
		2 ) 	clear; liberar_espacio_sistema; pulsa_tecla ;;
		3 )	    clear; borra_dev_doc; pulsa_tecla ;;
		4 )		clear; borra_bluetooh ;;
		5 ) 	clear; borra_modem ;;
		6 )		clear; borra_programas_kde; pulsa_tecla ;;
		7 ) 	clear; borra_programas_huerfanos ;;
		* )		break ;;
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

liberar_espacio_sistema() {
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
}

liberar_espacio_home() {
	clear
	df -h >> /tmp/espacioendisco.txt

    cd $HOME
    rm -rf .cache/* .mozilla/firefox/Crash Reports .thumbnails/*


	clear
	echo "Espacio antes de liberar"
	cat /tmp/espacioendisco.txt
	rm /tmp/espacioendisco.txt
	echo
	echo "Espacio despues de liberar"
	df -h
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

EOF
				pulsa_tecla
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
