#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <qmonix/event_dispatcher.h>
#include <qmonix/http_event_dispatcher.h>
#include <qmonix/util/forward_list.h>
#include <qmonix/tracker.h>

#include <json/json.h>
#include <curl/curl.h>

#include "util/qassert.h"


/**
 * Event dispatch callback. Serializes collected events to json and sends
 * them over HTTP.
 *
 * @param[in] event dispatcher (not NULL).
 * @return 0 on success.
 * @retrun -1 on error.
 */
static int dispatch_cb(struct qmonix_event_dispatcher *disp);


/**
 * Deinitializes curl library.
 *
 * @param[in] pointer to dispatcher data (might be NULL). Not used.
 */
static void deinit_libcurl(void *data);


/**
 * Sends HTTP POST request to the specified URL.
 *
 * @param[in] target URL (not NULL).
 * @param[in] post data (not NULL).
 * @return 0 on success.
 * @return -1 on error.
 */
static int http_post(const char *url, const char *data);


/**
 * This callback is called when curl receives HTTP response data.
 */
size_t curl_write_result_cb(char *ptr, size_t size, size_t nmemb,
	void *userdata);


struct qmonix_event_dispatcher *
qmonix_http_event_dispatcher_create(const char *server_address)
{
	struct qmonix_event_dispatcher *retval = NULL;
	struct qmonix_event_dispatcher *disp = NULL;

	q_assert(server_address != NULL);

	disp = qmonix_event_dispatcher_create(dispatch_cb,
		(void *)server_address, deinit_libcurl);
	q_assert_verbose(disp != NULL, "Failed to create event dispatcher.");

	curl_global_init(CURL_GLOBAL_ALL);

	// success.
	retval = disp;
	disp = NULL;

done:
	return retval;
}


static int
dispatch_cb(struct qmonix_event_dispatcher *disp)
{
	int retval = -1;
	const struct qmonix_forward_list *events = NULL;
	struct qmonix_forward_list_item *flist_item = NULL;
	struct json_node *root_node = NULL;
	struct json_node *events_node = NULL;
	struct json_node *version_node = NULL;
	struct json_node *when_sent_node = NULL;
	char *post_data = NULL;
	const char *post_url = NULL;
	int ret = -1;

	q_assert(disp != NULL);

	events = qmonix_event_dispatcher_get_events(disp);
	q_assert_verbose(events != NULL, "Failed to get event list.");

	root_node = json_mkobject();
	q_assert_verbose(root_node != NULL, "Failed to create json root node.");

	version_node = json_mkstring(QMONIX_TRACKER_VERSION);
	q_assert_verbose(version_node != NULL, "Failed to create version node.");

	json_append_member(root_node, "version", version_node);

	events_node = json_mkarray();
	q_assert_verbose(events_node != NULL, "Failed to create event array "
		"node.");

	json_append_member(root_node, "events", events_node);

	flist_item = qmonix_forward_list_first(events);
	while (flist_item != NULL)
	{
		struct qmonix_event *event = NULL;
		struct json_node *event_node = NULL;

		event = (struct qmonix_event *)qmonix_forward_list_data(
			flist_item);
		q_assert_verbose(event != NULL, "Failed to get event "
			"form the list.");

		event_node = qmonix_event_tojson(event);
		q_assert_verbose(event_node != NULL, "Failed to get event "
			"node.");

		json_append_element(events_node, event_node);

		flist_item = qmonix_forward_list_next(flist_item);
	}

	when_sent_node = json_mknumber((double)time(NULL));
	q_assert_verbose(when_sent_node != NULL, "Failed to create when sent "
		"node.");

	json_append_member(root_node, "whenSent", when_sent_node);

	post_data = json_encode(root_node);
	q_assert_verbose(post_data != NULL, "Failed to encode json to string.");

	post_url = (const char *)qmonix_event_dispatcher_get_data(disp);
	q_assert_verbose(post_url != NULL, "Http dispatcher url is NULL.");

	ret = http_post(post_url, post_data);
	q_assert_verbose(ret == 0, "Failed to send http request.");

	// success.
	retval = 0;

done:
	if (root_node != NULL)
	{
		json_delete(root_node);
		root_node = NULL;
	}

	if (post_data != NULL)
	{
		free(post_data);
		post_data = NULL;
	}

	return retval;
}


static int
http_post(const char *url, const char *data)
{
	int retval = -1;
	CURL *curl = NULL;
	CURLcode ret;

	q_assert(url != NULL);
	q_assert(data != NULL);

	curl = curl_easy_init();
	q_assert_verbose(curl != NULL, "Failed to init curl.");

	ret = curl_easy_setopt(curl, CURLOPT_URL, url);
	q_assert_verbose(ret == CURLE_OK, "Failed to set url.");

	ret = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	q_assert_verbose(ret == CURLE_OK, "Failed to set POST data.");

	ret = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
		curl_write_result_cb);
	q_assert_verbose(ret == CURLE_OK, "Failed to set curl write callback.");

	ret = curl_easy_perform(curl);
	q_assert_verbose(ret == CURLE_OK, "Failed to send http request.");

	curl_easy_cleanup(curl);
	curl = NULL;

	// success.
	retval = 0;

done:
	return retval;
}


size_t
curl_write_result_cb(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t retval = 0;

	userdata = userdata;

	log_info("Received: %s\n", ptr);

	retval = size * nmemb;

	return retval;
}


static void
deinit_libcurl(void *data)
{
	data = data;

	curl_global_cleanup();
}
