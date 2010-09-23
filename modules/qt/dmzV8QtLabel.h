#ifndef DMZ_V8_QT_Label_DOT_H
#define DMZ_V8_QT_Label_DOT_H

#include <dmzV8QtObject.h>

class QComboBox;

namespace dmz {

   class V8QtLabel : public V8QtObject {

      Q_OBJECT

      public:
         V8QtLabel (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtLabel ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
   };
};


#endif
