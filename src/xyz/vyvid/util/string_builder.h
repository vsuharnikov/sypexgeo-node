/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef _XYZ_VYVID_SYPEXGEO_UTIL_STRING_BUILDER_H_
#define _XYZ_VYVID_SYPEXGEO_UTIL_STRING_BUILDER_H_

#include <sstream>

struct StringBuilder {
  std::stringstream ss;

  template<typename T>
  StringBuilder &operator<<(const T &data) {
    ss << data;
    return *this;
  }

  operator std::string() {
    return ss.str();
  }
};

#endif // _XYZ_VYVID_SYPEXGEO_UTIL_STRING_BUILDER_H_