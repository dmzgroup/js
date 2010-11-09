var puts = require('sys').puts
  , ui = require('dmz/ui/uiLoader')
  , layout = require('dmz/ui/layout')
  , groupBox = require("dmz/ui/groupBox")
  , form = ui.load("./scripts/GroupBoxForm")
  , hbox
  , vbox
  , gb1
  , gb2
  , label
  ;

puts("Script:", self.name);

form.show();
gb1 = groupBox.create("GB1", form);
gb2 = groupBox.create("GB2", form)

form.observe(self, "b0", "clicked", function () {

   vbox = layout.createVBoxLayout()
   vbox.addWidget(form.lookup("rb0"));
   vbox.addWidget(form.lookup("rb1"));
   gb1.layout(vbox);
   gb1.show();
});

form.observe(self, "b1", "clicked", function () {

   vbox = layout.createVBoxLayout()
   vbox.addWidget(form.lookup("rb2"));
   vbox.addWidget(form.lookup("rb3"));
   gb2.layout(vbox);
   gb2.show();
});

form.observe(self, "b2", "clicked", function () {

   hbox = layout.createHBoxLayout();
   hbox.addWidget(gb1);
   hbox.addWidget(gb2);
});

form.observe(self, "b3", "clicked", function () {

   vbox = layout.createVBoxLayout();
   vbox.addLayout(hbox);
   hbox = layout.createHBoxLayout();
   hbox.addWidget(form.lookup("b0"));
   hbox.addWidget(form.lookup("b1"));
   hbox.addWidget(form.lookup("b2"));
   hbox.addWidget(form.lookup("b3"));
   vbox.addLayout(hbox);
   form.layout(vbox);
});

gb1.observe(self, "clicked", function () { puts("gb1 clicked"); });
gb2.observe(self, "clicked", function () { puts("gb2 clicked"); });
gb1.observe(self, "toggled", function (b) { puts("gb1 toggled:", b); });
gb2.observe(self, "toggled", function (b) { puts("gb2 toggled:", b); });
