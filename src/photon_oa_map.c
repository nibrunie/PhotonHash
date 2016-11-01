#include "photonoamap.h"


void photon_oa_map_init(photon_oa_map_t* map, int size)
{
  map->size = size;
  map->buckets = calloc(size, sizeof(photon_oa_cell_t));
}

int photon_oa_map_insert(photon_oa_map_t* map, PHOTON_KEY_TYPE key, PHOTON_VALUE_TYPE value) 
{
  
}

PHOTON_VALUE_TYPE photon_oa_map_get(photon_oa_map_t* map, PHOTON_KEY_TYPE key)
{
}
