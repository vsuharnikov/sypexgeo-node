/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#include "./sypexgeo_node.h"
#include <node.h>
#include "nan.h"

using namespace v8;

Persistent<FunctionTemplate> SypexGeoNode::constructor;

void SypexGeoNode::Init(Handle<Object> target) {
    NanScope();

    Local<String> name = NanNew<String>("Geo");
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(SypexGeoNode::New);
    NanAssignPersistent(constructor, tpl);

    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->SetClassName(name);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getCountry",  GetCountry);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getCityFull", GetCityFull);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getCity",     GetCity);

    target->Set(NanNew<String>("MODE_MEMORY"), NanNew<Integer>(SypexGeo::MODE_MEMORY));
    target->Set(name, tpl->GetFunction());
}

SypexGeoNode::SypexGeoNode()
    : node::ObjectWrap() {}

NAN_METHOD(SypexGeoNode::New) {
    NanScope();

    if (!args.IsConstructCall())
        NanThrowTypeError("Use the new operator to create instances of this object.");

    if (args.Length() < 2)
        NanThrowTypeError("You need specify path to db file and mode.");

    SypexGeoNode *obj = new SypexGeoNode();

    size_t count;
    char* path = NanCString(args[0]->ToString(), &count);
    uint8_t code = obj->geo.open(path, (int8_t)args[1]->ToInteger()->Value());
    delete path;

    switch (code) {
        case SYPEXGEO_ERROR_FILE_NOT_FOUND:
            NanThrowTypeError("File not found.");
        break;

        case SYPEXGEO_ERROR_INVALID_TAG:
            NanThrowTypeError("Invalid file format.");
        break;
    }

    obj->Wrap(args.This());

    NanReturnValue(args.This());
}

NAN_METHOD(SypexGeoNode::GetCountry) {
    NanScope();

    if (args.Length() < 1)
        NanThrowTypeError("You need specify ip.");

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    size_t count;
    char* ip = NanCString(args[0]->ToString(), &count);
    const char *country = obj->geo.getCountry(ip);
    delete ip;

    if (NULL == country) {
        NanReturnValue( NanNull() );
    } else {
        NanReturnValue( NanNew<String>(country) );
    }
}

NAN_METHOD(SypexGeoNode::GetCityFull) {
    NanScope();

    if (args.Length() < 1)
        NanThrowTypeError("You need specify ip.");

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    size_t count;
    char* ip = NanCString(args[0]->ToString(), &count);
    sxgeo_city *city = obj->geo.getCityFull(ip);
    delete ip;

    if (NULL == city) {
        NanReturnValue( NanNull() );
    } else {
        Local<Object> ret = NanNew<Object>();

        if(city->timezone)
            ret->Set(NanNew<String>("timezone"), NanNew<String>(city->timezone));
        else
            ret->Set(NanNew<String>("timezone"), NanNull());

        if(city->region)
            ret->Set(NanNew<String>("region"), NanNew<String>(city->region));
        else
            ret->Set(NanNew<String>("region"), NanNull());

        ret->Set(NanNew<String>("country"),   NanNew<String>(city->country));
        ret->Set(NanNew<String>("name"),      NanNew<String>(city->city));
        ret->Set(NanNew<String>("fips"),      NanNew<Integer>(city->fips));
        ret->Set(NanNew<String>("latitude"),  NanNew<Number>(city->latitude));
        ret->Set(NanNew<String>("longitude"), NanNew<Number>(city->longitude));

        delete city;
        NanReturnValue( ret );
    }
}

NAN_METHOD(SypexGeoNode::GetCity) {
    NanScope();

    if (args.Length() < 1) {
        NanThrowTypeError("You need specify ip.");
    }

    SypexGeoNode *obj = ObjectWrap::Unwrap<SypexGeoNode>(args.This());

    size_t count;
    char* ip = NanCString(args[0]->ToString(), &count);
    sxgeo_city *city = obj->geo.getCity(ip);
    delete ip;

    if (NULL == city) {
        NanReturnValue( NanNull() );
    } else {
        Local<Object> ret = NanNew<Object>();
        ret->Set(NanNew<String>("country"),   NanNew<String>(city->country));
        ret->Set(NanNew<String>("name"),      NanNew<String>(city->city));
        ret->Set(NanNew<String>("fips"),      NanNew<Integer>(city->fips));
        ret->Set(NanNew<String>("latitude"),  NanNew<Number>(city->latitude));
        ret->Set(NanNew<String>("longitude"), NanNew<Number>(city->longitude));

        delete city;
        NanReturnValue(ret);
    }
}

void RegisterModule(Handle<Object> target) {
    SypexGeoNode::Init(target);
}

NODE_MODULE(sypexgeo, RegisterModule);
