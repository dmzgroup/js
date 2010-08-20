#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <v8.h>


namespace dmz {

   class V8QtObject {
   
      public:
         V8QtObject (QWidget *widget);
         virtual ~V8QtObject ();
         
         QWidget *get_qt_widget () const;
         
         void add_callback (const V8Object &Self, const V8Function &Func);
         
         virtual Boolean bind (QWidget *sender, const String &Signal) { return False; }
         
      protected:
         struct CallbackStruct {
         
            V8ObjectPersist self;
            V8FunctionPersist func;
         
            CallbackStruct () {;}
         
            ~CallbackStruct () {
         
               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };
         
         QWidget *_widget;
         QList<CallbackStruct *>_cbList;
   };

   class V8QtWidget : public QObject, public V8QtObject {
      
      public:
         V8QtWidget (QWidget *widget, QObject *parent = 0);
         virtual ~V8QtWidget ();
   };

   class V8QtButton : public QObject, public V8QtObject {
      
      Q_OBJECT
      
      public:
         V8QtButton (QWidget *widget, QObject *parent = 0);
         virtual ~V8QtButton ();
         
         virtual Boolean bind (QWidget *sender, const String &Signal);
         
      public Q_SLOTS:
         void on_clicked ();
   };
};


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_WIDGETS_DOT_H
