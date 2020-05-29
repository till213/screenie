include (src/Common.pri)

CONFIG(debug, debug|release) {
    BIN_DIR = ./bin/debug
    message(Distributing $$APP_NAME in DEBUG mode)
} else {
    BIN_DIR = ./bin/release
    message(Distributing $$APP_NAME in RELEASE mode)
}

DIST_DIR = ./dist

#
# Distribution
#

distribution.commands += @echo Making distribution for Linux;

# Copy compiled binaries
distribution.commands += cp -R $${BIN_DIR}/* ./$${DIST_DIR};
