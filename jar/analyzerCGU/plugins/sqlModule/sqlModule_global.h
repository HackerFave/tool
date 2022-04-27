#ifndef SQLMODULE_GLOBAL_H
#define SQLMODULE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SQLMODULE_LIBRARY)
#  define SQLMODULE_EXPORT Q_DECL_EXPORT
#else
#  define SQLMODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // SQLMODULE_GLOBAL_H
