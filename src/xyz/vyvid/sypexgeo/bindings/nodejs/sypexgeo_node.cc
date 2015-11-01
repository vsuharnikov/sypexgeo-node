/**
* Copyright by Vyatcheslav Suharnikov and contributors.
* See contributors list in AUTHORS.
*
* Licensed under the MIT license, see license text in LICENSE file.
*/

#include "xyz/vyvid/sypexgeo/bindings/nodejs/sypexgeo_node.h"

#include <nan.h>

Nan::Persistent<v8::Function> SypexGeoNode::constructor;

void SypexGeoNode::Init(v8::Local<v8::Object> exports) {
  Nan::HandleScope scope;

  v8::Local<v8::String> name = Nan::New("Geo").ToLocalChecked();

  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(SypexGeoNode::New);
  tpl->SetClassName(name);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "find", Find);
  Nan::SetPrototypeMethod(tpl, "getCountryIso", GetCountryIso);
  Nan::SetPrototypeMethod(tpl, "getCountry", GetCountryIso);
  Nan::SetPrototypeMethod(tpl, "getCityFull", GetCityFull);
  Nan::SetPrototypeMethod(tpl, "getCity", GetCityFull);

  constructor.Reset(tpl->GetFunction());

  exports->Set(Nan::New("MODE_MEMORY").ToLocalChecked(), Nan::New<v8::Integer>(2));
  exports->Set(name, tpl->GetFunction());
}

SypexGeoNode::SypexGeoNode()
    : Nan::ObjectWrap() {
}

void SypexGeoNode::Connect(const char *filePath) {
  db = std::unique_ptr<xyz::vyvid::sypexgeo::Db>(xyz::vyvid::sypexgeo::Db::fromFile(filePath));
}

NAN_METHOD(SypexGeoNode::New) {
  if (!info.IsConstructCall())
    Nan::ThrowTypeError("You should use the new operator to create instances of this object.");

  if (info.Length() < 1)
    Nan::ThrowTypeError("You should specify a path to the db file.");

  SypexGeoNode *obj = new SypexGeoNode();
  Nan::Utf8String path(info[0]);

  try {
    obj->Connect(*path);
  } catch (std::exception e) {
    Nan::ThrowTypeError(e.what());
  }

  obj->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(SypexGeoNode::Find) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("You should specify the ip.");
  }

  auto obj = Nan::ObjectWrap::Unwrap<SypexGeoNode>(info.This());
  Nan::Utf8String ip(info[0]);
  auto location = obj->db->find(*ip);

  if (nullptr == location) {
    info.GetReturnValue().Set(Nan::Null());
  }

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  ret->Set(Nan::New("latitude").ToLocalChecked(), Nan::New<v8::Number>(location->getLatitude()));
  ret->Set(Nan::New("longitude").ToLocalChecked(), Nan::New<v8::Number>(location->getLongitude()));

  v8::Local<v8::Object> country = Nan::New<v8::Object>();

  country->Set(Nan::New("id").ToLocalChecked(), Nan::New<v8::Number>(location->getCountryId()));
  country->Set(Nan::New("iso").ToLocalChecked(), Nan::New(location->getCountryIso()).ToLocalChecked());
  country->Set(Nan::New("name").ToLocalChecked(),
               GetNames(location->getEnglishCountryName(),
                        location->getRussianCountryName()));

  ret->Set(Nan::New("country").ToLocalChecked(), country);

  uint32_t region_id = location->getRegionId();
  if (region_id > 0) {
    v8::Local<v8::Object> region = Nan::New<v8::Object>();
    region->Set(Nan::New("id").ToLocalChecked(), Nan::New<v8::Number>(region_id));
    region->Set(Nan::New("iso").ToLocalChecked(), Nan::New(location->getRegionIso()).ToLocalChecked());
    region->Set(Nan::New("name").ToLocalChecked(),
                GetNames(location->getEnglishRegionName(),
                         location->getRussianRegionName()));
    ret->Set(Nan::New("region").ToLocalChecked(), region);
  }

  uint32_t city_id = location->getCityId();
  if (city_id > 0) {
    v8::Local<v8::Object> city = Nan::New<v8::Object>();
    city->Set(Nan::New("id").ToLocalChecked(), Nan::New<v8::Number>((uint32_t) location->getCityId()));
    city->Set(Nan::New("name").ToLocalChecked(),
              GetNames(location->getEnglishCityName(),
                       location->getRussianCityName()));
    ret->Set(Nan::New("city").ToLocalChecked(), city);
  }

  info.GetReturnValue().Set(ret);
}

NAN_METHOD(SypexGeoNode::GetCountryIso) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("You should specify the ip.");
  }

  SypexGeoNode *obj = Nan::ObjectWrap::Unwrap<SypexGeoNode>(info.This());
  Nan::Utf8String ip(info[0]);
  auto location = obj->db->find(*ip);

  if (nullptr == location) {
    info.GetReturnValue().Set(Nan::Null());
  }

  info.GetReturnValue().Set(Nan::New(location->getCountryIso()).ToLocalChecked());
}

NAN_METHOD(SypexGeoNode::GetCityFull) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("You should specify the ip.");
  }

  auto obj = Nan::ObjectWrap::Unwrap<SypexGeoNode>(info.This());
  Nan::Utf8String ip(info[0]);
  auto location = obj->db->find(*ip);

  if (nullptr == location) {
    info.GetReturnValue().Set(Nan::Null());
  }

  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  Nan::Set(ret, Nan::New("latitude").ToLocalChecked(), Nan::New<v8::Number>(location->getLatitude()));
  Nan::Set(ret, Nan::New("longitude").ToLocalChecked(), Nan::New<v8::Number>(location->getLongitude()));
  Nan::Set(ret, Nan::New("country").ToLocalChecked(), Nan::New(location->getCountryIso()).ToLocalChecked());

  uint32_t region_id = location->getRegionId();
  if (region_id > 0) {
    Nan::Set(ret, Nan::New("region").ToLocalChecked(), Nan::New(location->getRussianRegionName()).ToLocalChecked());
  }

  uint32_t city_id = location->getCityId();
  if (city_id > 0) {
    Nan::Set(ret, Nan::New("name").ToLocalChecked(), Nan::New(location->getRussianCityName()).ToLocalChecked());
  }

  info.GetReturnValue().Set(ret);
}

v8::Local<v8::Object> SypexGeoNode::GetNames(const std::string &englishName, const std::string &russianName) {
  v8::Local<v8::Object> ret = Nan::New<v8::Object>();

  Nan::Set(ret, Nan::New("en").ToLocalChecked(), Nan::New(englishName).ToLocalChecked());
  Nan::Set(ret, Nan::New("ru").ToLocalChecked(), Nan::New(russianName).ToLocalChecked());

  return ret;
}

void RegisterModule(v8::Handle<v8::Object> target) {
  SypexGeoNode::Init(target);
}

NODE_MODULE(sypexgeo, RegisterModule);
