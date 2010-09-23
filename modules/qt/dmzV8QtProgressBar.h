#ifndef DMZ_V8_QT_PROGRESS_BAR_DOT_H
#define DMZ_V8_QT_PROGRESS_BAR_DOT_H

#include <dmzV8QtObject.h>

class QProgressBar;

namespace dmz {

   class V8QtProgressBar : public V8QtObject {

      Q_OBJECT

      public:
         V8QtProgressBar (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtProgressBar ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_valueChanged (int);
   };
};


#endif
