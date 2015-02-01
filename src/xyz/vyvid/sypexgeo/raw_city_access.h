/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_RAW_CITY_ACCESS_H_
#define SRC_XYZ_VYVID_SYPEXGEO_RAW_CITY_ACCESS_H_

#include <string>
#include <sstream>
#include <cstdint>
#include <cmath>

#include "xyz/vyvid/util/uint24_t.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

#pragma pack(push, 1)

struct RawCityHeader {
  uint24_t     region_seek;  // M:region_seek
  std::uint8_t country_id;   // T:country_id
  uint24_t     id;           // M:id
  std::int32_t lat;          // N5:lat
  std::int32_t lon;          // N5:lon
};

struct RawCityAccess {
  static constexpr const char *FORMAT = "M:region_seek/T:country_id/M:id/N5:lat/N5:lon/b:name_ru/b:name_en";

  const RawCityHeader *header;
  const char          *name_ru;  // b:name_ru
  const char          *name_en;  // b:name_en

  RawCityAccess(const RawCityHeader *header, const char *name_ru, const char *name_en)
    : header(header), name_ru(name_ru), name_en(name_en) {
  }

  double getLongitude() const {
    return header->lon / std::pow(10, 5);  // N5:lon
  }

  double getLatitude() const {
    return header->lat / std::pow(10, 5);  // N5:lat
  }

  explicit operator std::string() const {
    std::stringstream result;

    result << "region_seek: " << header->region_seek << std::endl;
    result << "country_id: " << header->country_id << std::endl;
    result << "id: " << header->id << std::endl;
    result << "lat: " << header->lat << " (" << getLatitude() << ")" << std::endl;
    result << "lon: " << header->lon << " (" << getLongitude() << ")" << std::endl;
    result << "name_ru: " << name_ru << std::endl;
    result << "name_en: " << name_en;

    return result.str();
  }
};

#pragma pack(pop)

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_RAW_CITY_ACCESS_H_
