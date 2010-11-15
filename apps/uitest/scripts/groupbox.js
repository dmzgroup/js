var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , groupBox: require("dmz/ui/groupBox")
          , layout: require('dmz/ui/layout')
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts
  , form = dmz.ui.loader.load("./scripts/GroupBoxForm.ui")
  , hbox
  , vbox
  , gb1
  , gb2
  , label
  ;

puts("Script:", self.name);

form.show();
form.layout(dmz.ui.layout.createVBoxLayout());
gb1 = dmz.ui.groupBox.create("GB1", form);
gb2 = dmz.ui.groupBox.create("GB2", form);

form.observe(self, "b0", "clicked", function () {

   vbox = dmz.ui.layout.createVBoxLayout()
   vbox.addWidget(form.lookup("rb0"));
   vbox.addWidget(form.lookup("rb1"));
   gb1.layout(vbox);
   gb1.show();
});

form.observe(self, "b1", "clicked", function () {

   vbox = dmz.ui.layout.createVBoxLayout()
   vbox.addWidget(form.lookup("rb2"));
   vbox.addWidget(form.lookup("rb3"));
   gb2.layout(vbox);
   gb2.show();
});

form.observe(self, "b2", "clicked", function () {

   hbox = dmz.ui.layout.createHBoxLayout();
   hbox.addWidget(gb1);
   hbox.addWidget(gb2);
});

form.observe(self, "b3", "clicked", function () {

   vbox = dmz.ui.layout.createVBoxLayout();
   vbox.addLayout(hbox);
   hbox = dmz.ui.layout.createHBoxLayout();
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

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});
