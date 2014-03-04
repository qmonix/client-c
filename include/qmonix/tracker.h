/**
 * Qmonix tracker is a singleton object that allows you to fire single or
 * volume events and start timing events.
 */


#ifndef INCLUDE_Y2YxN2YxY2QxZjViNDY1MzJkMmRhOWY2
#define INCLUDE_Y2YxN2YxY2QxZjViNDY1MzJkMmRhOWY2 1

#ifdef __cplusplus
extern "C"
{
#endif


#define QMONIX_TRACKER_VERSION "0.1.0"


// TODO(povilas): implement thread safe tracker.


/**
 * Initialises event tracker: creates http event dispatcher with the specified
 * server address.
 *
 * @param[in] tracker config file (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_tracker_init(const char *server_address);


/**
 * Destroys the resources used by the tracker.
 */
void qmonix_tracker_destroy(void);


/**
 * Fires event and associated the specified tag with it.
 *
 * @param[in] event tag name (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_tracker_fire(const char *tag);


/**
 * The tracke creates a timing event and assigns the specified tag.
 *
 * @param[in] event tag name (not NULL).
 * @return pointer to timing event structure on success.
 * @return NULL on error.
 */
struct qmonix_event *qmonix_tracker_start(const char *tag);


/**
 * Dispatches collected events.
 *
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_tracker_dispatch(void);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_Y2YxN2YxY2QxZjViNDY1MzJkMmRhOWY2 */
