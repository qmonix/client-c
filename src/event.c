#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <json/json.h>

#include "util/qassert.h"

#include "qmonix/event.h"


struct qmonix_event
{
	char *tag;
	uint64_t time_raised;

	void *data;
	qmonix_event_destroy_data_cb destroy_data_cb;

	qmonix_event_tojson_cb tojson_cb;
};


struct qmonix_event *
qmonix_event_create(char *tag, void *data,
	qmonix_event_destroy_data_cb destroy_data_cb,
	qmonix_event_tojson_cb tojson_cb)
{
	struct qmonix_event *retval = NULL;
	struct qmonix_event *event = NULL;

	q_assert(tag != NULL);

	if ((data != NULL) && (destroy_data_cb == NULL))
	{
		log_error("Event data is not NULL, but data destroy callback "
			"was not specified.");
		goto done;
	}

	event = (struct qmonix_event *)malloc(sizeof(*event));
	q_assert_verbose(event != NULL, "Failed to allocate memory for event.");
	event->tag = tag;
	event->time_raised = time(NULL);
	event->data = data;
	event->destroy_data_cb = destroy_data_cb;
	event->tojson_cb = tojson_cb;

	// success.
	retval = event;
	event = NULL;

done:
	if (event != NULL)
	{
		qmonix_event_destroy(event);
		event = NULL;
	}

	return retval;
}


void
qmonix_event_destroy(struct qmonix_event *event)
{
	q_assert(event != NULL);

	if (event->tag != NULL)
	{
		free(event->tag);
		event->tag = NULL;
	}

	if (event->data != NULL)
	{
		if (event->destroy_data_cb != NULL)
		{
			event->destroy_data_cb(event->data);
			event->data = NULL;
		}
	}

	free(event);
	event = NULL;

done:
	return;
}


const char *
qmonix_event_get_tag(const struct qmonix_event *event)
{
	const char *retval = NULL;

	q_assert(event != NULL);

	retval = event->tag;

done:
	return retval;
}


void  *
qmonix_event_get_data(const struct qmonix_event *event)
{
	void *retval = NULL;

	q_assert(event != NULL);

	retval = event->data;

done:
	return retval;
}


uint64_t
qmonix_event_get_time_raised(const struct qmonix_event *event)
{
	uint64_t retval = 0;

	q_assert(event != NULL);

	retval = event->time_raised;

done:
	return retval;
}


struct json_node *
qmonix_event_tojson_default(const struct qmonix_event *event)
{
	struct json_node *retval = NULL;
	struct json_node *root_node = NULL;
	struct json_node *tag_node = NULL;
	struct json_node *time_raised_node = NULL;

	q_assert(event != NULL);
	q_assert(event->tag != NULL);

	root_node = json_mkobject();
	q_assert_verbose(root_node != NULL, "Failed to create root json node.");

	tag_node = json_mkstring(event->tag);
	q_assert_verbose(tag_node != NULL, "Failed to create json node for "
		"event tag.");

	json_append_member(root_node, "tag", tag_node);
	tag_node = NULL;

	time_raised_node = json_mknumber((double)event->time_raised);
	q_assert_verbose(time_raised_node != NULL, "Failed to create json "
		"node for event raised time value.");

	json_append_member(root_node, "whenArised", time_raised_node);
	time_raised_node = NULL;

	// success.
	retval = root_node;
	root_node = NULL;

done:
	if (root_node != NULL)
	{
		json_delete(root_node);
		root_node = NULL;
	}

	if (tag_node != NULL)
	{
		json_delete(tag_node);
		tag_node = NULL;
	}

	if (time_raised_node != NULL)
	{
		json_delete(time_raised_node);
		time_raised_node = NULL;
	}

	return retval;
}


struct json_node *
qmonix_event_tojson(const struct qmonix_event *event)
{
	struct json_node *retval = NULL;
	struct json_node *root_node = NULL;

	q_assert(event != NULL);

	if (event->tojson_cb != NULL)
	{
		root_node = event->tojson_cb(event);
		q_assert_verbose(root_node != NULL, "Failed to exec tojson "
			"callback.");
	}
	else
	{
		root_node = qmonix_event_tojson_default(event);
		q_assert_verbose(root_node != NULL, "Failed to exec default "
			"tojson serializer.");
	}

	// success.
	retval = root_node;
	root_node = NULL;

done:
	if (root_node != NULL)
	{
		json_delete(root_node);
		root_node = NULL;
	}

	return retval;
}
