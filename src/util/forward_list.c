#include <stdlib.h>
#include <stdio.h>

#include <qmonix/util/forward_list.h>


#ifndef LOG
#define LOG(format, ...) \
	fprintf(stderr, "QMONIX_FORWARD_LIST: %s:%d: " format "\n", \
		__FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif


struct qmonix_forward_list_item
{
	void *data;
	struct qmonix_forward_list_item *next;
};


struct qmonix_forward_list
{
	size_t size;
	struct qmonix_forward_list_item *first_item;
	struct qmonix_forward_list_item *last_item;

	qmonix_forward_list_destroy_item_data_cb destroy_item_data_cb;
	qmonix_forward_list_cmp_item_data_cb cmp_item_data_cb;
};


/**
 * Creates new forward list.
 *
 * @param[in] list item data destroy callback (not NULL).
 * @param[in] list item data compare callback (not NULL).
 * @return pointer to created forward list on success.
 * @return NULL on error.
 */
struct qmonix_forward_list *
qmonix_forward_list_create(
	qmonix_forward_list_destroy_item_data_cb destroy_item_data_cb,
	qmonix_forward_list_cmp_item_data_cb cmp_item_data_cb)
{
	struct qmonix_forward_list *retval = NULL;
	struct qmonix_forward_list *new_list = NULL;

	if (destroy_item_data_cb == NULL)
	{
		LOG("%s", "Destroy item data callback is NULL.");
		goto done;
	}

	if (cmp_item_data_cb == NULL)
	{
		LOG("%s", "Compate item data callback is NULL.");
		goto done;
	}

	new_list = (struct qmonix_forward_list *)malloc(sizeof(*new_list));
	if (new_list == NULL)
	{
		LOG("%s", "Failed to allocated memory.");
		goto done;
	}

	new_list->size = 0;
	new_list->first_item = NULL;
	new_list->last_item = NULL;
	new_list->destroy_item_data_cb = destroy_item_data_cb;
	new_list->cmp_item_data_cb = cmp_item_data_cb;

	// success.
	retval = new_list;
	new_list = NULL;

done:
	if (new_list != NULL)
	{
		free(new_list);
		new_list = NULL;
	}

	return retval;
}


void
qmonix_forward_list_destroy(struct qmonix_forward_list *flist)
{
	struct qmonix_forward_list_item *item = NULL;

	if (flist == NULL)
	{
		goto done;
	}

	item = flist->first_item;
	flist->first_item = NULL;
	while (item != NULL)
	{
		struct qmonix_forward_list_item *next_item = item->next;

		flist->destroy_item_data_cb(item->data);
		item->data = NULL;

		free(item);
		item = NULL;

		item = next_item;
	}

	free(flist);
	flist = NULL;

done:
	return;
}


struct qmonix_forward_list_item *
qmonix_forward_list_add(struct qmonix_forward_list *flist, void *data)
{
	struct qmonix_forward_list_item *retval = NULL;
	struct qmonix_forward_list_item *new_item = NULL;

	if (flist == NULL)
	{
		LOG("%s", "Forward list is NULL.");
		goto done;
	}

	new_item = (struct qmonix_forward_list_item *)malloc(sizeof(*new_item));
	if (new_item == NULL)
	{
		LOG("%s", "Failed to allocate memory for new "
			"item.");
		goto done;
	}
	new_item->data = data;
	new_item->next = NULL;

	// empty list
	if (flist->first_item == NULL)
	{
		flist->first_item = new_item;
		flist->last_item = new_item;
		flist->size = 1;
	}
	else
	{
		flist->last_item->next = new_item;
		flist->last_item = new_item;
		flist->size++;
	}

	// success.
	retval = new_item;
	new_item = NULL;

done:
	if (new_item != NULL)
	{
		free(new_item);
		new_item = NULL;
	}

	return retval;
}


int
qmonix_forward_list_remove(struct qmonix_forward_list *flist,
	struct qmonix_forward_list_item *flist_item)
{
	int retval = -1;
	struct qmonix_forward_list_item *prev_item = NULL;
	int removed = 0;

	if (flist == NULL)
	{
		LOG("%s", "Forward list is NULL.");
		goto done;
	}

	if (flist_item == NULL)
	{
		LOG("%s", "Item to remove is NULL.");
		goto done;
	}

	// remove first item.
	if (flist_item == flist->first_item)
	{
		flist->first_item = flist->first_item->next;

		flist->destroy_item_data_cb(flist_item->data);
		flist_item->data = NULL;
		free(flist_item);
		flist_item = NULL;

		removed = 1;
	}
	else
	{
		prev_item = flist->first_item;
		while (prev_item->next != NULL)
		{
			struct qmonix_forward_list_item *rm_item =
				prev_item->next;

			if (rm_item == flist_item)
			{
				prev_item->next = rm_item->next;

				// removing last item.
				if (flist->last_item == flist_item)
				{
					flist->last_item = flist_item->next;
				}

				flist->destroy_item_data_cb(rm_item->data);
				rm_item->data = NULL;

				free(rm_item);
				rm_item = NULL;

				removed = 1;

				break;
			}

			prev_item = prev_item->next;
		}
	}

	// success.
	if (removed)
	{
		flist->size--;
		retval = 0;
	}

done:
	return retval;
}


struct qmonix_forward_list_item *
qmonix_forward_list_find(const struct qmonix_forward_list *flist, void *data)
{
	struct qmonix_forward_list_item *retval = NULL;
	struct qmonix_forward_list_item *item = NULL;
	int found = 0;

	if (flist == NULL)
	{
		LOG("%s", "Forward list is NULL.");
		goto done;
	}

	item = flist->first_item;
	while (item != NULL)
	{
		if (flist->cmp_item_data_cb(item->data, data))
		{
			found = 1;
			break;
		}

		item = item->next;
	}

	if (found)
	{
		retval = item;
		item = NULL;
	}

done:
	return retval;
}


struct qmonix_forward_list_item *
qmonix_forward_list_first(const struct qmonix_forward_list *flist)
{
	struct qmonix_forward_list_item *retval = NULL;

	if (flist == NULL)
	{
		LOG("%s", "Forward list is NULL.");
		goto done;
	}

	retval = flist->first_item;

done:
	return retval;
}


struct qmonix_forward_list_item *
qmonix_forward_list_next(const struct qmonix_forward_list_item *flist_item)
{
	struct qmonix_forward_list_item *retval = NULL;

	if (flist_item  == NULL)
	{
		LOG("%s", "Forward list item is NULL.");
		goto done;
	}

	retval = flist_item->next;

done:
	return retval;

}


size_t
qmonix_forward_list_size(const struct qmonix_forward_list *flist)
{
	size_t retval = 0;

	if (flist == NULL)
	{
		LOG("%s", "Forward list is NULL.");
		goto done;
	}

	retval = flist->size;

done:
	return retval;

}


void *
qmonix_forward_list_data(const struct qmonix_forward_list_item *flist_item)
{
	void *retval = NULL;

	if (flist_item == NULL)
	{
		LOG("%s", "Forward list item is NULL.");
		goto done;
	}

	retval = flist_item->data;

done:
	return retval;

}
