#ifndef DMZ_V8_QT_ACTION_DOT_H
#define DMZ_V8_QT_ACTION_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtAction :  public V8QtObject {

      Q_OBJECT

      public:
         V8QtAction (
            const V8Object &Self,
            QObject *object,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtAction ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_triggered (bool checked);
         void on_toggled (bool checked);
   };
};


#endif // DMZ_V8_QT_ACTION_DOT_H
