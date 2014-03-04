#ifndef INCLUDE_MjBjOWVlYzkzZjVjYTU1OWE1NzllODVi
#define INCLUDE_MjBjOWVlYzkzZjVjYTU1OWE1NzllODVi 1


#include <stdint.h>


#ifdef __cplusplus
extern "C"
{
#endif


#include <qmonix/event.h>


/**
 * Creates new volume event with the specified tag and volume value.
 * Use qmonix_event_destroy() to destroy it.
 *
 * @param[in] event tag name (not NULL).
 * @param[in] event volume.
 * @return pointer to new event on success.
 * @return NULL on error.
 */
struct qmonix_event *qmonix_volume_event_create(char *tag, uint64_t volume);


/**
 * Returns volume of the specified volume event.
 *
 * @param[in] pointer to volume event (not NULL).
 * @return event volume.
 */
uint64_t qmonix_volume_event_get_volume(struct qmonix_event *event);


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MjBjOWVlYzkzZjVjYTU1OWE1NzllODVi */
