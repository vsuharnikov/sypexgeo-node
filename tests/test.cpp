/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include <iostream>
#include <vector>
#include "xyz/vyvid/sypexgeo/db.h"

int main() {
  try {
    auto db = xyz::vyvid::sypexgeo::Db::fromFile("tests/runtime/SxGeoCity.dat");

    const std::string cityIps[] = {
      "77.50.211.86",
      "46.148.53.103",
      "89.254.208.188",
      "213.87.96.218",
      "128.72.86.166",
      "91.215.78.225"
    };

    for (auto cityIp: cityIps) {
      auto city = db->find(cityIp.c_str());
      if (city) {
        std::cout << "=== City at " << cityIp << " ===" << std::endl << (std::string) *city << std::endl << std::endl;
      }
    }
  } catch (std::exception const &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return 0;
}