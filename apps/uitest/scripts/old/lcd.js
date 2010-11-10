var puts = require('sys').puts
  , uiLoader = require('dmz/ui/uiLoader')
  , frame = require("dmz/ui/frame")
  , form
  , form2
  , lcd
  , b0
  , b1
  , b2
  ;

puts("Script: " + self.name);

form = uiLoader.load("./scripts/lcd.ui");
form.show();

b0 = form.lookup("b0");
b1 = form.lookup("b1");
b2 = form.lookup("b2");

lcd = form.lookup("lcd");

lcd.value(100);
lcd.value(lcd.value() * 2);

b0.observe(self, "clicked", function () {

   lcd.frameShadow(frame.ShadowSunken);
   lcd.frameShape(frame.ShapePanel);
   lcd.lineWidth(2);
   lcd.midLineWidth(3);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

b1.observe(self, "clicked", function () {

   lcd.frameShadow(frame.ShadowRaised);
   lcd.frameShape(frame.ShapeBox);
   lcd.lineWidth(3);
   lcd.midLineWidth(1);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

b2.observe(self, "clicked", function () {

   lcd.frameShadow(frame.ShadowPlain);
   lcd.frameShape(frame.ShapeStyledPanel);
   lcd.lineWidth(1);
   lcd.midLineWidth(2);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

puts("Done.");
