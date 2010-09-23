#ifndef DMZ_V8_QT_BUTTON_DOT_H
#define DMZ_V8_QT_BUTTON_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtButton :  public V8QtObject {

      Q_OBJECT

      public:
         V8QtButton (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtButton ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_clicked ();
         void on_toggle (bool checked);
   };
};


#endif // DMZ_V8_QT_BUTTON_DOT_H
