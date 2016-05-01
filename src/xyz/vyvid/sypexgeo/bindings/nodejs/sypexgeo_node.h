/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#ifndef SRC_XYZ_VYVID_SYPEXGEO_BINDINGS_NODEJS_SYPEXGEO_NODE_H_
#define SRC_XYZ_VYVID_SYPEXGEO_BINDINGS_NODEJS_SYPEXGEO_NODE_H_

#include <nan.h>
#include <string>
#include <memory>

#include "xyz/vyvid/sypexgeo/db.h"

class SypexGeoNode: public Nan::ObjectWrap {
 public:
  static NAN_MODULE_INIT(Init);

 protected:
  std::unique_ptr<xyz::vyvid::sypexgeo::Db> db;

  SypexGeoNode();

  void Connect(const char *filePath);

  static v8::Local <v8::Object> GetNames(const std::string &englishName, const std::string &russianName);

  static NAN_METHOD(New);

  static NAN_METHOD(Find);

  static NAN_METHOD(GetCountryIso);

  /*!
   * \deprecated
   */
  static NAN_METHOD(GetCountry);

  /*!
   * \deprecated
   */
  static NAN_METHOD(GetCityFull);

  static Nan::Persistent<v8::Function> constructor;
};

#endif  // SRC_XYZ_VYVID_SYPEXGEO_BINDINGS_NODEJS_SYPEXGEO_NODE_H_
