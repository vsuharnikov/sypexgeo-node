/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#include "./sypexgeo.h"
#include <arpa/inet.h>
#include <byteswap.h>
#include <cstdlib>
#include <cstring>

#define SXGEO_ASCII_ZERO_CODE 48

const char SypexGeo::MODE_FILE;
const char SypexGeo::MODE_BATCH;
const char SypexGeo::MODE_MEMORY;

const char * SypexGeo::cc2iso[] = {
    "", "AP", "EU", "AD", "AE", "AF", "AG", "AI", "AL", "AM", "AN", "AO", "AQ",
    "AR", "AS", "AT", "AU", "AW", "AZ", "BA", "BB", "BD", "BE", "BF", "BG", "BH",
    "BI", "BJ", "BM", "BN", "BO", "BR", "BS", "BT", "BV", "BW", "BY", "BZ", "CA",
    "CC", "CD", "CF", "CG", "CH", "CI", "CK", "CL", "CM", "CN", "CO", "CR", "CU",
    "CV", "CX", "CY", "CZ", "DE", "DJ", "DK", "DM", "DO", "DZ", "EC", "EE", "EG",
    "EH", "ER", "ES", "ET", "FI", "FJ", "FK", "FM", "FO", "FR", "FX", "GA", "GB",
    "GD", "GE", "GF", "GH", "GI", "GL", "GM", "GN", "GP", "GQ", "GR", "GS", "GT",
    "GU", "GW", "GY", "HK", "HM", "HN", "HR", "HT", "HU", "ID", "IE", "IL", "IN",
    "IO", "IQ", "IR", "IS", "IT", "JM", "JO", "JP", "KE", "KG", "KH", "KI", "KM",
    "KN", "KP", "KR", "KW", "KY", "KZ", "LA", "LB", "LC", "LI", "LK", "LR", "LS",
    "LT", "LU", "LV", "LY", "MA", "MC", "MD", "MG", "MH", "MK", "ML", "MM", "MN",
    "MO", "MP", "MQ", "MR", "MS", "MT", "MU", "MV", "MW", "MX", "MY", "MZ", "NA",
    "NC", "NE", "NF", "NG", "NI", "NL", "NO", "NP", "NR", "NU", "NZ", "OM", "PA",
    "PE", "PF", "PG", "PH", "PK", "PL", "PM", "PN", "PR", "PS", "PT", "PW", "PY",
    "QA", "RE", "RO", "RU", "RW", "SA", "SB", "SC", "SD", "SE", "SG", "SH", "SI",
    "SJ", "SK", "SL", "SM", "SN", "SO", "SR", "ST", "SV", "SY", "SZ", "TC", "TD",
    "TF", "TG", "TH", "TJ", "TK", "TM", "TN", "TO", "TL", "TR", "TT", "TV", "TW",
    "TZ", "UA", "UG", "UM", "US", "UY", "UZ", "VA", "VC", "VE", "VG", "VI", "VN",
    "VU", "WF", "WS", "YE", "YT", "RS", "ZA", "ZM", "ME", "ZW", "A1", "A2", "O1",
    "AX", "GG", "IM", "JE", "BL", "MF", NULL
};

const char * SypexGeo::timezones[] = {
    "",  "Africa/Abidjan", "Africa/Accra", "Africa/Addis_Ababa", "Africa/Algiers", "Africa/Bamako", "Africa/Banjul",
    "Africa/Blantyre", "Africa/Brazzaville", "Africa/Bujumbura", "Africa/Cairo", "Africa/Casablanca", "Africa/Ceuta",
    "Africa/Conakry", "Africa/Dakar", "Africa/Dar_es_Salaam", "Africa/Douala", "Africa/Freetown", "Africa/Gaborone",
    "Africa/Harare", "Africa/Johannesburg", "Africa/Kampala", "Africa/Khartoum", "Africa/Kigali", "Africa/Kinshasa",
    "Africa/Lagos", "Africa/Libreville", "Africa/Luanda", "Africa/Lubumbashi", "Africa/Lusaka", "Africa/Malabo",
    "Africa/Maputo", "Africa/Maseru", "Africa/Mbabane", "Africa/Mogadishu", "Africa/Monrovia", "Africa/Nairobi",
    "Africa/Ndjamena", "Africa/Niamey", "Africa/Nouakchott", "Africa/Ouagadougou", "Africa/Porto-Novo", "Africa/Tripoli",
    "Africa/Tunis", "Africa/Windhoek", "America/Anchorage", "America/Anguilla", "America/Antigua", "America/Araguaina",
    "America/Argentina/Buenos_Aires", "America/Argentina/Catamarca", "America/Argentina/Cordoba", "America/Argentina/Jujuy",
    "America/Argentina/La_Rioja", "America/Argentina/Mendoza", "America/Argentina/Rio_Gallegos", "America/Argentina/Salta",
    "America/Argentina/San_Juan", "America/Argentina/San_Luis", "America/Argentina/Tucuman", "America/Argentina/Ushuaia",
    "America/Asuncion", "America/Bahia", "America/Bahia_Banderas", "America/Barbados", "America/Belem", "America/Belize",
    "America/Boa_Vista", "America/Bogota", "America/Campo_Grande", "America/Cancun", "America/Caracas", "America/Chicago",
    "America/Chihuahua", "America/Costa_Rica", "America/Cuiaba", "America/Denver", "America/Dominica", "America/Edmonton",
    "America/El_Salvador", "America/Fortaleza", "America/Godthab", "America/Grenada", "America/Guatemala", "America/Guayaquil",
    "America/Guyana", "America/Halifax", "America/Havana", "America/Hermosillo", "America/Indianapolis", "America/Iqaluit",
    "America/Jamaica", "America/La_Paz", "America/Lima", "America/Los_Angeles", "America/Maceio", "America/Managua",
    "America/Manaus", "America/Matamoros", "America/Mazatlan", "America/Merida", "America/Mexico_City", "America/Moncton",
    "America/Monterrey", "America/Montevideo", "America/Montreal", "America/Nassau", "America/New_York", "America/Ojinaga",
    "America/Panama", "America/Paramaribo", "America/Phoenix", "America/Port_of_Spain", "America/Port-au-Prince",
    "America/Porto_Velho", "America/Recife", "America/Regina", "America/Rio_Branco", "America/Santo_Domingo",
    "America/Sao_Paulo", "America/St_Johns", "America/St_Kitts", "America/St_Lucia", "America/St_Vincent",
    "America/Tegucigalpa", "America/Thule", "America/Tijuana", "America/Vancouver", "America/Whitehorse", "America/Winnipeg",
    "America/Yellowknife", "Asia/Aden", "Asia/Almaty", "Asia/Amman", "Asia/Anadyr", "Asia/Aqtau", "Asia/Aqtobe", "Asia/Baghdad",
    "Asia/Bahrain", "Asia/Baku", "Asia/Bangkok", "Asia/Beirut", "Asia/Bishkek", "Asia/Choibalsan", "Asia/Chongqing",
    "Asia/Colombo", "Asia/Damascus", "Asia/Dhaka", "Asia/Dubai", "Asia/Dushanbe", "Asia/Harbin", "Asia/Ho_Chi_Minh",
    "Asia/Hong_Kong", "Asia/Hovd", "Asia/Irkutsk", "Asia/Jakarta", "Asia/Jayapura", "Asia/Jerusalem", "Asia/Kabul",
    "Asia/Kamchatka", "Asia/Karachi", "Asia/Kashgar", "Asia/Kolkata", "Asia/Krasnoyarsk", "Asia/Kuala_Lumpur", "Asia/Kuching",
    "Asia/Kuwait", "Asia/Macau", "Asia/Magadan", "Asia/Makassar", "Asia/Manila", "Asia/Muscat", "Asia/Nicosia", "Asia/Novokuznetsk",
    "Asia/Novosibirsk", "Asia/Omsk", "Asia/Oral", "Asia/Phnom_Penh", "Asia/Pontianak", "Asia/Qatar", "Asia/Qyzylorda", "Asia/Riyadh",
    "Asia/Sakhalin", "Asia/Seoul", "Asia/Shanghai", "Asia/Singapore", "Asia/Taipei", "Asia/Tashkent", "Asia/Tbilisi", "Asia/Tehran",
    "Asia/Thimphu", "Asia/Tokyo", "Asia/Ulaanbaatar", "Asia/Urumqi", "Asia/Vientiane", "Asia/Vladivostok", "Asia/Yakutsk",
    "Asia/Yekaterinburg", "Asia/Yerevan", "Atlantic/Azores", "Atlantic/Bermuda", "Atlantic/Canary", "Atlantic/Cape_Verde",
    "Atlantic/Madeira", "Atlantic/Reykjavik", "Australia/Adelaide", "Australia/Brisbane", "Australia/Darwin", "Australia/Hobart",
    "Australia/Melbourne", "Australia/Perth", "Australia/Sydney", "Chile/Santiago", "Europe/Amsterdam", "Europe/Andorra",
    "Europe/Athens", "Europe/Belgrade", "Europe/Berlin", "Europe/Bratislava", "Europe/Brussels", "Europe/Bucharest", "Europe/Budapest",
    "Europe/Chisinau", "Europe/Copenhagen", "Europe/Dublin", "Europe/Gibraltar", "Europe/Helsinki", "Europe/Istanbul",
    "Europe/Kaliningrad", "Europe/Kiev", "Europe/Lisbon", "Europe/Ljubljana", "Europe/London", "Europe/Luxembourg", "Europe/Madrid",
    "Europe/Malta", "Europe/Mariehamn", "Europe/Minsk", "Europe/Monaco", "Europe/Moscow", "Europe/Oslo", "Europe/Paris",
    "Europe/Prague", "Europe/Riga", "Europe/Rome", "Europe/Samara", "Europe/San_Marino", "Europe/Sarajevo", "Europe/Simferopol",
    "Europe/Skopje", "Europe/Sofia", "Europe/Stockholm", "Europe/Tallinn", "Europe/Tirane", "Europe/Uzhgorod", "Europe/Vaduz",
    "Europe/Vatican", "Europe/Vienna", "Europe/Vilnius", "Europe/Volgograd", "Europe/Warsaw", "Europe/Yekaterinburg", "Europe/Zagreb",
    "Europe/Zaporozhye", "Europe/Zurich", "Indian/Antananarivo", "Indian/Comoro", "Indian/Mahe", "Indian/Maldives", "Indian/Mauritius",
    "Pacific/Auckland", "Pacific/Chatham", "Pacific/Efate", "Pacific/Fiji", "Pacific/Galapagos", "Pacific/Guadalcanal", "Pacific/Honolulu",
    "Pacific/Port_Moresby", NULL
};

uint16_t lengthOfArray(const char ** array) {
    uint16_t length = 0;
    while (array[length] != NULL)
        length++;
    return length;
}

uint16_t CC2ISO_LENGTH    = lengthOfArray(SypexGeo::cc2iso);
uint16_t TIMEZONES_LENGTH = lengthOfArray(SypexGeo::timezones);

SypexGeo::SypexGeo()
    :b_idx_str(NULL),
     m_idx_str(NULL) {}

uint8_t SypexGeo::open(const char *path, const uint8_t mode) {
    this->mode = mode;

    this->dbFile = fopen(path, "rb");
    if (NULL == this->dbFile) {
        return SYPEXGEO_ERROR_FILE_NOT_FOUND;
    } else {
        fread(&this->header, sizeof(this->header), 1, this->dbFile);

        if ('S' == this->header.tag[0] && 'x' == this->header.tag[1] && 'G' == this->header.tag[2]) {
            this->header.time        = bswap_32(this->header.time);
            this->header.m_idx_len   = bswap_16(this->header.m_idx_len);
            this->header.range       = bswap_16(this->header.range);
            this->header.db_items    = bswap_32(this->header.db_items);
            this->header.max_region  = bswap_16(this->header.max_region);
            this->header.max_city    = bswap_16(this->header.max_city);
            this->header.region_size = bswap_32(this->header.region_size);
            this->header.city_size   = bswap_32(this->header.city_size);

            this->b_idx_str = new uint32_t[this->header.b_idx_len];
            fread(this->b_idx_str, sizeof(uint32_t), this->header.b_idx_len, this->dbFile);

            this->m_idx_str = new uint32_t[this->header.m_idx_len];
            fread(this->m_idx_str, sizeof(uint32_t), this->header.m_idx_len, this->dbFile);

            this->block_len = 3 + this->header.id_len;

            this->db_begin  = sizeof(this->header) + sizeof(uint32_t) * (this->header.b_idx_len + this->header.m_idx_len);

            this->regions_begin = this->db_begin + this->header.db_items * this->block_len;
            this->cities_begin  = this->regions_begin + this->header.region_size;

            if (this->mode & SypexGeo::MODE_MEMORY) {
                this->db = new char[this->header.db_items * this->block_len];
                fread(this->db, 1, this->header.db_items * this->block_len, this->dbFile);

                this->regions_db = new char[this->header.region_size];
                fread(this->regions_db, 1, this->header.region_size, this->dbFile);

                this->cities_db = new char[this->header.city_size];
                fread(this->cities_db, 1, this->header.city_size, this->dbFile);

                fclose(this->dbFile);
                this->dbFile = NULL;
            }

            return 0;
        } else {
            return SYPEXGEO_ERROR_INVALID_TAG;
        }
    }
}

SypexGeo::~SypexGeo() {
    if (this->b_idx_str)
        delete[] this->b_idx_str;

    if (this->m_idx_str)
        delete[] this->m_idx_str;

    if (this->dbFile)
        fclose(this->dbFile);
}

sxgeo_city* SypexGeo::getCity(const char *ip) {
    uint32_t seek = this->getNum(ip);
    return (seek > 0) ? this->parseCity(seek) : NULL;
}

sxgeo_city* SypexGeo::getFullCity(const char *ip) {
    uint32_t seek = this->getNum(ip);
    if (seek > 0) {
        sxgeo_city *city = this->parseCity(seek);
        this->parseRegion(city->regionId, city);
        return city;
    } else {
        return NULL;
    }
}

const char* SypexGeo::getCountry(const char *ip) {
    if (SYPEXGEO_TYPE_SXGEO_CITY == this->header.type) {
        uint32_t seek = this->getNum(ip);
        if (seek > 0 && seek < CC2ISO_LENGTH)
            return SypexGeo::cc2iso[seek];
    }

    return NULL;
}

uint32_t SypexGeo::getNum(const char *ip) {
    uint8_t firstByte = this->getFirstIpByte(ip);
    uint32_t ipn = inet_addr(ip);
    if (0 == firstByte || 10 == firstByte || 127 == firstByte || firstByte > this->header.b_idx_len || INADDR_NONE == ipn)
        return 0;

    ipn = bswap_32(ipn);

    uint32_t minIndex = bswap_32(this->b_idx_str[firstByte - 1]);
    uint32_t maxIndex = bswap_32(this->b_idx_str[firstByte]);

    uint32_t part = this->searchIdx(ipn, (uint32_t)(minIndex / this->header.range), (uint32_t)(maxIndex / this->header.range) - 1);

    uint32_t min = part > 0 ? part * this->header.range : 0;
    uint32_t max = part > this->header.m_idx_len
        ? this->header.db_items
        : (part + 1) * this->header.range;

    if (min < minIndex)
        min = minIndex;

    if (max > maxIndex)
        max = maxIndex;

    uint32_t len = max - min;

    if (this->mode & SypexGeo::MODE_MEMORY) {
        return this->searchDb(this->db, ipn, min, max);
    } else {
        uint32_t readLen = len * this->block_len;

        char rawIpBlocks[readLen + 1];
        rawIpBlocks[readLen] = '\0';

        fseek(this->dbFile, this->db_begin + min * this->block_len, SEEK_SET);
        fread(&rawIpBlocks[0], 1, readLen, this->dbFile);

        return this->searchDb(&rawIpBlocks[0], ipn, 0, len-1);
    }
}

uint32_t SypexGeo::searchDb(const char* str, uint32_t ipn, uint32_t min, uint32_t max) {
    uint32_t buffer = 0;

    if (max - min > 0) {
        ipn &= 0x00FFFFFF;  // We've found area by first byte, clear his
        uint32_t offset = 0;

        while (max - min > 8) {
            offset = (min + max) >> 1;
            std::memcpy(&buffer, str + offset * this->block_len, 3);
            buffer = bswap_32(buffer) >> 8;  // We read only 3 bytes, so let's shift 8 bits

            if (ipn > buffer)
                min = offset;
            else
                max = offset;
        }

        str += min * this->block_len;  // Move cursor
        std::memcpy(&buffer, str, 3);
        buffer = bswap_32(buffer) >> 8;

        while (ipn >= buffer && min < max) {
            min++;
            str += block_len;
            std::memcpy(&buffer, str, 3);
            buffer = bswap_32(buffer) >> 8;
        }

        std::memcpy(&buffer, str - this->header.id_len, this->header.id_len);
    } else {
        std::memcpy(&buffer, str + min * this->block_len + 3, 3);
    }

    buffer = bswap_32(buffer) >> 8;

    return buffer;
}

uint32_t SypexGeo::searchIdx(uint32_t ipn, uint32_t min, uint32_t max) {
    uint32_t offset = 0;

    while (max - min > 8) {
        offset = (min + max) >> 1;

        if (ipn > bswap_32(this->m_idx_str[offset]))
            min = offset;
        else
            max = offset;
    }

    while (ipn > bswap_32(this->m_idx_str[min]) && min < max)
        min++;

    return min;
}

uint8_t SypexGeo::getFirstIpByte(const char* ip) {
    const char *pos = ip;
    int16_t result = 0;

    while (*pos != '.' && result < 256) {
        result = result * 10 + *pos - SXGEO_ASCII_ZERO_CODE;
        pos++;
    }

    return (result > 0 && result < 256) ? (uint8_t)result : 0;
}

sxgeo_city* SypexGeo::parseCity(uint32_t pos) {
    char *raw = NULL;

    if (this->mode & SypexGeo::MODE_MEMORY) {
        raw = this->cities_db + pos;
    } else {
        raw = new char[this->header.max_city];

        fseek(this->dbFile, this->cities_begin + pos, SEEK_SET);
        fread(raw, 1, this->header.max_city, this->dbFile);
    }

    _sxgeo_city *_city = reinterpret_cast<_sxgeo_city*>(raw);
    sxgeo_city *result = NULL;

    uint32_t len = strlen(raw + 15);
    if (len < 255) {
        result = new sxgeo_city(SypexGeo::cc2iso[_city->cc]);
        result->regionId  = bswap_32(_city->regid);
        result->fips      = (_city->fips[0] - SXGEO_ASCII_ZERO_CODE) * 10 + _city->fips[1] - SXGEO_ASCII_ZERO_CODE;
        result->latitude  = static_cast<float>(bswap_32(_city->lat)) / 1000000;
        result->longitude = static_cast<float>(bswap_32(_city->lon)) / 1000000;
        result->country   = SypexGeo::cc2iso[_city->cc];
        result->city      = new char[len + 1];
        std::strncpy(result->city, raw + 15, len);  // not copies \0
        result->city[len] = '\0';
    }

    if (!(this->mode & SypexGeo::MODE_MEMORY))
        delete[] raw;

    return result;
}

void SypexGeo::parseRegion(uint32_t seek, sxgeo_city* dest) {
    if (NULL != dest && seek > 0) {
        char *begin = NULL;

        if (this->mode & SypexGeo::MODE_MEMORY) {
            begin = this->regions_db + seek;
        } else {
            fseek(this->dbFile, this->regions_begin + seek, SEEK_SET);
            begin = new char[this->header.max_region];
            fread(begin, 1, this->header.max_region, this->dbFile);
        }

        char *end = begin + this->header.max_region;

        uint32_t len = strlen(begin) + 1;  // + \0

        dest->region = new char[len];
        std::strncpy(dest->region, begin, len);

        int16_t timezoneIndex = 0;

        char *pos = begin + len;
        while (*pos != '\0' && pos != end && timezoneIndex < TIMEZONES_LENGTH) {
            timezoneIndex = timezoneIndex * 10 + *pos - SXGEO_ASCII_ZERO_CODE;
            pos++;
        }

        if (timezoneIndex >= 0  && timezoneIndex < TIMEZONES_LENGTH)
            dest->timezone = SypexGeo::timezones[timezoneIndex];

        if (!(this->mode & SypexGeo::MODE_MEMORY))
            delete[] begin;
    }
}

sxgeo_city::sxgeo_city(const char *theCountry)
    : timezone(NULL),
      country(theCountry),
      region(NULL),
      city(NULL)
{}

sxgeo_city::sxgeo_city(const char theTimezone, const char *theCountry)
    : timezone(NULL),
      country(theCountry),
      region(NULL),
      city(NULL)
{}

sxgeo_city::~sxgeo_city() {
    if (NULL != region)
        delete[] region;

    if (NULL != city)
        delete[] city;
}
