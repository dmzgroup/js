var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/ui/uiLoader')
  , layout = require('dmz/ui/layout')
  , form
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

form = ui.load("./scripts/LayoutForm.ui");
form.show();

label = form.lookup("label");
box = layout.createFormLayout(form);
for (ix = 0; ix < buttons.length; ix += 1) {
   buttons[ix] = form.lookup(buttons[ix]);
}

buttons[0].observe(self, "clicked", function () {

   for (var ix = 0; ix < buttons.length; ix += 1) {
      box.addRow("label" + ix, buttons[ix]);
   }
});

buttons[1].observe(self, "clicked", function () {

   vbox = layout.createVBoxLayout();
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
   hbox = layout.createHBoxLayout();
   hbox.addWidget(buttons[2]);
   hbox.addWidget(buttons[3]);
   box.addRow("label3", hbox);
   box.addRow("label2", buttons[1]);
   box.addRow(buttons[4]);
});
