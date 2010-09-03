#ifndef DMZ_JS_OBSERVER_DOT_H
#define DMZ_JS_OBSERVER_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

namespace dmz {

enum JsObserverModeEnum {
   JsObserverCreate,
   JsObserverRelease,
   JsObserverDestroy
};

class JsObserver {

   public:
      static JsObserver *cast (
         const Plugin *PluginPtr,
         const String &PluginName = "");

      String get_js_observer_name () const;
      Handle get_js_observer_handle () const;

      // JsObserver Interface
      virtual void update_js_script (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Script) = 0;

      virtual void update_js_instance (
         const JsObserverModeEnum Mode,
         const Handle Module,
         const Handle Instance) = 0;

      virtual void update_js_error (
         const Handle Module,
         const Handle Script,
         const Handle Instance,
         const String &ErrorMessage,
         const String &StackTrace) = 0;
 
   protected:
      JsObserver (const PluginInfo &Info);
      ~JsObserver ();

   private:
      JsObserver ();
      JsObserver (const JsObserver &);
      JsObserver &operator= (const JsObserver &);

      const PluginInfo &__Info;
};

//! \cond
const char JsObserverInterfaceName[] = "JsObserverInterface";
//! \endcond
};


inline dmz::JsObserver *
dmz::JsObserver::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsObserver *)lookup_rtti_interface (
      JsObserverInterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsObserver::JsObserver (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsObserverInterfaceName, __Info, (void *)this);
}


inline
dmz::JsObserver::~JsObserver () {

   remove_rtti_interface (JsObserverInterfaceName, __Info);
}


inline dmz::String
dmz::JsObserver::get_js_observer_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsObserver::get_js_observer_handle () const { return __Info.get_handle (); }

#endif // DMZ_JS_OBSERVER_DOT_H
