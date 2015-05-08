/**
 * Copyright by Vyatcheslav Suharnikov and contributors.
 * See contributors list in AUTHORS.
 *
 * Licensed under the MIT license, see license text in LICENSE file.
 */

"use strict";

var sypex = require('../lib/sypexgeo');
var geoDb = null;

module.exports = {
  before: function () {
    var dbPath = __dirname + '/runtime/SxGeoCity.dat';

    geoDb = new sypex.Geo(dbPath);
    console.log("Checking ", dbPath);
  },

  'sypex::Geo': {
    '#getCountryIso': {
      'should return valid country': function () {
        expect(geoDb.getCountryIso('188.246.160.178')).to.be.eql('RU');
      }
    },

    '#find': {
      'should return valid structure:': {
        'sample': checkCity.bind(null, '46.148.53.103', {
          latitude: 55.3948,
          longitude: 43.8399,
          country: {
            id: 185,
            iso: 'RU',
            name: {
              en: 'Russia',
              ru: 'Россия'
            }
          },
          region: {
            id: 559838,
            iso: 'RU-NIZ',
            name: {
              en: 'Nizhegorodskaya Oblast\'',
              ru: 'Нижегородская область'
            }
          },
          city: {
            id: 580724,
            name: {
              en: 'Arzamas',
              ru: 'Арзамас'
            }
          }
        }),

        'only country': checkCountry.bind(null, '5.9.61.25', {
          latitude: 51.5,
          longitude: 10.5,
          country: {
            id: 56,
            iso: 'DE',
            name: {
              en: 'Germany',
              ru: 'Германия'
            }
          }
        }),

        'issue #5': checkCity.bind(null, '89.254.208.188', {
          latitude: 58.59665,
          longitude: 49.66007,
          country: {
            id: 185,
            iso: 'RU',
            name: {
              en: 'Russia',
              ru: 'Россия'
            }
          },
          region: {
            id: 548389,
            iso: 'RU-KIR',
            name: {
              en: 'Kirovskaya Oblast\'',
              ru: 'Кировская область'
            }
          },
          city: {
            id: 548408,
            name: {
              en: 'Kirov',
              ru: 'Киров'
            }
          }
        }),

        'issue #7': checkCity.bind(null, '128.72.86.166', {
          latitude: 55.7522,
          longitude: 37.6156,
          country: {
            id: 185,
            iso: 'RU',
            name: {
              en: 'Russia',
              ru: 'Россия'
            }
          },
          region: {
            id: 524894,
            iso: 'RU-MOW',
            name: {
              en: 'Moskva',
              ru: 'Москва'
            }
          },
          city: {
            id: 524901,
            name: {
              en: 'Moscow',
              ru: 'Москва'
            }
          }
        })
      }
    },

    'deprecated': {
      'mode': function () {
        expect(sypex).to.have.property('MODE_MEMORY');
      },

      '#getCountry': {
        'should return valid country': function () {
          expect(geoDb.getCountry('188.246.160.178')).to.be.eql('RU');
        }
      },

      '#getCityFull': {
        'should return valid structure:': [
          function () {
            var city = geoDb.getCityFull('46.148.53.103');

            expect(city).to.have.property('country', 'RU');
            expect(city).to.have.property('region', 'Нижегородская область');
            expect(city).to.have.property('name', 'Арзамас');
            expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
            expect(city).to.have.property('longitude').closeTo(43.83992, 0.01);
          },

          function () {
            var city = geoDb.getCityFull('5.9.61.25');

            expect(city).to.have.property('country', 'DE');
            expect(city).to.have.property('latitude').closeTo(51.5, 0.01);
            expect(city).to.have.property('longitude').closeTo(10.5, 0.01);
          }
        ]
      },

      '#getCity': {
        'should return valid structure': function () {
          var city = geoDb.getCity('46.148.53.103');

          expect(city).to.have.property('country', 'RU');
          expect(city).to.have.property('name', 'Арзамас');
          expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
          expect(city).to.have.property('longitude').closeTo(43.83992, 0.01);
        },

        'should return valid cities': function () {
          var city = geoDb.getCity('188.246.160.178');
          expect(city.name).to.be.eql('Москва');
        }
      }
    }
  }
};

function checkCity(ip, expected) {
  var city = geoDb.find(ip);

  expect(city, "country").to.have.property('country').deep.equals(expected.country);
  expect(city, "region").to.have.property('region').deep.equals(expected.region);
  expect(city, "city").to.have.property('city').deep.equals(expected.city);

  expect(city).to.have.property('latitude').closeTo(expected.latitude, 0.0001);
  expect(city).to.have.property('longitude').closeTo(expected.longitude, 0.0001);
}

function checkCountry(ip, expected) {
  var country = geoDb.find(ip);

  expect(country, "country").to.have.property('country').deep.equals(expected.country);

  // Should not have a region and a city information.
  expect(country, "region").to.not.have.property('region');
  expect(country, "country").to.not.have.property('city');

  expect(country).to.have.property('latitude').closeTo(expected.latitude, 0.01);
  expect(country).to.have.property('longitude').closeTo(expected.longitude, 0.01);
}