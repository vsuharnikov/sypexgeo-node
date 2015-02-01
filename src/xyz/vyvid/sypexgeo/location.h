/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_LOCATION_H_
#define SRC_XYZ_VYVID_SYPEXGEO_LOCATION_H_

#include <string>

#include "xyz/vyvid/util/uint24_t.h"

#include "xyz/vyvid/sypexgeo/raw_country_access.h"
#include "xyz/vyvid/sypexgeo/raw_region_access.h"
#include "xyz/vyvid/sypexgeo/raw_city_access.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

class Location {
 public:
  Location(RawCountryAccess country, RawRegionAccess region, RawCityAccess city);

  explicit Location(RawCountryAccess country);

  ~Location();

  double getLongitude() const;

  double getLatitude() const;

  const uint8_t &getCountryId() const;

  const std::string &getCountryIso() const;

  const std::string &getRussianCountryName() const;

  const std::string &getEnglishCountryName() const;

  const uint24_t &getRegionId() const;

  const std::string &getRegionIso() const;

  const std::string &getRussianRegionName() const;

  const std::string &getEnglishRegionName() const;

  const uint24_t &getCityId() const;

  const std::string &getRussianCityName() const;

  const std::string &getEnglishCityName() const;

  explicit operator std::string() const {
    std::stringstream result;

    result << "Longitude: " << getLongitude() << std::endl;
    result << "Latitude: " << getLatitude() << std::endl;

    result << "Country id: " << (uint32_t) getCountryId() << std::endl;
    result << "Country ISO: " << getCountryIso() << std::endl;
    result << "Russian country name: " << getRussianCountryName() << std::endl;
    result << "English country name: " << getEnglishCountryName() << std::endl;

    result << "Region id: " << getRegionId() << std::endl;
    result << "Region ISO: " << getRegionIso() << std::endl;
    result << "Russian region name: " << getRussianRegionName() << std::endl;
    result << "English region name: " << getEnglishRegionName() << std::endl;

    result << "City id: " << getCityId() << std::endl;
    result << "Russian city name: " << getRussianCityName() << std::endl;
    result << "English city name: " << getEnglishCityName();

    return result.str();
  }

 private:
  double longitude;
  double latitude;

  uint8_t     country_id;
  std::string country_iso;
  std::string russian_country_name;
  std::string english_country_name;

  uint24_t    region_id;
  std::string region_iso;
  std::string russian_region_name;
  std::string english_region_name;

  uint24_t    city_id;
  std::string russian_city_name;
  std::string english_city_name;
};

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_LOCATION_H_
