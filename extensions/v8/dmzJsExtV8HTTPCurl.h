#ifndef DMZ_JS_EXT_V8_HHTTP_CURL_DOT_H
#define DMZ_JS_EXT_V8_HHTTP_CURL_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzJsV8UtilTypes.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzSystemThread.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>

#include <v8.h>

namespace dmz {

   class JsModuleV8;

   class JsExtV8HTTPCurl :
         public Plugin,
         public TimeSlice,
         public JsExtV8 {

      public:
         JsExtV8HTTPCurl (const PluginInfo &Info, Config &local);
         ~JsExtV8HTTPCurl ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 DeltaTime);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

      protected:
         // JsExtV8HTTPCurl Interface
         class Download : protected ThreadFunction {

            public:
               static size_t write_func (
                  void *ptr,
                  size_t size,
                  size_t nmemb,
                  void *stream);

               Download (const String &Address);
               virtual ~Download ();

               Boolean done ();

               Download *next;
               V8FunctionPersist func;
               V8ObjectPersist self;

            protected:
               virtual void run_thread_function ();

               const String _Address;
               String _error;
               Boolean _done;
               String _result;
         };

         class Upload : protected ThreadFunction {

            public:
               static size_t write_func (
                  void *ptr,
                  size_t size,
                  size_t nmemb,
                  void *stream);

               static size_t header_func (
                  void *ptr,
                  size_t size,
                  size_t nmemb,
                  void *stream);

               static size_t read_func (
                  void *ptr,
                  size_t size,
                  size_t nmemb,
                  void *stream);

               Upload (const String &Address, const String &Value);
               virtual ~Upload ();

               Boolean done ();

               Upload *next;
               V8FunctionPersist func;
               V8ObjectPersist self;

            protected:
               virtual void run_thread_function ();

               const String _Address;
               const String _Value;
               Int32 _place;
               String _result;
               String _error;
               Boolean _done;
         };
 
         static JsExtV8HTTPCurl *_to_self (const v8::Arguments &Args);
         static V8Value _http_download (const v8::Arguments &Args);
         static V8Value _http_upload (const v8::Arguments &Args);

         void _add_download (
            const V8Object &Self,
            const String &Address,
            const V8Function &Func);

         void _add_upload (
            const V8Object &Self,
            const String &Address,
            const String &Value,
            const V8Function &Func);

         void _init (Config &local);

         Log _log;

         V8InterfaceHelper _httpApi;

         v8::Handle<v8::Context> _v8Context;
         V8ValuePersist _self;

         JsModuleV8 *_core;

         Download *_dlList;
         Upload *_ulList;

      private:
         JsExtV8HTTPCurl ();
         JsExtV8HTTPCurl (const JsExtV8HTTPCurl &);
         JsExtV8HTTPCurl &operator= (const JsExtV8HTTPCurl &);

   };
};


inline dmz::JsExtV8HTTPCurl *
dmz::JsExtV8HTTPCurl::_to_self (const v8::Arguments &Args) {

   return (dmz::JsExtV8HTTPCurl *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_EXT_V8_HHTTP_CURL_DOT_H
