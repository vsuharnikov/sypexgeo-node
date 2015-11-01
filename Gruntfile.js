/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-vyvid contributors
 * See contributors list in AUTHORS
 *
 * See license text in LICENSE file
 */

'use strict';

module.exports = function (grunt) {
  grunt.loadNpmTasks('grunt-simple-mocha');
  grunt.loadNpmTasks('grunt-shell');

  var downloadBaseCommand = 'mkdir -p tests/runtime/;test -f tests/runtime/SxGeoCity.dat || (\
wget -nc http://sypexgeo.net/files/SxGeoCity_utf8.zip -O tests/runtime/SxGeoCity_utf8.zip\
&& unzip tests/runtime/SxGeoCity_utf8.zip -d tests/runtime/)';

  var downloadPHPApiCommand =  'mkdir -p tests/runtime/php/;test -f tests/runtime/php/SxGeo.php || (\
wget -nc https://sypexgeo.net/files/SxGeo22_API.zip -O tests/runtime/php/SxGeo22_API.zip\
&& unzip tests/runtime/php/SxGeo22_API.zip -d tests/runtime/php/)';

  grunt.initConfig({
    pkg: '<json:package.json>',
    simplemocha: {
      all: {
        src: ['tests/**/*.js'],
        options: {
          globals: ['chai'],
          timeout: 3000,
          ui: 'exports',
          reporter: 'tap',
          ignoreLeaks: false
        }
      }
    },
    cpplint: {
      files: [
        'src/xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.cc',
        'src/xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.h',

        'src/xyz/vyvid/sypexgeo/db.cc',
        'src/xyz/vyvid/sypexgeo/db.h',
        'src/xyz/vyvid/sypexgeo/errors.h',
        'src/xyz/vyvid/sypexgeo/header.h',
        'src/xyz/vyvid/sypexgeo/location.cc',
        'src/xyz/vyvid/sypexgeo/location.h',
        'src/xyz/vyvid/sypexgeo/raw_city_access.h',
        'src/xyz/vyvid/sypexgeo/raw_country_access.h',
        'src/xyz/vyvid/sypexgeo/raw_region_access.h',

        'src/xyz/vyvid/sypexgeo/util/string_builder.h',
        'src/xyz/vyvid/sypexgeo/util/uint24_t.h'
      ],
      reporter: 'spec',
      verbosity: 0,
      filters: {
        whitespace: {
          braces: false,
          include_alpha: true
        },
        readability: {
          streams: false
        }
      },
      linelength: 120,
      // This could be an array of strings or a comma separated string
      extensions: [
        'cc',
        'h'
      ]
    },
    shell: {
      build: {
        command: 'node-gyp build .',
        stdout: true,
        stderr: true
      },

      removebase: {
        command: 'rm test/runtime/SxGeoCity.dat'
      },

      downloadbase: {
        command: downloadBaseCommand,
        stdout: true,
        stderr: true
      },

      downloadphpapi: {
        command: downloadPHPApiCommand,
        stdout: true,
        stderr: true
      }
    }
  });

  grunt.loadNpmTasks('node-cpplint');

  grunt.registerTask('chaihelper', 'Register chai helpers', function () {
    global.expect = require('chai').expect;
  });

  grunt.registerTask('test', ['shell:downloadbase', 'shell:downloadphpapi', 'chaihelper', 'simplemocha']);

  // Default task.
  grunt.registerTask('default', ['cpplint', 'shell:build', 'test']);
};