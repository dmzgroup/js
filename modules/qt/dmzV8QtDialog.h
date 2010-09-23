#ifndef DMZ_V8_QT_DIALOG_DOT_H
#define DMZ_V8_QT_DIALOG_DOT_H

#include <dmzV8QtObject.h>


namespace dmz {

   class V8QtDialog :  public V8QtObject {

      Q_OBJECT

      public:
         V8QtDialog (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtDialog ();

         virtual Boolean bind (QWidget *sender, const String &Signal);

         virtual void open  (const V8Object &Self, const V8Function &Func);

      public Q_SLOTS:
         void on_finished (int);
   };
};


#endif // DMZ_V8_QT_DIALOG_DOT_H
