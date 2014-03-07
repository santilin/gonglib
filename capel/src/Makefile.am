AM_CPPFLAGS=@AM_CPPFLAGS@ @GONGLIB_CPPFLAGS@
LIBS=@GONGLIB_LIBS@

bin_PROGRAMS = capel

capel_SOURCES = main.cpp capelcppmodule.cpp capelmodule.cpp \
	capelrtkmodule.cpp capelphpmodule.cpp \
	gonggenerators.cpp generalgenerator.cpp \
	gonggenfrmeditrecmaestro.cpp gonggendbmodule.cpp gonggenrecords.cpp \
	gonggenfrmeditbehavior.cpp \
	rtkcppclass.cpp rtkserver.cpp ventadirecta.cpp

noinst_HEADERS = capelmodule.h capelcppmodule.h \
	capelrtkmodule.h capelphpmodule.h \
	gonggenerators.h generalgenerator.h \
	gonggendbmodule.h \
	gonggenfrmeditrecmaestro.h gonggenrecords.h \
	rtkcppclass.h
	
if ADDON_VENTADIRECTA
	noinst_HEADERS += ventadirecta.h
	capel_SOURCES += ventadirecta.cpp
endif