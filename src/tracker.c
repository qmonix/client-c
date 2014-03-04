#include <string.h>

#include "util/qassert.h"

#include <qmonix/tracker.h>
#include <qmonix/event_dispatcher.h>
#include <qmonix/http_event_dispatcher.h>
#include <qmonix/event.h>
#include <qmonix/timing_event.h>


/**
 * Timing event fire callback.
 *
 * @param[in] pointer to timing event (not NULL).
 * @param[in] pointer to callback handler data (might be NULL).
 */
static int timing_event_fire_cb(struct qmonix_event *event, void *data);


static struct qmonix_tracker
{
	struct qmonix_event_dispatcher *dispatcher;
	int inited;
} tracker;


int
qmonix_tracker_init(const char *server_address)
{
	int retval = -1;

	q_assert(server_address != NULL);

	tracker.dispatcher = qmonix_http_event_dispatcher_create(
		server_address);
	q_assert_verbose(tracker.dispatcher != NULL, "Failed to create http "
		"event dispatcher.");

	// success.
	retval = 0;
	tracker.inited = 1;

done:
	if (retval != 0)
	{
		qmonix_tracker_destroy();
	}

	return retval;
}


void
qmonix_tracker_destroy(void)
{
	if (tracker.dispatcher != NULL)
	{
		qmonix_event_dispatcher_destroy(tracker.dispatcher);
		tracker.dispatcher = NULL;
	}

	tracker.inited = 0;
}


int
qmonix_tracker_fire(const char *tag)
{
	int retval = -1;
	struct qmonix_event *event = NULL;
	char *tag_copy = NULL;
	int ret = -1;

	q_assert(tag != NULL);

	tag_copy = strdup(tag);
	q_assert_verbose(tag_copy != NULL, "Failed to copy tag.");

	event = qmonix_event_create(tag_copy, NULL, NULL, NULL);
	q_assert_verbose(event != NULL, "Failed to create event: %s.", tag);
	tag_copy = NULL;

	ret = qmonix_event_dispatcher_submit(tracker.dispatcher, event);
	q_assert_verbose(ret == 0, "Failed to submit event to dispatcher.");

	// success.
	retval = 0;

done:
	if (tag_copy != NULL)
	{
		free(tag_copy);
		tag_copy = NULL;
	}

	return retval;
}


struct qmonix_event *
qmonix_tracker_start(const char *tag)
{
	struct qmonix_event *retval = NULL;
	struct qmonix_event *event = NULL;
	char *tag_copy = NULL;

	q_assert(tag != NULL);

	tag_copy = strdup(tag);
	q_assert_verbose(tag_copy != NULL, "Failed to copy tag.");

	event = qmonix_timing_event_create(tag_copy, timing_event_fire_cb,
		NULL);
	q_assert_verbose(event != NULL, "Failed to create event: %s.", tag);
	tag_copy = NULL;

	// success.
	retval = event;
	event = NULL;

done:
	if (event != NULL)
	{
		qmonix_event_destroy(event);
		event = NULL;
	}

	if (tag_copy != NULL)
	{
		free(tag_copy);
		tag_copy = NULL;
	}

	return retval;
}


int
qmonix_tracker_dispatch(void)
{
	int retval = -1;
	int ret = -1;

	ret = qmonix_event_dispatcher_dispatch(tracker.dispatcher);
	q_assert_verbose(ret == 0, "Failed to dispatch events.");

	// success.
	retval = 0;

done:
	return retval;
}


static int
timing_event_fire_cb(struct qmonix_event *event, void *data)
{
	int retval = -1;
	int ret = -1;

	q_assert(event != NULL);
	data = data;

	ret = qmonix_event_dispatcher_submit(tracker.dispatcher, event);
	q_assert_verbose(ret == 0, "Failed to submit event to dispatcher.");

	// success.
	retval = 0;

done:
	return retval;
}
