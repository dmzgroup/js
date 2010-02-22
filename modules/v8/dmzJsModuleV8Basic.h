#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzSystemFile.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>

namespace dmz {

   class JsExtV8;

   class JsModuleV8Basic :
         public Plugin,
         public JsModuleV8 {

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

         // JsModuleV8 Interface

         // JsModuleV8Basic Interface
         void add_stack_trace (
            const Int32 Level,
            const String &Script,
            const Int32 Line,
            const Int32 Column,
            const String &Code);

         v8::Handle<v8::Object> require (const String &Value);

      protected:
         struct ScriptStruct {

            const String Name;
            const String FileName;
            v8::Persistent<v8::Script> script;
            v8::Persistent<v8::Function> ctor;

            void clear () {

               if (!script.IsEmpty ()) {

                  script.Dispose ();
                  script.Clear ();
               }

               if (!ctor.IsEmpty ()) {

                  ctor.Dispose ();
                  ctor.Clear ();
               }
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

         struct StackTraceStruct {

            const Int32 Level;
            const String Script;
            const Int32 Line;
            const Int32 Column;
            const String Code;

            StackTraceStruct *next;

            StackTraceStruct (
                  const Int32 TheLevel,
                  const String &TheScript,
                  const Int32 TheLine,
                  const Int32 TheColumn,
                  const String &TheCode) :
                  Level (TheLevel),
                  Script (TheScript),
                  Line (TheLine),
                  Column (TheColumn),
                  Code (TheCode),
                  next (0) {;}

            ~StackTraceStruct () {

               while (next) {

                  StackTraceStruct *tmp = next;
                  next = next->next;
                  tmp->next = 0; // short circuit the recursive destructor.
                  delete tmp; tmp = 0;
               }
            }
         };

         void _empty_require ();
         void _init_context ();
         void _init_ext ();
         void _handle_exception (v8::TryCatch &tc);
         void _load_scripts ();
         ScriptStruct *_find_script (Config &script);
         void _init (Config &local);

         Log _log;
         StreamLog _out;
         Resources _rc;

         StringContainer _localPaths;

         HashTableStringTemplate<ScriptStruct> _scriptTable;
         HashTableStringTemplate<InstanceStruct> _instanceTable;
         HashTableHandleTemplate<JsExtV8> _extTable;
         HashTableStringTemplate<v8::Persistent<v8::Object> > _requireTable;

         v8::Persistent<v8::Context> _context;
         v8::Persistent<v8::Object> _root;
         v8::Persistent<v8::FunctionTemplate> _requireFuncTemplate;
         v8::Persistent<v8::Function> _requireFunc;

         StackTraceStruct *_stHead;
         StackTraceStruct *_stTail;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
