#ifndef DMZ_JS_V8_UTIL_TYPES_DOT_H
#define DMZ_JS_V8_UTIL_TYPES_DOT_H

#include <v8.h>

namespace dmz {

typedef v8::Handle<v8::Value> V8Value;
typedef v8::Handle<v8::Object> V8Object;
typedef v8::Handle<v8::Function> V8Function;
typedef v8::Handle<v8::Array> V8Array;
typedef v8::Handle<v8::Number> V8Number;

};

#endif // DMZ_JS_V8_UTIL_TYPES_DOT_H
