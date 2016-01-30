Unofficial [Sypex Geo API](https://sypexgeo.net) bindings for Node.JS.

    The latest version works only with:
    * SypexGeo 2.2! See changelog.
    * SxGeoCity_utf8 (See Gruntfile.js)

## Getting Started

### Requirements

* g++ >= 4.8 for supporting C++11.

### Setup

    $ npm i --save sypexgeo-vyvid

### Using

```javascript
var sypex = require('sypexgeo-vyvid');
var geoDb = new sypex.Geo('/a/full/path/to/the/SypexGeoCity.dat');
var location = geoDb.find('46.148.53.103');

console.log(JSON.stringify(location, null, '  '));
```

## Documentation

### sypex.Geo

Creates an instance.

```javascript
var geoDb = new sypex.Geo('/var/www/geo/SypexGeoCity.dat');
```

#### Parameters

1. {String} _filePath_ - A full path to the SypexGeo DB file.
2. **[deprecated]** {Number} _mode_ - Specifies a behaviour. Available only **SXGEO_MEMORY**.
  **It will be removed in 0.3.0**

The _mode_ is always **SXGEO_BATCH | SXGEO_MEMORY** regardless of arguments you pass in.

### find

Looks up for a location by its _IP_.

```javascript
var location = geoDb.find('46.148.53.103');

console.log(JSON.stringify(location, null, '  '));
/** Output:
{
  "latitude": 55.39485,
  "longitude": 43.83992,
  "country": {
    "id": 185,
    "iso": "RU",
    "name": {
      "en": "Russia",
      "ru": "Россия"
    }
  },
  "region": {
    "id": 559838,
    "iso": "RU-NIZ",
    "name": {
      "en": "Nizhegorodskaya Oblast'",
      "ru": "Нижегородская область"
    }
  },
  "city": {
    "id": 580724,
    "name": {
      "en": "Arzamas",
      "ru": "Арзамас"
    }
  }
} 
**/
```

#### Parameters

* {String} _ip_ - The IP-address, for example: **46.148.53.103**.

#### Returns

* {Object, null} - **null** if the database hasn't a location with a specified ip.

An object describes a found location:

* {Number} _longitude_ - **5** digits (e.g., **55.39485**)  after the decimal point if the city is detected. Otherwise, **2** (e.g. **51.5**)
* {Number} _latitude_
* {Object} _country_
  * {Number} _id_
  * {String} _iso_
  * {Object} _name_
      * {String} en
      * {String} ru
* **[optional]** {Object} _region_ - The information about a region. Omitted if wasn't detected
  * {Number} _id_
  * {String} _iso_
  * {Object} _name_
      * {String} en
      * {String} ru
* **[optional]** {Object} city - The information about a city. Omitted if wasn't detected
  * {Number} _id_
  * {Object} _name_
      * {String} en
      * {String} ru

### getCountryIso

Get a country ISO ([3166-1 alpha-2](https://en.wikipedia.org/wiki/ISO_3166-1_alpha-2)) by its _IP_.

```javascript
var location = geoDb.getCountryIso('46.148.53.103');

console.log(location); // Output: RU
```

#### Parameters

* {String} _ip_ - The IP-address, for example: **46.148.53.103**.


### [deprecated] getCity 

**The method will be removed in 0.3.0**.

```javascript
var city = geoDb.getCityFull('46.148.53.103');

console.log(JSON.stringify(city, null, '  '));
/** Output:
{
  "region": "Нижегородская область",
  "country": "RU",
  "name": "Арзамас",
  "latitude": 55.39485,
  "longitude": 43.83992
}
*/
```

### [deprecated] getCityFull 

**The method will be removed in 0.3.0**.

Alias to _getCity_.

### [deprecated] getCountry

**The method will be removed in 0.3.0**.

alias to _getCountryIso_.

## Grunt commands

* `grunt test` - run tests. Downloads a base if it necessary. At this time we have only JS-tests :)
* `grunt cpplint` - check a cpp code for some rules.

## Contributing

* Please check your code by the command: `grunt cpplint`
* If you need a manual build, then run:

    ```bash
    node-gyp configure &&\
    node-gyp install &&\
    node-gyp build
    ```

_(Coming soon)_

## Changelog

### 0.2.*

Works with SypexGeo 2.2.

* [-] The _fips_ field has been removed (removed from a database);
* [+] The _find_ and _getCountryIso_ methods have been added;
* [!] The _getCountry_, _getCity_ and _getCityFull_ now is deprecated;
* [!] Completely rewritten code;
* [!] Requires GCC 4.8.

### 0.1.*

Works with SypexGeo 2.1.

* 0.1.1 - Ported to [Nan](https://github.com/rvagg/nan)

## License

Copyright by Vyatcheslav Suharnikov and contributors.

See contributors list in AUTHORS.

Licensed under the MIT license, see license text in LICENSE file.
