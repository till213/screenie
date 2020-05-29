include (src/Common.pri)

CONFIG(debug, debug|release) {
    APP_BUNDLE = bin/debug/$${APP_NAME}.app
    message(Distributing $$APP_NAME in DEBUG mode)
} else {
    APP_BUNDLE = bin/release/$${APP_NAME}.app
    message(Distributing $$APP_NAME in RELEASE mode)
}

DIST_DIR = ./dist
DIST_APP_BUNDLE = dist/$${TARGET}.app

# Name of the application signing certificate
APP_CERT = Test\\ Code\\ Signing\\ Cert

# Name of the installer signing certificate
INSTALLER_CERT = Test\\ Installer\\ Signing\\ Cert

# Bundle identifier for your application
BUNLDE_ID = net.till-art.Screenie

# Name of the entitlements file (sandboxing)
ENTITLEMENTS = src/Screenie/Screenie-Entitlements.plist


#
# Distribution
#

distribution.commands += @echo Making distribution for Mac;

# Copy compiled binaries
distribution.commands += cp -R ./$${APP_BUNDLE} ./$${DIST_APP_BUNDLE};
distribution.commands += macdeployqt $${DIST_APP_BUNDLE};
# qt.conf
distribution.commands += cp ./src/Resources/Mac/qt.conf $${DIST_APP_BUNDLE}/Contents/Resources/qt.conf;
# Translations
distribution.commands += cp -r $${DIST_DIR}/translations $${DIST_APP_BUNDLE}/Contents/Resources/translations;

# Remove unnecessary plug-ins
distribution.commands += rm -r $${DIST_APP_BUNDLE}/Contents/PlugIns/printsupport;
distribution.commands += rm $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqico.dylib;
distribution.commands += rm $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqmng.dylib;

#
# Code Signing
#

codesign.depends = distribution

# Sign Frameworks
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/QtCore.framework/Versions/5/QtCore;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/QtGui.framework/Versions/5/QtGui;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets;

codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/libKernel.$${VERSION}.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/libModel.$${VERSION}.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/libResources.$${VERSION}.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/Frameworks/libUtils.$${VERSION}.dylib;

# Sign Plugins
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqgif.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqjpeg.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqtga.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqtiff.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/imageformats/libqwbmp.dylib;
codesign.commands += codesign -s $${APP_CERT} -v -i $${BUNLDE_ID} $${DIST_APP_BUNDLE}/Contents/PlugIns/platforms/libqcocoa.dylib;

# Sign the application bundle, using the provided entitlements
codesign.commands += codesign -f -s $${APP_CERT} -v --entitlements $${ENTITLEMENTS} $${DIST_APP_BUNDLE};


#
# Installer
#

installer.depends = codesign

# Extract debug symbols
installer.commands += dsymutil $${DIST_APP_BUNDLE}/Contents/MacOS/$${TARGET} -o dist/$${TARGET}.app.dSYM;
# Build and sign the installer package
# installer.commands += installerbuild --component $${DIST_APP_BUNDLE} /Applications --sign $${INSTALLER_CERT} $${TARGET}.pkg;
installer.commands += productbuild --component $${DIST_APP_BUNDLE} /Applications dist/$${TARGET}.pkg;

QMAKE_EXTRA_TARGETS += codesign


#
# Disk Image (Installer 2)
#

# TODO: Create compressed DMG disk image with some fancy background image ;)
#diskimage.commands  = hdiutil create -srcfolder dist -volname $${TARGET} -fs HFS+ -fsargs \"-c c=64,a=16,e=16\" -format UDRW -size 64000k pack.temp.dmg;
diskimage.depends = distribution
diskimage.commands = @echo Creating Disk Image...;
diskimage.commands += Installer/Mac/build_disk_image.sh;

QMAKE_EXTRA_TARGETS += diskimage

OTHER_FILES += Installer/Mac/build_disk_image.sh



