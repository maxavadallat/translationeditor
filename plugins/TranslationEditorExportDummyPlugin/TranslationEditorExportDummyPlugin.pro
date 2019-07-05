# Target
TARGET              = ExportDummyPlugin
# Template
TEMPLATE            = lib

# Qt Configuration
QT                  += qml quick
QT                  -= gui

# Config
CONFIG              += plugin

# Defines
DEFINES             += EXPORT_TE_DUMMY_PLUGIN_LIBRARY

# Sources
SOURCES             += src/exportdummyplugin.cpp

# Headers
HEADERS             += src/exportdummyplugin.h \
                    src/exportdummyplugin_global.h

# Include Path
INCLUDEPATH         += ../../src/shared

# Shared Headers
HEADERS             += ../../src/shared/translationeditorexportplugininterface.h

# Other Files
OTHER_FILES         += ExportDummyPlugin.json

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
