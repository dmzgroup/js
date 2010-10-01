var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , layout = require('dmz/components/ui/layout')
  , form
  , pbar = ui.load("./scripts/ProgressBarForm.ui")
  , widget = ui.load("./scripts/SliderForm.ui")
  , cb = ui.load("./scripts/CheckBoxForm.ui")
  , label = ui.load("./scripts/LabelForm.ui")
  , combo = ui.load("./scripts/comboBoxForm.ui")
  , buttons = ["pushButton", "pushButton_2", "pushButton_3", "pushButton_4", "pushButton_5",
            "pushButton_6", "pushButton_7", "pushButton_8", "pushButton_9"]
  , tw
  , at
  , ix
  , box
  , hbox
  , vbox
  ;

puts("Script:", self.name);

form = ui.load("./scripts/LayoutForm.ui");
form.show()

for (ix = 0; ix < buttons.length; ix += 1) {
   buttons[ix] = form.lookup(buttons[ix]);
}

box = layout.createBoxLayout(layout.LeftToRight)
form.setLayout (box);

buttons[0].observe(self, "clicked", function () {

   box.direction(layout.TopToBottom);
});

buttons[1].observe(self, "clicked", function () {

   box.direction(layout.LeftToRight);
   vbox = layout.createVBoxLayout();
   vbox.addWidget(buttons[1]);
   vbox.addWidget(buttons[2]);
   box.addLayout(vbox);
});

buttons[2].observe(self, "clicked", function () {

   hbox = layout.createHBoxLayout();
   hbox.addWidget(buttons[3]);
   hbox.addWidget(buttons[4]);
   box.addLayout(hbox);
});

buttons[3].observe(self, "clicked", function () {

   vbox = layout.createVBoxLayout();
   hbox = layout.createHBoxLayout();
   hbox.addWidget(buttons[5]);
   hbox.addWidget(buttons[6]);
   vbox.addLayout(hbox);
   hbox = layout.createHBoxLayout();
   hbox.addWidget(buttons[7]);
   hbox.addWidget(buttons[8]);
   vbox.addLayout(hbox);
   box.addLayout(vbox);
});
