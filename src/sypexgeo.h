/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#ifndef _SYPEXGEO_NODE_SRC_SYPEXGEO_H_
#define _SYPEXGEO_NODE_SRC_SYPEXGEO_H_

#include <stdint.h>
#include <cstdio>

enum SYPEXGEO_ERROR {
    SYPEXGEO_ERROR_INVALID_TAG    = 1,
    SYPEXGEO_ERROR_FILE_NOT_FOUND = 2
};

enum SYPEXGEO_TYPE {
    SYPEXGEO_TYPE_UNIVERSAL       = 0,

    SYPEXGEO_TYPE_SXGEO_COUNTRY   = 1,
    SYPEXGEO_TYPE_SXGEO_CITY      = 2,

    SYPEXGEO_TYPE_GEOIP_COUNTRY   = 11,
    SYPEXGEO_TYPE_GEOIP_CITY      = 12,

    SYPEXGEO_TYPE_IPGEOBASE       = 21
};

#pragma pack(push, 1)

struct _sxgeo_header {
    char     tag[3];
    uint8_t  version;
    uint32_t time;
    uint8_t  type;
    uint8_t  charset;
    uint8_t  b_idx_len;
    uint16_t m_idx_len;
    uint16_t range;
    uint32_t db_items;
    uint8_t  id_len;
    uint16_t max_region;
    uint16_t max_city;
    uint32_t region_size;
    uint32_t city_size;
};

struct _sxgeo_minmax {
    uint32_t min;
    uint32_t max;
};

struct _sxgeo_city {
    uint32_t regid;
    uint8_t  cc;
    char     fips[2];
    uint32_t lat;
    uint32_t lon;
};

#pragma pack(pop)

struct sxgeo_city {
    uint32_t     regionId;
    uint16_t     fips;
    float        latitude;
    float        longitude;

    const char  *timezone;
    const char  *country;
    char        *region;
    char        *city;

    explicit sxgeo_city(const char *theCountry);

    explicit sxgeo_city(const char theTimezone, const char *theCountry);

    ~sxgeo_city();
};

class SypexGeo {
    public:
        static const char MODE_FILE   = 0;
        static const char MODE_BATCH  = 1;
        static const char MODE_MEMORY = 2;

        static const char* cc2iso[];
        static const char* timezones[];

        SypexGeo();
        ~SypexGeo();

        uint8_t     open(const char *path, const uint8_t mode);

        sxgeo_city* getCity(const char *ip);
        sxgeo_city* getCityFull(const char *ip);
        const char* getCountry(const char *ip);

    private:
        uint8_t       mode;

        std::FILE     *dbFile;

        char          *db;
        char          *regions_db;
        char          *cities_db;

        uint32_t      *b_idx_str;
        uint32_t      *m_idx_str;

        uint32_t      db_begin;
        uint8_t       block_len;
        uint32_t      regions_begin;
        uint32_t      cities_begin;
        _sxgeo_header header;

        uint32_t     getNum(const char *ip);
        uint8_t      getFirstIpByte(const char *ip);
        sxgeo_city*  parseCity(uint32_t pos);
        void         parseRegion(uint32_t seek, sxgeo_city* dest);

        uint32_t     searchDb(const char* str, uint32_t ipn, uint32_t min, uint32_t max);
        uint32_t     searchIdx(uint32_t ipn, uint32_t min, uint32_t max);
};

#endif  // _SYPEXGEO_NODE_SRC_SYPEXGEO_H_
