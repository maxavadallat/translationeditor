# Target
TARGET              = ExportTSPlugin
# Template
TEMPLATE            = lib

# Qt Configuration
QT                  += widgets xml
QT                  -= gui

# Config
CONFIG              += plugin

# Defines
DEFINES             += EXPORT_TE_TS_PLUGIN_LIBRARY

# Sources
SOURCES             += src/exporttsplugin.cpp

# Headers
HEADERS             += src/exporttsplugin.h \
                    src/exporttsplugin_global.h

# Include Path
INCLUDEPATH         += ../../src/shared

# Shared Headers
HEADERS             += ../../src/shared/translationeditorexportplugininterface.h

# Other Files
OTHER_FILES         += ExportTSPlugin.json

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
