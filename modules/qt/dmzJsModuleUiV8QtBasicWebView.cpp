#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebPage>


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_find_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         if (Args.Length () && Args[0]->IsString ()) {

            result = v8::Boolean::New (view->findText (v8_to_qstring (Args[0])));
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         if (Args.Length ()) {

            QUrl url;
            if (Args[0]->IsString ()) { url.setUrl (v8_to_qstring (Args[0])); }
            else {

               QString host, path, queryItem;
               V8Object obj = v8_to_object (Args[0]);
               host = v8_to_qstring (obj->Get (v8::String::NewSymbol ("host")));
               path = v8_to_qstring (obj->Get (v8::String::NewSymbol ("path")));
               url.setHost (host);
               url.setPath (path);
               V8Object queryObj = v8_to_object (obj->Get (v8::String::NewSymbol ("queryItems")));
               url.addQueryItem ("SERVICE", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SERVICE"))));
               url.addQueryItem ("REQUEST", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("REQUEST"))));
               url.addQueryItem ("VERSION", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("VERSION"))));
               url.addQueryItem ("SRS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SRS"))));
               url.addQueryItem ("LAYERS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("LAYERS"))));
               url.addQueryItem ("FORMAT", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("FORMAT"))));
               url.addQueryItem ("STYLES", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("STYLES"))));
            }
            if (!url.isEmpty () && url.isValid ()) { view->load (url); }
         }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_modified (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) { result = v8::Boolean::New (view->isModified ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_selected_text (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) { result = qstring_to_v8 (view->selectedText ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_text_size_mult (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         if (Args.Length ()) {

            qreal factor = v8_to_number (Args[0]);
            view->setTextSizeMultiplier (factor);
         }

         result = v8::Number::New (view->textSizeMultiplier ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_url (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         QUrl url;
         if (Args.Length ()) {

            if (Args[0]->IsString ()) { url.setUrl (v8_to_qstring (Args[0])); }
            else {

               QString host, path, queryItem;
               V8Object obj = v8_to_object (Args[0]);
               host = v8_to_qstring (obj->Get (v8::String::NewSymbol ("host")));
               path = v8_to_qstring (obj->Get (v8::String::NewSymbol ("path")));
               url.setHost (host);
               url.setPath (path);
               V8Object queryObj = v8_to_object (obj->Get (v8::String::NewSymbol ("queryItems")));
               url.addQueryItem ("SERVICE", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SERVICE"))));
               url.addQueryItem ("REQUEST", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("REQUEST"))));
               url.addQueryItem ("VERSION", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("VERSION"))));
               url.addQueryItem ("SRS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SRS"))));
               url.addQueryItem ("LAYERS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("LAYERS"))));
               url.addQueryItem ("FORMAT", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("FORMAT"))));
               url.addQueryItem ("STYLES", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("STYLES"))));
            }
            if (!url.isEmpty () && url.isValid ()) { view->setUrl (url); }
         }

         url = view->url ();
         V8Object urlObject = v8::Object::New ();
         urlObject->Set (v8::String::NewSymbol ("host"), qstring_to_v8 (url.host ()));
         urlObject->Set (v8::String::NewSymbol ("path"), qstring_to_v8 (url.path ()));

         QList<QPair<QString, QString> > list = url.queryItems ();
         int length = list.count ();
         V8Array array = v8::Array::New ();
         for (int idx = 0; idx < length; ++idx) {

            QPair<QString, QString> query = list.at (idx);
            array->Set (qstring_to_v8 (query.first), qstring_to_v8 (query.second));
         }

         urlObject->Set (v8::String::NewSymbol ("queryItems"), array);
         result = urlObject;
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_zoom_factor (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         if (Args.Length ()) {

            qreal factor = v8_to_number (Args[0]);
            view->setZoomFactor (v8_to_number (Args[0]));
         }

         result = v8::Number::New (view->zoomFactor ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_page (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) {

         if (Args.Length ()) {

            QWebPage *page = self->v8_to_qobject<QWebPage>(Args.This ());
            if (page) { view->setPage (page); }
         }

         result = self->create_v8_qobject(view->page ());
      }
   }

   return scope.Close (result);
}



dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webview_reload (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebView *view = self->v8_to_qobject<QWebView>(Args.This ());
      if (view) { view->reload (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_webview (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent (0);
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      QWebView *view = new QWebView (parent);
      view->settings ()->setAttribute (QWebSettings::JavascriptEnabled, true);
      view->settings ()->setAttribute (QWebSettings::JavascriptCanAccessClipboard, true);
      view->settings ()->setAttribute (QWebSettings::JavascriptCanOpenWindows, true);
      view->settings ()->setAttribute (QWebSettings::PluginsEnabled, true);
      view->settings ()->setAttribute (QWebSettings::LocalContentCanAccessRemoteUrls, true);
      result = self->create_v8_qobject (view);
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_webview () {

   v8::HandleScope scope;

   _webviewTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _webviewTemp->Inherit (_widgetTemp);

   V8ObjectTemplate instance = _webviewTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _webviewTemp->PrototypeTemplate ();
   proto->Set ("findText", v8::FunctionTemplate::New (_webview_find_text, _self));
   proto->Set ("isModified", v8::FunctionTemplate::New (_webview_modified, _self));
   proto->Set ("load", v8::FunctionTemplate::New (_webview_load, _self));
   proto->Set ("selectedText", v8::FunctionTemplate::New (_webview_selected_text, _self));
   proto->Set ("textSizeMultiplier", v8::FunctionTemplate::New (_webview_text_size_mult, _self));
   proto->Set ("zoomFactor", v8::FunctionTemplate::New (_webview_zoom_factor, _self));
   proto->Set ("url", v8::FunctionTemplate::New (_webview_url, _self));
   proto->Set ("load", v8::FunctionTemplate::New (_webview_reload, _self));
   proto->Set ("page", v8::FunctionTemplate::New (_webview_page, _self));

   _webviewApi.add_function ("create", _create_webview, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webframe_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebFrame *frame = self->v8_to_qobject<QWebFrame>(Args.This ());
      if (frame) {

         if (Args.Length ()) {

            QUrl url;
            if (Args[0]->IsString ()) { url.setUrl (v8_to_qstring (Args[0])); }
            else {

               QString host, path, queryItem;
               V8Object obj = v8_to_object (Args[0]);
               host = v8_to_qstring (obj->Get (v8::String::NewSymbol ("host")));
               path = v8_to_qstring (obj->Get (v8::String::NewSymbol ("path")));
               url.setHost (host);
               url.setPath (path);
               V8Object queryObj = v8_to_object (obj->Get (v8::String::NewSymbol ("queryItems")));
               url.addQueryItem ("SERVICE", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SERVICE"))));
               url.addQueryItem ("REQUEST", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("REQUEST"))));
               url.addQueryItem ("VERSION", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("VERSION"))));
               url.addQueryItem ("SRS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("SRS"))));
               url.addQueryItem ("LAYERS", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("LAYERS"))));
               url.addQueryItem ("FORMAT", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("FORMAT"))));
               url.addQueryItem ("STYLES", v8_to_qstring (queryObj->Get(v8::String::NewSymbol ("STYLES"))));
            }
            if (!url.isEmpty () && url.isValid ()) { frame->load (url); }
         }
      }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_webframe () {

   v8::HandleScope scope;

   _webframeTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _webframeTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _webframeTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _webframeTemp->PrototypeTemplate ();
   proto->Set ("load", v8::FunctionTemplate::New (_webframe_load, _self));
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_webpage_mainframe (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWebPage *page = self->v8_to_qobject<QWebPage>(Args.This ());
      if (page) { result = self->create_v8_qobject(page->mainFrame ()); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_webpage () {

   v8::HandleScope scope;

   _webpageTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _webpageTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _webpageTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _webpageTemp->PrototypeTemplate ();
   proto->Set ("mainFrame", v8::FunctionTemplate::New (_webpage_mainframe, _self));
}

