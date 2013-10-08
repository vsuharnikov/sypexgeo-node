'use strict';

module.exports = function(grunt) {
	grunt.loadNpmTasks('grunt-simple-mocha');
	grunt.loadNpmTasks('grunt-shell');

	var downloadBaseCommand = 'mkdir test/runtime/;test -f test/runtime/SxGeoCity.dat || (\
wget -nc http://sypexgeo.net/files/SxGeoCity.zip -O test/runtime/SxGeoCity.zip\
&& unzip test/runtime/SxGeoCity.zip -d test/runtime/)';

	grunt.initConfig({
		pkg: '<json:package.json>',
		lint: {
			files: ['grunt.js', 'lib/**/*.js'] //, 'test/**/*.js'] // have no idea, how lint chaijs's bdd code
		},
		watch: {
			files: '<config:lint.files>',
			tasks: 'default'
		},
		jshint: {
			options: {
				curly    : true,
				eqeqeq   : true,
				immed    : true,
				latedef  : true,
				newcap   : true,
				noarg    : true,
				sub      : true,
				undef    : true,
				boss     : true,
				eqnull   : true,
				node     : true,
				multistr : true
			},
			globals: {
				exports  : true,
				expect   : true
			}
		},
		simplemocha: {
			all: {
				src: ['test/**/*.js'],
				options: {
					globals:     ['chai'],
					timeout:     3000,
					ui:          'exports',
					reporter:    'tap',
					ignoreLeaks: false
				}
			}
		},
		shell: {
			cpplint: {
				command: './tools/cpplint.py --filter=-whitespace/parens,-whitespace/line_length src/*.*',
				stdout: true,
				stderr: true
			},

			build: {
				command: 'npm build .',
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
			}
		}
	});

	grunt.registerTask('chaihelper', 'Register chai helpers', function() {
		global.expect = require('chai').expect;
	});

	grunt.registerTask('test', ['shell:downloadbase', 'chaihelper', 'simplemocha']);

	grunt.registerTask('jslint', 'lint');
	grunt.registerTask('cpplint', 'shell:cpplint');

	// Default task.
	grunt.registerTask('default', ['jslint', 'shell:cpplint', 'shell:build', 'test']);

};