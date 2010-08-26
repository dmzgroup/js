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
      button1.enabled(true);
      button1.show();
   });
});

button1 = form.lookup("button1");
button1.observe (self, "clicked", function () {
   button1.text(self.name);
   button1.enabled(false);
   button1.hide();
});

// lw = form.lookup("list1");
// lw.observer (self, "itemActivated", function (item) {
//    puts(item.name);
// });
// lw.observer(self, "currentChanged", function(curr, prev) {
//    
// });

// timer.setRepeatingTimer(self, 10, function () {
   // var btn1 = button;
   // var form1 = form;
// });

puts("Done.");
