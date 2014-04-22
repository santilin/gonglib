#!/bin/bash

PING_TEST_IP="208.67.222.222"
NORMAL_USER=`who | cut -d " " -f1`

set -o functrace
trap 'echo -ne "\e]0;$BASH_COMMAND\007"' DEBUG
export PS1="\e]0;$TERM\007$PS1"

init() {
#todo check you are root
	clear
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
	dialog_path=`which dialog`
	echo a${dialog_path}a
	if [[ -e $dialog_path ]] ; then
		echo Existe $dialog_path
	else
		echo No existe $dialog_path
#		aptitude install dialog -y
	fi	
	exit
	dialog_path=`which dialog`
	if [ -x $dialog_path ] ; then
		HAY_DIALOG=false
	else
		HAY_DIALOG=true
	fi	
	echo $HAY_DIALOG
	exit
}

welcome() {
	clear
	my_dialog --title "*** MENÚ PRINCIPAL ***" \
		--menu "Elige la sección" 20 60 3 \
		configuracion "Configuración" programas "Instalación de programas" 
	case `cat /tmp/menuoption.txt` in
	configuracion)	configuracion ;;
	programas)	programas ;;
	esac
}

configuracion() {
	clear
	my_dialog --title "*** CONFIGURACIÓN ***" \
		--menu "Elige la sección" 20 60 3 \
		repositorios "Repositorios de programas de internet" \
		update_time  "Poner el reloj en hora" 2> /tmp/menuoption.txt
	case `cat /tmp/menuoption.txt` in
	repositorios)	repositorios ;;
	update_time)	ntpdate pool.ntp.org ;;
	esac
}

repositorios() {

	aptitude update
	aptitude safe-upgrade -y
	cat >/etc/apt/sources.list <<EOF
deb http://ftp.fr.debian.org/debian lenny main contrib

deb http://security.debian.org lenny/updates main contrib
deb-src http://security.debian.org lenny/updates main contrib

deb http://volatile.debian.org/debian-volatile lenny/volatile main contrib
deb-src http://volatile.debian.org/debian-volatile lenny/volatile main contrib
EOF
	aptitude update
	aptitude safe-upgrade -y
}

programas() {
	clear
	dialog --title "*** INSTALACIÓN DE PROGRAMAS ***" \
		--menu "Elige la sección" 20 60 3 \
		basico "Programas básicos" multimedia "Multimedia" graficos "Gráficos"  2> /tmp/menuoption.txt
	case `cat /tmp/menuoption.txt` in
	basicos)	basicos ;;
	multimedia)	multimedia ;;
	graficos)	graficos ;;
	perifericos)	perifericos ;;
	esac
}

multimedia() {

	clear
	dialog --title "*** MENÚ MULTIMEDIA ***" \
		--menu "Elige los programas a instalar" 20 60 3 \
		libre "Codecs y programas libres" mp3 "Codecs mp3" feo "Codecs feos" 2> /tmp/menuoption.txt
	case `cat /tmp/menuoption.txt` in
	libre)	instala_programa "Multimedia libre" vlc kaffeine mplayer ;;
	basico) instala_programa "Codecs mp3" ;;
	feos) instala_programa "Codecs feos" ;;
	esac
}

perifericos() {

	clear
	dialog --title "*** MENÚ PERIFÉRICOS ***" \
		--menu "Elige el periférico a instalar" 20 60 3 \
		impresora "Impresora" 2> /tmp/menuoption.txt
	case `cat /tmp/menuoption.txt` in
	impresora)	instala_programa "Impresoras" cupsys smbclient ;;
	esac
}


graficos() {

	clear
	dialog --title "*** MENÚ PROGRAMAS GRÁFICOS ***" \
		--menu "Elige el programa a instalar" 20 60 2 \
		gimp "Gimp -- Retoque fotográfico" inkscape "InkScape -- Dibujo vectorial" 2> /tmp/menuoption.txt
	case `cat /tmp/menuoption.txt` in
	gimp)		instala_programa Gimp gimp ;;
	inkscape) 	instala_programa InkScape inkscape ;;
	esac
}


instala_programa() {

	clear
	echo "Instalando $1"
	shift
	aptitude -y install $2 
	read
}


desinstalar_kde() {

	echo "Desinstalar kde"
#  kde: Depende: kdeaccessibility (>= 4:3.5.5) pero no es instalable
#        Depende: kdeaddons (>= 4:3.5.5) pero no es instalable
#        Depende: kdeedu (>= 4:3.5.5) pero no es instalable
#        Depende: kdegames (>= 4:3.5.5) pero no es instalable
#        Depende: kdetoys (>= 4:3.5.5) pero no es instalable
#        Depende: kdewebdev (>= 4:3.5.5) pero no es instalable
#   kdeutils: Depende: ark (>= 4:3.5.9-2) pero no es instalable
#             Depende: kcalc (>= 4:3.5.9-2) pero no es instalable
#             Depende: kcharselect (>= 4:3.5.9-2) pero no es instalable
#             Depende: kdelirc (>= 4:3.5.9-2) pero no es instalable
#             Depende: kdessh (>= 4:3.5.9-2) pero no es instalable
#             Depende: kdf (>= 4:3.5.9-2) pero no es instalable
#             Depende: kedit (>= 4:3.5.9-2) pero no es instalable
#             Depende: kfloppy (>= 4:3.5.9-2) pero no es instalable
#             Depende: kgpg (>= 4:3.5.9-2) pero no es instalable
#             Depende: khexedit (>= 4:3.5.9-2) pero no es instalable
#             Depende: kjots (>= 4:3.5.9-2) pero no es instalable
#             Depende: kmilo (>= 4:3.5.9-2) pero no es instalable
#             Depende: ktimer (>= 4:3.5.9-2) pero no es instalable
#             Depende: superkaramba (>= 4:3.5.9-2) pero no es instalable
}

run_command() {
	echo $*
	`$*`	
}

check_internet() {
	ping -n -q -c 3 $PING_TEST_IP
	local result=$?
	while true; do 
		if [ $result != 0 ]; then
			cat <<EOF

HORROR: No estás conectada a Internet.

¿Qué quieres hacer?
1) Probar otra vez
2) Intentar conectarme, ya lo he solucionado
3) Configurar la conexión
4) Continuar con el programa sin Internet
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
				su -c "kcmshell kcm_knetworkconfmodule" $NORMAL_USER ; 
				exit ;;

			4)	return 2 ;;

			9)	echo "Hasta la vista"; exit ;;
			esac	
			ping -n -v -c 3 $PING_TEST_IP
			result=$?
		else
			return 0
		fi
	done
	exit
}

init	
welcome

my_dialog() {
	if [ $HAY_DIALOG ]; then
		dialog $* 2> /tmp/menuoption.txt
	else
		echo "No hay dialog"
		echo $*
	fi
}
