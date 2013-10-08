/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

"use strict";

var sypex  = require('../lib/sypexgeo');
var object = null;

module.exports = {
	before: function(done) {
		object = new sypex.Geo(__dirname + '/runtime/SxGeoCity.dat', sypex.MODE_MEMORY);
		done();
	},

	'sypex::Geo': {
		'should have constants': function (done) {
			expect(sypex).to.have.property('MODE_MEMORY');
			done();
		},

		'#getCountry': {
			'should return valid country': function (done) {
				expect(object.getCountry('188.246.160.178')).to.be.not.exist; // eql('RU'); because of city base, not country
				done();
			}
		},

		'#getCityFull': {
			'should return valid structure': function (done) {
				var city = object.getCityFull('46.148.53.103');
				//expect(city).to.have.property('timezone',  'Europe/Moscow');
				expect(city).to.have.property('country',   'RU');
				expect(city).to.have.property('region',    'Нижегородская область');
				expect(city).to.have.property('name',      'Арзамас');
				expect(city).to.have.property('fips',      51);
				expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
				expect(city).to.have.property('longitude').closeTo(43.816, 0.001);

				var city = object.getCityFull('5.9.61.25');
				expect(city).to.have.property('country', 'DE');
				expect(city).to.have.property('latitude').closeTo(51, 0.01);
				expect(city).to.have.property('longitude').closeTo(9, 0.001);
				done();
			}
		},

		'#getCity': {
			'should return valid structure': function (done) {
				var city = object.getCity('46.148.53.103');
				expect(city).to.have.property('country',   'RU');
				expect(city).to.have.property('name',      'Арзамас');
				expect(city).to.have.property('fips',      51);
				expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
				expect(city).to.have.property('longitude').closeTo(43.816, 0.001);
				done();
			},

			'should return valid cities': function (done) {
				var city = object.getCity('188.246.160.178');
				expect(city.name).to.be.eql('Москва');
				done();
			}
		}
	}
};