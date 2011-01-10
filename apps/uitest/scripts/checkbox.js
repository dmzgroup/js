var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
   , _main
   , _exports = {}
   , form = dmz.ui.loader.load("./scripts/CheckBoxForm.ui")
   , puts = require('sys').puts
   , array
   , cb
   , rb1
   , rb2
   , pb
   ;

puts("Script: " + self.name);

form.show();

cb = form.lookup("checkBox");
rb1 = form.lookup("radioButton");
rb2 = form.lookup("radioButton_2");
pb = form.lookup("pushButton");

array =
   [ {var: cb, name: "cb"}
   , {var: rb1, name: "rb1"},
   , {var: rb2, name: "rb2"},
   , {var: pb, name: "pb"}
   ];

array.forEach(function (obj) {
   obj.var.observe(self, "clicked", function () { puts(obj.name, "clicked");});
   obj.var.observe(self, "toggled", function (val) {
      puts(obj.name, "toggled", val);
   });
   puts(obj.name);
   obj.var.text(obj.name);
   puts("isChecked:", obj.var.isChecked());
   obj.var.click();
   obj.var.setChecked(true);
   puts("isChecked:", obj.var.isChecked());
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (form.title(), form);
   }
});

puts("Done.");
