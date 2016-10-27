#pragma ONCE

#ifndef PHOTON_KEY_TYPE 
#define PHOTON_KEY_TYPE long
#endif
#ifndef PHOTON_VALUE_TYPE
#define PHOTON_VALUE_TYPE void*
#endif


struct photon_cell_s;

typedef struct photon_cell_s {
  PHOTON_VALUE_TYPE value;
  struct photon_cell_s* next;
} photon_cell_t;

typedef struct {
  photon_cell_t** key2first_map;
  photon_cell_t*  cells;
  int             cell_num;
  photon_cell_t*  first_empty_cell;
} photon_map_t;

/** Initialize a photon map of @p cell_nums value 
 *  @param map point to the Photon Map to initialize
 *  @param cell_num number of cells (max capacity) of the map
 */
void photon_map_init(photon_map_t* map, cell_num);

/** Insert a new <key, value> record in the map
 *  @param map address of the Photon Map where to insert the new record
 *  @param key key of the new record
 *  @param value value of the new_record
 */
void photon_map_insert(photon_map_t* map, PHOTON_KEY_TYPE key, PHOTON_VALUE_TYPE value);

/** Test if the table @p map containes the <key>
 *  @param map address of the Photon Map where to insert the new record
 *  @param key key of the record to be tested
 *  @return zero if the record is not in @p map else non-zero value
 */
int photon_map_contains(photon_map_t* map, PHOTON_VALUE_TYPE key);

/** Remove the record whose key is @p key in @p map 
 *  @param map address of the Photon Map where to insert the new record
 *  @param key key of the record to be removed
 *  @return zero if the record was not removed from @p map else non-zero value
 */
int photon_map_remove(photon_map_t* map, PHOTON_VALUE_TYPE key);

