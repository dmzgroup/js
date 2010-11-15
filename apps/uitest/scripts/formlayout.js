var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , layout: require('dmz/ui/layout')
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts
  , form = dmz.ui.loader.load("./scripts/LayoutForm.ui")
  , buttons = ["pushButton", "pushButton_2", "pushButton_3", "pushButton_4", "pushButton_5",
            "pushButton_6", "pushButton_7", "pushButton_8", "pushButton_9"]
  , tw
  , at
  , ix
  , box
  , hbox
  , vbox
  , label
  ;

puts("Script:", self.name);

form.show();

label = form.lookup("label");
box = dmz.ui.layout.createFormLayout(form);
for (ix = 0; ix < buttons.length; ix += 1) {
   buttons[ix] = form.lookup(buttons[ix]);
}

buttons[0].observe(self, "clicked", function () {

   for (var ix = 0; ix < buttons.length; ix += 1) {
      box.addRow("label" + ix, buttons[ix]);
   }
});

buttons[1].observe(self, "clicked", function () {

   vbox = dmz.ui.layout.createVBoxLayout();
   vbox.addWidget(buttons[1]);
   vbox.addWidget(buttons[2]);
   box.addRow("vbox", vbox);
});

buttons[2].observe(self, "clicked", function () {

   puts("row Count:", box.rowCount ());
   puts("vertSpacing:", box.verticalSpacing());
   box.verticalSpacing(50);
   puts("vertSpacing:", box.verticalSpacing());
   puts("spacing:", box.spacing());
   box.spacing(50);
   puts("spacing:", box.spacing());
});

buttons[3].observe(self, "clicked", function () {

   box.addRow(label, buttons[0]);
   hbox = dmz.ui.layout.createHBoxLayout();
   hbox.addWidget(buttons[2]);
   hbox.addWidget(buttons[3]);
   box.addRow("label3", hbox);
   box.addRow("label2", buttons[1]);
   box.addRow(buttons[4]);
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
