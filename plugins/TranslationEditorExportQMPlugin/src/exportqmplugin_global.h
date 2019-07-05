#ifndef EXPORT_QM_PLUGIN_GLOBAL_H
#define EXPORT_QM_PLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EXPORT_TE_QM_PLUGIN_LIBRARY)
#  define EXPORT_QM_PLUGIN_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define EXPORT_QM_PLUGIN_LIBRARY_EXPORT Q_DECL_IMPORT
#endif


#endif // EXPORT_QM_PLUGIN_GLOBAL_H