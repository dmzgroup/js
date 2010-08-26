#ifndef DMZ_V8_QT_OBJECT_DOT_H
#define DMZ_V8_QT_OBJECT_DOT_H

#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <v8.h>

class QWidget;


namespace dmz {

   class V8QtObject : public QObject {
   
      Q_OBJECT
      
      public:
         V8QtObject (QWidget *widget, QObject *parent);
         virtual ~V8QtObject ();
         
         QWidget *get_qt_widget () const;
         
         void add_callback (const V8Object &Self, const V8Function &Func);
         
         virtual Boolean bind (QWidget *sender, const String &Signal);
         
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
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
