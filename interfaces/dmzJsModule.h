#ifndef DMZ_JS_MODULE_DOT_H
#define DMZ_JS_MODULE_DOT_H

#include <dmzRuntimePlugin.h>
#include <dmzRuntimeRTTI.h>
#include <dmzTypesBase.h>
#include <dmzTypesString.h>

namespace dmz {

class Config;
class HandleContainer;
class StringContainer;

class JsModule {

   public:
      static JsModule *cast (
         const Plugin *PluginPtr,
         const String &PluginName = "");

      String get_js_module_name () const;
      Handle get_js_module_handle () const;

      // JsModule Interface
      virtual void lookup_script_names (StringContainer &list) = 0;
      virtual void lookup_script_file_names (StringContainer &list) = 0;
      virtual void lookup_script_handles (HandleContainer &list) = 0;

      virtual Handle compile_script (
         const String &Name,
         const String &FileName,
         const char *Script,
         const Int32 Size) = 0;

      virtual Boolean recompile_script (
         const Handle ScriptHandle,
         const char *Script,
         const Int32 Size) = 0;

      virtual Handle lookup_script (const String &Name) = 0;
      virtual String lookup_script_name (const Handle ScriptHandle) = 0;
      virtual String lookup_script_file_name (const Handle ScriptHandle) = 0;
      virtual Boolean destroy_script (const Handle ScriptHandle) = 0;

      virtual void lookup_instance_names (const Handle Script, StringContainer &list) = 0;

      virtual void lookup_instance_handles (
         const Handle Script,
         HandleContainer &list) = 0;

      virtual Handle create_instance (
         const String &Name,
         const Handle ScriptHandle,
         const Config &Init) = 0;

      virtual Handle lookup_instance (const String &InstanceName) = 0;
      virtual Handle lookup_instance_script (const Handle Instance) = 0;
      virtual String lookup_instance_name (const Handle Instance) = 0;
      virtual Boolean recreate_instance (const Handle Instance, const Config &Init) = 0;
      virtual Boolean destroy_instance (const Handle Instance) = 0;

   protected:
      JsModule (const PluginInfo &Info);
      ~JsModule ();

   private:
      JsModule ();
      JsModule (const JsModule &);
      JsModule &operator= (const JsModule &);

      const PluginInfo &__Info;
};

//! \cond
const char JsModuleInterfaceName[] = "JsModuleInterface";
//! \endcond
};


inline dmz::JsModule *
dmz::JsModule::cast (const Plugin *PluginPtr, const String &PluginName) {

   return (JsModule *)lookup_rtti_interface (
      JsModuleInterfaceName,
      PluginName,
      PluginPtr);
}


inline
dmz::JsModule::JsModule (const PluginInfo &Info) :
      __Info (Info) {

   store_rtti_interface (JsModuleInterfaceName, __Info, (void *)this);
}


inline
dmz::JsModule::~JsModule () {

   remove_rtti_interface (JsModuleInterfaceName, __Info);
}


inline dmz::String
dmz::JsModule::get_js_module_name () const { return __Info.get_name (); }


inline dmz::Handle
dmz::JsModule::get_js_module_handle () const { return __Info.get_handle (); }

#endif // DMZ_JS_MODULE_DOT_H
