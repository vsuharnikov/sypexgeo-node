/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "xyz/vyvid/sypexgeo/db.h"

#include <arpa/inet.h>

#if defined(__APPLE__)

#include <libkern/OSByteOrder.h>

#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#else

#include <byteswap.h>
#endif  // defined(__APPLE__)

#include <cstring>

#define SXGEO_ASCII_ZERO_CODE 48

namespace xyz {
namespace vyvid {
namespace sypexgeo {

std::unique_ptr<Db> Db::fromFile(const char *path) {
  std::ifstream source(path, std::ios::in | std::ios::binary);
  if (!source.good()) {
    throw DbOpenException(path);
  }

  std::unique_ptr<Db> result(new Db());
  result->readFromStream(&source);

  source.close();
  return result;
}

Db::Db()
  : db(nullptr),
    regions(nullptr),
    cities(nullptr),
    b_idx(nullptr),
    m_idx(nullptr) {
}

void Db::readFromStream(std::istream *source) {
  readMeta(source);

  b_idx = readDb<std::uint32_t>(source, header.b_idx_len, "b_idx");
  m_idx = readDb<std::uint32_t>(source, header.m_idx_len, "m_idx_len");

  db = readDb<char>(source, header.db_items * block_len, "db");

  if (header.region_size > 0) {
    regions = readDb<char>(source, header.region_size, "regions");
  }

  if (header.city_size > 0) {
    cities = readDb<char>(source, header.city_size, "cities");
  }
}

Db::~Db() {
  if (db != nullptr) {
    delete[] db;
  }

  if (regions != nullptr) {
    delete[] regions;
  }

  if (cities != nullptr) {
    delete[] cities;
  }

  if (b_idx != nullptr) {
    delete[] b_idx;
  }

  if (m_idx != nullptr) {
    delete[] m_idx;
  }
}

std::unique_ptr<Location> Db::find(const char *ip) const {
  std::uint32_t seek = getLocationOffset(ip);
  if (seek > 0) {
    return this->parseCityFull(seek);
  } else {
    return nullptr;
  }
}

std::unique_ptr<Location> Db::parseCityFull(std::uint32_t pos) const {
  Location *result = nullptr;

  if (pos < header.country_size) {  // has only a country
    result = new Location(getCountry(pos));
  } else {
    auto raw_city   = getCity(pos);
    auto raw_region = getRegion(raw_city);
    result = new Location(getCountry(raw_region), raw_region, raw_city);
  }

  return std::unique_ptr<Location>(result);
}

RawCountryAccess Db::getCountry(RawRegionAccess region_access) const {
  return getCountry(region_access.header->country_seek);
}

RawCountryAccess Db::getCountry(std::uint32_t pos) const {
  // @todo check that length <= header.max_country
  const char *offset = cities + pos;

  const RawCountryHeader *raw_header = reinterpret_cast<const RawCountryHeader *>(offset);
  const char             *name_ru    = reinterpret_cast<char *>((uint64_t) offset + sizeof(RawCountryHeader));
  const char             *name_en    = name_ru + strlen(name_ru) + sizeof(char);

  return RawCountryAccess(raw_header, name_ru, name_en);
}

RawRegionAccess Db::getRegion(RawCityAccess city_access) const {
  return getRegion(city_access.header->region_seek);
}

RawRegionAccess Db::getRegion(std::uint32_t pos) const {
  // @todo check that length <= header.max_region
  const char *offset = regions + pos;

  const RawRegionHeader *raw_header = reinterpret_cast<const RawRegionHeader *>(offset);
  const char            *name_ru    = reinterpret_cast<char *>((uint64_t) offset + sizeof(RawRegionHeader));
  const char            *name_en    = name_ru + strlen(name_ru) + sizeof(char);
  const char            *iso        = name_en + strlen(name_en) + sizeof(char);

  return RawRegionAccess(raw_header, name_ru, name_en, iso);
}

RawCityAccess Db::getCity(std::uint32_t pos) const {
  // @todo check that length <= header.max_city
  const char *offset = cities + pos;

  const RawCityHeader *raw_header = reinterpret_cast<const RawCityHeader *>(offset);
  const char          *name_ru    = reinterpret_cast<char *>((uint64_t) offset + sizeof(RawCityHeader));
  const char          *name_en    = name_ru + strlen(name_ru) + sizeof(char);

  return RawCityAccess(raw_header, name_ru, name_en);
}

std::uint32_t Db::getLocationOffset(const char *ip) const {
  std::uint8_t  firstByte = getFirstIpByte(ip);
  std::uint32_t ipn       = inet_addr(ip);
  if (0 == firstByte || 10 == firstByte || 127 == firstByte || firstByte > header.b_idx_len || INADDR_NONE == ipn)
    return 0;

  ipn = bswap_32(ipn);

  std::uint32_t minIndex = bswap_32(b_idx[firstByte - 1]);
  std::uint32_t maxIndex = bswap_32(b_idx[firstByte]);

  std::uint32_t min;
  std::uint32_t max;

  if (maxIndex - minIndex > header.range) {
    std::uint32_t part = searchIdx(
      ipn,
      (std::uint32_t) (minIndex / header.range),
      (std::uint32_t) (maxIndex / header.range) - 1);

    min = part > 0 ? part * header.range : 0;
    max = part > header.m_idx_len
          ? header.db_items
          : (part + 1) * header.range;

    if (min < minIndex)
      min = minIndex;

    if (max > maxIndex)
      max = maxIndex;
  } else {
    min = minIndex;
    max = maxIndex;
  }

  return this->searchDb(ipn, min, max);
}

std::uint32_t Db::searchDb(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const {
  const char *str = db;

  std::uint32_t buffer = 0;

  if (max - min > 0) {
    ipn &= 0x00FFFFFF;  // We've found area by the first byte, clear its 00FF..
    std::uint32_t offset = 0;

    while (max - min > 8) {
      offset = (min + max) >> 1;
      std::memcpy(&buffer, str + offset * block_len, 3);
      buffer = bswap_32(buffer) >> 8;  // We read only 3 bytes, so let's shift 8 bits

      if (ipn > buffer)
        min = offset;
      else
        max = offset;
    }

    str += min * block_len;  // Move the cursor
    std::memcpy(&buffer, str, 3);
    buffer = bswap_32(buffer) >> 8;

    while (ipn >= buffer && min < max) {
      min++;
      str += block_len;
      std::memcpy(&buffer, str, 3);
      buffer = bswap_32(buffer) >> 8;
    }

    std::memcpy(&buffer, str - header.id_len, header.id_len);
  } else {
    std::memcpy(&buffer, str + min * block_len + 3, 3);
  }

  buffer = bswap_32(buffer) >> 8;

  return buffer;
}

std::uint32_t Db::searchIdx(std::uint32_t ipn, std::uint32_t min, std::uint32_t max) const {
  std::uint32_t offset = 0;

  if (max < min) {
    std::uint32_t t = min;
    min = max;
    max = t;
  }

  while (max - min > 8) {
    offset = (min + max) >> 1;

    if (ipn > bswap_32(m_idx[offset])) {
      min = offset;
    } else {
      max = offset;
    }
  }

  while (ipn > bswap_32(m_idx[min]) && min++ <= max) { }

  return min;
}

std::uint8_t Db::getFirstIpByte(const char *ip) const {
  const char   *pos   = ip;
  std::int32_t result = 0;

  while (*pos != '.' && result < 256) {
    result = result * 10 + *pos - SXGEO_ASCII_ZERO_CODE;
    pos++;
  }

  return (result > 0 && result < 256)
         ? static_cast<std::uint8_t>(result)
         : 0;
}

void Db::readMeta(std::istream *source) {
  source->read(reinterpret_cast<char *>(&header), (uint32_t) (sizeof(Header) / sizeof(char)));
  if (!source->good()) {
    throw DbReadException("header");
  }

  header.time         = bswap_32(header.time);
  header.m_idx_len    = bswap_16(header.m_idx_len);
  header.range        = bswap_16(header.range);
  header.db_items     = bswap_32(header.db_items);
  header.max_region   = bswap_16(header.max_region);
  header.max_city     = bswap_16(header.max_city);
  header.region_size  = bswap_32(header.region_size);
  header.city_size    = bswap_32(header.city_size);
  header.max_country  = bswap_16(header.max_country);
  header.country_size = bswap_32(header.country_size);
  header.pack_size    = bswap_16(header.pack_size);

  block_len = 3 + header.id_len;

  header.validate();

  char *pack = new char[this->header.pack_size];
  source->read(pack, header.pack_size);
  if (!source->good()) {
    throw DbReadException("pack");
  }

  char   *format_checking   = pack;
  size_t country_format_len = strlen(RawCountryAccess::FORMAT);
  if (strncmp(format_checking, RawCountryAccess::FORMAT, country_format_len) != 0) {
    throw InvalidFormatException("The country has unsupported format");
  }

  format_checking += country_format_len + 1;
  size_t region_format_len = strlen(RawRegionAccess::FORMAT);
  if (strncmp(format_checking, RawRegionAccess::FORMAT, region_format_len) != 0) {
    throw InvalidFormatException("The region has unsupported format");
  }

  format_checking += region_format_len + 1;
  size_t city_format_len = strlen(RawCityAccess::FORMAT);
  if (strncmp(format_checking, RawCityAccess::FORMAT, city_format_len) != 0) {
    throw InvalidFormatException("The city has unsupported format");
  }

  delete[] pack;
}

template<typename T>
T *Db::readDb(std::istream *source, std::uint32_t elements_count, const char *label) {
  T *result = new T[elements_count];

  source->read(reinterpret_cast<char * >(result), sizeof(T) * elements_count);
  if (!source->good()) {
    throw DbReadException(label);
  }

  return result;
}

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz
