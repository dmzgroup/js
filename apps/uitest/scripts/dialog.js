var puts = require('sys').puts
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/TestDialog.ui");
form.open(self, function () {
   puts("dialog closed");
});

puts("Done.");
