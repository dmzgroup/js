#ifndef DMZ_V8_QT_OBJECT_DOT_H
#define DMZ_V8_QT_OBJECT_DOT_H

#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QObject>
#include <v8.h>

class QWidget;

namespace dmz {
   
   class V8QtObject : public QObject {
   
      Q_OBJECT
      
      public:
         V8QtObject (QWidget *widget, JsModuleUiV8QtBasic::State *state);
         virtual ~V8QtObject ();
         
         QWidget *get_qt_widget () const;
         
         virtual Boolean bind (QWidget *sender, const String &Signal);
         
         void add_callback (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);
            
      protected:
         struct CallbackStruct {
         
            CallbackStruct *next;
            V8ObjectPersist self;
            V8FunctionPersist func;
         
            CallbackStruct () : next (0) {;}
         
            ~CallbackStruct () {
         
               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };
         
         struct ObsStruct {
            
            CallbackStruct *list;
            
            ObsStruct () : list (0) {;}
            ~ObsStruct () { delete_list (list); }
         };
         
         QWidget *_widget;
         JsModuleUiV8QtBasic::State *_state;
         HashTableStringTemplate<ObsStruct> _obsTable;
   };
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
