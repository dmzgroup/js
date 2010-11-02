#ifndef DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
#define DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H

#include <dmzJsExtV8.h>
#include <dmzJsModuleUiV8Qt.h>
#include <dmzJsV8UtilHelpers.h>
#include <dmzRuntimeLog.h>
#include <dmzRuntimePlugin.h>
#include <dmzRuntimeTimeSlice.h>
#include <dmzTypesHashTableHandleTemplate.h>
#include <dmzTypesStringContainer.h>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <v8.h>

class QInputDialog;
class QMessageBox;


namespace dmz {

   class JsModuleUiV8QtBasic;
   class JsModuleV8;
   class QtModuleMainWindow;
   class V8QtObject;
   class V8QtDialog;
   class V8QtWidget;
   struct V8ValueRef;


   struct JsModuleUiV8QtBasicState {

      JsModuleV8 *core;
      JsModuleUiV8QtBasic *ui;
      QtModuleMainWindow *mainWindowModule;

      v8::Handle<v8::Context> context;

      JsModuleUiV8QtBasicState () :
            core (0),
            ui (0),
            mainWindowModule (0) {;}
   };


   class JsModuleUiV8QtBasic :
         public Plugin,
         public TimeSlice,
         public JsModuleUiV8Qt,
         public JsExtV8 {

      public:
         JsModuleUiV8QtBasic (const PluginInfo &Info, Config &local);
         ~JsModuleUiV8QtBasic ();

         // Plugin Interface
         virtual void update_plugin_state (
            const PluginStateEnum State,
            const UInt32 Level);

         virtual void discover_plugin (
            const PluginDiscoverEnum Mode,
            const Plugin *PluginPtr);

         // TimeSlice Interface
         virtual void update_time_slice (const Float64 DeltaTime);

         // JsModuleUiV8Qt Interface
         virtual v8::Handle<v8::Value> create_v8_qobject (QObject *value);
         virtual v8::Handle<v8::Value> create_v8_qwidget (QWidget *value);
         virtual v8::Handle<v8::Value> create_v8_qlistwidgetitem (QListWidgetItem *value);
         virtual v8::Handle<v8::Value> create_v8_qtreewidgetitem (QTreeWidgetItem *value);
         virtual v8::Handle<v8::Value> create_v8_qtablewidgetitem (QTableWidgetItem *value);
         virtual v8::Handle<v8::Value> create_v8_qtablewidgetselectionrange (QTableWidgetSelectionRange *value);

         // JsExtV8 Interface
         virtual void update_js_module_v8 (const ModeEnum Mode, JsModuleV8 &module);
         virtual void update_js_context_v8 (v8::Handle<v8::Context> context);
         virtual void update_js_ext_v8_state (const StateEnum State);

         virtual void release_js_instance_v8 (
            const Handle InstanceHandle,
            const String &InstanceName,
            v8::Handle<v8::Object> &instance);

         template <class T> T *
         v8_to_qobject (V8Value value) {

            T *result (0);
            QObject *object = _to_qobject (value);
            if (object) { result = qobject_cast<T *>(object); }
            return result;
         }

      protected:
         struct ObsStruct {

            const Handle Observer;
            QList<V8QtObject *>list;

            ObsStruct (const Handle TheObserver) : Observer (TheObserver) {;}
            ~ObsStruct () { list.clear (); }
         };

         // Static Functions
         static JsModuleUiV8QtBasic *_to_self (const v8::Arguments &Args);

         // QUiLoader bindings
         static V8Value _uiloader_load (const v8::Arguments &Args);

         // QObject bindings implemented in JsModuleUiV8QtBasicObject.cpp
         static V8Value _object_lookup (const v8::Arguments &Args);
         static V8Value _object_name (const v8::Arguments &Args);
         static V8Value _object_observe (const v8::Arguments &Args);
         static V8Value _object_parent (const v8::Arguments &Args);
         static V8Value _object_property (const v8::Arguments &Args);
         static V8Value _object_callback (const v8::Arguments &Args);
         static V8Value _object_to_string (const v8::Arguments &Args);

         // QWidget bindings implemented in JsModuleUiV8QtBasicWidget.cpp
         static V8Value _widget_close (const v8::Arguments &Args);
         static V8Value _widget_enabled (const v8::Arguments &Args);
         static V8Value _widget_hide (const v8::Arguments &Args);
         static V8Value _widget_layout (const v8::Arguments &Args);
         static V8Value _widget_show (const v8::Arguments &Args);
         static V8Value _widget_title (const v8::Arguments &Args);
         static V8Value _widget_visible (const v8::Arguments &Args);
         static V8Value _widget_window (const v8::Arguments &Args);

         // QAbstractButton bindings implemented in JsModuleUiV8QtBasicButton.cpp
         static V8Value _button_text (const v8::Arguments &Args);
         static V8Value _button_is_checked (const v8::Arguments &Args);
         static V8Value _button_set_checked (const v8::Arguments &Args);
         static V8Value _button_click (const v8::Arguments &Args);

         // QDialog bindings implemented in JsModuleUiV8QtBasicDialog.cpp
         static V8Value _dialog_open (const v8::Arguments &Args);

         // QSpinBox bindings implemented in JsModuleUiV8QtBasicSpinBox.cpp
         static V8Value _spinbox_maximum (const v8::Arguments &Args);
         static V8Value _spinbox_minimum (const v8::Arguments &Args);
         static V8Value _spinbox_value (const v8::Arguments &Args);
         static V8Value _spinbox_text (const v8::Arguments &Args);

         // QComboBox bindings implemented in JsModuleUiV8QtBasicComboBox.cpp
         static V8Value _combobox_count (const v8::Arguments &Args);
         static V8Value _combobox_current_index (const v8::Arguments &Args);
         static V8Value _combobox_current_text (const v8::Arguments &Args);
         static V8Value _combobox_add_item (const v8::Arguments &Args);
         static V8Value _combobox_add_items (const v8::Arguments &Args);
         static V8Value _combobox_find_text (const v8::Arguments &Args);
         static V8Value _combobox_remove_item (const v8::Arguments &Args);
         static V8Value _combobox_clear (const v8::Arguments &Args);
         static V8Value _combobox_item_text (const v8::Arguments &Args);

         // QSlider bindings implemented in JsModuleUiV8QtBasicSlider.cpp
         static V8Value _slider_maximum (const v8::Arguments &Args);
         static V8Value _slider_minimum (const v8::Arguments &Args);
         static V8Value _slider_value (const v8::Arguments &Args);
         static V8Value _slider_is_down (const v8::Arguments &Args);

         // QDial bindings implemented in JsModuleUiV8QtBasicSlider.cpp
         static V8Value _dial_wrapping (const v8::Arguments &Args);
         static V8Value _dial_notches_visible (const v8::Arguments &Args);

         // QLineEdit bindings implemented in JsModuleUiV8QtBasicLineEdit.cpp
         static V8Value _lineEdit_text (const v8::Arguments &Args);
         static V8Value _lineEdit_clear (const v8::Arguments &Args);
         static V8Value _lineEdit_undo (const v8::Arguments &Args);
         static V8Value _lineEdit_redo (const v8::Arguments &Args);

         // QTextEdit bindings implemented in JsModuleUiV8QtBasicTextEdit.cpp
         static V8Value _textEdit_append (const v8::Arguments &Args);
         static V8Value _textEdit_clear (const v8::Arguments &Args);
         static V8Value _textEdit_redo (const v8::Arguments &Args);
         static V8Value _textEdit_undo (const v8::Arguments &Args);
         static V8Value _textEdit_text (const v8::Arguments &Args);
         static V8Value _textEdit_allow_undo (const v8::Arguments &Args);

         // QLabel bindings implemented in JsModuleUiV8QtLabel.cpp
         static V8Value _label_text (const v8::Arguments &Args);
         static V8Value _label_word_wrap (const v8::Arguments &Args);
         static V8Value _label_clear (const v8::Arguments &Args);

         // QProgressBar bindings implemented in JsModuleUiV8QtLabel.cpp
         static V8Value _progress_bar_maximum (const v8::Arguments &Args);
         static V8Value _progress_bar_minimum (const v8::Arguments &Args);
         static V8Value _progress_bar_text (const v8::Arguments &Args);
         static V8Value _progress_bar_value (const v8::Arguments &Args);
         static V8Value _progress_bar_reset (const v8::Arguments &Args);

         // QLCDNumber bindings implemented in JsModuleUiV8QtLCDNumber.cpp
         static V8Value _lcd_number_value (const v8::Arguments &Args);

         // QListWidgetItem bindings implemented in JsModuleUiV8QtBasicListWidget.cpp
         static V8Value _list_widget_item_text (const v8::Arguments &Args);
         static V8Value _list_widget_item_data (const v8::Arguments &Args);

         // QListWidget bindings implemented in JsModuleUiV8QtBasicListWidget.cpp
         static V8Value _list_widget_add_item (const v8::Arguments &Args);
         static V8Value _list_widget_take_item (const v8::Arguments &Args);
         static V8Value _list_widget_current_item (const v8::Arguments &Args);
         static V8Value _list_widget_clear (const v8::Arguments &Args);
         // static V8Value _list_widget_item (const v8::Arguments &Args);

         // QStackedWidget bindings implemented in JsModuleUiV8QtBasicStackedWidget.cpp
         static V8Value _stacked_widget_add (const v8::Arguments &Args);
         static V8Value _stacked_widget_remove (const v8::Arguments &Args);
         static V8Value _stacked_widget_count (const v8::Arguments &Args);
         static V8Value _stacked_widget_current_index (const v8::Arguments &Args);
         static V8Value _stacked_widget_current_widget (const v8::Arguments &Args);
         static V8Value _stacked_widget_at (const v8::Arguments &Args);
         static V8Value _stacked_widget_index_of (const v8::Arguments &Args);

         // QTabWidget bindings implemented in JsModuleUiV8QtBasicTabWidget.cpp
         static V8Value _tab_widget_add (const v8::Arguments &Args);
         static V8Value _tab_widget_remove (const v8::Arguments &Args);
         static V8Value _tab_widget_current_widget (const v8::Arguments &Args);
         static V8Value _tab_widget_current_index (const v8::Arguments &Args);
         static V8Value _tab_widget_count (const v8::Arguments &Args);
         static V8Value _tab_widget_index_of (const v8::Arguments &Args);
         static V8Value _tab_widget_tab_text (const v8::Arguments &Args);
         static V8Value _tab_widget_at (const v8::Arguments &Args);

         // QMessageBox bindings implemented in JsModuleUiV8QtBasicMessageBox.cpp
         static V8Value _create_message_box (const v8::Arguments &Args);
         QMessageBox *_create_message_box (V8Object params, QWidget *parent);

         // QLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _layout_index_of (const v8::Arguments &Args);
         static V8Value _layout_enabled (const v8::Arguments &Args);
         static V8Value _layout_at (const v8::Arguments &Args);
         static V8Value _layout_count (const v8::Arguments &Args);
         static V8Value _layout_parent (const v8::Arguments &Args);
         static V8Value _layout_parent_widget (const v8::Arguments &Args);

         // QBoxLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _box_layout_add_layout (const v8::Arguments &Args);
         static V8Value _box_layout_add_stretch (const v8::Arguments &Args);
         static V8Value _box_layout_add_widget (const v8::Arguments &Args);
         static V8Value _box_layout_direction (const v8::Arguments &Args);
         static V8Value _create_box_layout (const v8::Arguments &Args);
         static V8Value _create_hbox_layout (const v8::Arguments &Args);
         static V8Value _create_vbox_layout (const v8::Arguments &Args);

         // QGridLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _grid_layout_add_layout (const v8::Arguments &Args);
         static V8Value _grid_layout_add_widget (const v8::Arguments &Args);
         static V8Value _grid_layout_column_count (const v8::Arguments &Args);
         static V8Value _grid_layout_row_count (const v8::Arguments &Args);
         static V8Value _grid_layout_column_stretch (const v8::Arguments &Args);
         static V8Value _grid_layout_row_stretch (const v8::Arguments &Args);
         static V8Value _create_grid_layout (const v8::Arguments &Args);

         // QFormLayout bindings implemented in JsModuleUiV8QtBasicLayout.cpp
         static V8Value _form_layout_add_row (const v8::Arguments &Args);
         static V8Value _form_layout_insert_row (const v8::Arguments &Args);
         static V8Value _form_layout_row_count (const v8::Arguments &Args);
         static V8Value _form_layout_vertical_spacing (const v8::Arguments &Args);
         static V8Value _form_layout_spacing (const v8::Arguments &Args);
         static V8Value _create_form_layout (const v8::Arguments &Args);

         // QFileDialog bindings implemented in JsModuleUiV8QtBasicFileDialog.cpp
         static V8Value _file_dialog_get_existing_directory (const v8::Arguments &Args);
         static V8Value _file_dialog_get_open_file_name (const v8::Arguments &Args);
         static V8Value _file_dialog_get_save_file_name (const v8::Arguments &Args);

         // QMainWindow bindings implemented in JsModuleUiV8QtBasicMainWindow.cpp
         static V8Value _main_window_central_widget (const v8::Arguments &Args);
         static V8Value _main_window_close (const v8::Arguments &Args);
         static V8Value _main_window_create_dock_widget (const v8::Arguments &Args);
         static V8Value _main_window_remove_dock_widget (const v8::Arguments &Args);
         static V8Value _main_window_add_menu (const v8::Arguments &Args);
         static V8Value _main_window_add_separator (const v8::Arguments &Args);
         static V8Value _main_window_window (const v8::Arguments &Args);

         // QDockWidget bindings implemented in JsModuleUiV8QtBasicDockWidget.cpp
         static V8Value _create_dock_widget (const v8::Arguments &Args);
//         static V8Value _dock_window_widget (const v8::Arguments &Args);
//         static V8Value _dock_window_toggle_view_action (const v8::Arguments &Args);
         static V8Value _dock_widget_floating (const v8::Arguments &Args);

         // QFrame bindings implemented in JsModuleUiV8QtBasicFrame.cpp
//         static V8Value _frame_create (const v8::Arguments &Args);
         static V8Value _frame_shadow (const v8::Arguments &Args);
         static V8Value _frame_shape (const v8::Arguments &Args);
         static V8Value _frame_width (const v8::Arguments &Args);
         static V8Value _frame_line_width (const v8::Arguments &Args);
         static V8Value _frame_mid_line_width (const v8::Arguments &Args);

         // QGroupBox implemented in JsModuleUiV8QtBasicGroupBox.cpp
         static V8Value _create_group_box (const v8::Arguments &Args);
         static V8Value _group_box_flat (const v8::Arguments &Args);
         static V8Value _group_box_title (const v8::Arguments &Args);
         static V8Value _group_box_alignment (const v8::Arguments &Args);
         static V8Value _group_box_checkable (const v8::Arguments &Args);

         // QDateTimeEdit implemented in JsModulkeUiV8QtBasicDateTimeEdit.cpp
         static V8Value _dt_maximum (const v8::Arguments &Args);
         static V8Value _dt_minimum (const v8::Arguments &Args);
         static V8Value _dt_datetime (const v8::Arguments &Args);

         // QAction bindings implemented in JsModuleUiV8QtBasicAction.cpp
         static V8Value _action_enabled (const v8::Arguments &Args);
         static V8Value _action_shortcut (const v8::Arguments &Args);
         static V8Value _action_text (const v8::Arguments &Args);
         static V8Value _action_trigger (const v8::Arguments &Args);

         // QTreeWidget bindings implemented in JsModuleUiV8QtBasicTreeWidget.cpp
         static V8Value _tree_clear (const v8::Arguments &Args);
         static V8Value _tree_header_labels (const v8::Arguments &Args);
         static V8Value _tree_col_width (const v8::Arguments &Args);
         static V8Value _tree_col_hidden (const v8::Arguments &Args);
         static V8Value _tree_word_wrap (const v8::Arguments &Args);
         static V8Value _tree_add (const v8::Arguments &Args);
         static V8Value _tree_col_count (const v8::Arguments &Args);
         static V8Value _tree_curr_col (const v8::Arguments &Args);
         static V8Value _tree_curr_item (const v8::Arguments &Args);
         static V8Value _tree_find_items (const v8::Arguments &Args);
         static V8Value _tree_item_above (const v8::Arguments &Args);
         static V8Value _tree_item_below (const v8::Arguments &Args);
         static V8Value _tree_item_widget (const v8::Arguments &Args);
         static V8Value _tree_rem_item_widget (const v8::Arguments &Args);
         static V8Value _tree_selected_items (const v8::Arguments &Args);
         static V8Value _tree_sort_col (const v8::Arguments &Args);
         static V8Value _tree_take_item_at (const v8::Arguments &Args);
         static V8Value _tree_item_at (const v8::Arguments &Args);
         static V8Value _tree_item_count (const v8::Arguments &Args);

         // QTreeWidgetItem bindings implemented in JsModuleUiV8QtBasicTreeWidget.cpp
         static V8Value _tree_item_add (const v8::Arguments &Args);
         static V8Value _tree_item_child (const v8::Arguments &Args);
         static V8Value _tree_item_child_count (const v8::Arguments &Args);
         static V8Value _tree_item_col_count (const v8::Arguments &Args);
         static V8Value _tree_item_data (const v8::Arguments &Args);
         static V8Value _tree_item_parent (const v8::Arguments &Args);
         static V8Value _tree_item_hidden (const v8::Arguments &Args);
         static V8Value _tree_item_text (const v8::Arguments &Args);
         static V8Value _tree_item_sort_children (const v8::Arguments &Args);
         static V8Value _tree_item_take_child (const v8::Arguments &Args);
         static V8Value _tree_item_take_children (const v8::Arguments &Args);
         static V8Value _tree_item_tree_widget (const v8::Arguments &Args);

         // QTableWidget bindings implemented in JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_clear_spans (const v8::Arguments &Args);
         static V8Value _table_set_span (const v8::Arguments &Args);
         static V8Value _table_col_width (const v8::Arguments &Args);
         static V8Value _table_col_hidden (const v8::Arguments &Args);
         static V8Value _table_row_height (const v8::Arguments &Args);
         static V8Value _table_word_wrap (const v8::Arguments &Args);
         static V8Value _table_row_hidden (const v8::Arguments &Args);
         static V8Value _table_resize_single_col (const v8::Arguments &Args);
         static V8Value _table_resize_all_col (const v8::Arguments &Args);
         static V8Value _table_resize_single_row (const v8::Arguments &Args);
         static V8Value _table_resize_all_row (const v8::Arguments &Args);
         static V8Value _table_select_col (const v8::Arguments &Args);
         static V8Value _table_select_row (const v8::Arguments &Args);
         static V8Value _table_cell_widget (const v8::Arguments &Args);
         static V8Value _table_col (const v8::Arguments &Args);
         static V8Value _table_col_count (const v8::Arguments &Args);
         static V8Value _table_curr_col (const v8::Arguments &Args);
         static V8Value _table_curr_item (const v8::Arguments &Args);
         static V8Value _table_curr_row (const v8::Arguments &Args);
         static V8Value _table_find_items (const v8::Arguments &Args);
         static V8Value _table_horiz_header (const v8::Arguments &Args);
         static V8Value _table_item_at (const v8::Arguments &Args);
         static V8Value _table_remove_cell_widget (const v8::Arguments &Args);
         static V8Value _table_row (const v8::Arguments &Args);
         static V8Value _table_row_count (const v8::Arguments &Args);
         static V8Value _table_selected_items (const v8::Arguments &Args);
         static V8Value _table_selected_ranges (const v8::Arguments &Args);
         static V8Value _table_set_current_cell (const v8::Arguments &Args);
         static V8Value _table_set_h_labels (const v8::Arguments &Args);
         static V8Value _table_set_v_labels (const v8::Arguments &Args);
         static V8Value _table_set_range_selected (const v8::Arguments &Args);
         static V8Value _table_sort_items (const v8::Arguments &Args);
         static V8Value _table_take_item (const v8::Arguments &Args);
         static V8Value _table_clear (const v8::Arguments &Args);
         static V8Value _table_clear_contents (const v8::Arguments &Args);
         static V8Value _table_insert_col (const v8::Arguments &Args);
         static V8Value _table_insert_row (const v8::Arguments &Args);
         static V8Value _table_remove_col (const v8::Arguments &Args);
         static V8Value _table_remove_row (const v8::Arguments &Args);
         static V8Value _table_vert_header (const v8::Arguments &Args);

         // QTableWidgetItem bindings implemented in JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_item_column (const v8::Arguments &Args);
         static V8Value _table_item_row (const v8::Arguments &Args);
         static V8Value _table_item_data (const v8::Arguments &Args);
         static V8Value _table_item_selected (const v8::Arguments &Args);
         static V8Value _table_item_text (const v8::Arguments &Args);

         // QTableWidgetSelectionRange bindings implemented in JsModuleUiV8QtBasicTableWidget.cpp
         static V8Value _table_range_bottom (const v8::Arguments &Args);
         static V8Value _table_range_top (const v8::Arguments &Args);
         static V8Value _table_range_left (const v8::Arguments &Args);
         static V8Value _table_range_right (const v8::Arguments &Args);
         static V8Value _table_range_col_count (const v8::Arguments &Args);
         static V8Value _table_range_row_count (const v8::Arguments &Args);

         // QInputDialog bindings implemented in JsModuleUiV8QtBasicInputDialog.cpp
         static V8Value _create_input_dialog (const v8::Arguments &Args);
         QInputDialog *_create_input_dialog (V8Object params, QWidget *parent);

         QWidget *_to_qwidget (V8Value value) { return v8_to_qobject<QWidget>(value); }
         QObject *_to_qobject (V8Value value);
         QListWidgetItem *_to_qlistwidgetitem (V8Value value);
         QTreeWidgetItem *_to_qtreewidgetitem (V8Value value);
         QTableWidgetItem *_to_qtablewidgetitem (V8Value value);
         QTableWidgetSelectionRange *_to_qtablewidgetselectionrange (V8Value value);

         V8QtDialog *_to_v8_qt_dialog (V8Value value);
         V8QtWidget *_to_v8_qt_widget (V8Value value);
         V8QtObject *_to_v8_qt_object (V8Value value);

         QVariant _qvariant_wrap_v8 (V8Value value);
         V8Value _qvariant_unwrap_v8 (const QVariant &Value);

         void _get_file_dialog_params (
            V8Object params,
            QString &caption,
            QString &dir,
            QString &filter,
            Boolean &allowMultiple);

         void _init_object ();
         void _init_widget ();
         void _init_frame ();
         void _init_button ();
         void _init_list_widget ();
         void _init_list_widget_item ();
         void _init_tree_widget ();
         void _init_tree_widget_item ();
         void _init_table_widget ();
         void _init_table_widget_item ();
         void _init_table_selection ();
         void _init_spinbox ();
         void _init_combobox ();
         void _init_slider ();
         void _init_dial ();
         void _init_lineEdit ();
         void _init_textEdit ();
         void _init_label ();
         void _init_progressBar ();
         void _init_dialog ();
         void _init_message_box ();
         void _init_lcdNumber ();
         void _init_stacked_widget ();
         void _init_tab_widget ();
         void _init_file_dialog ();
         void _init_main_window ();
         void _init_dock_widget ();
         void _init_group_box ();
         void _init_dt ();
         void _init_action ();
         void _init_input_dialog ();

         void _init_layout ();
         void _init_box_layout ();
         void _init_hbox_layout ();
         void _init_vbox_layout ();
         void _init_grid_layout ();
         void _init_form_layout ();

         String _find_ui_file (const String &Name);
         void _init (Config &local);

         Log _log;

         StringContainer _searchPaths;
         String _mainWindowModuleName;

         JsModuleUiV8QtBasicState _state;
         V8ValuePersist _self;

         HashTableHandleTemplate<ObsStruct> _obsTable;
         QMap<QObject *, V8QtObject *> _objectMap;
         QMap<QString, QAction *> _menuActionMap;
         QList<QWidget *> _dialogList;
         QList<String> _dockList;
         QByteArray _mainWindowState;
         QList<V8ValueRef *>_valueDeleteList;

         V8InterfaceHelper _qtApi;
         V8InterfaceHelper _uiLoaderApi;
         V8InterfaceHelper _mainWindowApi;
         V8InterfaceHelper _dockWidgetApi;
         V8InterfaceHelper _messageBoxApi;
         V8InterfaceHelper _layoutApi;
         V8InterfaceHelper _fileDialogApi;
         V8InterfaceHelper _actionApi;
         V8InterfaceHelper _frameApi;
         V8InterfaceHelper _groupBoxApi;
         V8InterfaceHelper _inputDialogApi;

         V8FunctionTemplatePersist _objectTemp;
         V8FunctionPersist _objectCtor;

         V8FunctionTemplatePersist _widgetTemp;
         V8FunctionPersist _widgetCtor;

         V8FunctionTemplatePersist _buttonTemp;
         V8FunctionPersist _buttonCtor;

         V8FunctionTemplatePersist _listWidgetItemTemp;
         V8FunctionPersist _listWidgetItemCtor;

         V8FunctionTemplatePersist _listWidgetTemp;
         V8FunctionPersist _listWidgetCtor;

         V8FunctionTemplatePersist _spinBoxTemp;
         V8FunctionPersist _spinBoxCtor;

         V8FunctionTemplatePersist _comboBoxTemp;
         V8FunctionPersist _comboBoxCtor;

         V8FunctionTemplatePersist _sliderTemp;
         V8FunctionPersist _sliderCtor;

         V8FunctionTemplatePersist _dialTemp;
         V8FunctionPersist _dialCtor;

         V8FunctionTemplatePersist _lineEditTemp;
         V8FunctionPersist _lineEditCtor;

         V8FunctionTemplatePersist _textEditTemp;
         V8FunctionPersist _textEditCtor;

         V8FunctionTemplatePersist _labelTemp;
         V8FunctionPersist _labelCtor;

         V8FunctionTemplatePersist _progressBarTemp;
         V8FunctionPersist _progressBarCtor;

         V8FunctionTemplatePersist _dialogTemp;
         V8FunctionPersist _dialogCtor;

         V8FunctionTemplatePersist _lcdNumberTemp;
         V8FunctionPersist _lcdNumberCtor;

         V8FunctionTemplatePersist _stackedWidgetTemp;
         V8FunctionPersist _stackedCtor;

         V8FunctionTemplatePersist _tabWidgetTemp;
         V8FunctionPersist _tabCtor;

         V8FunctionTemplatePersist _layoutTemp;
         V8FunctionPersist _layoutCtor;

         V8FunctionTemplatePersist _boxLayoutTemp;
         V8FunctionPersist _boxLayoutCtor;

         V8FunctionTemplatePersist _hBoxLayoutTemp;
         V8FunctionPersist _hBoxLayoutCtor;

         V8FunctionTemplatePersist _vBoxLayoutTemp;
         V8FunctionPersist _vBoxLayoutCtor;

         V8FunctionTemplatePersist _gridLayoutTemp;
         V8FunctionPersist _gridLayoutCtor;

         V8FunctionTemplatePersist _formLayoutTemp;
         V8FunctionPersist _formLayoutCtor;

         V8FunctionTemplatePersist _mainWindowTemp;
         V8FunctionPersist _mainWindowCtor;

         V8FunctionTemplatePersist _dockWidgetTemp;
         V8FunctionPersist _dockWidgetCtor;

         V8FunctionTemplatePersist _actionTemp;
         V8FunctionPersist _actionCtor;

         V8FunctionTemplatePersist _frameTemp;
         V8FunctionPersist _frameCtor;

         V8FunctionTemplatePersist _groupBoxTemp;
         V8FunctionPersist _groupBoxCtor;

         V8FunctionTemplatePersist _dteTemp;
         V8FunctionPersist _dteCtor;

         V8FunctionTemplatePersist _treeWidgetTemp;
         V8FunctionPersist _treeWidgetCtor;

         V8FunctionTemplatePersist _treeWidgetItemTemp;
         V8FunctionPersist _treeWidgetItemCtor;

         V8FunctionTemplatePersist _tableWidgetTemp;
         V8FunctionPersist _tableWidgetCtor;

         V8FunctionTemplatePersist _tableWidgetItemTemp;
         V8FunctionPersist _tableWidgetItemCtor;

         V8FunctionTemplatePersist _tableSelectionTemp;
         V8FunctionPersist _tableSelectionCtor;

         V8StringPersist _allowMultipleStr;
         V8StringPersist _allowedAreasStr;
         V8StringPersist _areaStr;
         V8StringPersist _captionStr;
         V8StringPersist _defaultButtonStr;
         V8StringPersist _dirStr;
         V8StringPersist _editableStr;
         V8StringPersist _featuresStr;
         V8StringPersist _filterStr;
         V8StringPersist _floatingStr;
         V8StringPersist _infoTextStr;
         V8StringPersist _optionsStr;
         V8StringPersist _standardButtonsStr;
         V8StringPersist _statusTipStr;
         V8StringPersist _textStr;
         V8StringPersist _toolTipStr;
         V8StringPersist _typeStr;
         V8StringPersist _visibleStr;
         V8StringPersist _currentStr;
         V8StringPersist _decimalStr;
         V8StringPersist _itemsStr;
         V8StringPersist _labelStr;
         V8StringPersist _maxStr;
         V8StringPersist _minStr;
         V8StringPersist _modeStr;
         V8StringPersist _stepStr;
         V8StringPersist _titleStr;
         V8StringPersist _valueStr;
         V8StringPersist _shortcutStr;
         V8StringPersist _iconStr;

      private:
         JsModuleUiV8QtBasic ();
         JsModuleUiV8QtBasic (const JsModuleUiV8QtBasic &);
         JsModuleUiV8QtBasic &operator= (const JsModuleUiV8QtBasic &);
   };
};


inline dmz::JsModuleUiV8QtBasic *
dmz::JsModuleUiV8QtBasic::_to_self (const v8::Arguments &Args) {

   return (dmz::JsModuleUiV8QtBasic *)v8::External::Unwrap (Args.Data ());
}


#endif // DMZ_JS_MODULE_UI_V8_QT_BASIC_DOT_H
