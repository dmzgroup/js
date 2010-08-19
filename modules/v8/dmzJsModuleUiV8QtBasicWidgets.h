#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <QtCore/QObject>
#include <v8.h>


namespace dmz {

   class V8QtObserver : public QObject {
      
      Q_OBJECT
      
      public:
         V8QtObserver (
            const V8Object &Self,
            const V8Function &Func,
            QWidget *widget,
            QObject *parent = 0);
            
         ~V8QtObserver ();
         
      public Q_SLOTS:
         void observe ();
         
      protected:
         QWidget *_widget;
         V8ObjectPersist _self;
         V8FunctionPersist _func;
   };

   class V8QtBaseWidget {
   
      public:
         V8QtBaseWidget (QWidget *widget);
         ~V8QtBaseWidget ();
         
      protected:
         QWidget *_widget;
   };
   
   class V8QtButton : public QObject, public V8QtBaseWidget {
      
      Q_OBJECT
      
      public:
         V8QtButton (QWidget *widget, QObject *parent = 0);
         ~V8QtButton ();
         
      public Q_SLOTS:
         void slot_clicked ();
   };
};


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H
