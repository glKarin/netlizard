#ifndef _KARIN_ERROR_H
#define _KARIN_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#define E_NO_ERROR clear_errno();
#define E_INVALID_VALUE set_errno(NL_INVALID_VALUE);
#define E_INVALID_ENUM set_errno(NL_INVALID_ENUM);
#define E_INVALID_OPERATION set_errno(NL_INVALID_OPERATION);
#define E_STACK_OVERFLOW set_errno(NL_STACK_OVERFLOW);
#define E_STACK_UNDERFLOW set_errno(NL_STACK_UNDERFLOW);
#define E_OUT_OF_MEMORY set_errno(NL_OUT_OF_MEMORY);

int set_errno(int err);
int clear_errno(void);
int current_errno(void);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_ERROR_H
