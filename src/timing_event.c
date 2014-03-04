#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include <json/json.h>

#include "util/qassert.h"

#include "qmonix/event.h"
#include "qmonix/timing_event.h"


enum timing_event_state
{
	TEVENT_STATE_INIT,
	TEVENT_STATE_STARTED,
	TEVENT_STATE_PAUSED,
	TEVENT_STATE_STOPPED
};


struct event_data
{
	uint64_t total_time;
	uint64_t time_started;

	qmonix_timing_event_handle_cb on_fire_cb;
	void *handler_data;

	enum timing_event_state state;
};


/**
 * Serializes timing event to json.
 *
 * @param[in] event to serialize (not NULL).
 * @return json object on success.
 * @return NULL on error.
 */
struct json_node *event_tojson(const struct qmonix_event *event);


struct qmonix_event *
qmonix_timing_event_create(char *tag, qmonix_timing_event_handle_cb on_fire_cb,
	void *handler_data)
{
	struct qmonix_event *retval = NULL;
	struct qmonix_event *event = NULL;
	struct event_data *event_data = NULL;

	q_assert(tag != NULL);

	event_data = (struct event_data *)malloc(sizeof(*event_data));
	q_assert_verbose(event_data != NULL, "Failed to allocated memory for "
		"timing event data.");

	event_data->total_time = 0;
	event_data->time_started = 0;
	event_data->on_fire_cb = on_fire_cb;
	event_data->handler_data = handler_data;
	event_data->state = TEVENT_STATE_INIT;

	event = qmonix_event_create(tag, event_data, free, event_tojson);
	q_assert_verbose(event != NULL, "Failed to create new event.");

	event_data->state = TEVENT_STATE_STARTED;
	event_data->time_started = qmonix_event_get_time_raised(event);
	event_data = NULL;

	// success.
	retval = event;
	event = NULL;

done:
	if (event_data != NULL)
	{
		free(event_data);
		event_data = NULL;
	}

	if (event != NULL)
	{
		qmonix_event_destroy(event);
		event = NULL;
	}

	return retval;
}


int
qmonix_timing_event_pause(struct qmonix_event *event)
{
	int retval = -1;
	struct event_data *event_data = NULL;

	q_assert(event != NULL);

	event_data = (struct event_data *)qmonix_event_get_data(event);
	q_assert_verbose(event_data != NULL, "Timing event data is NULL.");

	q_assert_verbose(event_data->state == TEVENT_STATE_STARTED, "Event was "
		"not started.");

	event_data->state = TEVENT_STATE_PAUSED;
	event_data->total_time += time(NULL) - event_data->time_started;

	// success.
	retval = 0;

done:
	return retval;
}


int
qmonix_timing_event_resume(struct qmonix_event *event)
{
	int retval = -1;
	struct event_data *event_data = NULL;

	q_assert(event != NULL);

	event_data = (struct event_data *)qmonix_event_get_data(event);
	q_assert_verbose(event_data != NULL, "Timing event data is NULL.");

	q_assert_verbose(event_data->state == TEVENT_STATE_PAUSED, "Event was "
		"not paused.");

	event_data->state = TEVENT_STATE_STARTED;
	event_data->time_started = time(NULL);

	// success.
	retval = 0;

done:
	return retval;
}


int
qmonix_timing_event_fire(struct qmonix_event *event)
{
	int retval = -1;
	struct event_data *event_data = NULL;
	int ret = -1;

	q_assert(event != NULL);

	event_data = (struct event_data *)qmonix_event_get_data(event);
	q_assert_verbose(event_data != NULL, "Timing event data is NULL.");

	q_assert_verbose(event_data->state != TEVENT_STATE_STOPPED, "Event was "
		"already fired.");

	if (event_data->state == TEVENT_STATE_STARTED)
	{
		event_data->total_time += time(NULL) - event_data->time_started;
	}

	event_data->state = TEVENT_STATE_STOPPED;

	ret = event_data->on_fire_cb(event, event_data->handler_data);
	q_assert_verbose(ret == 0, "Failed to execute on event fire "
		"callback.");

	// success.
	retval = 0;

done:
	return retval;
}


uint64_t
qmonix_timing_event_get_time(const struct qmonix_event *event)
{
	uint64_t retval = 0;
	struct event_data *event_data = NULL;

	q_assert(event != NULL);

	event_data = (struct event_data *)qmonix_event_get_data(event);
	q_assert_verbose(event_data != NULL, "Timing event data is NULL.");

	retval = event_data->total_time;

done:
	return retval;
}


struct json_node *
event_tojson(const struct qmonix_event *event)
{
	struct json_node *retval = NULL;
	struct json_node *root_node = NULL;
	struct event_data *evdata = NULL;
	struct json_node *volume_node = NULL;

	q_assert(event != NULL);

	root_node = qmonix_event_tojson_default(event);
	q_assert_verbose(root_node != NULL, "Failed to serialize base event "
		"to json.");

	evdata = (struct event_data *)qmonix_event_get_data(event);
	q_assert_verbose(evdata != NULL, "Failed to get timing event data.");

	volume_node = json_mknumber((double)evdata->total_time);
	q_assert_verbose(volume_node != NULL, "Failed to create timing event "
		"volume node.");

	json_append_member(root_node, "volume", volume_node);
	volume_node = NULL;

	// success.
	retval = root_node;
	root_node = NULL;

done:
	if (root_node != NULL)
	{
		json_delete(root_node);
		root_node = NULL;
	}

	if (volume_node != NULL)
	{
		json_delete(volume_node);
		volume_node = NULL;
	}

	return retval;
}
