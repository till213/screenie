# Translation.pro is a dummy project which is supposed to
# contain all sources, as to extract all strings to be
# translated
#
# Run lupdate Translation.pro, as to produce the different
# *.ts translation files in the 'i18n' subfolder

# Add all module sources here (except plugins)
include(Utils/Sources.pri)
include(Model/Sources.pri)
include(Resources/Sources.pri)
include(Kernel/Sources.pri)
include(Screenie/Sources.pri)

TRANSLATIONS = translations/screenie_de.ts

