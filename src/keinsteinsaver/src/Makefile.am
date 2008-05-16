# Makefile.am for keinsteinsaver.
#

AM_CPPFLAGS = -UQT_NO_ASCII_CAST
METASOURCES = AUTO

INCLUDES = $(all_includes)
LDFLAGS = $(all_libraries) $(KDE_RPATH)
LDADD = $(LIB_KDEUI) -lkscreensaver -lm
bin_PROGRAMS = keinsteinsaver.kss

keinsteinsaver_kss_SOURCES = keinsteinsaverui.ui keinsteinsaver.cpp
keinsteinsaver_kss_LDADD = $(LIB_QT) $(LIB_KDECORE) $(LDADD)  $(LIB_KSYCOCA)

#datadir = $(kde_datadir)/keinsteinsaver

desktop_DATA = keinsteinsaver.desktop
desktopdir = $(kde_appsdir)/System/ScreenSavers

EXTRA_DIST = $(desktop_DATA)

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/keinsteinsaver.pot
