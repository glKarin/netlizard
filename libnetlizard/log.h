#ifndef _KARIN_LOG_H
#define _KARIN_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#if 0
#define ENDL printf("\n");

#define printfi(x) printf(#x"->%d\n", x)
#define printfu(x) printf(#x"->%u\n", x)
#define printfl(x) printf(#x"->%ld\n", x)
#define printful(x) printf(#x"->%lu\n", x)
#define printff(x) printf(#x"->%f\n", x)
#define printfx(x) printf(#x"->%x\n", x)
#define printfs(x) printf(#x"->%s\n", x)
#define printfc(x) printf(#x"->%c\n", x)
#define printfp(x) printf(#x"->%p\n", x)
#define printf2f(x) printf(#x"->%.2f\n", x)
#define printf1f(x) printf(#x"->%.1f\n", x)
#define printfln(fmt, args...) printf(fmt"\n", ##args)
#endif

int nlflogfln(int type, const char *fmt, ...);
int nlflogf(int type, const char *fmt, ...);
int nllogf(const char *fmt, ...);
int nllogfln(const char *fmt, ...);

int log_get_func(int name, void **ptr);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_LOG_H
