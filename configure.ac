AC_PREREQ([2.63])

m4_define([nma_major_version], [1])
m4_define([nma_minor_version], [8])
m4_define([nma_micro_version], [2])
m4_define([nma_version],
          [nma_major_version.nma_minor_version.nma_micro_version])

AC_INIT([nm-applet],
        [nma_version],
        [https://bugzilla.gnome.org/enter_bug.cgi?product=NetworkManager],
        [network-manager-applet])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz -Wno-portability])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

NMA_MAJOR_VERSION=nma_major_version
NMA_MINOR_VERSION=nma_minor_version
NMA_MICRO_VERSION=nma_micro_version
NMA_VERSION=nma_version
AC_SUBST(NMA_MAJOR_VERSION)
AC_SUBST(NMA_MINOR_VERSION)
AC_SUBST(NMA_MICRO_VERSION)
AC_SUBST(NMA_VERSION)

dnl
dnl Require programs
dnl
AC_PROG_CC
AM_PROG_CC_C_O
PKG_PROG_PKG_CONFIG

dnl Define _GNU_SOURCE for various things like strcasestr()
AC_USE_SYSTEM_EXTENSIONS

dnl Initialize libtool
LT_PREREQ([2.2.6])
LT_INIT

GIT_SHA_RECORD(NMA_GIT_SHA)

dnl
dnl Checks for typedefs, structures, and compiler characteristics.
dnl
AC_TYPE_PID_T

dnl
dnl translation support
dnl
IT_PROG_INTLTOOL([0.50.1])

GETTEXT_PACKAGE=nm-applet
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE", [Gettext package])
AM_GLIB_GNU_GETTEXT

# Check for iso-codes for country names translation
AC_MSG_CHECKING([whether to disable iso-codes at build-time])
AC_ARG_ENABLE([iso-codes],
    AS_HELP_STRING([--disable-iso-codes],[do not check for iso-codes at build-time]),
    [],[disable_iso_codes_check=no])
if test x$disable_iso_codes_check = xno ; then
    AC_MSG_RESULT([no])
else
    AC_MSG_RESULT([yes])
fi

if test x$disable_iso_codes_check = "xno" ; then
    AC_MSG_CHECKING([whether iso-codes has iso_3166 domain])
    if $PKG_CONFIG --variable=domains iso-codes | grep iso_3166 >/dev/null ; then
        AC_MSG_RESULT([yes])
    else
        AC_MSG_RESULT([no])
    fi
    AC_DEFINE_UNQUOTED([ISO_CODES_PREFIX],["`$PKG_CONFIG --variable=prefix iso-codes`"],[ISO codes prefix])
    PKG_CHECK_MODULES(ISO_CODES, [iso-codes],
        [],
        [echo -e "\n$ISO_CODES_PKG_ERRORS.\n"
         echo "Consider installing the package or adjusting the PKG_CONFIG_PATH environment variable."
         echo "You can also disable build-time check for 'iso-codes' via --disable-iso-codes";
         exit 1;])
else
    AC_DEFINE_UNQUOTED([ISO_CODES_PREFIX],["$prefix"],[ISO codes prefix])
fi

dnl
dnl API documentation
dnl
GTK_DOC_CHECK(1.0)

AC_ARG_WITH(libnm-gtk, AS_HELP_STRING([--without-libnm-gtk], [dont build legacy library libnm-gtk which depends on libnm-glib/libnm-util/dbus-glib]))
if test "$with_libnm_gtk" != no; then
	PKG_CHECK_MODULES(LIBNM_GLIB,
			[gio-2.0 >= 2.32
			 NetworkManager >= 1.7
			 libnm-glib >= 1.7
			 libnm-util >= 1.7
			 libnm-glib-vpn >= 1.7
			 gmodule-export-2.0],
			 :,
			 [AC_MSG_FAILURE([$LIBNM_GLIB_PKG_ERRORS

Configure with --without-libnm-gtk if you do not need the legacy library])])
	with_libnm_gtk=yes
	LIBNM_GLIB_CFLAGS="$LIBNM_GLIB_CFLAGS -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_32"
	LIBNM_GLIB_CFLAGS="$LIBNM_GLIB_CFLAGS -DNM_VERSION_MIN_REQUIRED=NM_VERSION_1_4"
	LIBNM_GLIB_CFLAGS="$LIBNM_GLIB_CFLAGS -DNM_VERSION_MAX_ALLOWED=NM_VERSION_1_4"
fi
AM_CONDITIONAL(WITH_LIBNM_GTK, test "$with_libnm_gtk" != "no")

PKG_CHECK_MODULES(LIBNM, [libnm >= 1.7 gio-2.0 >= 2.32 gmodule-export-2.0])
LIBNM_CFLAGS="$LIBNM_CFLAGS -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_32"
LIBNM_CFLAGS="$LIBNM_CFLAGS -DNM_VERSION_MIN_REQUIRED=NM_VERSION_1_8"
LIBNM_CFLAGS="$LIBNM_CFLAGS -DNM_VERSION_MAX_ALLOWED=NM_VERSION_1_8"

PKG_CHECK_MODULES(LIBSECRET, [libsecret-1 >= 0.18])

# Check for libnotify >= 0.7
PKG_CHECK_MODULES(LIBNOTIFY_07, [libnotify >= 0.7], [have_libnotify_07=yes],[have_libnotify_07=no])
if test x"$have_libnotify_07" = "xyes"; then
	AC_DEFINE(HAVE_LIBNOTIFY_07, 1, [Define if you have libnotify 0.7 or later])
fi
PKG_CHECK_MODULES(NOTIFY, [libnotify >= 0.4.3])

PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.24)
GTK_CFLAGS="$GTK_CFLAGS"

AC_ARG_WITH([appindicator], AS_HELP_STRING([--with-appindicator|--without-appindicator], [Build with libappindicator support instead of xembed systray support.]))
if test "$with_appindicator" == "yes"; then
	PKG_CHECK_MODULES(APPINDICATOR, appindicator3-0.1)
	PKG_CHECK_MODULES(DBUSMENU, dbusmenu-gtk3-0.4 >= 16.04.0)
	AC_DEFINE([WITH_APPINDICATOR], 1, [Enable using libappindicator])
fi

AM_CONDITIONAL(HAVE_GBT, test x"$have_gbt" = "xyes")

dnl ModemManager1 with libmm-glib for WWAN support
AC_ARG_WITH(wwan, AS_HELP_STRING([--with-wwan], [Enable WWAN support (default: yes)]))
if (test "${with_wwan}" == "no"); then
    AC_DEFINE(WITH_WWAN, 0, [Define if you have ModemManager/WWAN support])
else
    PKG_CHECK_MODULES(MM_GLIB,
                      [mm-glib],,
                      AC_MSG_ERROR([libmm-glib is needed for WWAN support. Use --without-wwan to build without it.]))
    AC_DEFINE(WITH_WWAN, 1, [Define if you have ModemManager/WWAN support])
fi
AM_CONDITIONAL(WITH_WWAN, test "${with_wwan}" != "no")

dnl Check for gudev
PKG_CHECK_MODULES(GUDEV, gudev-1.0 >= 147)

dnl Jansson for team configuration editing
AC_ARG_WITH(team, AS_HELP_STRING([--with-team], [Enable team configuration editor (default: yes)]))
if (test "${with_team}" == "no"); then
    AC_DEFINE(WITH_JANSSON, 0, [Define if Jansson is available])
else
    PKG_CHECK_MODULES(JANSSON,
                      [jansson >= 2.3],,
                      AC_MSG_ERROR([jansson is needed for team configuration editor. Use --without-team to build without it.]))
    AC_DEFINE(WITH_JANSSON, 1, [Define if Jansson is available])
fi
AM_CONDITIONAL(WITH_JANSSON, test "${with_team}" != "no")

dnl Check for gobject introspection
GOBJECT_INTROSPECTION_CHECK([0.9.6])

GLIB_CONFIG_NMA
GLIB_GSETTINGS
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

NM_COMPILER_WARNINGS([yes])
NM_LTO
NM_LD_GC


AC_ARG_WITH(more-asserts,
            AS_HELP_STRING([--with-more-asserts=level], [Enable more assertions for debugging (default: 0)]),
            [more_asserts=${with_more_asserts}],
            [more_asserts=no])
if test "${more_asserts}" = "no"; then
    more_asserts=0
else
    if test "${more_asserts}" = "yes"; then
        more_asserts=100
    fi
fi
AC_DEFINE_UNQUOTED(NM_MORE_ASSERTS, $more_asserts, [Define if more asserts are enabled])


AC_CONFIG_FILES([
Makefile
src/libnm-gtk/libnm-gtk.pc
src/libnma/libnma.pc
po/Makefile.in
man/nm-applet.1
man/nm-connection-editor.1
org.gnome.nm-applet.gschema.xml
src/libnma/nma-version.h
])
AC_OUTPUT

echo ""
echo "  Build legacy library libnm-gtk: --with-libnm-gtk=${with_libnm_gtk}"
echo "  LTO: --enable-lto=$enable_lto"
echo "  Linker garbage collection: --enable-ld-gc=$enable_ld_gc"
echo ""
