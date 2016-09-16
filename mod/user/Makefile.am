AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgonguser.la
libgonguser_ladir = $(includedir)/gonglib

libgonguser_la_SOURCES = usermodule.cpp \
	userfrmeditrol.cpp userfrmeditusuaria.cpp \
	userrecrol.cpp userrecusuaria.cpp \
	userfrmlogin.cpp
	

libgonguser_la_HEADERS = usermodule.h \
	userfrmeditrol.h userfrmeditusuaria.h \
	userrecrol.h userrecusuaria.h \
	userfrmlogin.h

nodist_libgonguser_la_SOURCES = \
	moc_userfrmeditrol.cpp moc_userfrmeditusuaria.cpp \
	moc_usermodule.cpp moc_userfrmlogin.cpp

moc_%.cpp: %.h
	moc $< -o $@

user_sharedir = $(datadir)/gestiong/user
dist_user_share_DATA = $(srcdir)/share/user.ddd

