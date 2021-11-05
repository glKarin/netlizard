#ifndef _KARIN_IO_H
#define _KARIN_IO_H

#include "jport.h"

#ifdef __cplusplus
extern "C" {
#endif

jlong file_get_contents(const char *name, array *array);
jlong file_size(const char *name);
jlong file_get_contents_s(const char *name, array *array);
jlong file_put_contents(const char *name, const array *array);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_IO_H
