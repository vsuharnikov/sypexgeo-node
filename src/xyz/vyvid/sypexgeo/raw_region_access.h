/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_RAW_REGION_ACCESS_H_
#define SRC_XYZ_VYVID_SYPEXGEO_RAW_REGION_ACCESS_H_

#include <string>
#include <sstream>
#include <cstdint>

#include "xyz/vyvid/util/uint24_t.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

#pragma pack(push, 1)

struct RawRegionHeader {
  std::uint16_t country_seek;  // S:country_seek
  uint24_t      id;            // M:id
};

struct RawRegionAccess {
  static constexpr const char *FORMAT = "S:country_seek/M:id/b:name_ru/b:name_en/b:iso";

  const RawRegionHeader *header;
  const char            *name_ru;  // b:name_ru
  const char            *name_en;  // b:name_en
  const char            *iso;      // b:iso

  RawRegionAccess(const RawRegionHeader *header, const char *name_ru, const char *name_en, const char *iso)
    : header(header), name_ru(name_ru), name_en(name_en), iso(iso) {
  }

  explicit operator std::string() const {
    std::stringstream result;

    result << "country_seek: " << header->country_seek << std::endl;
    result << "id: " << header->id << std::endl;
    result << "name_ru: " << name_ru << std::endl;
    result << "name_en: " << name_en << std::endl;
    result << "iso: " << iso;

    return result.str();
  }
};

#pragma pack(pop)

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_RAW_REGION_ACCESS_H_
