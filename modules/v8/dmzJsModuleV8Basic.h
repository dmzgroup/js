#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzSystemFile.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>

namespace dmz {

   class JsExtV8;
   class JsModuleRuntimeV8;

   class JsModuleV8Basic :
         public Plugin,
         public JsModuleV8,
         public TimeSlice {

      public:
         JsModuleV8Basic (const PluginInfo &Info, Config &local);
         ~JsModuleV8Basic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 DeltaTime);

         // JsModuleV8 Interface
         virtual void reset_v8 ();

         virtual v8::Handle<v8::Context> get_v8_context ();

         virtual Boolean register_interface (
            const String &Name,
            v8::Persistent<v8::Object> object);

         virtual v8::Handle<v8::Object> require (const String &Value);

         virtual void handle_v8_exception (v8::TryCatch &tc);

      protected:
         struct ScriptStruct {

            const String Name;
            const String FileName;
            v8::Persistent<v8::Script> script;
            v8::Persistent<v8::Function> ctor;

            void clear () {

               script.Dispose (); script.Clear ();
               ctor.Dispose (); ctor.Clear ();
            }

            ScriptStruct (const String &TheName, const String &TheFileName) :
                  Name (TheName),
                  FileName (TheFileName) {;}

            ~ScriptStruct () { clear (); }
         };

         struct InstanceStruct {

            const String Name;
            ScriptStruct &script;

            InstanceStruct (const String &TheName, ScriptStruct &theScript) :
                  Name (TheName),
                  script (theScript) {;}
         };

         void _empty_require ();
         void _release_instances ();
         void _init_context ();
         void _init_builtins ();
         void _init_ext ();
         void _shutdown_ext ();
         void _load_scripts ();
         ScriptStruct *_find_script (Config &script);
         void _init (Config &local);

         Log _log;
         StreamLog _out;
         Resources _rc;
         Definitions _defs;
         Boolean _shutdown;

         JsModuleRuntimeV8 *_runtime;

         StringContainer _localPaths;

         HashTableStringTemplate<ScriptStruct> _scriptTable;
         HashTableStringTemplate<InstanceStruct> _instanceTable;
         HashTableHandleTemplate<JsExtV8> _extTable;
         HashTableStringTemplate<v8::Persistent<v8::Object> > _requireTable;

         v8::Persistent<v8::Context> _context;
         v8::Persistent<v8::ObjectTemplate> _globalTemplate;
         v8::Persistent<v8::FunctionTemplate> _requireFuncTemplate;
         v8::Persistent<v8::Function> _requireFunc;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
