#ifndef DMZ_JS_MODULE_V8_BASIC_DOT_H
#define DMZ_JS_MODULE_V8_BASIC_DOT_H

#include <dmzJsModuleV8.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeResources.h>
#include <dmzSystemFile.h>

namespace dmz {

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

         void _init_context ();
         void _handle_exception (v8::TryCatch &tc);
         void _load_kernel ();
         void _load_scripts ();
         void _run_scripts (ScriptStruct *list);
         void _init (Config &local);

         Log _log;
         StreamLog _out;
         Resources _rc;

         PathContainer _localPaths;

         v8::Persistent<v8::Context> _context;

         ScriptStruct *_kernelList;
         ScriptStruct *_scriptList;

      private:
         JsModuleV8Basic ();
         JsModuleV8Basic (const JsModuleV8Basic &);
         JsModuleV8Basic &operator= (const JsModuleV8Basic &);

   };
};

#endif // DMZ_JS_MODULE_V8_BASIC_DOT_H
