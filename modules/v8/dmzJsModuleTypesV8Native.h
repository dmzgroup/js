#ifndef DMZ_JS_MODULE_TYPES_V8_NATIVE_DOT_H
#define DMZ_JS_MODULE_TYPES_V8_NATIVE_DOT_H

#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzJsV8UtilTypes.h>

namespace dmz {

   class JsModuleTypesV8Native :
         public Plugin,
         public JsModuleTypesV8,
         public JsExtV8 {

      public:
         JsModuleTypesV8Native (const PluginInfo &Info, Config &local);
         ~JsModuleTypesV8Native ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsModuleTypesV8 Interface
         virtual v8::Handle<v8::Object> to_v8_vector (const Vector &Value);
         virtual Vector to_dmz_vector (const v8::Handle<v8::Value> Value);

         virtual v8::Handle<v8::Object> to_v8_matrix (const Matrix &Value);
         virtual Matrix to_dmz_matrix (const v8::Handle<v8::Value> Value);

         virtual v8::Handle<v8::Object> to_v8_mask (const Mask &Value);
         virtual Mask to_dmz_mask (const v8::Handle<v8::Value> Value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context) {;}
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         static JsModuleTypesV8Native *_to_self (const v8::Arguments &Args);
         static Vector *_to_vector (const V8Object &Obj);

         static V8Value _vector_create (const v8::Arguments &Args);

         static V8Value _vector_get_x (
            v8::Local<v8::String> property,
            const v8::AccessorInfo &Info);

         static void _vector_set_x (
            v8::Local<v8::String> property,
            v8::Local<v8::Value> value,
            const v8::AccessorInfo &Info);

         static V8Value _vector_get_y (
            v8::Local<v8::String> property,
            const v8::AccessorInfo &Info);

         static void _vector_set_y (
            v8::Local<v8::String> property,
            v8::Local<v8::Value> value,
            const v8::AccessorInfo &Info);

         static V8Value _vector_get_z (
            v8::Local<v8::String> property,
            const v8::AccessorInfo &Info);

         static void _vector_set_z (
            v8::Local<v8::String> property,
            v8::Local<v8::Value> value,
            const v8::AccessorInfo &Info);

         static V8Value _vector_copy (const v8::Arguments &Args);
         static V8Value _vector_to_string (const v8::Arguments &Args);
         static V8Value _vector_from_array (const v8::Arguments &Args);
         static V8Value _vector_to_array (const v8::Arguments &Args);
         static V8Value _vector_set (const v8::Arguments &Args);
         static V8Value _vector_set_xyz (const v8::Arguments &Args);
         static V8Value _vector_magnitude (const v8::Arguments &Args);
         static V8Value _vector_normalize (const v8::Arguments &Args);
         static V8Value _vector_add (const v8::Arguments &Args);
         static V8Value _vector_subtract (const v8::Arguments &Args);
         static V8Value _vector_multiply_const (const v8::Arguments &Args);
         static V8Value _vector_dot (const v8::Arguments &Args);
         static V8Value _vector_cross (const v8::Arguments &Args);
         static V8Value _vector_get_angle (const v8::Arguments &Args);

         Vector _to_vector (const V8Value &Value);
         void _clear ();
         void _create_symbols ();
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;
         v8::Persistent<v8::Object> _mask;
         v8::Persistent<v8::Function> _maskCtor;
         v8::Persistent<v8::Object> _matrix;
         v8::Persistent<v8::Function> _matrixCtor;

         V8ValuePersist _self;

         V8InterfaceHelper _vectorApi;
         V8FunctionTemplatePersist _vectorTemplate;
         V8FunctionPersist _vectorCtor;

         v8::Persistent<v8::String> _bitsStr;

         v8::Persistent<v8::String> _vStr;

         v8::Persistent<v8::String> _xStr;
         v8::Persistent<v8::String> _yStr;
         v8::Persistent<v8::String> _zStr;

      private:
         JsModuleTypesV8Native ();
         JsModuleTypesV8Native (const JsModuleTypesV8Native &);
         JsModuleTypesV8Native &operator= (const JsModuleTypesV8Native &);
   };
};


inline dmz::JsModuleTypesV8Native *
dmz::JsModuleTypesV8Native::_to_self (const v8::Arguments &Args) {

   return (dmz::JsModuleTypesV8Native *)v8::External::Unwrap (Args.Data ());
}


inline dmz::Vector *
dmz::JsModuleTypesV8Native::_to_vector (const V8Object &Obj) {

   return (Vector *)Obj->GetPointerFromInternalField (0);
}

#endif // DMZ_JS_MODULE_TYPES_V8_NATIVE_DOT_H
