{
  "targets": [
    {
      "target_name": "sypexgeo",
      "sources": [
        "src/xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.cc",
        "src/xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.h",
        "src/xyz/vyvid/sypexgeo/util/string_builder.h",
        "src/xyz/vyvid/sypexgeo/uint24_t.h",
        "src/xyz/vyvid/sypexgeo/db.cc",
        "src/xyz/vyvid/sypexgeo/db.h",
        "src/xyz/vyvid/sypexgeo/errors.h",
        "src/xyz/vyvid/sypexgeo/header.h",
        "src/xyz/vyvid/sypexgeo/location.cc",
        "src/xyz/vyvid/sypexgeo/location.h",
        "src/xyz/vyvid/sypexgeo/raw_city_access.h",
        "src/xyz/vyvid/sypexgeo/raw_country_access.h",
        "src/xyz/vyvid/sypexgeo/raw_region_access.h"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "./src"
      ],
      "cflags": [
        "-std=gnu++11",
        "-fno-rtti",
        "-Wall",
        "-fexceptions"
      ],
      "cflags_cc": [
        "-std=gnu++11",
        "-fno-rtti",
        "-Wall",
        "-fexceptions"
      ],
      "conditions": [
        [
          "OS=='mac'",
          {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "GCC_ENABLE_CPP_RTTI": "NO",
              "CLANG_CXX_LANGUAGE_STANDARD": "gnu++11",
              "CLANG_CXX_LIBRARY": "libc++",
              "MACOSX_DEPLOYMENT_TARGET": "10.7"
            }
          }
        ]
      ]
    }
  ]
}