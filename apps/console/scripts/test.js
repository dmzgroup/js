var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , button1
  , button2
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
self.form = form;
form.show();

button2 = form.lookup("button2");
button2.observe(self, "clicked", function () {
   button2.text("Don't Push Me!!!");
   timer.setTimer(self, 2, function () {
      button2.text("Push Me");
   });
});

button1 = form.lookup("button1");
button1.observe (self, "clicked", function () {
   puts("form close");
   // form.close();
});


// timer.setRepeatingTimer(self, 10, function () {
   // var btn1 = button;
   // var form1 = form;
// });

puts("Done.");
