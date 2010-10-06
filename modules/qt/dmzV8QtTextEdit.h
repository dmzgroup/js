#ifndef DMZ_V8_QT_TEXT_EDIT_DOT_H
#define DMZ_V8_QT_TEXT_EDIT_DOT_H

#include <dmzV8QtWidget.h>

class QTextEdit;

namespace dmz {

   class V8QtTextEdit : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtTextEdit (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtTextEdit ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_textChanged ();
   };
};

#endif

