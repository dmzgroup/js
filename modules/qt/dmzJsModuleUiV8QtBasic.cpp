#include "dmzJsModuleUiV8QtBasic.h"
#include <dmzJsModuleV8.h>
#include <dmzJsV8UtilConvert.h>
#include <dmzRuntimePluginFactoryLinkSymbol.h>
#include <dmzRuntimePluginInfo.h>
#include <dmzTypesStringTokenizer.h>
#include "dmzV8QtTypes.h"
#include <QtCore/QFile>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include <QtUiTools/QUiLoader>

//using namespace dmz;

//namespace {

//static void
//local_v8_qt_callback_struct_delete (V8ValuePersist object, void *param) {

//   if (param) {

//      V8QtCallbackStruct *obj = (V8QtCallbackStruct *)param;
//      delete obj;
//      obj = 0;
//   }

//   object.Dispose ();
//   object.Clear ();
//}

//};


//dmz::V8QtCallbackStruct::V8QtCallbackStruct (
//      QWidget *theWidget,
//      JsModuleUiV8QtBasicState &TheState) :
//      QObject (0),
//      next (0),
//      widget (theWidget),
//      state (TheState) {;}


//dmz::V8QtCallbackStruct::~V8QtCallbackStruct () {

//   if (widget) { delete widget; widget = 0; }

//   self.Dispose (); self.Clear ();
//   callback.Dispose (); callback.Clear ();
//}


dmz::V8Value
dmz::JsModuleUiV8QtBasic::_uiloader_load (const v8::Arguments &Args) {

   v8::HandleScope scope;
   V8Value result = v8::Undefined ();

   JsModuleUiV8QtBasic *self = _to_self (Args);
   const String Name = v8_to_string (Args[0]);

   if (self && Name) {

      QUiLoader loader;
      QFile file (Name.get_buffer ());
      file.open (QFile::ReadOnly);
      QWidget *widget = loader.load (&file, 0);
      file.close ();

      result = self->create_v8_widget (widget);

      if (!result.IsEmpty ()) {

         self->_log.info << "Loaded UI: " << Name << endl;
      }
   }

   return scope.Close (result);
}


dmz::JsModuleUiV8QtBasic::JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local) :
      Plugin (Info),
      JsModuleUiV8Qt (Info),
      JsExtV8 (Info),
      _log (Info) {

   _state.ui = this;
   _init (local);
}


dmz::JsModuleUiV8QtBasic::~JsModuleUiV8QtBasic () {

}


// Plugin Interface
void
dmz::JsModuleUiV8QtBasic::update_plugin_state (
      const PluginStateEnum State,
      const UInt32 Level) {

   if (State == PluginStateInit) {

   }
   else if (State == PluginStateStart) {

   }
   else if (State == PluginStateStop) {

   }
   else if (State == PluginStateShutdown) {

      _self.Dispose (); _self.Clear ();
   }
}


void
dmz::JsModuleUiV8QtBasic::discover_plugin (
      const PluginDiscoverEnum Mode,
      const Plugin *PluginPtr) {

   if (Mode == PluginDiscoverAdd) {

   }
   else if (Mode == PluginDiscoverRemove) {

   }
}


// JsModuleUiV8Qt Interface
dmz::V8Value
dmz::JsModuleUiV8QtBasic::create_v8_widget (QWidget *value) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   V8Value result = v8::Undefined ();

   if (value) {

      V8Object vobj;
      V8QtObject *qobj = _widgetMap[value];

      if (!qobj) {

         if (value->inherits ("QTabWidget")) {

            if (!_tabCtor.IsEmpty ()) {

               vobj = _tabCtor->NewInstance ();
               qobj = new V8QtTabWidget (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QStackedWidget")) {

            if (!_stackedCtor.IsEmpty ()) {

               vobj = _stackedCtor->NewInstance ();
               qobj = new V8QtStackedWidget (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QLCDNumber")) {

            if (!_lcdNumberCtor.IsEmpty ()) {

               vobj = _lcdNumberCtor->NewInstance ();
               qobj = new V8QtLCDNumber (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QDialog")) {

            if (!_dialogCtor.IsEmpty ()) {

               vobj = _dialogCtor->NewInstance ();
               qobj = new V8QtDialog (vobj, value, &_state);

               _dialogList.append (value);
            }
         }
         else if (value->inherits ("QProgressBar")) {

            if (!_progressBarCtor.IsEmpty ()) {

               vobj = _progressBarCtor->NewInstance ();
               qobj = new V8QtProgressBar (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QTextEdit")) {

            if (!_textEditCtor.IsEmpty ()) {

               vobj = _textEditCtor->NewInstance ();
               qobj = new V8QtTextEdit (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QLineEdit")) {

            if (!_lineEditCtor.IsEmpty ()) {

               vobj = _lineEditCtor->NewInstance ();
               qobj = new V8QtLineEdit (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QComboBox")) {

            if (!_comboBoxCtor.IsEmpty ()) {

               vobj = _comboBoxCtor->NewInstance ();
               qobj = new V8QtComboBox (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QSlider")) {

            if (!_sliderCtor.IsEmpty ()) {

               vobj = _sliderCtor->NewInstance ();
               qobj = new V8QtSlider (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QDial")) {

            if (!_dialCtor.IsEmpty ()) {

               vobj = _dialCtor->NewInstance ();
               qobj = new V8QtDial (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QLabel")) {

            if (!_labelCtor.IsEmpty ()) {

               vobj = _labelCtor->NewInstance ();
               qobj = new V8QtLabel (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QAbstractButton")) {

            if (!_buttonCtor.IsEmpty ()) {

               vobj = _buttonCtor->NewInstance ();
               qobj = new V8QtButton (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QAbstractSpinBox")) {

            if (!_spinBoxCtor.IsEmpty ()) {

               vobj = _spinBoxCtor->NewInstance ();
               if (qobject_cast<QSpinBox *>(value)) {

                  qobj = new V8QtSpinBox (vobj, value, &_state);
               }
               else if (qobject_cast<QDoubleSpinBox *>(value)) {

                  qobj = new V8QtDoubleSpinBox (vobj, value, &_state);
               }
            }
         }
         else if (value->inherits ("QListWidget")) {

            if (!_listWidgetCtor.IsEmpty ()) {

               vobj = _listWidgetCtor->NewInstance ();
               qobj = new V8QtListWidget (vobj, value, &_state);
            }
         }
         else if (value->inherits ("QWidget")) {

            if (!_widgetCtor.IsEmpty ()) {

               vobj = _widgetCtor->NewInstance ();
               qobj = new V8QtWidget (vobj, value, &_state);
            }
         }

         if (qobj) { _widgetMap.insert (value, qobj); }
      }

      if (qobj && !(qobj->self.IsEmpty ())) { result = qobj->self; }
   }

   return scope.Close (result);
}


// JsExtV8 Interface
void
dmz::JsModuleUiV8QtBasic::update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module) {

   if (Mode == JsExtV8::Store) {

      if (!_state.core) { _state.core = &module; }
   }
   else if (Mode == JsExtV8::Remove) {

      if (_state.core == &module) { _state.core = 0; }
   }
}


void
dmz::JsModuleUiV8QtBasic::update_js_context_v8 (v8::Handle<v8::Context> context) {

   _state.context = context;
}


void
dmz::JsModuleUiV8QtBasic::update_js_ext_v8_state (const StateEnum State) {

   v8::Context::Scope cscope (_state.context);
   v8::HandleScope scope;

   if (State == JsExtV8::Register) {

      if (!_widgetTemp.IsEmpty ()) {

         _widgetCtor = V8FunctionPersist::New (_widgetTemp->GetFunction ());
      }

      if (!_buttonTemp.IsEmpty ()) {

         _buttonCtor = V8FunctionPersist::New (_buttonTemp->GetFunction ());
      }

      if (!_listWidgetItemTemp.IsEmpty ()) {

         _listWidgetItemCtor = V8FunctionPersist::New (_listWidgetItemTemp->GetFunction ());
      }

      if (!_listWidgetTemp.IsEmpty ()) {

         _listWidgetCtor = V8FunctionPersist::New (_listWidgetTemp->GetFunction ());
      }

      if (!_spinBoxTemp.IsEmpty ()) {

         _spinBoxCtor = V8FunctionPersist::New (_spinBoxTemp->GetFunction ());
      }

      if (!_comboBoxTemp.IsEmpty ()) {

         _comboBoxCtor = V8FunctionPersist::New (_comboBoxTemp->GetFunction ());
      }

      if (!_sliderTemp.IsEmpty ()) {

         _sliderCtor = V8FunctionPersist::New (_sliderTemp->GetFunction ());
      }

      if (!_lineEditTemp.IsEmpty ()) {

         _lineEditCtor = V8FunctionPersist::New (_lineEditTemp->GetFunction ());
      }

      if (!_textEditTemp.IsEmpty ()) {

         _textEditCtor = V8FunctionPersist::New (_textEditTemp->GetFunction ());
      }

      if (!_dialTemp.IsEmpty ()) {

         _dialCtor = V8FunctionPersist::New (_dialTemp->GetFunction ());
      }

      if (!_labelTemp.IsEmpty ()) {

         _labelCtor = V8FunctionPersist::New (_labelTemp->GetFunction ());
      }

      if (!_progressBarTemp.IsEmpty ()) {

         _progressBarCtor = V8FunctionPersist::New (_progressBarTemp->GetFunction ());
      }

      if (!_dialogTemp.IsEmpty ()) {

         _dialogCtor = V8FunctionPersist::New (_dialogTemp->GetFunction ());
      }

      if (!_lcdNumberTemp.IsEmpty ()) {

         _lcdNumberCtor = V8FunctionPersist::New (_lcdNumberTemp->GetFunction ());
      }

      if (!_stackedWidgetTemp.IsEmpty ()) {

         _stackedCtor = V8FunctionPersist::New (_stackedWidgetTemp->GetFunction ());
      }

      if (!_tabWidgetTemp.IsEmpty ()) {

         _tabCtor = V8FunctionPersist::New (_tabWidgetTemp->GetFunction ());
      }

      if (!_layoutTemp.IsEmpty ()) {
         
         _layoutCtor = V8FunctionPersist::New (_layoutTemp->GetFunction ());
      }

      if (!_boxLayoutTemp.IsEmpty ()) {
         
         _boxLayoutCtor = V8FunctionPersist::New (_boxLayoutTemp->GetFunction ());
      }

      if (!_hBoxLayoutTemp.IsEmpty ()) {
         
         _hBoxLayoutCtor = V8FunctionPersist::New (_hBoxLayoutTemp->GetFunction ());
      }

      if (!_vBoxLayoutTemp.IsEmpty ()) {
         
         _vBoxLayoutCtor = V8FunctionPersist::New (_vBoxLayoutTemp->GetFunction ());
      }

      if (_state.core) {

         _state.core->register_interface ("dmz/components/ui", _qtApi.get_new_instance ());

         _state.core->register_interface (
            "dmz/components/ui/messageBox",
            _messageBoxApi.get_new_instance ());

         _state.core->register_interface (
            "dmz/components/ui/layout",
            _layoutApi.get_new_instance ());
      }

      _mbTypeStr = V8StringPersist::New (v8::String::NewSymbol ("type"));
      _mbTextStr = V8StringPersist::New (v8::String::NewSymbol ("text"));
      _mbInfoTextStr = V8StringPersist::New (v8::String::NewSymbol ("informativeText"));
      _mbStandardButtonsStr = V8StringPersist::New (v8::String::NewSymbol ("standardButtons"));
      _mbDefaultButtonStr = V8StringPersist::New (v8::String::NewSymbol ("defaultButton"));
   }
   else if (State == JsExtV8::Init) {

   }
   else if (State == JsExtV8::Stop) {

   }
   else if (State == JsExtV8::Shutdown) {

      foreach (QWidget *dialog, _dialogList) {

         dialog->close ();
      }

      _dialogList.clear ();

      QMapIterator<QWidget *, V8QtObject *> it (_widgetMap);
      while (it.hasNext ()) {

         it.next ();

         V8QtObject *obj = it.value ();
         delete obj; obj = 0;
      }

      _widgetMap.clear ();

      _widgetCtor.Dispose (); _widgetCtor.Clear ();
      _buttonCtor.Dispose (); _buttonCtor.Clear ();
      _listWidgetItemCtor.Dispose (); _listWidgetItemCtor.Clear ();
      _listWidgetCtor.Dispose (); _listWidgetCtor.Clear ();
      _spinBoxCtor.Dispose (); _spinBoxCtor.Clear ();
      _comboBoxCtor.Dispose (); _comboBoxCtor.Clear ();
      _sliderCtor.Dispose (); _sliderCtor.Clear ();
      _lineEditCtor.Dispose (); _lineEditCtor.Clear ();
      _textEditCtor.Dispose (); _textEditCtor.Clear ();
      _dialCtor.Dispose (); _dialCtor.Clear ();
      _labelCtor.Dispose (); _labelCtor.Clear ();
      _progressBarCtor.Dispose (); _progressBarCtor.Clear ();
      _dialogCtor.Dispose (); _dialogCtor.Clear ();
      _lcdNumberCtor.Dispose (); _lcdNumberCtor.Clear ();
      _stackedCtor.Dispose (); _stackedCtor.Clear ();
      _tabCtor.Dispose (); _tabCtor.Clear ();
      _hBoxLayoutCtor.Dispose (); _hBoxLayoutCtor.Clear ();
      _vBoxLayoutCtor.Dispose (); _vBoxLayoutCtor.Clear ();
      _boxLayoutCtor.Dispose (); _boxLayoutCtor.Clear ();
      _layoutCtor.Dispose (); _layoutCtor.Clear ();

      _qtApi.clear ();
      _messageBoxApi.clear ();
      _layoutApi.clear ();
      _state.context.Clear ();

      _obsTable.empty ();
   }
}


void
dmz::JsModuleUiV8QtBasic::release_js_instance_v8 (
      const Handle InstanceHandle,
      const String &InstanceName,
      v8::Handle<v8::Object> &instance) {

   ObsStruct *os = _obsTable.remove (InstanceHandle);

   if (os) {

      foreach (V8QtObject *obj, os->list) {

         obj->release_callback (InstanceHandle);
      }

      delete os; os = 0;
   }
}


QListWidgetItem *
dmz::JsModuleUiV8QtBasic::_to_qt_list_widget_item (V8Value value) {

   v8::HandleScope scope;
   QListWidgetItem *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_listWidgetItemTemp->HasInstance (obj)) {

         result = (QListWidgetItem *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


QWidget *
dmz::JsModuleUiV8QtBasic::_to_qt_widget (V8Value value) {

   QWidget *result (0);
   V8QtObject *object = _to_js_qt_object (value);
   if (object) { result = object->get_qt_widget (); }
   return result;
}


QLayout *
dmz::JsModuleUiV8QtBasic::_to_qt_layout (V8Value value) {

   v8::HandleScope scope;
   QLayout *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_layoutTemp->HasInstance (obj)) {

         result = (QLayout *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}

dmz::V8QtObject *
dmz::JsModuleUiV8QtBasic::_to_js_qt_object (V8Value value) {

   v8::HandleScope scope;
   V8QtObject *result (0);

   V8Object obj = v8_to_object (value);
   if (!obj.IsEmpty ()) {

      if (_widgetTemp->HasInstance (obj)) {

         result = (V8QtObject *)v8::External::Unwrap (obj->GetInternalField (0));
      }
   }

   return result;
}


// JsModuleUiV8QtBasic Interface
void
dmz::JsModuleUiV8QtBasic::_init (Config &local) {

   v8::HandleScope scope;

   _self = V8ValuePersist::New (v8::External::Wrap (this));

   // API
   _qtApi.add_function ("load", _uiloader_load, _self);

   _init_widget ();
   _init_button ();
   _init_list_widget_item ();
   _init_list_widget ();
   _init_spinbox ();
   _init_combobox ();
   _init_slider ();
   _init_lineEdit ();
   _init_textEdit ();
   _init_dial ();
   _init_label ();
   _init_progressBar ();
   _init_message_box ();
   _init_dialog ();
   _init_lcdNumber ();
   _init_stacked_widget ();
   _init_tab_widget ();

   _init_layout ();
   _init_box_layout ();
   _init_hbox_layout ();
   _init_vbox_layout ();
}


extern "C" {

DMZ_PLUGIN_FACTORY_LINK_SYMBOL dmz::Plugin *
create_dmzJsModuleUiV8QtBasic (
      const dmz::PluginInfo &Info,
      dmz::Config &local,
      dmz::Config &global) {

   return new dmz::JsModuleUiV8QtBasic (Info, local);
}

};
