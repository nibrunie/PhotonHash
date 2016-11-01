#pragma ONCE

#include <stdint.h>

#ifndef PHOTON_KEY_TYPE 
#define PHOTON_KEY_TYPE uint64_t
#endif
#ifndef PHOTON_VALUE_TYPE
#define PHOTON_VALUE_TYPE uint64_t
#endif
#ifndef PHOTON_HASH_TYPE
#define PHOTON_HASH_TYPE uint32_t
#endif


/** Photon Cell for Open Addressing based map */
typedef struct 
{
  PHOTON_KEY_TYPE key;
  PHOTON_VALUE_TYPE value;
}
photon_oa_cell_t;

/** Photon Map using Open Addressing
 *  scheme */
typedef struct 
{
  photon_oa_cell_t* buckets;
  int size;
} photon_oa_map_t;


/** Initialize an open-addressing based map
 *  @param map to be initialized
 *  @param size number of bucket to allocate in the map
 */
void photon_oa_map_init(photon_oa_map_t* map, int size);

/** Insert a new value for the record <key, value> 
 *  @param map hashtable where to insert a record
 *  @param key key of the new record
 *  @param value value of the new record
 *  @return 1 if the insertion succeed, 0 otherwise
 */
int photon_oa_map_insert(photon_oa_map_t* map, PHOTON_KEY_TYPE key, PHOTON_VALUE_TYPE value);

/** Retrieve the record value associated with @p key (if any)
 *  @param map hashtable where to search for a record
 *  @param key key to be searched 
 *  @return value associated to @p key in @p map
 */
PHOTON_VALUE_TYPE photon_oa_map_get(photon_oa_map_t* map, PHOTON_KEY_TYPE key);
