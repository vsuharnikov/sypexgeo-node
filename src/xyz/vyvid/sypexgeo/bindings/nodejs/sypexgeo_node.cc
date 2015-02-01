/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.h"

#include <string>

v8::Persistent<v8::FunctionTemplate> SypexGeoNode::constructor;

void SypexGeoNode::Init(v8::Handle<v8::Object> target) {
  NanScope();

  v8::Local<v8::String>           name = NanNew<v8::String>("Geo");
  v8::Local<v8::FunctionTemplate> tpl  = NanNew<v8::FunctionTemplate>(SypexGeoNode::New);
  NanAssignPersistent(constructor, tpl);

  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(name);

  NODE_SET_PROTOTYPE_METHOD(tpl, "find", Find);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCountryIso", GetCountryIso);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCountry", GetCountryIso);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCityFull", GetCityFull);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCity", GetCityFull);

  target->Set(NanNew<v8::String>("MODE_MEMORY"), NanNew<v8::Integer>(2));
  target->Set(name, tpl->GetFunction());
}

SypexGeoNode::SypexGeoNode()
  : node::ObjectWrap() {
}

void SypexGeoNode::Connect(const char *filePath) {
  db = std::unique_ptr<xyz::vyvid::sypexgeo::Db>(xyz::vyvid::sypexgeo::Db::fromFile(filePath));
}

NAN_METHOD(SypexGeoNode::New) {
  NanScope();

  if (!args.IsConstructCall())
    NanThrowTypeError("You should use the new operator to create instances of this object.");

  if (args.Length() < 1)
    NanThrowTypeError("You should specify a path to the db file.");

  SypexGeoNode   *obj = new SypexGeoNode();
  NanAsciiString path(args[0]);

  try {
    obj->Connect(*path);
  } catch (std::exception e) {
    NanThrowTypeError(e.what());
  }

  obj->Wrap(args.This());
  NanReturnValue(args.This());
}

NAN_METHOD(SypexGeoNode::Find) {
  NanScope();

  if (args.Length() < 1) {
    NanThrowTypeError("You should specify the ip.");
  }

  auto           obj      = node::ObjectWrap::Unwrap<SypexGeoNode>(args.This());
  NanAsciiString ip(args[0]);
  auto           location = obj->db->find(*ip);

  if (nullptr == location) {
    NanReturnValue(NanNull());
  }

  v8::Local<v8::Object> ret = NanNew<v8::Object>();

  ret->Set(NanNew<v8::String>("latitude"), NanNew<v8::Number>(location->getLatitude()));
  ret->Set(NanNew<v8::String>("longitude"), NanNew<v8::Number>(location->getLongitude()));

  v8::Local<v8::Object> country = NanNew<v8::Object>();
  country->Set(NanNew<v8::String>("id"), NanNew<v8::Number>(location->getCountryId()));
  country->Set(NanNew<v8::String>("iso"), NanNew<v8::String>(location->getCountryIso()));
  country->Set(NanNew<v8::String>("name"),
    GetNames(location->getEnglishCountryName(), location->getRussianCountryName()));

  ret->Set(NanNew<v8::String>("country"), country);

  uint32_t region_id = location->getRegionId();
  if (region_id > 0) {
    v8::Local<v8::Object> region = NanNew<v8::Object>();
    region->Set(NanNew<v8::String>("id"), NanNew<v8::Number>(region_id));
    region->Set(NanNew<v8::String>("iso"), NanNew<v8::String>(location->getRegionIso()));
    region->Set(NanNew<v8::String>("name"),
      GetNames(location->getEnglishRegionName(), location->getRussianRegionName()));
    ret->Set(NanNew<v8::String>("region"), region);
  }

  uint32_t city_id = location->getCityId();
  if (city_id > 0) {
    v8::Local<v8::Object> city = NanNew<v8::Object>();
    city->Set(NanNew<v8::String>("id"), NanNew<v8::Number>((uint32_t) location->getCityId()));
    city->Set(NanNew<v8::String>("name"),
      GetNames(location->getEnglishCityName(), location->getRussianCityName()));
    ret->Set(NanNew<v8::String>("city"), city);
  }

  NanReturnValue(ret);
}

NAN_METHOD(SypexGeoNode::GetCountryIso) {
  NanScope();

  if (args.Length() < 1) {
    NanThrowTypeError("You should specify the ip.");
  }

  SypexGeoNode   *obj     = node::ObjectWrap::Unwrap<SypexGeoNode>(args.This());
  NanAsciiString ip(args[0]);
  auto           location = obj->db->find(*ip);

  if (nullptr == location) {
    NanReturnValue(NanNull());
  }

  NanReturnValue(NanNew<v8::String>(location->getCountryIso()));
}

NAN_METHOD(SypexGeoNode::GetCityFull) {
  NanScope();

  if (args.Length() < 1) {
    NanThrowTypeError("You should specify the ip.");
  }

  auto           obj      = node::ObjectWrap::Unwrap<SypexGeoNode>(args.This());
  NanAsciiString ip(args[0]);
  auto           location = obj->db->find(*ip);

  if (nullptr == location) {
    NanReturnValue(NanNull());
  }

  v8::Local<v8::Object> ret = NanNew<v8::Object>();

  ret->Set(NanNew<v8::String>("latitude"), NanNew<v8::Number>(location->getLatitude()));
  ret->Set(NanNew<v8::String>("longitude"), NanNew<v8::Number>(location->getLongitude()));
  ret->Set(NanNew<v8::String>("country"), NanNew<v8::String>(location->getCountryIso()));

  uint32_t region_id = location->getRegionId();
  if (region_id > 0) {
    ret->Set(NanNew<v8::String>("region"), NanNew<v8::String>(location->getRussianRegionName()));
  }

  uint32_t city_id = location->getCityId();
  if (city_id > 0) {
    ret->Set(NanNew<v8::String>("name"), NanNew<v8::String>(location->getRussianCityName()));
  }

  NanReturnValue(ret);
}

v8::Local<v8::Object> SypexGeoNode::GetNames(const std::string &englishName, const std::string &russianName) {
  v8::Local<v8::Object> ret = NanNew<v8::Object>();

  ret->Set(NanNew<v8::String>("en"), NanNew<v8::String>(englishName));
  ret->Set(NanNew<v8::String>("ru"), NanNew<v8::String>(russianName));

  return ret;
}

void RegisterModule(v8::Handle<v8::Object> target) {
  SypexGeoNode::Init(target);
}

NODE_MODULE(sypexgeo, RegisterModule);
