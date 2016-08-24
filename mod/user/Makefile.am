AM_CPPFLAGS = @GONGLIB_CPPFLAGS@

lib_LTLIBRARIES = libgonguser.la
libgonguser_ladir = $(includedir)/gonglib

libgonguser_la_SOURCES = usermodule.cpp \
	userfrmeditpermiso.cpp userfrmeditrol.cpp userfrmeditusuaria.cpp \
	userrecpermiso.cpp userrecrol.cpp userrecusuaria.cpp 
	

libgonguser_la_HEADERS = usermodule.h \
	userfrmeditpermiso.h userfrmeditrol.p userfrmeditusuaria.h \
	userrecpermiso.h userrecrol.h userrecusuaria.h 

nodist_libgonguser_la_SOURCES = \
	moc_userfrmeditpermiso.cpp moc_userfrmeditrol.cpp moc_frmedituser.cpp 

moc_%.cpp: %.h
	moc $< -o $@

user_sharedir = $(datadir)/gestiong/user
dist_user_share_DATA = $(srcdir)/share/user.ddd

