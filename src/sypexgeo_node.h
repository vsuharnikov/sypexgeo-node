/**
 * Copyright by Vyatcheslav Suharnikov and sypexgeo-node contributors
 * See contributors list in README
 *
 * See license text in LICENSE file
 */

#ifndef _SYPEXGEO_NODE_SRC_SYPEXGEO_NODE_H_
#define _SYPEXGEO_NODE_SRC_SYPEXGEO_NODE_H_

#include "./sypexgeo.h"
#include <node.h>
#include "nan.h"

using namespace v8;

class SypexGeoNode : public node::ObjectWrap {
    public:
        static Persistent<FunctionTemplate> constructor;
        static void Init(Handle<Object> target);

    protected:
        SypexGeoNode();

        static NAN_METHOD(New);

        static NAN_METHOD(GetCountry);
        static NAN_METHOD(GetCityFull);
        static NAN_METHOD(GetCity);

        SypexGeo geo;
};

#endif  // _SYPEXGEO_NODE_SRC_SYPEXGEO_NODE_H_
