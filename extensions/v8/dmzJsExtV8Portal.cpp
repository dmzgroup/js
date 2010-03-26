#include <dmzAudioModulePortal.h>
#include "dmzJsExtV8Portal.h"
#include <dmzJsModuleTypesV8.h>
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRenderModulePortal.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>

dmz::JsExtV8Portal::JsExtV8Portal (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      _log (Info),
      _types (0),
      _core (0),
      _render (0),
      _audio (0) {

   _init (local);
}


dmz::JsExtV8Portal::~JsExtV8Portal () {

}


// Plugin Interface
void
dmz::JsExtV8Portal::update_plugin_state (
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
dmz::JsExtV8Portal::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

      if (!_audio) { _audio = AudioModulePortal::cast (PluginPtr); }
      if (!_render) { _render = RenderModulePortal::cast (PluginPtr); }
      if (!_types) { _types = JsModuleTypesV8::cast (PluginPtr); }
   }
   else if (Mode == PluginDiscoverRemove) {

      if (_audio && (_audio == AudioModulePortal::cast (PluginPtr))) { _audio = 0; }
      if (_render && (_render = RenderModulePortal::cast (PluginPtr))) { _render = 0; }
      if (_types && (_types == JsModuleTypesV8::cast (PluginPtr))) { _types = 0; }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8Portal::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Portal::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Portal::update_js_ext_v8_state (const StateEnum State) {

   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/portal",
            _portalApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

      _posStr = V8StringPersist::New (v8::String::NewSymbol ("position"));
      _oriStr = V8StringPersist::New (v8::String::NewSymbol ("orientation"));
   }
   else if (State == JsExtV8::Shutdown) {

      _posStr.Dispose (); _posStr.Clear ();
      _oriStr.Dispose (); _oriStr.Clear ();
      _portalApi.clear ();
      _v8Context.Clear ();
   }
}


dmz::V8Value
dmz::JsExtV8Portal::_portal_view (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Portal *self = to_self (Args);

   if (self && self->_types) {

      const int Length = Args.Length ();

      if (Length == 0) {

         Vector vec;
         Matrix mat;

         if (self->_render) { self->_render->get_view (vec, mat); }
         else if (self->_audio) { Vector tmp; self->_audio->get_view (vec, mat, tmp); }

         V8Object obj = v8::Object::New ();
         obj->Set (self->_posStr, self->_types->to_v8_vector (vec));
         obj->Set (self->_oriStr, self->_types->to_v8_matrix (mat));
         result = obj;
      }
      else if (Length >= 2) {

         const Vector Vec = self->_types->to_dmz_vector (Args[0]);
         const Matrix Mat = self->_types->to_dmz_matrix (Args[1]);
         
         if (self->_render) { self->_render->set_view (Vec, Mat); }
         if (self->_audio) { Vector tmp; self->_audio->set_view (Vec, Mat, tmp); }

         V8Object obj = v8::Object::New ();
         obj->Set (self->_posStr, self->_types->to_v8_vector (Vec));
         obj->Set (self->_oriStr, self->_types->to_v8_matrix (Mat));
         result = obj;
      }
   }

   return scope.Close (result);
}


void
dmz::JsExtV8Portal::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _portalApi.add_function ("view", _portal_view, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Portal (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Portal (Info, local);
}

};
