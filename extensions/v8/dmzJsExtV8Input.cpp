#include <dmzInputModule.h>
#include "dmzJsExtV8Input.h"
#include <dmzJsModuleV8.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzInputConsts.h>
#include <dmzInputEventController.h>
#include <dmzInputEventKey.h>
#include <dmzInputEventMasks.h>
#include <dmzInputEventMouse.h>

dmz::JsExtV8Input::JsExtV8Input (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsExtV8 (Info),
      InputObserverUtil (Info, local),
      _log (Info),
      _defs (Info),
      _defaultChannel (0),
      _core (0) {

   _init (local);
}


dmz::JsExtV8Input::~JsExtV8Input () {

}


// Plugin Interface
void
dmz::JsExtV8Input::update_plugin_state (
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
dmz::JsExtV8Input::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// Input Observer Interface
void
dmz::JsExtV8Input::update_channel_state (const Handle Channel, const Boolean State) {

   if (State) { _active.add_handle (Channel); }
   else { _active.remove_handle (Channel); }

   CallbackTable *ct = _stateTable.lookup (Channel);

   if (ct && (_v8Context.IsEmpty () == false)) {

      v8::Context::Scope cscope(_v8Context);
      v8::HandleScope scope;

      const int Argc (3);
      V8Value argv[Argc];
      argv[0] = v8::Integer::New (Channel);
      argv[1] = v8::Boolean::New (State);

      _do_callback (Argc, argv, *ct);
   }
}



void
dmz::JsExtV8Input::receive_axis_event (
      const Handle Channel,
      const InputEventAxis &Value) {

}


void
dmz::JsExtV8Input::receive_button_event (
      const Handle Channel,
      const InputEventButton &Value) {

}


void
dmz::JsExtV8Input::receive_lever_event (
      const Handle Channel,
      const InputEventSwitch &Value) {

}


void
dmz::JsExtV8Input::receive_key_event (
      const Handle Channel,
      const InputEventKey &Value) {

}


void
dmz::JsExtV8Input::receive_mouse_event (
      const Handle Channel,
      const InputEventMouse &Value) {

}


void
dmz::JsExtV8Input::receive_data_event (
      const Handle Channel,
      const Handle Source,
      const Data &Value) {

}


// JsExtV8 Interface
void
dmz::JsExtV8Input::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8Input::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8Input::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface (
            "dmz/components/input",
            _inputApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      deactivate_all_input_channels ();

      _obsTable.empty ();

      _stateTable.empty ();
      _axisTable.empty ();
      _buttonTable.empty ();
      _leverTable.empty ();
      _keyTable.empty ();
      _mouseTable.empty ();
      _dataTable.empty ();

      _inputApi.clear ();
      _v8Context.Clear ();
   }
}

// API Bindings
dmz::V8Value
dmz::JsExtV8Input::_register_callback_static (
      const v8::Arguments &Args,
      const Mask &Type) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Input *self = _to_self (Args);
   if (self) { result = self->_register_callback (Args, Type); } 
   
   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_release (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_channel (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();
   JsExtV8Input *self = _to_self (Args);
   InputModule *input = self ? self->get_input_module () : 0;

   if (self && input) {

      const int Length = Args.Length ();

      if (Length == 1) {

         const Handle Channel = self->_to_handle (Args[0]);

         if (Channel) {

            result = v8::Boolean::New (input->get_channel_state (Channel));
         }
      }
      else if (Length > 1) {

         const Handle Channel = self->_to_handle (Args[0]);
         const Boolean State = v8_to_boolean (Args[1]);

         if (Channel) {

            result = v8::Boolean::New (input->set_channel_state (Channel, State));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_channel_create (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8Input *self = _to_self (Args);
   InputModule *input = self ? self->get_input_module () : 0;

   if (self && input) {

      const Handle Channel = self->_to_handle (Args[0]);

      if (Channel && input->create_channel (Channel)) {

         result = v8::Integer::New (Channel);
      }
   }
 
   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_channel_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventChannelStateMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_axis (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_axis_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventAxisMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_button (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_button_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventButtonMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_lever (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_lever_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventSwitchMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_key (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_key_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventKeyMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_mouse (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_mouse_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventMouseMask);

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_data (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   return scope.Close (result);
}


dmz::V8Value
dmz::JsExtV8Input::_input_data_observe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = _register_callback_static (Args, InputEventDataMask);

   return scope.Close (result);
}


// JsExtV8Input Interface
dmz::V8Value
dmz::JsExtV8Input::_register_callback (const v8::Arguments &Args, const Mask &Type) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   const int Length = Args.Length ();

   V8Object src = v8_to_object (Args[0]);
   Handle channel (_defaultChannel);
   V8Function func;

   if (Length == 2) {

      func = v8_to_function (Args[1]);
   }
   else if (Length > 2) {

      channel = _to_handle (Args[1]);
      func = v8_to_function (Args[2]);
   }

   const Handle Obs = _core ? _core->get_instance_handle (src) : 0;

   ObsStruct *os = _obsTable.lookup (Obs);

   if (Obs && !os) {

      os = new ObsStruct;
      if (os && !_obsTable.store (Obs, os)) { delete os; os = 0; }
   }

   CallbackTable *ct (0);

   if (channel) {

      if (Type.contains (InputEventChannelStateMask)) {

         ct = _stateTable.lookup (channel);
      }
      else if (Type.contains (InputEventAxisMask)) {

         ct = _axisTable.lookup (channel);
      }
      else if (Type.contains (InputEventButtonMask)) {

         ct = _buttonTable.lookup (channel);
      }
      else if (Type.contains (InputEventSwitchMask)) {

         ct = _leverTable.lookup (channel);
      }
      else if (Type.contains (InputEventKeyMask)) {

         ct = _keyTable.lookup (channel);
      }
      else if (Type.contains (InputEventMouseMask)) {

         ct = _mouseTable.lookup (channel);
      }
      else if (Type.contains (InputEventDataMask)) {

         ct = _dataTable.lookup (channel);
      }

      if (!ct) {

         ct = new CallbackTable (channel, Type);

         if (ct) {

            if (Type.contains (InputEventChannelStateMask)) {

               if (!_stateTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventAxisMask)) {

               if (!_axisTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventButtonMask)) {

               if (!_buttonTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventSwitchMask)) {

               if (!_leverTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventKeyMask)) {

               if (!_keyTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventMouseMask)) {

               if (!_mouseTable.store (channel, ct)) { delete ct; ct = 0; }
            }
            else if (Type.contains (InputEventDataMask)) {

               if (!_dataTable.store (channel, ct)) { delete ct; ct = 0; }
            }
         }
      }

      if (ct && os && (src.IsEmpty () == false) && (func.IsEmpty () == false)) {

         CallbackStruct *cb = new CallbackStruct (*ct);

         if (cb) {

            const Boolean Activate = (ct->table.get_count () == 0);

            if (ct->table.store (Obs, cb)) {

               cb->self = V8ObjectPersist::New (src);
               cb->func = V8FunctionPersist::New (func);
               result = func;

               cb->next = os->list;
               os->list = cb;

               if (Activate) { activate_input_channel (channel, Type); }
            }
            else { delete cb; cb = 0; }
         }
      }
   }

   return scope.Close (result);
}


dmz::Handle
dmz::JsExtV8Input::_to_handle (V8Value value) {

   v8::HandleScope scope;

   Handle result (0);

   if (value.IsEmpty () == false) {

      if (value->IsString ()) {

         result = _defs.create_named_handle (*(v8::String::AsciiValue (value)));
      }
      else if (value->IsNumber ()) {

         result = value->Uint32Value ();
      }
   }

   return result;
}


void
dmz::JsExtV8Input::_do_callback (const int Argc, V8Value argv[], CallbackTable &ct) {
 
   HashTableHandleIterator it;
   CallbackStruct *cb (0);
   while (ct.table.get_next (it, cb)) {

      if ((cb->self.IsEmpty () == false) && (cb->func.IsEmpty () == false)) {

         v8::TryCatch tc;

         argv[Argc - 1] = cb->self;
         cb->func->Call (cb->self, Argc, argv);

         if (tc.HasCaught ()) {

         }
      }
   }
}


void
dmz::JsExtV8Input::_init (Config &local) {

   v8::HandleScope scope;

   _defaultChannel = _defs.create_named_handle (InputChannelDefaultName);

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // Bind API
   _inputApi.add_function ("channel", _input_channel, _self);
   _inputApi.add_function ("channel.create", _input_channel_create, _self);
   _inputApi.add_function ("channel.observe", _input_channel_observe, _self);
   _inputApi.add_function ("axis", _input_axis, _self);
   _inputApi.add_function ("axis.observe", _input_axis_observe, _self);
   _inputApi.add_function ("button", _input_button, _self);
   _inputApi.add_function ("button.observe", _input_button_observe, _self);
   _inputApi.add_function ("lever", _input_lever, _self);
   _inputApi.add_function ("lever.observe", _input_lever_observe, _self);
   _inputApi.add_function ("key", _input_key, _self);
   _inputApi.add_function ("key.observe", _input_key_observe, _self);
   _inputApi.add_function ("mouse", _input_mouse, _self);
   _inputApi.add_function ("mouse.observe", _input_mouse_observe, _self);
   _inputApi.add_function ("data", _input_data, _self);
   _inputApi.add_function ("data.observe", _input_data_observe, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8Input (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8Input (Info, local);
}

};
