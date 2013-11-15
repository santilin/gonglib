AM_CPPFLAGS = @AM_CPPFLAGS@ -DPACKAGE_DATA_DIR=\""$(datadir)"\"
AM_CXXFLAGS = @AM_CXXFLAGS@

INCLUDES = @GONGLIB_CPPFLAGS@

gestiong_LDADD = @GONGLIB_LIBS@
empresasg_LDADD = @GONGLIB_LIBS@
integralg_LDADD = @GONGLIB_LIBS@

bin_PROGRAMS = gestiong empresasg integralg
gestiongdir = $(includedir)/gonglib/gestiong

gestiong_SOURCES = main.cpp gestiongmodule.cpp
gestiong_HEADERS = gestiongmodule.h gestiongmastertable.h

nodist_gestiong_SOURCES = moc_gestiongmodule.cpp

empresasg_SOURCES = main_empresasg.cpp

integralg_SOURCES = main_integralg.cpp


moc_%.cpp: %.h
	moc $< -o $@

gestiong_sharedir = $(datadir)/gestiong
gestiong_share_DATA = $(top_srcdir)/share/gestiong/gestiong.ddd \
	$(top_srcdir)/share/gestiong/integralg.ddd
