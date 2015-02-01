/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_ERRORS_H_
#define SRC_XYZ_VYVID_SYPEXGEO_ERRORS_H_

#include <stdexcept>
#include <cstdint>

#include "xyz/vyvid/util/string_builder.h"

namespace xyz {
namespace vyvid {
namespace sypexgeo {

class InvalidTagException : public std::runtime_error {
 public:
  explicit InvalidTagException() : std::runtime_error("The tag is invalid") {
  }
};

class InvalidVersionException : public std::runtime_error {
 public:
  explicit InvalidVersionException(std::uint8_t expected_version, std::uint8_t actual_version)
    : std::runtime_error(StringBuilder()
    << "Expected version" << expected_version
    << ", but given: " << actual_version) {
  }
};

class UnknownCharsetException : public std::runtime_error {
 public:
  explicit UnknownCharsetException(std::uint8_t charset)
    : std::runtime_error(StringBuilder() << "The file has unknown charset: " << charset) {
  }
};

class InvalidCharsetException : public std::runtime_error {
 public:
  explicit InvalidCharsetException(const char *expected_charset)
    : std::runtime_error(StringBuilder() << "The charset should be " << expected_charset) {
  }
};

class InvalidFormatException : public std::runtime_error {
 public:
  explicit InvalidFormatException(const char *message = "The format is invalid") : std::runtime_error(message) {
  }
};

class DbOpenException : public std::runtime_error {
 public:
  explicit DbOpenException(const char *file_path)
    : std::runtime_error(StringBuilder() << "Can't readFromStream the file: " << file_path) {
  }
};

class DbReadException : public std::runtime_error {
 public:
  explicit DbReadException(const char *part)
    : std::runtime_error(StringBuilder() << "Can't read " << part) {
  }
};

}  // namespace sypexgeo
}  // namespace vyvid
}  // namespace xyz

#endif  // SRC_XYZ_VYVID_SYPEXGEO_ERRORS_H_
