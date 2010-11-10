var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/ui/uiLoader')
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
  ;

puts("Script:", self.name);

form = uiLoader.load("./scripts/LayoutForm.ui");
form.show();


box = layout.createBoxLayout(layout.LeftToRight);
form.layout (box);
for (ix = 0; ix < buttons.length; ix += 1) {
   buttons[ix] = form.lookup(buttons[ix]);
//   box.addStretch(100);
   box.addWidget(buttons[ix]);
}

puts("Button[2]:", box.indexOf(buttons[2]));
puts("Button[4]:", box.indexOf(box.at(box.indexOf(buttons[4]))));
puts("Count:", box.count());

buttons[0].observe(self, "clicked", function () {

   puts("toptobottom");
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
   hbox.addStretch(100);
   hbox.addWidget(buttons[6]);
   vbox.addLayout(hbox);
   hbox = layout.createHBoxLayout();
   hbox.addWidget(buttons[7]);
   hbox.addWidget(buttons[8]);
   vbox.addLayout(hbox);
   box.addLayout(vbox);
});

buttons[4].observe(self, "clicked", function () {

   puts("enabled:", box.enabled());
   box.enabled(!box.enabled());
   puts("enabled:", box.enabled());
});

buttons[5].observe(self, "clicked", function (btn) {

   puts("Box Count:", box.count());
   puts("Vbox count:", vbox.count());
   puts("Vbox parent count:", vbox.parent().count());
   btn.window().layout().direction(layout.TopToBottom);
   // vbox.parentWidget().layout().direction(layout.TopToBottom);
});
