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
    geoDb = new sypex.Geo(__dirname + '/runtime/SxGeoCity.dat');
  },

  'sypex::Geo': {
    '#getCountryIso': {
      'should return valid country': function () {
        expect(geoDb.getCountryIso('188.246.160.178')).to.be.eql('RU');
      }
    },

    '#getCountry': {
      'should return valid country': function () {
        expect(geoDb.getCountry('188.246.160.178')).to.be.eql('RU');
      }
    },

    '#find': {
      'should return valid structure:': [
        function () {
          var city = geoDb.find('46.148.53.103');

          expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
          expect(city).to.have.property('longitude').closeTo(43.83992, 0.01);

          expect(city).to.have.property('country').deep.equals({
            id: 185,
            iso: 'RU',
            name: {
              en: 'Russia',
              ru: 'Россия'
            }
          });

          expect(city).to.have.property('region').deep.equals({
            id: 559838,
            iso: 'RU-NIZ',
            name: {
              en: 'Nizhegorodskaya Oblast\'',
              ru: 'Нижегородская область'
            }
          });

          expect(city).to.have.property('city').deep.equals({
            id: 580724,
            name: {
              en: 'Arzamas',
              ru: 'Арзамас'
            }
          });
        },

        function () {
          var country = geoDb.find('5.9.61.25'); // Haven't region and city information.

          expect(country).to.have.property('latitude').closeTo(51.5, 0.01);
          expect(country).to.have.property('longitude').closeTo(10.5, 0.01);

          expect(country).to.have.property('country').deep.equals({
            id: 56,
            iso: 'DE',
            name: {
              en: 'Germany',
              ru: 'Германия'
            }
          });

          expect(country).to.not.have.property('region');
          expect(country).to.not.have.property('city');
        }
      ]
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
};