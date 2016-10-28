#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <photonhash.h>

#define RECORD_NUM 32

int main(void) {
  photon_map_t* map = calloc(1, sizeof(photon_map_t));;
  int key_map_size = RECORD_NUM / 4;
  int cell_num = RECORD_NUM;

  printf("initializing photon map\n");
  photon_map_init(map, 16, 32); 

  long excluded_key = rand();

  long keys[RECORD_NUM]; // = {17, 42, 1337};
  long values[RECORD_NUM]; // = {171, 421, 13371};
  for (int i = 0; i < RECORD_NUM; ++i) {
    while ((keys[i] = rand()) == excluded_key) {};
    values[i] = rand();
  }

  printf("inserting %d records into map\n", RECORD_NUM);
  for (int i = 0; i < RECORD_NUM; ++i) {
    int insert_r = photon_map_insert(map, keys[i], values[i]);
  };
  printf("retrieving records from map\n");
  for (int i = RECORD_NUM - 1; i >= 0; i--) {
    long value = (long) photon_map_get(map, keys[i]);
    printf("record<%ld, %ld> -> %ld\n", keys[i], values[i], value);
    if (value != values[i]) return -1;
  };

  printf("testing contain functionnality\n");
  for (int i = RECORD_NUM - 1; i >= 0; i--) {
    int contain = photon_map_contains(map, keys[i]);
    printf("map contains keys[%d]: %d\n", i, contain);
    if (!contain) return -1;
  }

  int contain_excluded = photon_map_contains(map, excluded_key);
  printf("map contains excluded_key=%d\n", contain_excluded);
  if (contain_excluded) return -1;



  return 0;
}
