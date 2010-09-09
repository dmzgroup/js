#ifndef DMZ_V8_QT_OBJECT_DOT_H
#define DMZ_V8_QT_OBJECT_DOT_H

#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QObject>
#include <v8.h>

class QWidget;

namespace dmz {
   
   struct V8QtObjectCallbackStruct {

      const Handle Observer;
      V8ObjectPersist self;
      V8FunctionPersist func;
   
      V8QtObjectCallbackStruct (const Handle TheObserver) : Observer (TheObserver) {;}

      ~V8QtObjectCallbackStruct () {
         
         func.Dispose (); func.Clear ();
         self.Dispose (); self.Clear ();
      }
   };
   
   
   class V8QtObject : public QObject {
   
      Q_OBJECT
      
      public:
         V8QtObject (
            const V8Object &Self,
            QWidget *widget,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtObject ();
         
         QWidget *get_qt_widget () const;
         
         virtual Boolean bind (QWidget *sender, const String &Signal);
         
         void register_callback (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);
         
         void release_callback (const Handle Observer);
         
         V8ObjectPersist self;
         
      protected:
         typedef V8QtObjectCallbackStruct CallbackStruct;
         
         struct CallbackTable {
           
            const String &Signal;
            HashTableHandleTemplate<CallbackStruct> table;
            
            CallbackTable (const String &TheSignal) : Signal (TheSignal) {;}
            ~CallbackTable () { table.empty (); }
         };
         
         QWidget *_widget;
         JsModuleUiV8QtBasicState *_state;
         HashTableStringTemplate<CallbackTable> _cbTable;
   };
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
