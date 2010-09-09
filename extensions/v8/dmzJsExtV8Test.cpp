#include "dmzJsExtV8Test.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8Test::JsExtV8Test (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _testLog ("", Info.get_context ()),
      _exit (Info),
      _failed (False),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Test::~JsExtV8Test () {

}


// Plugin Interface
void
dmz::JsExtV8Test::update_plugin_state (
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
dmz::JsExtV8Test::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Test::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Test::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Test::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/test",
            _testApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      _testApi.clear ();
      _v8Context.Clear ();
   }
}


void
dmz::JsExtV8Test::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {;}

// JsExtV8Test Interface
dmz::V8Value
dmz::JsExtV8Test::_start_test (const v8::Arguments &Args) {

   v8::HandleScope scope;

   JsExtV8Test *self = to_self (Args);

   if (self) {

      const String Name = v8_to_string (Args[0]);

      self->_testLog.out
         << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl
         << "Running JavaScript " << Name << " unit test." << endl
         << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;
   }

   return v8::Undefined ();
}


dmz::V8Value
dmz::JsExtV8Test::_validate_test (const v8::Arguments &Args) {

   v8::HandleScope scope;

   JsExtV8Test *self = to_self (Args);

   if (self) {

      const String Message = v8_to_string (Args[0]);
      const Boolean Value = v8_to_boolean (Args[1]);
      if (Value) { self->_testLog.out << "PASSED: "; }
      else { self->_testLog.out << "FAILED: "; self->_failed = True; }
      self->_testLog.out << Message << endl;
   }

   return v8::Undefined ();
}


dmz::V8Value
dmz::JsExtV8Test::_stop_test (const v8::Arguments &Args) {

   v8::HandleScope scope;

   JsExtV8Test *self = to_self (Args);

   if (self) {

      const ExitStatusEnum Status = self->_failed ? ExitStatusError : ExitStatusNormal;
      self->_exit.request_exit (Status, "End of unit test.");
   }

   return v8::Undefined ();
}


void
dmz::JsExtV8Test::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _testApi.add_function ("start", _start_test, _self);
   _testApi.add_function ("validate", _validate_test, _self);
   _testApi.add_function ("stop", _stop_test, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Test (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Test (Info, local);
}

};
