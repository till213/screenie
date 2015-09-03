macx {
  include(Distribution_Mac.pri)
}
win32 {
  include(Distribution_Windows.pri)
}
linux {
  include(Distribution_Linux.pri)
}

installer.depends = distribution

# Remove previous bundle
i18n.commands += test -d $${DIST_DIR} && rm -rf $${DIST_DIR};
i18n.commands += mkdir dist;
i18n.commands += mkdir $${DIST_DIR}/translations;
i18n.commands += lrelease \"$$PWD/src/translations/screenie_de.ts\" -qm $${DIST_DIR}/translations/screenie_de.qm;
!linux {
  i18n.commands += test -f $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm && cp $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm $${DIST_DIR}/translations/qtbase_de.qm;
}
i18n.depends = all

distribution.depends  = i18n

help.commands += @echo 'Available Custom Commands:';
help.commands += echo 'distribution - Copy all files required for distribution into directory dist';
help.commands += echo 'installer - Create an Installer. Mac: PKG installer - Windows: MSI installer';
help.commands += echo 'diskimage - Mac only: Create a Disk Image for installation';
help.commands += echo 'codesign - Mac only: sign all frameworks, plugins and the bundle';
help.commands += echo 'dox: create Doxygen documentation';
help.commands += echo 'help - This help text';

doxygen.depends  =
doxygen.target   = dox
doxygen.commands = doxygen Doxyfile

QMAKE_EXTRA_TARGETS += i18n distribution installer doxygen help
