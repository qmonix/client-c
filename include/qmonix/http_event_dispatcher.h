#ifndef INCLUDE_MzRlMzhmZTU1ZDg5ZmQyNzQ1MWRlZThk
#define INCLUDE_MzRlMzhmZTU1ZDg5ZmQyNzQ1MWRlZThk 1

#ifdef __cplusplus
extern "C"
{
#endif


#include <qmonix/event_dispatcher.h>


/**
 * Creates new event disaptcher that sends events over HTTP connection.
 *
 * @param[in] url to send events to (not NULL).
 * @return pointer to HTTP event dispatcher on success.
 * @return NULL on error.
 */
struct qmonix_event_dispatcher *qmonix_http_event_dispatcher_create(
	const char *server_address);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MzRlMzhmZTU1ZDg5ZmQyNzQ1MWRlZThk */
