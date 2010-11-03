#ifndef DMZ_V8_QT_TOOL_BOX_DOT_H
#define DMZ_V8_QT_TOOL_BOX_DOT_H

#include <dmzV8QtWidget.h>

class QToolBox;

namespace dmz {

   class V8QtToolBox : public V8QtWidget {

      Q_OBJECT

      public:
         V8QtToolBox (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtToolBox ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_currentChanged (int value);
   };
};


#endif
