#ifndef _KARIN_STATE_H
#define _KARIN_STATE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NETLizard_State
{
    unsigned log_enabled;
} NETLizard_State;

const NETLizard_State * current_state(void);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_STATE_H
