/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef _XYZ_VYVID_SYPEXGEO_UTIL_UINT24_T_H_
#define _XYZ_VYVID_SYPEXGEO_UTIL_UINT24_T_H_

#include <stdint.h>

typedef struct uint24_t {
  uint8_t bytes[3];

  uint24_t() : bytes{0, 0, 0} {
  }

  operator uint32_t() const {
    uint32_t uint32 = 0;

    uint32 += 0 // For pretty formatting.
      + (bytes[0] << 0)
      + (bytes[1] << 8)
      + (bytes[2] << 16);

    return uint32;
  }
} uint24_t;

#endif  // _XYZ_VYVID_SYPEXGEO_UTIL_UINT24_T_H_