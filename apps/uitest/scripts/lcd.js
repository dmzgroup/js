var puts = require('sys').puts
  , timer = require('dmz/runtime/time')
  , ui = require('dmz/components/ui')
  , form
  , lcd
  ;

puts("Script: " + self.name);

form = ui.load("./scripts/lcd.ui");
form.show();

lcd = form.lookup("lcd");

lcd.value(100);
lcd.value(lcd.value() * 2);

puts("Done.");
