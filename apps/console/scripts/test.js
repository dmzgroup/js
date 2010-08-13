var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
form.show();

timer.setRepeatingTimer(self, function () {
   var form1 = form;
//   form.show()
});

puts("Done.");