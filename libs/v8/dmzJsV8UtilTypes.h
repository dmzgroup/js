#ifndef DMZ_JS_V8_UTIL_TYPES_DOT_H
#define DMZ_JS_V8_UTIL_TYPES_DOT_H

#include <v8.h>

namespace dmz {

typedef v8::Handle<v8::Value> V8Value;
typedef v8::Persistent<v8::Value> V8ValuePersist;
typedef v8::Handle<v8::ObjectTemplate> V8ObjectTemplate;
typedef v8::Persistent<v8::ObjectTemplate> V8ObjectTemplatePersist;
typedef v8::Handle<v8::Object> V8Object;
typedef v8::Persistent<v8::Object> V8ObjectPersist;
typedef v8::Handle<v8::FunctionTemplate> V8FunctionTemplate;
typedef v8::Persistent<v8::FunctionTemplate> V8FunctionTemplatePersist;
typedef v8::Handle<v8::Function> V8Function;
typedef v8::Persistent<v8::Function> V8FunctionPersist;
typedef v8::Handle<v8::Array> V8Array;
typedef v8::Persistent<v8::Array> V8ArrayPersist;
typedef v8::Handle<v8::Number> V8Number;
typedef v8::Persistent<v8::Number> V8NumberPersist;
typedef v8::Handle<v8::String> V8String;
typedef v8::Persistent<v8::String> V8StringPersist;
typedef v8::Handle<v8::External> V8External;
typedef v8::Persistent<v8::External> V8ExternalPersist;

};

#endif // DMZ_JS_V8_UTIL_TYPES_DOT_H
