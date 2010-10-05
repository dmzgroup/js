var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , uiLoader = require('dmz/components/ui/uiLoader')
  , form
  , lcd
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/lcd.ui");
form.show();

lcd = form.lookup("lcd");

lcd.value(100);
lcd.value(lcd.value() * 2);

puts("Done.");
