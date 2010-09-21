#ifndef DMZ_V8_QT_COMBO_BOX_DOT_H
#define DMZ_V8_QT_COMBO_BOX_DOT_H

#include <dmzV8QtObject.h>

class QComboBox;

namespace dmz {

   class V8QtComboBox : public V8QtObject {

      Q_OBJECT

      public:
         V8QtComboBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtComboBox ();

         virtual Boolean bind (QWidget *sender, const String &Signal);

      public Q_SLOTS:
         void on_currentIndexChanged (int value);
   };
};


#endif
