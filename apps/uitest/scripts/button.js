var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , array
  , cb
  , rb1
  , rb2
  , pb
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/CheckBoxForm.ui");
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

puts("Done.");
