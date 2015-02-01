/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_DB_H_
#define SRC_XYZ_VYVID_SYPEXGEO_DB_H_

#include <cstdio>
#include <fstream>
#include <memory>
#include <cstdint>

#include "xyz/vyvid/sypexgeo/header.h"
#include "xyz/vyvid/sypexgeo/location.h"
#include "xyz/vyvid/sypexgeo/raw_city_access.h"
#include "xyz/vyvid/sypexgeo/raw_country_access.h"
#include "xyz/vyvid/sypexgeo/raw_region_access.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

class Db {
 public:
  static std::unique_ptr<Db> fromFile(const char *path);

  ~Db();

  std::unique_ptr<Location> find(const char *ip) const;

 private:
  char *db;
  char *regions;
  char *cities;

  std::uint32_t *b_idx;
  std::uint32_t *m_idx;

  std::uint32_t block_len;
  Header        header;

  Db();

  void readFromStream(std::istream *source);

  std::uint32_t getLocationOffset(const char *ip) const;

  std::uint8_t getFirstIpByte(const char *ip) const;

  std::unique_ptr<Location> parseCityFull(std::uint32_t pos) const;

  std::uint32_t searchDb(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const;

  std::uint32_t searchIdx(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const;

  RawCountryAccess getCountry(RawRegionAccess region_access) const;

  RawCountryAccess getCountry(std::uint32_t pos) const;

  RawRegionAccess getRegion(RawCityAccess city_access) const;

  RawRegionAccess getRegion(std::uint32_t pos) const;

  RawCityAccess getCity(std::uint32_t pos) const;

  /*!
   * Loads meta information from source.
   */
  void readMeta(std::istream *source);

  /*!
   * Creates T[elements_count], fills an array from the source and returns pointer to array.
   */
  template<typename T>
  T *readDb(std::istream *source, std::uint32_t elements_count, const char *label);
};

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_DB_H_
