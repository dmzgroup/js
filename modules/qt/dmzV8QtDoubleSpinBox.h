#ifndef DMZ_V8_QT_DOUBLE_SPIN_BOX_DOT_H
#define DMZ_V8_QT_DOUBLE_SPIN_BOX_DOT_H

#include <dmzV8QtObject.h>

class QDoubleSpinBox ;


namespace dmz {

   class V8QtDoubleSpinBox : public V8QtObject {

      Q_OBJECT

      public:
         V8QtDoubleSpinBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtDoubleSpinBox ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_valueChanged (double val);
   };
};

#endif // DMZ_V8_QT_DOUBLE_SPIN_BOX_DOT_H
