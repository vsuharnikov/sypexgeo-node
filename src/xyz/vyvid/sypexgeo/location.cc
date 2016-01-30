/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "xyz/vyvid/sypexgeo/location.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

Location::Location(RawCountryAccess country, RawRegionAccess region, RawCityAccess city) :
  longitude(city.getLongitude()),
  latitude(city.getLatitude()),

  country_id(country.header->id),
  country_iso(std::string(country.header->iso, 2)),
  russian_country_name(country.name_ru),
  english_country_name(country.name_en),

  region_id(region.header->id),
  region_iso(region.iso),
  russian_region_name(region.name_ru),
  english_region_name(region.name_en),

  city_id(city.header->id),
  russian_city_name(city.name_ru),
  english_city_name(city.name_en) {
}

Location::Location(RawCountryAccess country) :
  longitude(country.getLongitude()),
  latitude(country.getLatitude()),

  country_id(country.header->id),
  country_iso(std::string(country.header->iso, 2)),
  russian_country_name(country.name_ru),
  english_country_name(country.name_en),

  region_id(),
  region_iso(""),
  russian_region_name(""),
  english_region_name(""),

  city_id(),
  russian_city_name(""),
  english_city_name("") {
}

Location::Location() :
  longitude(0),
  latitude(0),

  country_id(0),
  country_iso(""),
  russian_country_name(""),
  english_country_name(""),

  region_id(),
  region_iso(""),
  russian_region_name(""),
  english_region_name(""),

  city_id(),
  russian_city_name(""),
  english_city_name("") {
}

Location::~Location() {
}

double Location::getLongitude() const {
  return longitude;
}

double Location::getLatitude() const {
  return latitude;
}

const uint8_t &Location::getCountryId() const {
  return country_id;
}

const std::string &Location::getCountryIso() const {
  return country_iso;
}

const std::string &Location::getRussianCountryName() const {
  return russian_country_name;
}

const std::string &Location::getEnglishCountryName() const {
  return english_country_name;
}

const uint24_t &Location::getRegionId() const {
  return region_id;
}

const std::string &Location::getRegionIso() const {
  return region_iso;
}

const std::string &Location::getRussianRegionName() const {
  return russian_region_name;
}

const std::string &Location::getEnglishRegionName() const {
  return english_region_name;
}

const uint24_t &Location::getCityId() const {
  return city_id;
}

const std::string &Location::getEnglishCityName() const {
  return english_city_name;
}

const std::string &Location::getRussianCityName() const {
  return russian_city_name;
}

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz
