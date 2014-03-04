#ifndef INCLUDE_MGExOTc4OGYzMDI1YTg0NTNmMzdiZWQw
#define INCLUDE_MGExOTc4OGYzMDI1YTg0NTNmMzdiZWQw 1

#ifdef __cplusplus
extern "C"
{
#endif


#include <qmonix/util/forward_list.h>
#include <qmonix/event.h>


struct qmonix_event_dispatcher;


/**
 * This function is called whenever qmonix_event_dispatcher_dispatch is called.
 *
 * @param[in] pointer to event dispatcher (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
typedef int (*qmonix_event_dispatcher_dispatch_cb)(
	struct qmonix_event_dispatcher *disp);


/**
 * This function destroys custom data provided to
 * qmonix_event_dispatcher_create().
 *
 * @param[in] pointer to data to destroy (not NULL).
 */
typedef void (*qmonix_event_dispatcher_data_destroy_cb)(void *data);


/**
 * Creates a new event dispatcher with the specified data which depends on
 * the specific dispatcher created. E.g. HTTP dispatcher might have a server
 * address, file system dispatcher might have a file name, etc.
 * You can optionally pass the data destroy callback which will be invoked
 * on qmonix_event_dispatcher_destroy().
 *
 * @param[in] dispatch events callback (not NULL).
 * @param[in] pointer to dispatcher data (might be NULL).
 * @param[in] destroy the specified data callback (might be NULL).
 * @return pointer to event dispatcher on success.
 * @return NULL on error.
 */
struct qmonix_event_dispatcher *qmonix_event_dispatcher_create(
	qmonix_event_dispatcher_dispatch_cb dispatch_cb,
	void *dispatcher_data,
	qmonix_event_dispatcher_data_destroy_cb data_destroy_cb);


/**
 * Destroys the specified dispatcher. If dispatcher has any collected events
 * that were not dispatched, they are destroyed too.
 * Also destroys dispatcher data, if appropriate callback was set during
 * qmonix_event_dispatcher_create().
 *
 * @param[in] pointer to event dispatcher (not NULL).
 */
void qmonix_event_dispatcher_destroy(struct qmonix_event_dispatcher *disp);


/**
 * Submits the specified event to the specified dispatcher.
 * Once events are submited to the dispatcher, they should not be destroyed,
 * the dispatcher will destroy them on successful dispath.
 *
 * @param[in] pointer to event dispatcher (not NULL).
 * @param[in] pointer to event (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_event_dispatcher_submit(struct qmonix_event_dispatcher *disp,
	struct qmonix_event *event);


/**
 * Calls dispatch callback for the specified event dispatcher. After
 * successful dispatch the collected event list is cleared.
 *
 * @param[in] pointer to event dispatcher (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_event_dispatcher_dispatch(struct qmonix_event_dispatcher *disp);


/**
 * Returns event dispatcher specific data which was provided to constructor.
 *
 * @param[in] pointer to event dispatcher (not NULL).
 * @return pointer to dispatcher data (might be NULL).
 */
void *qmonix_event_dispatcher_get_data(
	const struct qmonix_event_dispatcher *disp);


/**
 * Returns a list of collected events by the specified dispatcher.
 *
 * @param[in] pointer to event dispatcher (not NULL).
 * @return collected event list on success.
 * @return NULL on error.
 */
const struct qmonix_forward_list *qmonix_event_dispatcher_get_events(
	const struct qmonix_event_dispatcher *disp);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MGExOTc4OGYzMDI1YTg0NTNmMzdiZWQw */
