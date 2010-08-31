#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModule.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeHandle.h>
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
         public TimeSlice,
         public JsModule,
         public JsModuleV8 {

      public:
         struct InstanceStructBase {

            const String Name;
            const Handle Object;

            InstanceStructBase (
                  const String &TheName,
                  const Handle TheObject) :
                  Name (TheName),
                  Object (TheObject) {;}
         };

         JsModuleV8Basic (const PluginInfo &Info, Config &local, Config &global);
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

         // JsModule Interface
         virtual void lookup_script_names (StringContainer &list);
         virtual void lookup_script_file_names (StringContainer &list);
         virtual void lookup_script_handles (HandleContainer &list);

         virtual Handle compile_script (
            const String &Name,
            const String &FileName,
            const char *Script,
            const Int32 Size);

         virtual Boolean recompile_script (
            const Handle ScriptHandle,
            const char *Script,
            const Int32 Size);

         virtual Handle lookup_script (const String &Name);
         virtual String lookup_script_name (const Handle ScriptHandle);
         virtual String lookup_script_file_name (const Handle ScriptHandle);
         virtual Boolean destroy_script (const Handle ScriptHandle);

         virtual void lookup_instance_names (const Handle Script, StringContainer &list);

         virtual void lookup_instance_handles (
            const Handle Script,
            HandleContainer &list);

         virtual Handle create_instance (
            const String &Name,
            const Handle ScriptHandle,
            const Config &Init);

         virtual Handle lookup_instance (const String &InstanceName);
         virtual Handle lookup_instance_script (const Handle Instance);
         virtual String lookup_instance_name (const Handle Instance);
         virtual Boolean recreate_instance (const Handle Instance, const Config &Init);
         virtual Boolean destroy_instance (const Handle Instance);

         // JsModuleV8 Interface
         virtual void reset_v8 ();

         virtual v8::Handle<v8::Context> get_v8_context ();

         virtual Boolean register_interface (
            const String &Name,
            v8::Persistent<v8::Object> object);

         virtual v8::Handle<v8::Object> require (const String &Value);
         virtual void get_require_list (StringContainer &list);

         virtual void handle_v8_exception (v8::TryCatch &tc);

         virtual String get_instance_name (v8::Handle<v8::Value> value);
         virtual Handle get_instance_handle (v8::Handle<v8::Value> value);

         virtual Boolean set_external_instance_handle_and_name (
            const Handle TheHandle,
            const String &TheName,
            v8::Handle<v8::Value> value);

      protected:
         struct InstanceStruct;

         struct ScriptStruct {

            const RuntimeHandle HandleObject;
            const Handle ScriptHandle;
            const String Name;
            const String FileName;
            v8::Persistent<v8::Script> script;
            v8::Persistent<v8::Function> ctor;
            HashTableHandleTemplate<InstanceStruct> table;

            void clear () {

               script.Dispose (); script.Clear ();
               ctor.Dispose (); ctor.Clear ();
            }

            ScriptStruct (
                  const String &TheName,
                  const String &TheFileName,
                  RuntimeContext *context) :
                  HandleObject (String ("JavaScript Script: ") + TheName, context),
                  ScriptHandle (HandleObject.get_runtime_handle ()),
                  Name (TheName),
                  FileName (TheFileName) {;}

            ~ScriptStruct () { clear (); }
         };

         struct InstanceStruct : public InstanceStructBase {

            Config local;
            ScriptStruct &script;
            V8ObjectPersist self;

            InstanceStruct (
                  const String &TheName,
                  const Handle TheObject,
                  ScriptStruct &theScript) :
                  InstanceStructBase (TheName, TheObject),
                  script (theScript) { script.table.store (TheObject, this); }

            ~InstanceStruct () {

               InstanceStruct *instance = script.table.remove (Object);
               if (instance != this) { script.table.store (Object, instance); }
               self.Dispose ();
               self.Clear ();
            }
         };

         void _empty_require ();
         void _release_instances ();
         void _init_context ();
         void _init_builtins ();
         void _init_ext ();
         void _stop_ext ();
         void _shutdown_ext ();
         void _load_scripts ();
         Boolean _create_instance (InstanceStruct &instance);
         void _shutdown_instance (InstanceStruct &instance);
         void _release_instance (InstanceStruct &instance);
         ScriptStruct *_find_script (Config &script);
         void _init (Config &local, Config &global);

         Log _log;
         StreamLog _out;
         Resources _rc;
         Definitions _defs;
         Boolean _shutdown;
         Boolean _reset;

         JsModuleRuntimeV8 *_runtime;

         StringContainer _localPaths;

         HashTableHandleTemplate<ScriptStruct> _scriptTable;
         HashTableStringTemplate<ScriptStruct> _scriptNameTable;
         HashTableHandleTemplate<InstanceStruct> _instanceTable;
         HashTableStringTemplate<InstanceStruct> _instanceNameTable;
         HashTableHandleTemplate<JsExtV8> _extTable;
         HashTableStringTemplate<v8::Persistent<v8::Object> > _requireTable;

         v8::Persistent<v8::Context> _context;
         v8::Persistent<v8::ObjectTemplate> _instanceTemplate;
         v8::Persistent<v8::ObjectTemplate> _globalTemplate;
         v8::Persistent<v8::FunctionTemplate> _requireFuncTemplate;
         v8::Persistent<v8::Function> _requireFunc;
         v8::Persistent<v8::String> _instanceAttrName;
         v8::Persistent<v8::String> _shutdownFuncName;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
