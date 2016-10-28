#include "photonhash.h"
#include <stdlib.h>


void photon_map_init(photon_map_t* map, int key_map_size, int cell_num)
{
  map->cell_num     = cell_num;
  map->key_map_size = key_map_size;

  // allocating internal arrays
  map->key2first_map = calloc(key_map_size, sizeof(photon_map_t*));
  map->cells = calloc(cell_num, sizeof(photon_cell_t));

  // building empty list
  map->first_empty_cell = map->cells;
  if (cell_num > 0) {
    map->first_empty_cell->next     = map->cells + 1;
    map->cells[0].previous = NULL;
    map->cells[0].key      = 0;
    map->cells[0].value    = 0;
  }
  for (int cid = 1; cid < cell_num - 1; ++cid) {
    map->cells[cid].next     = map->cells + (cid+1);
    map->cells[cid].previous = map->cells + (cid-1);
    map->cells[cid].key      = 0;
    map->cells[cid].value    = 0;
  }

  map->cells[cell_num].next     = NULL;
  map->cells[cell_num].previous = map->cells + (cell_num - 1);

};

int photon_map_insert(photon_map_t* map, PHOTON_KEY_TYPE key, PHOTON_VALUE_TYPE value)
{
  photon_cell_t* cell = map->first_empty_cell;
  // no empty cell available
  if (!cell) return 0;

  map->first_empty_cell = cell->next;

  cell->key = key;
  cell->value = value;

  PHOTON_HASH_TYPE hash = PHOTON_HASH_FUNCTION(key); 
  hash %= map->key_map_size;

  photon_cell_t* list_head = map->key2first_map[hash];

  if (list_head) 
  {
    list_head->previous = cell;
    cell->next = list_head;
  } else {
    cell->next = NULL;
  }

  cell->previous = NULL;
  map->key2first_map[hash] = cell;


  return 1;
}

static photon_cell_t* photon_map_get_cell(photon_map_t* map, PHOTON_KEY_TYPE key)
{
  photon_cell_t* cell = NULL;
  PHOTON_HASH_TYPE hash = PHOTON_HASH_FUNCTION(key); 
  hash %= map->key_map_size;

  for (cell = map->key2first_map[hash]; cell; cell = cell->next)
  {
    if (cell->key == key) break;
  }

  return cell;
}

PHOTON_VALUE_TYPE photon_map_get(photon_map_t* map, PHOTON_KEY_TYPE key)
{
  photon_cell_t* cell = photon_map_get_cell(map, key);
  return cell->value;
}

int photon_map_contains(photon_map_t* map, PHOTON_KEY_TYPE key) 
{
  photon_cell_t* cell = photon_map_get_cell(map, key);

  // if cell is not null
  if (cell) return 1;
  return 0;
}

int photon_map_remove(photon_map_t* map, PHOTON_KEY_TYPE key)
{
  photon_cell_t* cell = photon_map_get_cell(map, key);
  if (cell) {
    if (cell->previous) {
      cell->previous->next = cell->next;
    } else {
      PHOTON_HASH_TYPE hash = PHOTON_HASH_FUNCTION(key); 
      hash %= map->key_map_size;

      map->key2first_map[hash] = cell->next;
    } 
    if (cell->next)     cell->next->previous = cell->previous;
    
    // enqueueing in empty cell
    cell->next = map->first_empty_cell;
    cell->previous = NULL;
    map->first_empty_cell = cell;
  }

  return 1;
}

