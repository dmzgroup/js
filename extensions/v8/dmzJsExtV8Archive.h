#ifndef DMZ_JS_EXT_V8_ARCHIVE_DOT_H
#define DMZ_JS_EXT_V8_ARCHIVE_DOT_H

#include <dmzArchiveObserverUtil.h>
#include <dmzJsExtV8.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeDefinitions.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzTypesHashTableHandleTemplate.h>

#include <v8.h>

namespace dmz {

   class ArchiveModule;
   class JsModuleRuntimeV8;
   class JsModuleV8;

   class JsExtV8Archive :
         public Plugin,
         public JsExtV8,
         public ArchiveObserverUtil {

      public:
         JsExtV8Archive (const PluginInfo &Info, Config &local);
         ~JsExtV8Archive ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

         // ArchiveObserver Interface.
         virtual void pre_create_archive (
            const Handle ArchiveHandle,
            const Int32 Version);

         virtual void create_archive (
            const Handle ArchiveHandle,
            const Int32 Version,
            Config &local,
            Config &global);

         virtual void post_create_archive (
            const Handle ArchiveHandle,
            const Int32 Version);

         virtual void pre_process_archive (
            const Handle ArchiveHandle,
            const Int32 Version);

         virtual void process_archive (
            const Handle ArchiveHandle,
            const Int32 Version,
            Config &local,
            Config &global);

         virtual void post_process_archive (
            const Handle ArchiveHandle,
            const Int32 Version);

      protected:
         // JsExtV8Archive Interface
         struct CallbackTable;

         struct CallbackStruct;

         struct InstanceStruct {

            const Handle Instance;
            const String Name;
            CallbackStruct *list;

            InstanceStruct (
                  const Handle TheInstance,
                  const String &TheName) :
                  Instance (TheInstance),
                  Name (TheName),
                  list (0) {;}

            ~InstanceStruct () { while (list) { delete list; }  }

            void remove (CallbackStruct *cs) {

               CallbackStruct *prev (0);
               CallbackStruct *current (list);

               while (current) {

                  if (current == cs) {

                     if (!prev) { list = current->next; }
                     else { prev->next = current->next; }

                     current->next = 0;
                     current = 0;
                  }
                  else { prev = current; current = current->next; }
               }
            }
         };

         struct CallbackStruct {

            CallbackStruct *next;
            InstanceStruct &is;
            CallbackTable &table;
            V8FunctionPersist func;
            V8ObjectPersist self;

            CallbackStruct (InstanceStruct &theIs, CallbackTable &theTable) :
                  next (0),
                  is (theIs),
                  table (theTable) {;}

            ~CallbackStruct () {

               table.table.remove (is.Instance);
               is.remove (this);
               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         struct CallbackTable {

            const Handle Archive;

            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (const Handle TheArchive) : Archive (TheArchive) {;}

            ~CallbackTable () { table.clear (); }
         };

         static JsExtV8Archive *_to_self (const v8::Arguments &Args);

         static V8Value _archive_observe (
            const v8::Arguments &Args,
            JsExtV8Archive &self,
            HashTableHandleTemplate<CallbackTable> &preTable,
            HashTableHandleTemplate<CallbackTable> &table,
            HashTableHandleTemplate<CallbackTable> &postTable);

         static V8Value _archive_release (const v8::Arguments &Args);
         static V8Value _archive_create (const v8::Arguments &Args);
         static V8Value _archive_create_observe (const v8::Arguments &Args);
         static V8Value _archive_process (const v8::Arguments &Args);
         static V8Value _archive_process_observe (const v8::Arguments &Args);

         Handle _to_handle (V8Value value);

         Boolean _register_callback (
            const Handle Instance,
            const String &Name,
            const Handle Archive,
            V8Object self,
            V8Function func,
            HashTableHandleTemplate<CallbackTable> &table);

         void _do_pre_post_callback (
            const Handle ArchiveHandle,
            const Int32 Version,
            CallbackTable &ct);

         void _init (Config &local);

         Log _log;
         Definitions _defs;

         Handle _defaultArchive;

         ArchiveModule *_archive;
         JsModuleV8 *_core;
         JsModuleRuntimeV8 *_runtime;

         V8InterfaceHelper _archiveApi;

         v8::Handle<v8::Context> _v8Context;

         V8ValuePersist _self;

         HashTableHandleTemplate<InstanceStruct> _instanceTable;

         HashTableHandleTemplate<CallbackTable> _preCreateTable;
         HashTableHandleTemplate<CallbackTable> _createTable;
         HashTableHandleTemplate<CallbackTable> _postCreateTable;

         HashTableHandleTemplate<CallbackTable> _preProcessTable;
         HashTableHandleTemplate<CallbackTable> _processTable;
         HashTableHandleTemplate<CallbackTable> _postProcessTable;

      private:
         JsExtV8Archive ();
         JsExtV8Archive (const JsExtV8Archive &);
         JsExtV8Archive &operator= (const JsExtV8Archive &);

   };
};

inline dmz::JsExtV8Archive *
dmz::JsExtV8Archive::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8Archive *)v8::External::Unwrap (Args.Data ());
}

#endif // DMZ_JS_EXT_V8_ARCHIVE_DOT_H
