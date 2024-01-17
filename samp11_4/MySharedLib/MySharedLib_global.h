#ifndef MYSHAREDLIB_GLOBAL_H
#define MYSHAREDLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MYSHAREDLIB_LIBRARY)
#define MYSHAREDLIB_EXPORT Q_DECL_EXPORT
#else
#define MYSHAREDLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // MYSHAREDLIB_GLOBAL_H
