#ifndef INCLUDE_OWZhZmJiZTc1NTRkZjk2MmVhMGNmNjc1
#define INCLUDE_OWZhZmJiZTc1NTRkZjk2MmVhMGNmNjc1 1

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>

#include <json/json.h>


struct qmonix_event;


/**
 * Event data destroy callback invoked on qmonix_event_destroy.
 *
 * @param[in] pointer to event data to deallocate (not NULL).
 */
typedef void (*qmonix_event_destroy_data_cb)(void *data);


/**
 * Event to json serialization callback.
 *
 * @param[in] event to serialize (not NULL).
 * @return json object on success.
 * @return NULL on error.
 */
typedef struct json_node *(*qmonix_event_tojson_cb)(
	const struct qmonix_event *event);


/**
 * Creates new event, assigns it the specified tag name and passes the specifed
 * data pointer. After creation qmonix event is responsible for managing
 * tag and data resources. They are destroyed on qmonix_event_destroy.
 * If you specify event data, you also must provide the data destroy callback.
 *
 * Sets time event was raised to the Unix time stamp when qmonix_event_create()
 * was invoked.
 *
 * You might pass an optional callback that serializes the specified event
 * to json object. If this callback is not specified, the default serializer
 * is used.
 *
 * @param[in] event tag name (not NULL).
 * @param[in] event optional data (might be NULL).
 * @param[in] event data destroy callback (might be NULL).
 * @param[in] event serialization to json callback (might be NULL).
 */
struct qmonix_event *qmonix_event_create(char *tag, void *data,
	qmonix_event_destroy_data_cb destroy_data_cb,
	qmonix_event_tojson_cb tojson_cb);


/**
 * Destroys the specified event together with it's data.
 *
 * @param[in] pointer to event structure (not NULL).
 */
void qmonix_event_destroy(struct qmonix_event *event);


/**
 * Returns tag name associated with the specified event.
 *
 * @param[in] pointer to event (not NULL).
 * @return pointer to tag name on success.
 * @return NULL on error.
 */
const char *qmonix_event_get_tag(const struct qmonix_event *event);


/**
 * Returns the specified event data.
 *
 * @param[in] pointer to event (not NULL).
 * @return pointer to data associated with the specified event. Might be NULL.
 */
void *qmonix_event_get_data(const struct qmonix_event *event);


/**
 * Returns Unix time stamp when event was raised.
 *
 * @param[in] pointer to event (not NULL).
 * @return time stamp when event was raised.
 */
uint64_t qmonix_event_get_time_raised(const struct qmonix_event *event);


/**
 * Returns json object representing the specified event structure.
 * Whoever receives json object is responsible for destroying it with
 * json_delete().
 *
 * @param[in] event (not NULL).
 * @return pointer to event representing json object on success.
 * @return NULL on error.
 */
struct json_node *qmonix_event_tojson_default(const struct qmonix_event *event);


/**
 * Serializes the specified event to json object. Uses the callback passed via
 * qmonix_event_create(), if it was NULL uses qmonix_evet_tojson_default().
 *
 * @param[in] event (not NULL).
 * @return pointer to event representing json object on success.
 * @return NULL on error.
 */
struct json_node *qmonix_event_tojson(const struct qmonix_event *event);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_OWZhZmJiZTc1NTRkZjk2MmVhMGNmNjc1 */
