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

class SypexGeoNode : public node::ObjectWrap {
    public:
        static v8::Persistent<v8::FunctionTemplate> constructor;
        static void Init(v8::Handle<v8::Object> target);

    protected:
        SypexGeoNode();

        static v8::Handle<v8::Value> New(const v8::Arguments& args);

        static v8::Handle<v8::Value> GetCountry(const v8::Arguments& args);
        static v8::Handle<v8::Value> GetCityFull(const v8::Arguments& args);
        static v8::Handle<v8::Value> GetCity(const v8::Arguments& args);

        SypexGeo geo;
};

#endif  // _SYPEXGEO_NODE_SRC_SYPEXGEO_NODE_H_
