#ifndef DMZ_JS_MODULE_TYPES_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_TYPES_V8_BASIC_DOT_H

#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzJsModuleTypesV8.h>
#include <dmzJsExtV8.h>

namespace dmz {

   class JsModuleTypesV8Basic :
         public Plugin,
         public JsModuleTypesV8,
         public JsExtV8 {

      public:
         JsModuleTypesV8Basic (const PluginInfo &Info, Config &local);
         ~JsModuleTypesV8Basic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsModuleTypesV8 Interface
         virtual v8::Handle<v8::Object> to_v8_vector (const Vector &Value);
         virtual Vector to_dmz_vector (const v8::Handle<v8::Object> Value);

         virtual v8::Handle<v8::Object> to_v8_matrix (const Matrix &Value);
         virtual Matrix to_dmz_matrix (const v8::Handle<v8::Object> Value);

         virtual v8::Handle<v8::Object> to_v8_mask (const Mask &Value);
         virtual Mask to_dmz_mask (const v8::Handle<v8::Object> Value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_ext_v8_state (const StateEnum State);

      protected:
         void _clear ();
         void _create_symbols ();
         void _init (Config &local);

         Log _log;

         JsModuleV8 *_core;
         v8::Persistent<v8::Object> _mask;
         v8::Persistent<v8::Function> _maskCtor;
         v8::Persistent<v8::Object> _matrix;
         v8::Persistent<v8::Function> _matrixCtor;
         v8::Persistent<v8::Object> _vector;
         v8::Persistent<v8::Function> _vectorCtor;

         v8::Persistent<v8::String> _bitsStr;

         v8::Persistent<v8::String> _vStr;

         v8::Persistent<v8::String> _xStr;
         v8::Persistent<v8::String> _yStr;
         v8::Persistent<v8::String> _zStr;

      private:
         JsModuleTypesV8Basic ();
         JsModuleTypesV8Basic (const JsModuleTypesV8Basic &);
         JsModuleTypesV8Basic &operator= (const JsModuleTypesV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_TYPES_V8_BASIC_DOT_H
