#include "dmzJsExtV8RenderIsect.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRenderIsect.h>
#include <dmzRenderModuleIsect.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8RenderIsect::JsExtV8RenderIsect (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _types (0),
      _core (0),
      _isect (0) {

   _init (local);
}


dmz::JsExtV8RenderIsect::~JsExtV8RenderIsect () {

}


// Plugin Interface
void
dmz::JsExtV8RenderIsect::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

   }
}


void
dmz::JsExtV8RenderIsect::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
      if (!_isect) { _isect = RenderModuleIsect::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
      if (_isect && (_isect == RenderModuleIsect::cast (PluginPtr))) { _isect = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8RenderIsect::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8RenderIsect::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8RenderIsect::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/framework/isect",
            _isectApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _startStr = V8StringPersist::New (v8::String::NewSymbol ("start"));
      _endStr = V8StringPersist::New (v8::String::NewSymbol ("end"));
      _directionStr = V8StringPersist::New (v8::String::NewSymbol ("direction"));

      _typeStr = V8StringPersist::New (v8::String::NewSymbol ("type"));
      _normalStr = V8StringPersist::New (v8::String::NewSymbol ("normal"));
      _objectStr = V8StringPersist::New (v8::String::NewSymbol ("object"));
      _distanceStr = V8StringPersist::New (v8::String::NewSymbol ("distance"));
      _cullStr = V8StringPersist::New (v8::String::NewSymbol ("cull"));

   }
   else if (State == JsExtV8::Shutdown) {

      _startStr.Dispose (); _startStr.Clear ();
      _endStr.Dispose (); _endStr.Clear ();
      _directionStr.Dispose (); _directionStr.Clear ();

      _typeStr.Dispose (); _typeStr.Clear ();
      _normalStr.Dispose (); _normalStr.Clear ();
      _objectStr.Dispose (); _objectStr.Clear ();
      _distanceStr.Dispose (); _distanceStr.Clear ();
      _cullStr.Dispose (); _cullStr.Clear ();

      _isectApi.clear ();
      _v8Context.Clear ();
   }
}


dmz::V8Value
dmz::JsExtV8RenderIsect::_isect_do_isect (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8RenderIsect *self = to_self (Args);

   if (self && self->_isect && self->_types) {

      V8Value testArg = Args[0];
      IsectTestContainer tests;

      if (testArg->IsArray ()) {

         String error;

         V8Array array = V8Array::Cast (testArg);

         const uint32_t Length = array->Length ();

         for (uint32_t ix = 0; ix < Length; ix++) {

            V8Value value = array->Get (v8::Integer::New (ix));

            if (value->IsObject ()) {

               V8Object obj = V8Object::Cast (value);

               if (!self->_add_test (obj, tests, error)) {

               }
            }
            else {

            }
         }
      }
      else if (testArg->IsObject ()) {

         String error;
         V8Object obj = testArg->ToObject ();
         if (!self->_add_test (obj, tests, error)) {

         }
      }

      IsectParameters params;

      if (Args.Length () > 1) {

         V8Value paramArg = Args[1];

         if (paramArg->IsObject ()) {

            self->_get_params (V8Object::Cast (paramArg), params);
         }
      }

      IsectResultContainer out;

      if (self->_isect->do_isect (params, tests, out)) {

      }
   }

   return scope.Close (result);
}


dmz::Boolean
dmz::JsExtV8RenderIsect::_get_params (V8Object obj, IsectParameters &params) {

   v8::HandleScope scope;
   Boolean result (False);

   if (obj.IsEmpty () == false) {

      const UInt32 Value = v8_to_uint32 (obj->Get (_typeStr));

      IsectTestResultTypeEnum type (IsectFirstPoint);

      if (Value == IsectFirstPoint) { type = IsectFirstPoint; }
      else if (Value == IsectClosestPoint) { type = IsectClosestPoint; }
      else if (Value == IsectAllPoints) { type = IsectAllPoints; }

      params.set_test_result_type (type);
      params.set_calculate_normal (v8_to_boolean (obj->Get (_normalStr), True));
      params.set_calculate_object_handle (v8_to_boolean (obj->Get (_objectStr), True));
      params.set_calculate_distance (v8_to_boolean (obj->Get (_distanceStr), True));
      params.set_calculate_cull_mode (v8_to_boolean (obj->Get (_cullStr), True));

      result = True;
   }

   return result;
}


dmz::Boolean
dmz::JsExtV8RenderIsect::_add_test (
      V8Object test,
      IsectTestContainer &list,
      String &error) {

   v8::HandleScope scope;
   Boolean result (False);

   if (_types && (test.IsEmpty () == false)) {

      IsectTestTypeEnum type = IsectUnknownTest;

      if (test->HasRealNamedProperty (_startStr)) {

         Vector v1 = _types->to_dmz_vector (test->Get (_startStr));
         Vector v2;

         if (test->HasRealNamedProperty (_endStr)) {

             result = True;
             type = IsectSegmentTest;
             v2 = _types->to_dmz_vector (test->Get (_endStr));
         }
         else if (test->HasRealNamedProperty (_directionStr)) {

             result = True;
             type = IsectRayTest;
             v2 = _types->to_dmz_vector (test->Get (_directionStr));
         }
         else {

            error.flush () << "Isect test parameter must contain either an "
               << "end point or a direction.";
         }

         list.add_test (type, v1, v2);
      }
      else {

         error.flush () << "Isect test parameter must contain a start point.";
      }
   }

   return result;
}


void
dmz::JsExtV8RenderIsect::_init (Config &local) {

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _isectApi.add_constant ("FirstPoint", (UInt32)IsectFirstPoint);
   _isectApi.add_constant ("ClosestPoint", (UInt32)IsectClosestPoint);
   _isectApi.add_constant ("AllPoints", (UInt32)IsectAllPoints);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8RenderIsect (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8RenderIsect (Info, local);
}

};
