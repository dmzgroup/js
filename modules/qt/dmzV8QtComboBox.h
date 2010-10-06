#ifndef DMZ_V8_QT_COMBO_BOX_DOT_H
#define DMZ_V8_QT_COMBO_BOX_DOT_H

#include <dmzV8QtWidget.h>

class QComboBox;

namespace dmz {

   class V8QtComboBox : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtComboBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtComboBox ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_currentIndexChanged (int value);
   };
};


#endif
