# Target
TARGET          = TranslationEditor

# Template
TEMPLATE        = app

# Qt Packages
QT              += core gui widgets quickwidgets quick qml

macx: {
# Icon
ICON            = resources/images/icons/main.icns
} else {
# Icon
ICON            = resources/images/icons/main.png
}

# Sources
SOURCES         += src/main.cpp \
                src/colorpickerbutton.cpp \
                src/finditemsdialog.cpp \
                src/finditemsresultmodel.cpp \
                src/mainwindow.cpp \
                src/preferencesdialog.cpp \
                src/aboutdialog.cpp \
                src/preferences.cpp \
                src/packagepreferencesdialog.cpp \
                src/packageinfo.cpp \
                src/packagegroup.cpp \
                src/translationitem.cpp \
                src/translationitemlayout.cpp \
                src/packagegroupfilter.cpp \
                src/confirmationdialog.cpp \
                src/infodialog.cpp \
                src/helpdialog.cpp \
                src/newgroupdialog.cpp \
                src/newtranslationdialog.cpp \
                src/plugininfo.cpp \
                src/alternatefontsdialog.cpp

# Headers
HEADERS         += src/mainwindow.h \
                src/colorpickerbutton.h \
                src/finditemsdialog.h \
                src/finditemsresultmodel.h \
                src/preferencesdialog.h \
                src/aboutdialog.h \
                src/preferences.h \
                src/setting_keys.h \
                src/constants.h \
                src/packagepreferencesdialog.h \
                src/packageinfo.h \
                src/packagegroup.h \
                src/translationitem.h \
                src/translationitemlayout.h \
                src/packagegroupfilter.h \
                src/confirmationdialog.h \
                src/infodialog.h \
                src/settings_defaults.h \
                src/helpdialog.h \
                src/newgroupdialog.h \
                src/newtranslationdialog.h \
                src/plugininfo.h \
                src/alternatefontsdialog.h

# Shared Headers
HEADERS         += src/shared/translationeditorimportplugininterface.h \
                src/shared/translationeditorexportplugininterface.h \
                src/shared/translation_keys.h

# Forms
FORMS           += forms/mainwindow.ui \
                forms/finditemsdialog.ui \
                forms/preferencesdialog.ui \
                forms/aboutdialog.ui \
                forms/packagepreferencesdialog.ui \
                forms/confirmationdialog.ui \
                forms/infodialog.ui \
                forms/helpdialog.ui \
                forms/newgroupdialog.ui \
                forms/newtranslationdialog.ui \
                forms/alternatefontsdialog.ui

# Resources
RESOURCES       += translationeditor.qrc \
                translationeditorflags.qrc

# Other Files
OTHER_FILES     += resources/locals.txt

# Dist Files
DISTFILES       += README.md

# Output/Intermediate Dirs
OBJECTS_DIR     = ./objs
OBJMOC          = ./objs
MOC_DIR         = ./objs
UI_DIR          = ./objs
RCC_DIR         = ./objs

