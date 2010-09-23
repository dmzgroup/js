var puts = require('sys').puts
  , ui = require('dmz/components/ui')
  , form
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/TestDialog.ui");
form.open(self, function () {
   puts("dialog closed");
});

puts("Done.");
