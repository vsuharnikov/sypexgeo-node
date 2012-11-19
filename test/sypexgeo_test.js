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
				expect(object.getCountry('78.109.16.159')).to.be.not.exist; // eql('RU'); because of city base, not country
				done();
			}
		},

		'#getFullCity': {
			'should return valid structure': function (done) {
				var city = object.getFullCity('46.148.53.103');
				expect(city).to.have.property('timezone',  'Europe/Moscow');
				expect(city).to.have.property('country',   'RU');
				expect(city).to.have.property('region',    'Нижегородская область');
				expect(city).to.have.property('name',      'Арзамас');
				expect(city).to.have.property('fips',      51);
				expect(city).to.have.property('latitude').closeTo(55.4, 0.01);
				expect(city).to.have.property('longitude').closeTo(43.816, 0.001);
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
				expect(object.getCity('78.109.16.159').name).to.be.eql('Москва');
				done();
			}
		}
	}
};