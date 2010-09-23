#ifndef DMZ_V8_QT_LINE_EDIT_DOT_H
#define DMZ_V8_QT_LINE_EDIT_DOT_H

#include <dmzV8QtObject.h>

class QLineEdit;

namespace dmz {

   class V8QtLineEdit : public V8QtObject {

      Q_OBJECT

      public:
         V8QtLineEdit (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtLineEdit ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_textChanged (const QString &);
   };
};

#endif
