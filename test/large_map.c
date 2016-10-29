#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include <photonhash.h>
#include <timing.h>

#define RECORD_NUM 1000000
#define KEY_MAP_SIZE 350000

int main(void) {
  photon_map_t* map = calloc(1, sizeof(photon_map_t));;
  int key_map_size = KEY_MAP_SIZE;
  int cell_num = RECORD_NUM;

  printf("initializing photon map\n");
  photon_map_init(map, key_map_size, cell_num); 

  long excluded_key = rand();

  long keys[RECORD_NUM]; 
  long values[RECORD_NUM]; 
  long valid_key = excluded_key + 1;
  for (int i = 0; i < RECORD_NUM; ++i) {
    // increment to ensure unicity
    keys[i] = valid_key++;
    values[i] = rand();
  }

  clock_t start, end;

  printf("inserting %d records into map\n", RECORD_NUM);
  start = clock();
  for (int i = 0; i < RECORD_NUM; ++i) {
    int insert_r = photon_map_insert(map, keys[i], (void*) values[i]);
    if (!insert_r) return -1;
  };
  end = clock();
  double timing_insert = clock2sec(end - start);

  printf("testing insertion of overflowing record\n");
  int insert_overflow = photon_map_insert(map, valid_key + 1, (void*) rand());
  if (insert_overflow) return -1;

  printf("retrieving records from map\n");
  start = clock();
  for (int i = RECORD_NUM - 1; i >= 0; i--) {
    long value = (long) photon_map_get(map, keys[i]);
    if (value != values[i]) return -1;
  };
  end = clock();
  double timing_get = clock2sec(end - start);

  printf("testing contain functionnality\n");
  start = clock();
  for (int i = RECORD_NUM - 1; i >= 0; i--) {
    int contain = photon_map_contains(map, keys[i]);
    if (!contain) return -1;
  }
  end = clock();
  double timing_contain = clock2sec(end - start);

  int contain_excluded = photon_map_contains(map, excluded_key);
  printf("map contains excluded_key=%d\n", contain_excluded);
  if (contain_excluded) return -1;

  printf("testing removal\n");
  start = clock();
  for (int i = 0; i < RECORD_NUM; i += 4) {
    int pre_contain_r = photon_map_contains(map, keys[i]);
    int remove_r = photon_map_remove(map, keys[i]);
    int contain_r = photon_map_contains(map, keys[i]);
    if (!remove_r || contain_r) return -1;
  }
  end = clock();
  double timing_remove = clock2sec(end - start);

  printf("testing insertion post removal\n");
  for (int i = 0; i < RECORD_NUM; i += 4) {
    int insert_r = photon_map_insert(map, 2 * keys[i], (void*) (2 * values[i]));
    if (!insert_r) return -1;
  }
  printf("testing contains post removal\n");
  for (int i = RECORD_NUM - 1; i >= 0; i--) {
    long key = (i % 4) ? keys[i] : (2 * keys[i]); 
    int contain = photon_map_contains(map, key);
    if (!contain) {
      return -1;
    }
  }

  printf("hash function statistics:\n");
  int* count_map = calloc(key_map_size, sizeof(int));
  for (int kid = 0; kid < RECORD_NUM; ++kid)
  {
    PHOTON_HASH_TYPE hash = PHOTON_HASH_FUNCTION(keys[kid]) % key_map_size;
    count_map[hash]++;
  }
  int max_count = 0, min_count = INT_MAX;
  double average = 0.0;
  double std_deviation = 0.0;
  for (int cid = 0; cid < key_map_size; ++cid)
  {
    if (count_map[cid] > max_count) max_count = count_map[cid];
    if (count_map[cid] < min_count) min_count = count_map[cid];

    average += cid * (double) count_map[cid];
  }

  printf("max count is %d\n", max_count);
  printf("min count is %d\n", min_count);

  average /= (double) RECORD_NUM;
  printf("average is %.3f\n", average);

  for (int cid = 0; cid < key_map_size; ++cid)
  {
    double diff = cid - average;
    std_deviation += count_map[cid] / (double) RECORD_NUM * diff * diff;
  }

  std_deviation = sqrt(std_deviation);
  printf("standard deviation is %.3f\n", std_deviation);

  printf("timing report: \n");
  printf("  inserting timing %.3fs\n", timing_insert);
  printf("  get       timing %.3fs\n", timing_get);
  printf("  contains  timing %.3fs\n", timing_contain);
  printf("  remove    timing %.3fs\n", timing_remove);


  return 0;
}
