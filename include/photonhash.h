#pragma ONCE

#ifndef PHOTON_KEY_TYPE 
#define PHOTON_KEY_TYPE long
#endif
#ifndef PHOTON_VALUE_TYPE
#define PHOTON_VALUE_TYPE void*
#endif
#ifndef PHOTON_HASH_TYPE
#define PHOTON_HASH_TYPE uint32_t
#endif


/* *******************************************************************
*  32-bits hash functions (xxhash from cyan4973/Y.C
*   https://github.com/Cyan4973/xxHash
*********************************************************************/
static const uint32_t PRIME32_1 = 2654435761U;
static const uint32_t PRIME32_2 = 2246822519U;
static const uint32_t PRIME32_3 = 3266489917U;
static const uint32_t PRIME32_4 =  668265263U;
static const uint32_t PRIME32_5 =  374761393U;

static uint32_t XXH32_round(uint32_t seed, uint32_t input)
{
    seed += input * PRIME32_2;
    seed  = (seed <<13) | (seed >> 19);
    seed *= PRIME32_1;
    return seed;
}

#ifndef PHOTON_HASH_FUNCTION
#define PHOTON_HASH_FUNCTION(x) XXH32_round(PRIME32_3, x)
#endif


struct photon_cell_s;

typedef struct photon_cell_s 
{
  PHOTON_KEY_TYPE   key;
  PHOTON_VALUE_TYPE value;
  struct photon_cell_s* next;
  struct photon_cell_s* previous;
} 
photon_cell_t;

typedef struct 
{
  /** size of the key2first_map */
  int key_map_size;
  /** correspondance table between hashed key and first cell
   *  of corresponding <record> list 
   */
  photon_cell_t** key2first_map;
  /** table containing the list of records */
  photon_cell_t*  cells;
  /** size of the @p cells table */
  int             cell_num;
  /** pointer toward the first element of the empty cell list */
  photon_cell_t*  first_empty_cell;
} 
photon_map_t;

/** Initialize a photon map of @p cell_nums value 
 *  @param map point to the Photon Map to initialize
 *  @param key_map_size size of the key to cell map
 *  @param cell_num number of cells (max capacity) of the map
 */
void photon_map_init(photon_map_t* map, int key_map_size, int cell_num);

/** Insert a new <key, value> record in the map
 *  @param map address of the Photon Map where to insert the new record
 *  @param key key of the new record
 *  @param value value of the new_record
 */
int photon_map_insert(photon_map_t* map, PHOTON_KEY_TYPE key, PHOTON_VALUE_TYPE value);

/** Retrieve a record indexed by @p key in @p map
 *  @param map to search the record in
 *  @param key index of the record
 *  @return value associated  with @p key in a record
 */
PHOTON_VALUE_TYPE photon_map_get(photon_map_t* map, PHOTON_KEY_TYPE key);

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

