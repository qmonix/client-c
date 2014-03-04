/**
 * Forward list is a singly linked list. It allows constant item add operation
 * time.
 */

#ifndef INCLUDE_MzhkYmE3Y2IzY2QwZjk4NjM1ZGYwNmRh
#define INCLUDE_MzhkYmE3Y2IzY2QwZjk4NjM1ZGYwNmRh 1

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif


struct qmonix_forward_list;
struct qmonix_forward_list_item;


/**
 * Callback used to deallocate list item data.
 *
 * @param[in] item data (not NULL).
 */
typedef void (*qmonix_forward_list_destroy_item_data_cb)(void *data);


/**
 * Item data compare callback used to find items.
 *
 * @param[in] first item data (might be NULL).
 * @param[in] second item data (might be NULL).
 * @return 1 if item datas matches.
 * @return 0 if item datas do not match.
 */
typedef int (*qmonix_forward_list_cmp_item_data_cb)(void *data1, void *data2);


/**
 * Creates new forward list.
 *
 * @param[in] list item data destroy callback (not NULL).
 * @param[in] list item data compare callback (not NULL).
 * @return pointer to created forward list on success.
 * @return NULL on error.
 */
struct qmonix_forward_list *qmonix_forward_list_create(
	qmonix_forward_list_destroy_item_data_cb destroy_item_data_cb,
	qmonix_forward_list_cmp_item_data_cb cmp_item_data_cb);


/**
 * Destroys the specified forward list.
 *
 * @param[in] pointer to forward list (not NULL).
 */
void qmonix_forward_list_destroy(struct qmonix_forward_list *flist);


/**
 * Creates new list item with the specified data pointer and adds it at the
 * end of the list.
 *
 * The specified item data is not copied. qmonix_forward_list_destroy destroys the
 * items data with the callback provided when creating forward list.
 *
 * @param[in] pointer to forward list (not NULL).
 * @param[in] pointer to item data (might be NULL).
 * @return pointer to added list item on success.
 * @return NULL on error.
 */
struct qmonix_forward_list_item *qmonix_forward_list_add(
	struct qmonix_forward_list *flist, void *data);


/**
 * Removes the specified item from the list. This item is destroyed.
 *
 * @param[in] pointer to forward list (not NULL).
 * @param[in] pointer to forward list item (not NULL).
 * @return 0 on success.
 * @return -1 on error. E.g. if the specified element is not found.
 */
int qmonix_forward_list_remove(struct qmonix_forward_list *flist,
	struct qmonix_forward_list_item *flist_item);


/**
 * Searches the forward list item with the specified data. To compare item
 * data, the callback provided when creating forward list is used.
 *
 * @param[in] pointer to forward list (not NULL).
 * @param[in] data to find (might be NULL).
 * @return pointer to link element on success.
 * @return NULL if element was not found or on error.
 */
struct qmonix_forward_list_item *qmonix_forward_list_find(
	const struct qmonix_forward_list *flist, void *data);


/**
 * Returns first element of the forward list.
 *
 * @param[in] pointer to forward list (not NULL).
 * @return pointer to first link element on success.
 * @return NULL if list is empty or on error.
 */
struct qmonix_forward_list_item *qmonix_forward_list_first(
	const struct qmonix_forward_list *flist);


/**
 * Returns next element from the specified list item.
 *
 * @param[in] pointer to forward list item (not NULL).
 * @return pointer to next link item on success.
 * @return NULL if the specified item is the last in the list or on error.
 */
struct qmonix_forward_list_item *qmonix_forward_list_next(
	const struct qmonix_forward_list_item *flist_item);


/**
 * Returns forward list size. This operation complexity is O(1).
 *
 * @param[in] pointer to forward list (not NULL).
 * @return forward list size.
 */
size_t qmonix_forward_list_size(const struct qmonix_forward_list *flist);


/**
 * Returns data of the specified forward list item.
 *
 * @param[in] pointer to forward list item (not NULL).
 * @return pointer to list item data (might be NULL).
 */
void *qmonix_forward_list_data(
	const struct qmonix_forward_list_item *flist_item);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MzhkYmE3Y2IzY2QwZjk4NjM1ZGYwNmRh */
