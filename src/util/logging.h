#ifndef INCLUDE_ZjU5YThhZDA0YjUyN2YxZGZiNmI3MDNk
#define INCLUDE_ZjU5YThhZDA0YjUyN2YxZGZiNmI3MDNk 1


#include <stdio.h>


#ifdef __cplusplus
extern "C"
{
#endif


/* do not use it directly */
#define log_(level, ...) { \
		fprintf(stderr, "%s:%d " level ": ", __FUNCTION__, __LINE__); \
		fprintf(stderr, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
}


#define log_error(...) log_("error", __VA_ARGS__)
#define log_warning(...) log_("warning", __VA_ARGS__)
#define log_info(...) log_("info", __VA_ARGS__)
#define log_debug(...) log_("debug", __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ZjU5YThhZDA0YjUyN2YxZGZiNmI3MDNk */
