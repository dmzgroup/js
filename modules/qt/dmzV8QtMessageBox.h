#ifndef DMZ_V8_QT_MESSAGE_BOX_DOT_H
#define DMZ_V8_QT_MESSAGE_BOX_DOT_H

#include <dmzV8QtDialog.h>

class QAbstractButton;

namespace dmz {

   class V8QtMessageBox :  public V8QtDialog {

      Q_OBJECT

      public:
         V8QtMessageBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtMessageBox ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_buttonClicked (QAbstractButton *button);
   };
};


#endif // DMZ_V8_QT_MESSAGE_BOX_DOT_H
