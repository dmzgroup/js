#ifndef DMZ_V8_QT_GROUP_BOX_DOT_H
#define DMZ_V8_QT_GROUP_BOX_DOT_H

#include <dmzV8QtWidget.h>


namespace dmz {

   class V8QtGroupBox :  public V8QtWidget {

      Q_OBJECT

      public:
         V8QtGroupBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtGroupBox ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_clicked ();
         void on_toggle (bool checked);
   };
};


#endif // DMZ_V8_QT_GROUP_BOX_DOT_H
