/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#include "./sypexgeo_node.h"
#include <node.h>

v8::Persistent<v8::FunctionTemplate> SypexGeoNode::constructor;

void SypexGeoNode::Init(v8::Handle<v8::Object> target) {
    v8::HandleScope scope;

    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(SypexGeoNode::New);
    v8::Local<v8::String> name = v8::String::NewSymbol("Geo");

    SypexGeoNode::constructor = v8::Persistent<v8::FunctionTemplate>::New(tpl);
    SypexGeoNode::constructor->InstanceTemplate()->SetInternalFieldCount(1);
    SypexGeoNode::constructor->SetClassName(name);

    NODE_SET_PROTOTYPE_METHOD(SypexGeoNode::constructor, "getCountry", SypexGeoNode::GetCountry);
    NODE_SET_PROTOTYPE_METHOD(SypexGeoNode::constructor, "getFullCity", SypexGeoNode::GetFullCity);
    NODE_SET_PROTOTYPE_METHOD(SypexGeoNode::constructor, "getCity", SypexGeoNode::GetCity);

    target->Set(v8::String::NewSymbol("MODE_MEMORY"), v8::Integer::New(SypexGeo::MODE_MEMORY));
    target->Set(name, SypexGeoNode::constructor->GetFunction());
}

SypexGeoNode::SypexGeoNode()
    : node::ObjectWrap() {}

v8::Handle<v8::Value> SypexGeoNode::New(const v8::Arguments &args) {
    v8::HandleScope scope;

    if (!args.IsConstructCall()) {
        return v8::ThrowException(v8::Exception::TypeError(
            v8::String::New("Use the new operator to create instances of this object."))
        );
    }

    if (args.Length() < 2) {
        return v8::ThrowException(v8::Exception::TypeError(
            v8::String::New("You need specify path to db file and mode.")
        ));
    }

    SypexGeoNode *obj = new SypexGeoNode();
    v8::String::AsciiValue path(args[0]->ToString());

    uint8_t code = obj->geo.open(*path, args[1]->ToInteger()->Value());
    switch (code) {
        case SYPEXGEO_ERROR_FILE_NOT_FOUND:
            return v8::ThrowException(v8::Exception::TypeError(
                v8::String::New("File not found.")
            ));
        break;

        case SYPEXGEO_ERROR_INVALID_TAG:
            return v8::ThrowException(v8::Exception::TypeError(
                v8::String::New("Invalid file format.")
            ));
        break;
    }

    obj->Wrap(args.This());

    return args.This();
}

v8::Handle<v8::Value> SypexGeoNode::GetCountry(const v8::Arguments &args) {
    v8::HandleScope scope;

    if (args.Length() < 1) {
        return v8::ThrowException(v8::Exception::TypeError(
            v8::String::New("You need specify ip.")
        ));
    }

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    v8::String::AsciiValue ip(args[0]->ToString());
    const char *country = obj->geo.getCountry(*ip);
    if (NULL == country)
        return scope.Close(v8::Null());
    else
        return scope.Close(v8::String::New(country));
}

v8::Handle<v8::Value> SypexGeoNode::GetFullCity(const v8::Arguments &args) {
    v8::HandleScope scope;

    if (args.Length() < 1) {
        return v8::ThrowException(v8::Exception::TypeError(
            v8::String::New("You need specify ip.")
        ));
    }

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    v8::String::AsciiValue ip(args[0]->ToString());
    sxgeo_city *city = obj->geo.getFullCity(*ip);

    v8::Local<v8::Object> ret = v8::Object::New();
    ret->Set(v8::String::New("timezone"),  city->timezone ? v8::String::New(city->timezone) : v8::Null());
    ret->Set(v8::String::New("country"),   v8::String::New(city->country));
    ret->Set(v8::String::New("region"),    city->region ? v8::String::New(city->region) : v8::Null());
    ret->Set(v8::String::New("name"),      v8::String::New(city->city));
    ret->Set(v8::String::New("fips"),      v8::Integer::New(city->fips));
    ret->Set(v8::String::New("latitude"),  v8::Number::New(city->latitude));
    ret->Set(v8::String::New("longitude"), v8::Number::New(city->longitude));

    delete city;

    return scope.Close(ret);
}

v8::Handle<v8::Value> SypexGeoNode::GetCity(const v8::Arguments &args) {
    v8::HandleScope scope;

    if (args.Length() < 1) {
        return v8::ThrowException(v8::Exception::TypeError(
            v8::String::New("You need specify ip.")
        ));
    }

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    v8::String::AsciiValue ip(args[0]->ToString());
    sxgeo_city *city = obj->geo.getCity(*ip);

    v8::Local<v8::Object> ret = v8::Object::New();
    ret->Set(v8::String::New("country"),   v8::String::New(city->country));
    ret->Set(v8::String::New("name"),      v8::String::New(city->city));
    ret->Set(v8::String::New("fips"),      v8::Integer::New(city->fips));
    ret->Set(v8::String::New("latitude"),  v8::Number::New(city->latitude));
    ret->Set(v8::String::New("longitude"), v8::Number::New(city->longitude));

    delete city;

    return scope.Close(ret);
}

void RegisterModule(v8::Handle<v8::Object> target) {
    SypexGeoNode::Init(target);
}

NODE_MODULE(sypexgeo, RegisterModule);
