# Target
TARGET              = UpdateFromTSPlugin
# Template
TEMPLATE            = lib

# Qt Configuration
QT                  += core gui widgets xml

# Config
CONFIG              += plugin

# Defines
DEFINES             += UPDATE_TE_TS_PLUGIN_LIBRARY

# Sources
SOURCES             += src/updatefromtsplugin.cpp

# Headers
HEADERS             += src/updatefromtsplugin.h \
                    src/updatefromtsplugin_global.h

# Include Path
INCLUDEPATH         += ../../src/shared

# Shared Headers
HEADERS             += ../../src/shared/translationeditorimportplugininterface.h

# Other Files
OTHER_FILES         += UpdateFromTSPlugin.json

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
