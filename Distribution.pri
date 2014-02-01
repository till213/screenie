include(src/Common.pri)
macx {
  include(Distribution_Mac.pri)
}
win32 {
  include(Distribution_Windows.pri)
  distribution.depends = install
}

installer.depends = distribution

help.commands  = @echo 'Available Custom Commands:';
help.commands += echo 'distribution - Copy all files required for distribution into directory dist';
help.commands += echo 'installer - Create an Installer. Mac: PKG installer - Windows: MSI installer';
help.commands += echo 'codesign - Mac: sign all frameworks, plugins and the bundle';
help.commands += echo 'help - This help text';

QMAKE_EXTRA_TARGETS += distribution installer help
