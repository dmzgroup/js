var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , frame: require("dmz/ui/frame")
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
   , _main
   , _exports = {}
   , puts = require('sys').puts
   , form = dmz.ui.loader.load("./scripts/lcd.ui")
  , form2
  , lcd
  , b0
  , b1
  , b2
  ;

puts("Script: " + self.name);

form.show();

b0 = form.lookup("b0");
b1 = form.lookup("b1");
b2 = form.lookup("b2");

lcd = form.lookup("lcd");

lcd.value(100);
lcd.value(lcd.value() * 2);

b0.observe(self, "clicked", function () {

   lcd.frameShadow(dmz.ui.frame.ShadowSunken);
   lcd.frameShape(dmz.ui.frame.ShapePanel);
   lcd.lineWidth(2);
   lcd.midLineWidth(3);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

b1.observe(self, "clicked", function () {

   lcd.frameShadow(dmz.ui.frame.ShadowRaised);
   lcd.frameShape(dmz.ui.frame.ShapeBox);
   lcd.lineWidth(3);
   lcd.midLineWidth(1);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

b2.observe(self, "clicked", function () {

   lcd.frameShadow(dmz.ui.frame.ShadowPlain);
   lcd.frameShape(dmz.ui.frame.ShapeStyledPanel);
   lcd.lineWidth(1);
   lcd.midLineWidth(2);
   puts("frame width:", lcd.frameWidth(), lcd.lineWidth(), lcd.midLineWidth());
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
