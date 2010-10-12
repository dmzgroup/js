#ifndef DMZ_V8_QT_OBJECT_DOT_H
#define DMZ_V8_QT_OBJECT_DOT_H

#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsV8UtilTypes.h>
#include <dmzTypes.h>
#include <dmzTypesDeleteListTemplate.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesHashTableStringTemplate.h>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <v8.h>


namespace dmz {

   class V8QtObject : public QObject {

      Q_OBJECT

      public:
         V8QtObject (
            const V8Object &Self,
            QObject *object,
            JsModuleUiV8QtBasicState *state);

         virtual ~V8QtObject ();

         QObject *get_qobject () const;

         virtual Boolean bind (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

         void release_callback (const Handle Observer);
         V8Value find_callback (const V8Object &Self, const String &Signal);

         V8ObjectPersist self;

      protected:
         struct CallbackStruct;

         struct CallbackTable {

            const String &Signal;
            HashTableHandleTemplate<CallbackStruct> table;

            CallbackTable (const String &TheSignal) : Signal (TheSignal) {;}
            ~CallbackTable () { table.empty (); }
         };

         struct CallbackStruct {

            const Handle Observer;
            V8ObjectPersist self;
            V8FunctionPersist func;

            CallbackStruct (const Handle TheObserver) : Observer (TheObserver) {;}

            ~CallbackStruct () {

               func.Dispose (); func.Clear ();
               self.Dispose (); self.Clear ();
            }
         };

         void _register_callback (
            const String &Signal,
            const V8Object &Self,
            const V8Function &Func);

         CallbackStruct *_get_first_callback (const String &Signal);
         CallbackStruct *_get_next_callback ();
         void _handle_exception (const Handle Source, v8::TryCatch &tc);

         void _do_callback (const String &Signal);
         void _do_callback (const String &Signal, const QVariant &Value);
         void _do_callback (const String &Signal, const QList<V8Value> &ValueList);

         V8Value _to_v8_value (const QVariant &Value);


         QPointer<QObject> _object;
         JsModuleUiV8QtBasicState *_state;
         HashTableStringTemplate<CallbackTable> _cbTable;
         HashTableHandleIterator _it;
         CallbackTable *_current;
   };
};


#endif // DMZ_V8_QT_OBJECT_DOT_H
