#ifndef _KARIN_NLINC_H
#define _KARIN_NLINC_H

#if defined(_NL_LIBRARY)
#  define NLLIB_EXPORT Q_DECL_EXPORT
#else
#  define NLLIB_EXPORT Q_DECL_IMPORT
#endif

#define NLINTERFACE
#define NLGETTER(x)
#define NLSETTER(x)
#define NLPROPERTYD(x)
#define NLPROPERTY(T, x)
#define NLACTOR(x)
#define NLSCENE(x)
#define NLCOMPONENT(x)
#define NLSCRIPT(x)
#define NLRENDERER(x)

#endif // _KARIN_NLINC_H
