#include "dmzJsExtV8HTTPCurl.h"
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzJsModuleV8.h>

#include <curl/curl.h>
#include <string.h>

#include <qdb.h>
static dmz::qdb out;


size_t
dmz::JsExtV8HTTPCurl::Download::write_func (
      void *ptr,
      size_t size,
      size_t nmemb,
      void *stream) {

   dmz::String *buf = (dmz::String *)stream;

   if (buf) {

      dmz::String value ((char *)ptr, size * nmemb);
      buf->append (value);
   }

   return size * nmemb;
}


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


size_t
dmz::JsExtV8HTTPCurl::Upload::write_func (
      void *ptr,
      size_t size,
      size_t nmemb,
      void *stream) {

   dmz::String *buf = (dmz::String *)stream;

   if (buf) {

      dmz::String value ((char *)ptr, size * nmemb);
      buf->append (value);
   }

   return size * nmemb;
}


size_t
dmz::JsExtV8HTTPCurl::Upload::header_func (
      void *ptr,
      size_t size,
      size_t nmemb,
      void *stream) {

   const size_t Result (size * nmemb);

   String value ((char *)ptr, (Int32)Result);

   (out << value).flush ();

   return Result;
}


size_t
dmz::JsExtV8HTTPCurl::Upload::read_func (
      void *ptr,
      size_t size,
      size_t nmemb,
      void *stream) {

   size_t result (0);

   const size_t OutSize = 6; size * nmemb;

   Upload *ul = (Upload *)stream;

   if (ul) {

      const Int32 Remaining = ul->_Value.get_length () - ul->_place;

      result = Remaining > OutSize ? OutSize : Remaining;

      if (result > 0) {

         memcpy (ptr, (void *)&(ul->_Value.get_buffer ()[ul->_place]), result);

         ul->_place += result;
      }
   }

   return result;
}


dmz::JsExtV8HTTPCurl::Upload::Upload (const String &Address, const String &Value) :
      next (0),
      _Address (Address),
      _Value (Value),
      _place (0),
      _done (False) {

   create_thread (*this);
}


dmz::JsExtV8HTTPCurl::Upload::~Upload ()  {

   if (func.IsEmpty () == false) { func.Dispose (); func.Clear (); }
   if (self.IsEmpty () == false) { self.Dispose (); self.Clear (); }
}


dmz::Boolean
dmz::JsExtV8HTTPCurl::Upload::done () {

   if (_done) {

      if (func.IsEmpty () == false) {

         v8::HandleScope scope;

         if (!_error && (_place < (_Value.get_length () - 1))) {

            _error = "Failed writing data.";
         }

         V8Value argv[] = {
            (_result ? v8::String::New (_result.get_buffer ()) : v8::Undefined ()),
            (_Address ? v8::String::New (_Address.get_buffer ()) : v8::Undefined ()),
            (_error ? v8::String::New (_error.get_buffer ()) : v8::Undefined ())
         };

         v8::TryCatch tc;

         func->Call (self, 3, argv);

         if (tc.HasCaught ()) {

            // Handle exception here.
         }
      }
   }

   return _done;
}


void
dmz::JsExtV8HTTPCurl::Upload::run_thread_function () {

   CURL *curl = curl_easy_init ();

   if (curl) {

      curl_easy_setopt (curl, CURLOPT_URL, _Address.get_buffer ());
      curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, write_func);
      curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *)&_result);
      // curl_easy_setopt (curl, CURLOPT_HEADERFUNCTION, header_func);
      curl_easy_setopt (curl, CURLOPT_READFUNCTION, read_func);
      curl_easy_setopt (curl, CURLOPT_READDATA, (void *)this);
      curl_easy_setopt (curl, CURLOPT_UPLOAD, 1L);
      curl_easy_setopt (curl, CURLOPT_INFILESIZE, _Value.get_length ());

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
      _dlList (0),
      _ulList (0) {

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

   v8::Context::Scope cscope (_v8Context);
   v8::HandleScope scope;

   if (_dlList) {

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

   if (_ulList) {

      Upload *prev (0);
      Upload *current (_ulList);

      while (current) {

         if (current->done ()) {

            if (prev) { prev->next = current->next; }
            else { _ulList = current->next; }

            Upload *tmp = current;

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
   else if (State == JsExtV8::Stop) {

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
   }
   else if (State == JsExtV8::Shutdown) {

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


dmz::V8Value
dmz::JsExtV8HTTPCurl::_http_upload (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsExtV8HTTPCurl *self = _to_self (Args);

   if (self && Args[0]->IsObject ()) {

      V8Object src = V8Object::Cast (Args[0]);
      String address = v8_to_string (Args[1]);
      String value = v8_to_string (Args[2]);
      V8Function func = v8_to_function (Args[3]);

      self->_add_upload (src, address, value, func);

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
dmz::JsExtV8HTTPCurl::_add_upload (
      const V8Object &Self,
      const String &Address,
      const String &Value,
      const V8Function &Func) {

   Upload *ul = new Upload (Address, Value);

   if (ul) {

      if (Self.IsEmpty () == false) { ul->self = V8ObjectPersist::New (Self); }
      if (Func.IsEmpty () == false) { ul->func = V8FunctionPersist::New (Func); }

      ul->next = _ulList;
      _ulList = ul;
   }
}


void
dmz::JsExtV8HTTPCurl::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   _httpApi.add_function("download", _http_download, _self);
   _httpApi.add_function("upload", _http_upload, _self);
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
