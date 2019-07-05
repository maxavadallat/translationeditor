# Target
TARGET              = ExportQMPlugin
# Template
TEMPLATE            = lib

# Qt Configuration
QT                  += widgets xml
QT                  -= gui

# Config
CONFIG              += plugin

# Defines
DEFINES             += EXPORT_TE_QM_PLUGIN_LIBRARY

# Sources
SOURCES             += src/exportqmplugin.cpp

# Headers
HEADERS             += src/exportqmplugin.h \
                    src/exportqmplugin_global.h

# Defines
DEFINES             += EXPORT_QM_PLUGIN_USE

# Export TS Plugin Include Path
INCLUDEPATH         += ../TranslationEditorExportTSPlugin/src

# Export TS Plugin Sources
SOURCES             += ../TranslationEditorExportTSPlugin/src/exporttsplugin.cpp
# Export TS Plugin Headers
HEADERS             += ../TranslationEditorExportTSPlugin/src/exporttsplugin.h

# Shared Include Path
INCLUDEPATH         += ../../src/shared

# Shared Headers
HEADERS             += ../../src/shared/translationeditorexportplugininterface.h

# Other Files
OTHER_FILES         += ExportQMPlugin.json

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
