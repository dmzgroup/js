#ifndef DMZ_V8_QT_Dial_DOT_H
#define DMZ_V8_QT_Dial_DOT_H

#include <dmzV8QtObject.h>

class QComboBox;

namespace dmz {

   class V8QtDial : public V8QtObject {

      Q_OBJECT

      public:
         V8QtDial (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtDial ();

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

      public Q_SLOTS:
         void on_sliderPressed ();
         void on_sliderReleased ();
         void on_valueChanged (int);
   };
};


#endif
