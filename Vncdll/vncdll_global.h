#ifndef VNCDLL_GLOBAL_H
#define VNCDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VNCDLL_LIBRARY)
#  define VNCDLLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define VNCDLLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VNCDLL_GLOBAL_H
