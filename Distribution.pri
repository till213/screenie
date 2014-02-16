include(src/Common.pri)
macx {
  include(Distribution_Mac.pri)
}
win32 {
  include(Distribution_Windows.pri)
  distribution.depends = install
}

installer.depends = distribution

help.commands += @echo 'Available Custom Commands:';
help.commands += echo 'distribution - Copy all files required for distribution into directory dist';
help.commands += echo 'installer - Create an Installer. Mac: PKG installer - Windows: MSI installer';
help.commands += echo 'diskimage - Mac only: Create a Disk Image for installation';
help.commands += echo 'codesign - Mac: sign all frameworks, plugins and the bundle';
help.commands += echo 'dox: create Doxygen documentation';
help.commands += echo 'help - This help text';

doxygen.depends  =
doxygen.target   = dox
doxygen.commands = doxygen Doxyfile

QMAKE_EXTRA_TARGETS += distribution installer doxygen help
