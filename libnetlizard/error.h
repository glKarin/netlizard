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

#define ES_INVALID_VALUE(x, args...) set_errno_string(NL_INVALID_VALUE, x, ##args);
#define ES_INVALID_ENUM(x, args...) set_errno_string(NL_INVALID_ENUM, x, ##args);
#define ES_INVALID_OPERATION(x, args...) set_errno_string(NL_INVALID_OPERATION, x, ##args);
#define ES_STACK_OVERFLOW(x, args...) set_errno_string(NL_STACK_OVERFLOW, x, ##args);
#define ES_STACK_UNDERFLOW(x, args...) set_errno_string(NL_STACK_UNDERFLOW, x, ##args);
#define ES_OUT_OF_MEMORY(x, args...) set_errno_string(NL_OUT_OF_MEMORY, x, ##args);

int set_errno(int err);
int set_errno_string(int err, const char *str, ...);
int clear_errno(void);
int current_errno(void);
const char * current_error(void);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_ERROR_H
