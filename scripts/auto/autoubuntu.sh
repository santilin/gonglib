#!/bin/bash

# Fundamentales
apt-get install ibus-qt4

# LANGUAGE PACKS
apt-get install language-pack-es language-pack-kde-es language-pack-gnome-es \
	language-support-writing-es language-support-es

# LAMP
apt-get install apache2 php5 libapache2-mod-php5 mysql-server php5-mysql php5-gd
a2enmod rewrite
apache2ctl restart

# Gestiong
apt-get install build-essential gcc-4.4-doc libstdc++6-4.4-dbg subversion \
        libtool autoconf libqt3-mt-dev libboost-dev libxml2-dev \
	libmysqlclient-dev libdb4.8-dev



# Desarrollo web
apt-get instal quanta 


# Desinstalar evolution
apt-get remove evolution evolution-data-server evolution-plugins \
	evolution-indicator libevolution libebackend1.2-0 evolution-common \
	evolution-webcal 

