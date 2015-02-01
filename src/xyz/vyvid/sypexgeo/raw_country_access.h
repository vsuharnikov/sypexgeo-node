/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_RAW_COUNTRY_ACCESS_H_
#define SRC_XYZ_VYVID_SYPEXGEO_RAW_COUNTRY_ACCESS_H_

#include <string>
#include <sstream>
#include <cstdint>
#include <cmath>

namespace xyz {
namespace vyvid {
namespace sypexgeo {

#pragma pack(push, 1)

struct RawCountryHeader {
  std::uint8_t id;      // T:id
  char         iso[2];  // c2:iso
  std::int16_t lat;     // n2:lat
  std::int16_t lon;     // n2:lon
};

struct RawCountryAccess {
  static constexpr const char *FORMAT = "T:id/c2:iso/n2:lat/n2:lon/b:name_ru/b:name_en";

  const RawCountryHeader *header;
  const char             *name_ru;  // b:name_ru
  const char             *name_en;  // b:name_en

  RawCountryAccess(const RawCountryHeader *header, const char *name_ru, const char *name_en)
    : header(header), name_ru(name_ru), name_en(name_en) {
  }

  double getLongitude() const {
    return header->lon / std::pow(10, 2);  // n2:lon
  }

  double getLatitude() const {
    return header->lat / std::pow(10, 2);  // n2:lat
  }

  explicit operator std::string() const {
    std::stringstream result;

    result << "id: " << (uint32_t) header->id << std::endl;
    result << "iso: " << header->iso << std::endl;
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

#endif  // SRC_XYZ_VYVID_SYPEXGEO_RAW_COUNTRY_ACCESS_H_
