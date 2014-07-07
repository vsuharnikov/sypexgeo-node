{
	"targets": [
		{
			"target_name": "sypexgeo",
			"sources": [
				"src/sypexgeo_node.cc",
				"src/sypexgeo.cc"
			],
			"include_dirs": [
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}