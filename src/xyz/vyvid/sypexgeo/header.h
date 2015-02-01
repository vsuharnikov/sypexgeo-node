/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_HEADER_H_
#define SRC_XYZ_VYVID_SYPEXGEO_HEADER_H_

#include <string>
#include <sstream>
#include <cstdint>
#include <cmath>

#include "xyz/vyvid/sypexgeo/errors.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

enum class DatabaseType : uint8_t {
  UNIVERSAL = 0,

  SXGEO_COUNTRY = 1,
  SXGEO_CITY    = 2,

  GEOIP_COUNTRY = 11,
  GEOIP_CITY    = 12,

  IPGEOBASE = 21
};

enum class Charset {
  UTF8   = 0,
  LATIN1 = 1,
  CP1251 = 2
};

#pragma pack(push, 1)

struct Header {
  static const std::uint8_t VERSION_SUPPORTED = 22;

  std::uint8_t  tag[3];
  std::uint8_t  version;
  std::uint32_t time;
  std::uint8_t  type;
  std::uint8_t  charset;
  std::uint8_t  b_idx_len;
  std::uint16_t m_idx_len;
  std::uint16_t range;
  std::uint32_t db_items;
  std::uint8_t  id_len;
  std::uint16_t max_region;
  std::uint16_t max_city;
  std::uint32_t region_size;
  std::uint32_t city_size;
  std::uint16_t max_country;
  std::uint32_t country_size;
  std::uint16_t pack_size;

  /*!
   * Checks the header has a valid format.
   */
  void validate() {
    bool isValidTag = 'S' == tag[0] && 'x' == tag[1] && 'G' == tag[2];
    if (!isValidTag) {
      throw InvalidTagException();
    }

    if (VERSION_SUPPORTED != version) {
      throw InvalidVersionException(VERSION_SUPPORTED, version);
    }

    if (Charset::UTF8 != getCharset()) {
      throw InvalidCharsetException("utf-8");
    }

    if (0 == b_idx_len
      || 0 == m_idx_len
      || 0 == range
      || 0 == db_items
      || 0 == time
      || 0 == id_len) {
      throw InvalidFormatException();
    }

    if (pack_size == 0) {
      throw InvalidFormatException("The file hasn't information about packing");
    }
  }

  Charset getCharset() {
    if (charset > 2) {
      throw UnknownCharsetException(charset);
    }

    return static_cast<Charset>(charset);
  }

  explicit operator std::string() const {
    std::stringstream result;

    result << "tag: " << tag[0] << tag[1] << tag[2] << std::endl;
    result << "version: " << (std::uint32_t) version << std::endl;
    result << "time: " << time << std::endl;
    result << "type: " << (std::uint32_t) type << std::endl;
    result << "b_idx_len: " << (std::uint32_t) b_idx_len << std::endl;
    result << "m_idx_len: " << m_idx_len << std::endl;
    result << "range: " << range << std::endl;
    result << "db_items: " << db_items << std::endl;
    result << "id_len: " << (std::uint32_t) id_len << std::endl;
    result << "max_region: " << max_region << std::endl;
    result << "max_city: " << max_city << std::endl;
    result << "region_size: " << region_size << std::endl;
    result << "city_size: " << city_size << std::endl;
    result << "max_country: " << max_country << std::endl;
    result << "country_size: " << country_size << std::endl;
    result << "pack_size: " << pack_size;

    return result.str();
  }
};

#pragma pack(pop)

// See the documentation: http://sypexgeo.net/ru/docs/sxgeo22/
typedef char assert_that_header_has_valid_size[(40 == sizeof(Header)) * 2 - 1];

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_HEADER_H_
