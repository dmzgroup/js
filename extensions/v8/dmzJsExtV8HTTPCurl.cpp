#include "dmzJsExtV8HTTPCurl.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzJsModuleV8.h>

#include <curl/curl.h>

dmz::JsExtV8HTTPCurl::Download::Download (const String &Address) :
      next (0),
      _Address (Address),
      _done (False) {

   create_thread (*this);
}


dmz::JsExtV8HTTPCurl::Download::~Download ()  {

   if (func.IsEmpty () == false) { func.Dispose (); func.Clear (); }
   if (self.IsEmpty () == false) { self.Dispose (); self.Clear (); }
}

dmz::Boolean
dmz::JsExtV8HTTPCurl::Download::done () {

   if (_done) {

      if (func.IsEmpty () == false) {

         v8::HandleScope scope;

         V8Value argv[] = {
            v8::String::New (_result ? _result.get_buffer () : ""),
            v8::String::New (_error ? _error.get_buffer () : "")
         };

         v8::TryCatch tc;

         func->Call (self, 2, argv);

         if (tc.HasCaught ()) {

            // Handle exception here.
         }
      }
   }

   return _done;
}


namespace {

static size_t
write_func (void *ptr, size_t size, size_t nmemb, void *stream) {

   dmz::String *buf = (dmz::String *)stream;

   if (buf) {

      dmz::String value ((char *)ptr, size * nmemb);
      buf->append (value);
   }

   return size * nmemb;
}

};


void
dmz::JsExtV8HTTPCurl::Download::run_thread_function () {

   CURL *curl = curl_easy_init ();

   if (curl) {

      curl_easy_setopt (curl, CURLOPT_URL, _Address.get_buffer ());
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_func);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *)&_result);
      const CURLcode Code = curl_easy_perform (curl);

      if (Code != 0) { _error = curl_easy_strerror (Code); }

      curl_easy_cleanup (curl);
      curl = 0;
   }

   _done = True;
}


dmz::JsExtV8HTTPCurl::JsExtV8HTTPCurl (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      TimeSlice (Info),
      JsExtV8 (Info),
      _log (Info),
      _core (0),
      _dlList (0) {

   curl_global_init (CURL_GLOBAL_ALL);

   _init (local);
}


dmz::JsExtV8HTTPCurl::~JsExtV8HTTPCurl () {

   curl_global_cleanup ();
}


// Plugin Interface
void
dmz::JsExtV8HTTPCurl::update_plugin_state (
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
dmz::JsExtV8HTTPCurl::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// TimeSlice Interface
void
dmz::JsExtV8HTTPCurl::update_time_slice (const Float64 DeltaTime) {

   if (_dlList) {

      v8::Context::Scope cscope (_v8Context);
      v8::HandleScope scope;

      Download *prev (0);
      Download *current (_dlList);

      while (current) {

         if (current->done ()) {

            if (prev) { prev->next = current->next; }
            else { _dlList = current->next; }

            Download *tmp = current;

            current = current->next;

            delete tmp; tmp = 0;
         }
         else { current = current->next; }
      }
   }
}


// JsExtV8 Interface
void
dmz::JsExtV8HTTPCurl::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) { if (!_core) { _core = &module; } }
   else if (Mode == JsExtV8::Remove) { if (&module == _core) { _core = 0; } }
}


void
dmz::JsExtV8HTTPCurl::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _v8Context = context;
}


void
dmz::JsExtV8HTTPCurl::update_js_ext_v8_state (const StateEnum State) {

   if (State == JsExtV8::Register) {

      if (_core) {

         _core->register_interface ("dmz/components/http", _httpApi.get_new_instance ());
      }
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Shutdown) {

      Download *current (_dlList);

      while (current) {

         if (current->func.IsEmpty () == false) {

            current->func.Dispose (); current->func.Clear ();
         }

         if (current->self.IsEmpty () == false) {

            current->self.Dispose (); current->self.Clear ();
         }

         current = current->next;
      }

      _httpApi.clear ();
      _v8Context.Clear ();
   }
}


// JsExtV8HTTPCurl Interface
dmz::V8Value
dmz::JsExtV8HTTPCurl::_http_download (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8HTTPCurl *self = _to_self (Args);

   if (self && Args[0]->IsObject () && Args[2]->IsFunction ()) {

      V8Object src = V8Object::Cast (Args[0]);
      String address = v8_to_string (Args[1]);
      V8Function func = v8_to_function (Args[2]);

      self->_add_download (src, address, func);

      result = func;
  }

   return result.IsEmpty () ? result : scope.Close (result);
}


void
dmz::JsExtV8HTTPCurl::_add_download (
      const V8Object &Self,
      const String &Address,
      const V8Function &Func) {

   Download *dl = new Download (Address);

   if (dl) {

      dl->self = V8ObjectPersist::New (Self);
      dl->func = V8FunctionPersist::New (Func);

      dl->next = _dlList;
      _dlList = dl;
   }
}


void
dmz::JsExtV8HTTPCurl::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _httpApi.add_function("download", _http_download, _self);
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsExtV8HTTPCurl (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsExtV8HTTPCurl (Info, local);
}

};
