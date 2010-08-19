var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , button
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
form.show();

button = form.lookup("button1");

button.observe(self, "clicked", function () {
   puts("=-=-=-=-=- button clicked");
});


timer.setRepeatingTimer(self, function () {
   var form1 = form;
//   form.show()
});

puts("Done.");