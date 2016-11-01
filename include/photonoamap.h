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

}

/** Photon Map using Open Addressing
 *  scheme */
typedef struct 
{
  int size;

} photon_oa_map_t;
