#include <stdlib.h>
#include <string.h>

#include "util/qassert.h"

#include <qmonix/event_dispatcher.h>
#include <qmonix/event.h>
#include <qmonix/util/forward_list.h>


/**
 * Forward list item compare callback. Compares two events by their tag names.
 *
 * @param[in] first event (might be NULL).
 * @param[in] second event (might be NULL).
 * @return 1 if item datas matches.
 * @return 0 if item datas do not match.
 */
static int event_cmp_db(void *data1, void *data2);


struct qmonix_event_dispatcher
{
	void *data;
	struct qmonix_forward_list *events;

	qmonix_event_dispatcher_dispatch_cb dispatch_cb;
	qmonix_event_dispatcher_data_destroy_cb data_destroy_cb;
};


struct qmonix_event_dispatcher *
qmonix_event_dispatcher_create(qmonix_event_dispatcher_dispatch_cb dispatch_cb,
	void *dispatcher_data,
	qmonix_event_dispatcher_data_destroy_cb data_destroy_cb)
{
	struct qmonix_event_dispatcher *retval = NULL;
	struct qmonix_event_dispatcher *disp = NULL;

	q_assert(dispatch_cb != NULL);

	disp = (struct qmonix_event_dispatcher *)malloc(sizeof(*disp));
	q_assert_verbose(disp != NULL, "Failed to allocate memory for event "
		"dispatcher.");

	disp->dispatch_cb = dispatch_cb;
	disp->data = dispatcher_data;
	disp->data_destroy_cb = data_destroy_cb;

	disp->events = qmonix_forward_list_create(
		(qmonix_forward_list_destroy_item_data_cb)qmonix_event_destroy,
		event_cmp_db);
	q_assert_verbose(disp->events != NULL, "Failed to create event list.");

	// success.
	retval = disp;
	disp = NULL;

done:
	if (disp != NULL)
	{
		qmonix_event_dispatcher_destroy(disp);
		disp = NULL;
	}

	return retval;
}


void
qmonix_event_dispatcher_destroy(struct qmonix_event_dispatcher *disp)
{
	q_assert(disp != NULL);

	qmonix_forward_list_destroy(disp->events);
	disp->events = NULL;
	disp->dispatch_cb = NULL;

	if (disp->data_destroy_cb != NULL)
	{
		disp->data_destroy_cb(disp->data);
		disp->data = NULL;
	}

	free(disp);
	disp = NULL;

done:
	return;
}


int
qmonix_event_dispatcher_submit(struct qmonix_event_dispatcher *disp,
	struct qmonix_event *event)
{
	int retval = -1;
	struct qmonix_forward_list_item *evlist_item = NULL;

	q_assert(disp != NULL);
	q_assert(event != NULL);

	evlist_item = qmonix_forward_list_add(disp->events, event);
	q_assert_verbose(evlist_item != NULL, "Failed to add specified "
		"event to the list.");

	// success.
	retval = 0;

done:
	return retval;
}


int
qmonix_event_dispatcher_dispatch(struct qmonix_event_dispatcher *disp)
{
	int retval = -1;
	int ret = -1;

	q_assert(disp != NULL);

	ret = disp->dispatch_cb(disp);
	q_assert_verbose(ret == 0, "Failed to dispatch events.");

	// success.
	retval = 0;

done:
	return retval;
}


void *
qmonix_event_dispatcher_get_data(const struct qmonix_event_dispatcher *disp)
{
	void *retval = NULL;

	q_assert(disp != NULL);

	retval = disp->data;

done:
	return retval;
}


const struct qmonix_forward_list *
qmonix_event_dispatcher_get_events(const struct qmonix_event_dispatcher *disp)
{
	const struct qmonix_forward_list *retval = NULL;

	q_assert(disp != NULL);

	retval = disp->events;

done:
	return retval;
}


static int
event_cmp_db(void *data1, void *data2)
{
	struct qmonix_event *event1 = data1;
	struct qmonix_event *event2 = data2;
	int retval = 0;

	if (event1 == event2)
	{
		retval = 1;
	}
	else if ((event1 == NULL) && (event2 == NULL))
	{
		int ret = -1;
		const char *tag1 = NULL;
		const char *tag2 = NULL;

		tag1 = qmonix_event_get_tag(event1);
		tag2 = qmonix_event_get_tag(event2);
		ret = strcmp(tag1, tag2);
		if (ret == 0)
		{
			retval = 1;
		}
	}

	return retval;
}
