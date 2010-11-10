var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , layout: require("dmz/ui/layout")
          , loader: require('dmz/ui/uiLoader')
          , mainWindow: require('dmz/ui/mainWindow')
          , messageBox: require('dmz/ui/messageBox')
          , widget: require("dmz/ui/widget")
          , button: require("dmz/ui/button")
          , comboBox: require("dmz/ui/comboBox")
          , dial: require("dmz/ui/dial")
          , dte: require("dmz/ui/dateTimeEdit")
          , lcd: require("dmz/ui/lcd")
          , label: require("dmz/ui/label")
          , lineEdit: require("dmz/ui/lineEdit")
          , list: require("dmz/ui/listWidget")
          , pbar: require("dmz/ui/progressBar")
          , slider: require("dmz/ui/slider")
          , spinbox: require("dmz/ui/spinBox")
          , stack: require("dmz/ui/stackedWidget")
          , tab: require("dmz/ui/tabWidget")
          , table: require("dmz/ui/tableWidget")
          , textEdit: require("dmz/ui/textEdit")
          , tool: require("dmz/ui/toolBox")
          , tree: require("dmz/ui/treeWidget")
          }
       }
   , _log = require('sys').puts
   , _print
   , _exports = {}
   , _form
   , _main
   , _layout
   , _stack
   , _button
   ;

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module;

      _form = dmz.ui.widget.create();
      _form.layout (dmz.ui.layout.createHBoxLayout());
      _form.layout().addWidget(dmz.ui.button.createPushButton("push", _form));
      _form.layout().addWidget(dmz.ui.button.createRadioButton("radio", _form));
      _form.layout().addWidget(dmz.ui.button.createRadioButton("radio_2", _form));
      _form.layout().addWidget(dmz.ui.button.createCheckBox("check", _form));
      _form.show();
      _main.addPage ("buttons", _form);

      _form = dmz.ui.widget.create();
      _form.layout (dmz.ui.layout.createHBoxLayout());
      _form.layout().addWidget(dmz.ui.comboBox.create(_form));
      _form.show();
      _main.addPage ("comboBox", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.dte.create(_form).show();
      _form.show();
      _main.addPage ("dte", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.lcd.create(4, _form).show();
      _form.show();
      _main.addPage ("lcd", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.label.create("label", _form);
      _form.show();
      _main.addPage ("label", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.lineEdit.create("line", _form);
      _form.show();
      _main.addPage ("line", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.list.create(_form);
      _form.show();
      _main.addPage ("list", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.pbar.create(_form);
      _form.show();
      _main.addPage ("pbar", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.slider.create(dmz.ui.consts.Horizontal, _form);
      _form.show();
      _main.addPage ("slider", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.dial.create(_form);
      _form.show();
      _main.addPage ("dial", _form);

      _form = dmz.ui.widget.create();
      _form.layout(dmz.ui.layout.createHBoxLayout());
      _button = dmz.ui.button.createPushButton("push");
      _button.observe(self, "clicked", function (btn) {

         var stack = btn.parent().lookup("stack");
         _log("stack:", stack, stack.currentIndex(), stack.count());
         stack.currentIndex((stack.currentIndex() + 1) % stack.count());
      });
      _form.layout().addWidget(_button);
      _stack = dmz.ui.stack.create(_form);
      _stack.name("stack");
      _stack.add(dmz.ui.button.createRadioButton("radio_1"));
      _stack.add(dmz.ui.button.createRadioButton("radio_2"));
      _stack.add(dmz.ui.button.createRadioButton("radio_3"));
      _stack.add(dmz.ui.button.createRadioButton("radio_4"));
      _form.layout().addWidget(_stack);
      _form.show();
      _main.addPage ("stack", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.tab.create(_form).add(dmz.ui.button.createPushButton("push"), "PushButton");
      _form.show();
      _main.addPage ("tab", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.table.create(_form);
      _form.show();
      dmz.ui.table.create(2, 3, _form);
      _main.addPage ("tables", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.textEdit.create("content", _form);
      _form.show();
      _main.addPage ("textEdit", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.tool.create(_form).addItem(dmz.ui.button.createPushButton("push"), "PushButton");
      _form.show();
      _main.addPage ("toolbox", _form);

      _form = dmz.ui.widget.create();
      dmz.ui.tree.create(_form);

      _form.show();
      _main.addPage ("tree", _form);

   }
});

