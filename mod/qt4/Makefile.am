AM_CPPFLAGS = @GONGLIB_CPPFLAGS@ -DQT_TRANSLATIONS=\""@QT_TRANSLATIONS@"\"

lib_LTLIBRARIES = libgongqt4.la
libgongqt4_ladir = $(includedir)/gonglib
libgongqt4_la_LDFLAGS = -version-info 0:5:0
libgongqt4_la_SOURCES = gongcombobox.cpp gongguiapplication.cpp gongguimainwindow.cpp \
	gongguicontrols.cpp gongdatatable.cpp gongfrmbase.cpp \
	gonglineedit.cpp gongosdwidget.cpp gongguimultiplelistbox.cpp \
	gongtextedit.cpp gongrichtextedit.cpp gongcheckbox.cpp \
	gongpushbutton.cpp gongimagebox.cpp gongfrmimgadvanced.cpp

libgongqt4_la_HEADERS =	gongguinamespace.h gongguiapplication.h \
	gongguimainwindow.h gongpushbutton.h \
	gongcombobox.h gongguicontrols.h gongdatatable.h gongfrmbase.h \
	gonglineedit.h gongosdwidget.h gongguimultiplelistbox.h \
	gongtextedit.h gongtreeitem.h gongrichtextedit.h gongcheckbox.h \
	gongq3treeitem.h gongimagebox.h gongfrmimgadvanced.h

nodist_libgongqt4_la_SOURCES = \
	moc_gongdatatable.cpp moc_gongfrmbase.cpp \
	moc_gongguicontrols.cpp moc_gongfrmimgadvanced.cpp \
	moc_gongguimainwindow.cpp moc_gonglineedit.cpp \
	moc_gongosdwidget.cpp moc_gongrichtextedit.cpp \
	moc_gongtextedit.cpp moc_gongimagebox.cpp

if COMPILE_EXPERIMENTAL
libgongqt4_la_SOURCES += gongviewtable.cpp
libgongqt4_la_HEADERS += gongviewtable.h
nodist_libgongqt4_la_SOURCES += moc_gongviewtable.cpp
endif

moc_%.cpp: %.h
	moc $< -o $@



