
#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilConvert.h>
#include "dmzV8QtUtil.h"
//#include <QtWebKit/QWebView>
//#include <QtWebKit/QWebFrame>
//#include <QtWebKit/QWebPage>
#include <QtCore/QUrl>

#include <phonon/MediaObject>
#include <phonon/VideoWidget>
#include <phonon/AudioOutput>
#include <phonon/Path>

namespace {

   QList<Phonon::AudioOutput *> _audioOutputList;
};


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_clear_paths (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QMutableListIterator<Phonon::AudioOutput *> itor (_audioOutputList);

      while (itor.hasNext ()) {

         Phonon::AudioOutput *output = itor.next ();
         delete output;
         itor.remove ();
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_phonon_media_object (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent (0);
      QUrl url;
      QString location;
      int length = Args.Length ();
      if (length) {

         if (Args[0]->IsString ()) {

            location = v8_to_qstring (Args[0]);
            if (length > 1) { parent = self->_to_qwidget (Args[1]); }
         }
         else { parent = self->_to_qwidget (Args[0]); }
      }

      Phonon::MediaObject *media = new Phonon::MediaObject (parent);
      url = QUrl (location);

      if (url.isValid () && !url.isEmpty ()) {

         media->setCurrentSource (Phonon::MediaSource (url));
      }
      else { media->setCurrentSource (Phonon::MediaSource (location)); }

      Phonon::AudioOutput *output = new Phonon::AudioOutput (Phonon::VideoCategory);
      Phonon::Path path = Phonon::createPath(media, output);
      if (path.isValid ()) { _audioOutputList.append (output); }
      result = self->create_v8_qobject (media);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_source (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) {

         if (Args.Length ()) {

            QString location = v8_to_qstring (Args[0]);
            QUrl url (location);
            if (url.isValid () && !url.isEmpty ()) {

               media->setCurrentSource (Phonon::MediaSource (url));
            }
            else { media->setCurrentSource (Phonon::MediaSource (location)); }
         }

         Phonon::MediaSource::Type type = media->currentSource ().type ();
         QString str;
         switch (type) {

            case Phonon::MediaSource::LocalFile:
               str = media->currentSource ().fileName ();
               break;
            case Phonon::MediaSource::Url:
               str = media->currentSource ().url ().toString ();
               break;
//            case Phonon::MediaSource::Invalid: break;
//            case Phonon::MediaSource::Empty: break;
            default: break;
         }

         if (!str.isEmpty ()) { result = v8::String::New (qPrintable(str)); }
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_has_video (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { result = v8::Boolean::New (media->hasVideo ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_curr_time (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { result = v8::Number::New (media->currentTime ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_rem_time (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { result = v8::Number::New (media->remainingTime ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_total_time (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { result = v8::Number::New (media->totalTime ()); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_tick_interval (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) {

         if (Args.Length ()) { media->setTickInterval (v8_to_uint32 (Args[0])); }
         result = v8::Integer::New (media->tickInterval ());
      }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_pause (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { media->pause (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_play (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { media->play (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_media_object_stop (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *media = self->v8_to_qobject<Phonon::MediaObject>(Args.This ());
      if (media) { media->stop (); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_media_object () {

   v8::HandleScope scope;

   _mediaObjectTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _mediaObjectTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _mediaObjectTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _mediaObjectTemp->PrototypeTemplate ();
   proto->Set ("currentSource", v8::FunctionTemplate::New (_phonon_media_object_source, _self));
   proto->Set ("hasVideo", v8::FunctionTemplate::New (_phonon_media_object_has_video, _self));
//   proto->Set ("errorString", v8::FunctionTemplate::New (_phonon_media_object_err_str, _self));
//   proto->Set ("errorType", v8::FunctionTemplate::New (_phonon_media_object_err_type, _self));
   proto->Set ("currentTime", v8::FunctionTemplate::New (_phonon_media_object_curr_time, _self));
   proto->Set ("remainingTime", v8::FunctionTemplate::New (_phonon_media_object_rem_time, _self));
   proto->Set ("totalTime", v8::FunctionTemplate::New (_phonon_media_object_total_time, _self));
   proto->Set ("tickInterval", v8::FunctionTemplate::New (_phonon_media_object_tick_interval, _self));
   proto->Set ("pause", v8::FunctionTemplate::New (_phonon_media_object_pause, _self));
   proto->Set ("play", v8::FunctionTemplate::New (_phonon_media_object_play, _self));
   proto->Set ("stop", v8::FunctionTemplate::New (_phonon_media_object_stop, _self));

   _phononApi.add_function ("createMediaObject", _create_phonon_media_object, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_create_phonon_video_widget (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      QWidget *parent (0);
      if (Args.Length ()) { parent = self->_to_qwidget (Args[0]); }
      Phonon::VideoWidget *video = new Phonon::VideoWidget(parent);
      result = self->create_v8_qobject (video);
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_video_enter_fullscreen (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::VideoWidget *video = self->v8_to_qobject<Phonon::VideoWidget>(Args.This ());
      if (video) { video->enterFullScreen (); }
   }

   return scope.Close (result);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_video_exit_fullscreen (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::VideoWidget *video = self->v8_to_qobject<Phonon::VideoWidget>(Args.This ());
      if (video) { video->exitFullScreen (); }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_video_player () {

   v8::HandleScope scope;

   _videoWidgetTemp = V8FunctionTemplatePersist::New (v8::FunctionTemplate::New ());
   _videoWidgetTemp->Inherit (_objectTemp);

   V8ObjectTemplate instance = _videoWidgetTemp->InstanceTemplate ();
   instance->SetInternalFieldCount (1);

   V8ObjectTemplate proto = _videoWidgetTemp->PrototypeTemplate ();
   proto->Set ("enterFullScreen", v8::FunctionTemplate::New (_phonon_video_enter_fullscreen, _self));
   proto->Set ("exitFullScreen", v8::FunctionTemplate::New (_phonon_video_exit_fullscreen, _self));

   _phononApi.add_function ("createVideoPlayer", _create_phonon_video_widget, _self);
}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_phonon_create_path (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   if (self) {

      Phonon::MediaObject *source (0);
      Phonon::VideoWidget *sink (0);
      if (Args.Length () >= 2) {

         source = self->v8_to_qobject<Phonon::MediaObject> (Args[0]);
         sink = self->v8_to_qobject<Phonon::VideoWidget> (Args[1]);
      }
      if (source && sink) {

         Phonon::Path path = Phonon::createPath (source, sink);
      }
      else { self->_log.warn << "Create path failed: " << source << " " << sink << endl; }
   }

   return scope.Close (result);
}


void
dmz::JsModuleUiV8QtBasic::_init_phonon () {

   v8::HandleScope scope;

   _phononApi.add_function ("createPath", _phonon_create_path, _self);
   _phononApi.add_function ("clearPaths", _phonon_clear_paths, _self);
}
