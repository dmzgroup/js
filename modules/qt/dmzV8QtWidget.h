#ifndef DMZ_V8_QT_WIDGET_DOT_H
#define DMZ_V8_QT_WIDGET_DOT_H

#include <dmzV8QtObject.h>

class QWidget;


namespace dmz {

   class V8QtWidget : public V8QtObject {

      Q_OBJECT

      public:
         V8QtWidget (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtWidget ();

         QWidget *get_qwidget () const;

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

         virtual void clean_up ();

      protected:
         void _make_weak (const Boolean Value = True);

         QPointer<QWidget> _widget;
   };
};


#endif // DMZ_V8_QT_WIDGET_DOT_H
