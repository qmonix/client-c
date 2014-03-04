#include <stdlib.h>
#include <stdint.h>

#include "util/qassert.h"

#include "qmonix/event.h"
#include "qmonix/volume_event.h"


struct event_data
{
	uint64_t volume;
};

/**
 * Serializes volume event to json.
 *
 * @param[in] event to serialize (not NULL).
 * @return json object on success.
 * @return NULL on error.
 */
struct json_node *event_tojson(const struct qmonix_event *event);


struct qmonix_event *
qmonix_volume_event_create(char *tag, uint64_t volume)
{
	struct qmonix_event *retval = NULL;
	struct qmonix_event *event = NULL;
	struct event_data *event_data = NULL;

	q_assert(tag != NULL);

	event_data = (struct event_data *)malloc(sizeof(*event_data));
	q_assert_verbose(event_data != NULL, "Failed to allocated memory for "
		"volume event data.");

	event_data->volume = volume;
	event = qmonix_event_create(tag, event_data, free, event_tojson);
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


uint64_t
qmonix_volume_event_get_volume(struct qmonix_event *event)
{
	uint64_t retval = 0;
	struct event_data *event_data = NULL;

	q_assert(event != NULL);

	event_data = qmonix_event_get_data(event);
	q_assert_verbose(event_data != NULL, "Event data is NULL.");

	retval = event_data->volume;

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

	volume_node = json_mknumber((double)evdata->volume);
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
