#ifndef DMZ_V8_QT_SLIDER_DOT_H
#define DMZ_V8_QT_SLIDER_DOT_H

#include <dmzV8QtObject.h>

class QComboBox;

namespace dmz {

   class V8QtSlider : public V8QtObject {

      Q_OBJECT

      public:
         V8QtSlider (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtSlider ();

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
