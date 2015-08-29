include (src/Common.pri)

CONFIG(debug, debug|release) {
    BIN_DIR = ./bin/debug
    message(Distributing $$APP_NAME in DEBUG mode)
} else {
    BIN_DIR = ./bin/release
    message(Distributing $$APP_NAME in RELEASE mode)
}

DIST_DIR = ./dist
DIST_EXE = $${DIST_DIR}/$${APP_NAME}.exe

#
# Distribution
#

distribution.commands += @echo Making distribution for Windows;

# Copy compiled binaries
distribution.commands += cp $${BIN_DIR}/Utils$${VERSION_MAJ}.dll ./$${DIST_DIR};
distribution.commands += cp $${BIN_DIR}/Model$${VERSION_MAJ}.dll ./$${DIST_DIR};
distribution.commands += cp $${BIN_DIR}/Kernel$${VERSION_MAJ}.dll ./$${DIST_DIR};
distribution.commands += cp $${BIN_DIR}/Resources$${VERSION_MAJ}.dll ./$${DIST_DIR};
distribution.commands += cp $${BIN_DIR}/$${APP_NAME}.exe ./$${DIST_EXE};

distribution.commands += windeployqt --no-angle --no-translations --no-svg $${DIST_EXE};

# qt.conf
distribution.commands += echo \"[Paths] Translations = translations\" | cat > $${DIST_DIR}/qt.conf;

# Remove unnecessary plug-ins
distribution.commands += rm $${DIST_DIR}/imageformats/qdds.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qicns.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qico.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qjp2.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qmng.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qtga.dll;
distribution.commands += rm $${DIST_DIR}/imageformats/qwebp.dll;

#
# Installer
#

# Make sure the WiX installer tools candle.exe and light.exe are in the PATH
# e.g. set PATH="c:\Programme\Windows Installer XML v3.5\bin";%PATH%
CONFIG(debug, debug|release) {
  message("Installer: skipping this build step. Target 'Installer' can only be done in RELEASE mode.")
} else {
  installer.commands = candle.exe -nologo "Installer/Windows/Screenie.wxs" -out "Installer/Windows/Screenie.wixobj"  -ext WixUIExtension &&
  installer.commands += light.exe -nologo "Installer/Windows/Screenie.wixobj" -out "Installer/Windows/Screenie-$${VERSION}.msi"  -ext WixUIExtension
}

OTHER_FILES += Installer/Windows/Screenie.wxs \
               Installer/Windows/GPL3.rtf \
               Installer/Windows/README.txt


