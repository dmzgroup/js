var puts = require('sys').puts
  , ui = require('dmz/components/ui')
  , form
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestForm.ui");
form.show();

puts("Done.");