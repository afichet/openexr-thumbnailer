AC_PREREQ(2.59)

AC_INIT([gnome-openexr-thumbnailer], [1.0.0], [alban.fichet@inria.fr])
AM_INIT_AUTOMAKE([foreign])

# Enable silent build when available (Automake 1.11)
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_SED
AM_PROG_CC_C_O

PKG_CHECK_MODULES(THUMBNAILER, gdk-pixbuf-2.0 gio-2.0 OpenEXR IlmBase)

dnl AC_PATH_PROG(UPDATE_MIME_DATABASE, update-mime-database, no)

dnl AC_ARG_ENABLE(update-mimedb,
dnl 	      AS_HELP_STRING([--disable-update-mimedb],
dnl 			     [disable the update-mime-database after install [default=no]]),,
dnl 			     enable_update_mimedb=yes)
dnl AM_CONDITIONAL(ENABLE_UPDATE_MIMEDB, test x$enable_update_mimedb = xyes)
AM_CONDITIONAL(ENABLE_UPDATE_MIMEDB, false)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
