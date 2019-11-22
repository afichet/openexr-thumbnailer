bin_PROGRAMS = gnome-openexr-thumbnailer

gnome_openexr_thumbnailer_SOURCES = \
	gnome-openexr-thumbnailer.cpp \
	gnome-thumbnailer-skeleton.c \
	gnome-thumbnailer-skeleton.h

gnome_openexr_thumbnailer_CPPFLAGS =		\
	$(THUMBNAILER_CFLAGS)			\
	-DTHUMBNAILER_RETURNS_PIXBUF		\
	-DTHUMBNAILER_USAGE="\"Thumbnail OpenEXR\""

gnome_openexr_thumbnailer_LDADD = $(THUMBNAILER_LIBS)

thumbnailerdir = $(datadir)/thumbnailers/
thumbnailer_DATA = gnome-openexr-thumbnailer.thumbnailer
gnome-openexr-thumbnailer.thumbnailer: gnome-openexr-thumbnailer.thumbnailer.in Makefile
	$(AM_V_GEN) $(SED) -e "s|\@bindir\@|$(bindir)|" $< > $@

mimedir = $(datadir)/mime/packages
mime_DATA = image-openexr.xml

install-data-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif

uninstall-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif

DISTCHECK_CONFIGURE_FLAGS = --disable-update-mimedb

EXTRA_DIST = \
	gnome-openexr-thumbnailer.thumbnailer.in \
	COPYING \
	README \
	image-openexr.xml

CLEANFILES = gnome-openexr-thumbnailer.thumbnailer
