#ifndef INCLUDE_MmMxZWYzZTJhMjliMDJkYWY0NzVkYzdl
#define INCLUDE_MmMxZWYzZTJhMjliMDJkYWY0NzVkYzdl 1

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>

#include <qmonix/event.h>


/**
 * Callback that handles timing event fire. It receives pointer to the fired
 * event and handler specific data.
 *
 * @param[in] fired event (not NULL).
 * @param[in] handler specific data (might be NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
typedef int (*qmonix_timing_event_handle_cb)(struct qmonix_event *event,
	void *handler_data);


/**
 * Creates timing event with the specified tag name and assigns callback
 * that is invoked when qmonix_timing_event_fire() is called.
 *
 * Timing event is started as soon when it is created.
 * After creation timing event manages tag name resource and will destroy it
 * on qmonix_event_destroy().
 *
 * @param[in] event tag name (not NULL).
 * @param[in] event fire handler callback (might be NULL).
 * @param[in] pointer to event fire handler specific data (might be NULL).
 * @return new timing event on success.
 * @return NULL on error.
 */
struct qmonix_event *qmonix_timing_event_create(char *tag,
	qmonix_timing_event_handle_cb on_fire_cb, void *handler_data);


/**
 * Pauses the specified timing event. Fails if event is already paused or
 * fired.
 *
 * @param[in] pointer to timing event (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_timing_event_pause(struct qmonix_event *event);


/**
 * Resumes the specified timing event. Fails if event was not started.
 *
 * @param[in] pointer to timing event (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_timing_event_resume(struct qmonix_event *event);


/**
 * Stops timing event and calls event fire handler callback that was registered
 * when event was created.
 * Fails if event was not started.
 *
 * @param[in] pointer to timing event (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
int qmonix_timing_event_fire(struct qmonix_event *event);


/**
 * Returns how much time in seconds the timing event has accumulated.
 *
 * @param[in] pointer to timing event (not NULL).
 * @return timing event value.
 */
uint64_t qmonix_timing_event_get_time(const struct qmonix_event *event);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MmMxZWYzZTJhMjliMDJkYWY0NzVkYzdl */
