#include "dmzJsModuleV8Basic.h"
#include <dmzJsV8Util.h>
#include <dmzRuntimeConfig.h>
#include <dmzRuntimeConfigToTypesBase.h>
#include <dmzRuntimeConfigToPathContainer.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

namespace {

static const char LocalDMZName[] = "DMZ";

const char *
to_c_string (const v8::String::Utf8Value& value) {

   return *value ? *value : "<string conversion failed>";
}


v8::Handle<v8::Value>
local_print (const v8::Arguments& args) {

   v8::HandleScope scope;

   dmz::StreamLog *out = (dmz::StreamLog *)v8::External::Unwrap (args.Data ());

   if (out) {

      const int Length = args.Length ();

      for (int ix = 0; ix < Length; ix++) {

         if (ix > 0) { *out << " "; }

         v8::String::Utf8Value str (args[ix]);
         const char *cstr = to_c_string (str);
         *out << cstr;
      }

      *out << dmz::endl;
   }

   return v8::Undefined();
}

};


dmz::JsModuleV8Basic::JsModuleV8Basic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleV8 (Info),
      _log (Info),
      _out ("", LogLevelOut, Info.get_context ()),
      _rc (Info) {

   _init (local);
}


dmz::JsModuleV8Basic::~JsModuleV8Basic () {

   _context.Dispose ();
}


// Plugin Interface
void
dmz::JsModuleV8Basic::update_plugin_state (
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
dmz::JsModuleV8Basic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


void
dmz::JsModuleV8Basic::_init (Config &local) {

   _context = v8::Context::New ();

   v8::Context::Scope scope (_context);

   v8::HandleScope hscope;

   v8::Handle<v8::Object> global = _context->Global ();

   if (!global.IsEmpty ()) {

      global->Set (v8::String::New (LocalDMZName), v8::Object::New ());

      global->Set (
         v8::String::New ("print"),
         v8::FunctionTemplate::New (
            local_print,
            v8::External::Wrap (&_out))->GetFunction ());
   }
   else { _log.error << "No global object." << endl; }

   _localPaths = config_to_path_container (local);

   Config scriptList;

   if (local.lookup_all_config ("script", scriptList)) {

      ConfigIterator it;
      Config script;

      while (scriptList.get_next_config (it, script)) {

         const String Name = config_to_string ("name", script);

         if (Name) {

            String scriptPath = _rc.find_file (Name);

            if (!scriptPath && (_localPaths.get_count () > 0)) {

               find_file (_localPaths, Name + ".js", scriptPath);
            }

            if (scriptPath) {

               V8ScriptBuffer *sptr = new V8ScriptBuffer (scriptPath);

               v8::Local<v8::Script> jsscript = v8::Script::Compile (v8::String::NewExternal (sptr));

               jsscript->Run ();
            }
            else {

               _log.error << "Failed to find script resource: " << Name << endl;
            }
         }
      }
   }
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleV8Basic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleV8Basic (Info, local);
}

};
