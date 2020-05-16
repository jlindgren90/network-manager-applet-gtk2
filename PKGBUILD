# Maintainer: John Lindgren <john@jlindgren.net>
# Maintainer: Jan de Groot <jgc@archlinux.org>
# Maintainer: Jan Alexander Steffens (heftig) <jan.steffens@gmail.com>
# Contributor: Arjan Timmerman <arjan@archlinux.org>
# Contributor: Wael Nasreddine <gandalf@siemens-mobiles.org>
# Contributor: Tor Krill <tor@krill.nu>
# Contributor: Will Rea <sillywilly@gmail.com>

pkgbase=network-manager-applet-gtk2
pkgname=(nm-connection-editor-gtk2 network-manager-applet-gtk2)
pkgdesc="Applet for managing network connections (GTK2 version)"
url="https://wiki.gnome.org/Projects/NetworkManager/"
pkgver=1.8.2
pkgrel=1
arch=(i686 x86_64)
license=(GPL2 LGPL2.1)
depends=(libnm-glib libgudev gtk2 mobile-broadband-provider-info iso-codes)
makedepends=(libsecret libnotify libmm-glib intltool gobject-introspection git gtk-doc)
options=(!emptydirs)

prepare() {
  cd ..
  mkdir -p nma/etc/xdg nma/usr/bin \
    nma/usr/share/{applications,icons/hicolor/22x22/apps,man/man1,nm-applet}
  NOCONFIGURE=1 ./autogen.sh
}

build() {
  cd ..
  ./configure --prefix=/usr \
    --sysconfdir=/etc \
    --localstatedir=/var \
    runstatedir=/run \
    --sbindir=/usr/bin \
    --libexecdir=/usr/lib/NetworkManager \
    --disable-maintainer-mode \
    --disable-static \
    --enable-gtk-doc \
    --enable-introspection \
    --enable-ld-gc \
    --with-team \
    --with-wwan \
    --without-appindicator

  # https://bugzilla.gnome.org/show_bug.cgi?id=655517
  sed -i -e 's/ -shared / -Wl,-O1,--as-needed\0/g' libtool

  make
}

check() {
  cd ..
  make -k check
}

package_nm-connection-editor-gtk2() {
  pkgdesc="NetworkManager GUI connection editor and widgets (GTK2 version)"
  provides=("libnm-gtk=$pkgver-$pkgrel" "libnma=$pkgver-$pkgrel")
  conflicts=(nm-connection-editor libnm-gtk)
  replaces=(nm-connection-editor libnm-gtk)

  cd ..
  make DESTDIR="$pkgdir" install

### Split network-manager-applet
  cd nma
  mv "$pkgdir"/etc/xdg/autostart etc/xdg/
  mv "$pkgdir"/usr/bin/nm-applet usr/bin/
  mv "$pkgdir"/usr/share/GConf usr/share/

  cd usr/share
  mv "$pkgdir"/usr/share/applications/nm-applet.desktop applications/
  mv "$pkgdir"/usr/share/man/man1/nm-applet.1 man/man1/

  cd icons/hicolor/22x22
  mv "$pkgdir"/usr/share/icons/hicolor/22x22/apps/nm-{adhoc,mb-roam,wwan-tower}.png apps/
  mv "$pkgdir"/usr/share/icons/hicolor/22x22/apps/nm-{secure,vpn-active}-lock.png apps/
  mv "$pkgdir"/usr/share/icons/hicolor/22x22/apps/nm-{signal,tech}-*.png apps/
  mv "$pkgdir"/usr/share/icons/hicolor/22x22/apps/nm-*connecting*.png apps/
}

package_network-manager-applet-gtk2() {
  depends=(nm-connection-editor-gtk2 libmm-glib libnotify libsecret)
  conflicts=(network-manager-applet)
  replaces=(network-manager-applet)
  mv ../nma/* "$pkgdir"
}
