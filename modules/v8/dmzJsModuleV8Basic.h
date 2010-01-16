#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzSystemFile.h>
#include <dmzTypesHashTableHandleTemplate.h>

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

      protected:
         struct ScriptStruct {

            const String FileName;
            v8::Persistent<v8::Script> script;
            ScriptStruct *next;

            void clear () {

               if (!script.IsEmpty ()) {

                  script.Dispose ();
                  script.Clear ();
               }
            }

            ScriptStruct (const String &TheFileName) :
                  FileName (TheFileName),
                  next (0) {;}

            ~ScriptStruct () {

               clear ();

               if (next) { delete next; next = 0; }
            }
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

         void _init_context ();
         void _init_ext ();
         void _handle_exception (v8::TryCatch &tc);
         void _load_kernel ();
         void _load_scripts ();
         void _run_scripts (ScriptStruct *list);
         void _init (Config &local);

         Log _log;
         StreamLog _out;
         Resources _rc;

         StringContainer _localPaths;

         HashTableHandleTemplate<JsExtV8> _extTable;

         v8::Persistent<v8::Context> _context;
         v8::Persistent<v8::Object> _root;

         ScriptStruct *_kernelList;
         ScriptStruct *_scriptList;

         StackTraceStruct *_stHead;
         StackTraceStruct *_stTail;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
