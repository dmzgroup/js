#ifndef DMZ_V8_QT_LCD_NUMBER_DOT_H
#define DMZ_V8_QT_LCD_NUMBER_DOT_H

#include <dmzV8QtObject.h>

class QLCDNumber;

namespace dmz {

   class V8QtLCDNumber : public V8QtObject {

      Q_OBJECT

      public:
         V8QtLCDNumber (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtLCDNumber ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
   };
};


#endif
