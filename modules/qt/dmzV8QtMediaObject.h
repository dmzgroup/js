#ifndef DMZ_V8_QT_MEDIA_OBJECT_DOT_H
#define DMZ_V8_QT_MEDIA_OBJECT_DOT_H

#include <dmzV8QtObject.h>

namespace dmz {

   class V8QtMediaObject : public V8QtObject {

      Q_OBJECT

      public:
         V8QtMediaObject (
            const V8Object &Self,
            QObject *object,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtMediaObject ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_finished ();
         void on_tick ();
         void on_has_video_changed (bool b);
   };
};

#endif
