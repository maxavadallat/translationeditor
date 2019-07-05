# Target
TARGET              = ImportDummyPlugin
# Template
TEMPLATE            = lib

# Qt Configuration
QT                  += qml quick
QT                  -= gui

# Config
CONFIG              += plugin

# Defines
DEFINES             += IMPORT_TE_DUMMY_PLUGIN_LIBRARY

# Sources
SOURCES             += src/importdummyplugin.cpp

# Headers
HEADERS             += src/importdummyplugin.h \
                    src/importdummyplugin_global.h

# Include Path
INCLUDEPATH         += ../../src/shared

# Shared Headers
HEADERS             += ../../src/shared/translationeditorimportplugininterface.h

# Other Files
OTHER_FILES         += ImportDummyPlugin.json

#macx: {
#    INSTALLDIR      = $$(HOME)/Library/TranslationEditor/plugins
#} else {
#    unix: {
#        INSTALLDIR  = /usr/lib/translationeditor/plugins
#    }
#}

#message(Install Dir is $$INSTALLDIR)

## Target Path
#target.path         = $$INSTALLDIR

## Installs
#INSTALLS            += target


# Output/Intermediate Dirs
OBJECTS_DIR         = ./objs
OBJMOC              = ./objs
MOC_DIR             = ./objs
UI_DIR              = ./objs
RCC_DIR             = ./objs
