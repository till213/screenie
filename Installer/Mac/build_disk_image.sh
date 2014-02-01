#!/bin/sh

hdiutil create -srcfolder dist -volname "Screenie" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size 64000k pack.temp.dmg
device=$(hdiutil attach -readwrite -noverify -noautoopen "pack.temp.dmg" | egrep '^/dev/' | sed 1q | awk '{print $1}')
echo '
   tell application "Finder"
     tell disk "Screenie"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {400, 100, 800, 400}
           set theViewOptions to the icon view options of container window
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 72
           -- set background picture of theViewOptions to file ".background:bgimage.tiff"
           make new alias file at container window to POSIX file "/Applications" with properties {name:"Applications"}
           set position of item "Screenie.app" of container window to {100, 100}
           set position of item "Applications" of container window to {300, 100}
           update without registering applications
           delay 5
           eject
     end tell
   end tell
' | osascript

chmod -Rf go-w /Volumes/Screenie
sync
sync
hdiutil detach ${device}
hdiutil convert "pack.temp.dmg" -format UDZO -imagekey zlib-level=9 -o "dist/Screenie.dmg"
rm -f pack.temp.dmg
