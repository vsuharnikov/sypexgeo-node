/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include <iostream>
#include "xyz/vyvid/sypexgeo/db.h"

int main() {
  try {
    auto db = xyz::vyvid::sypexgeo::Db::fromFile("/Users/freezy/projects/github/sypexgeo-node/tests/runtime/SxGeoCity.dat");

    auto city1 = db->find("77.50.211.86");
    std::cout << "=== City at 77.50.211.86 ===" << std::endl << (std::string) *city1 << std::endl;

    std::cout << std::endl;

    auto city2 = db->find("46.148.53.103");
    std::cout << "=== City at 46.148.53.103 ===" << std::endl << (std::string) *city2 << std::endl;

    auto city3 = db->find("89.254.208.188");
    std::cout << "=== City at 89.254.208.188 ===" << std::endl << (std::string) *city3 << std::endl;
  } catch (std::exception const &e) {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return 0;
}