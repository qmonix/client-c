#ifndef INCLUDE_Y2U3NTRhNmNlNDU3NzdiOWE2MGU0ZTY2
#define INCLUDE_Y2U3NTRhNmNlNDU3NzdiOWE2MGU0ZTY2 1

#ifdef __cplusplus
extern "C"
{
#endif


#include "util/logging.h"


#define q_assert_verbose(expr, ...) \
{ \
	if (!(expr)) \
	{ \
		log_error(__VA_ARGS__); \
		goto done; \
	}\
}


#define q_assert(expr) q_assert_verbose(expr, #expr)


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_Y2U3NTRhNmNlNDU3NzdiOWE2MGU0ZTY2 */
