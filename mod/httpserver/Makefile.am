AM_CPPFLAGS = @GONGLIB_CPPFLAGS@
AM_CXXFLAGS = @AM_CXXFLAGS@ -std=gnu++11

libgonghttpserver_la_LDFLAGS = -version-info 0:5:0
libgonghttpserver_ladir = $(includedir)/gonghttpserver
lib_LTLIBRARIES = libgonghttpserver.la
libgonghttpserver_la_SOURCES = httpserverserver.cpp \
	httpserverbladeinterpreter.cpp

libgonghttpserver_la_HEADERS = httpserverserver.h server_http.hpp client_http.hpp \
	httpserverbladeinterpreter.h

# httpserver_sharedir = $(datadir)/gestiong/httpserver
# dist_httpserver_share_DATA = $(srcdir)/httpdocs/public/index.html


