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
  , grid
  , hbox
  , vbox
  , label
  ;

puts("Script:", self.name);

form.show();

label = form.lookup("label");
grid = dmz.ui.layout.createGridLayout(form);
for (ix = 0; ix < buttons.length; ix += 1) { buttons[ix] = form.lookup(buttons[ix]); }

buttons[0].observe(self, "clicked", function () {

   for (var ix = 0; ix < buttons.length; ix += 1) {
//      grid.addRow("label" + ix, buttons[ix]);
      grid.addWidget(buttons[ix], ix / 2, ix % 2);
   }
});

buttons[1].observe(self, "clicked", function () {

   vbox = dmz.ui.layout.createVBoxLayout();
   vbox.addWidget(buttons[1]);
   vbox.addWidget(buttons[2]);
//   grid.addRow("vbox", vbox);
   grid.addLayout(vbox, 0, 2);
});

buttons[2].observe(self, "clicked", function () {

   grid.addWidget(buttons[0], 0, 0, 1, 0);
   grid.addWidget(buttons[8], 0, 0, 0, 1);
});

buttons[3].observe(self, "clicked", function () {

   hbox = dmz.ui.layout.createHBoxLayout();
   hbox.addWidget(buttons[2]);
   hbox.addWidget(buttons[3]);
   grid.addLayout(hbox, 0, 0, 1, 0);
});

buttons[4].observe(self, "clicked", function () {

   puts("cols:", grid.columnCount());
   puts("rows:", grid.rowCount());
});

buttons[5].observe(self, "clicked", function () {

   grid.rowStretch(0, 50);
   grid.columnStretch(1, 50);
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
