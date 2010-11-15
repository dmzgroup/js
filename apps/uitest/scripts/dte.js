var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
       }
  , _main
  , _exports = {}
  , puts = require('sys').puts
  , form = dmz.ui.loader.load("./scripts/DateTimeEditForm.ui")
  , dte = form.lookup ("dte")
  , de = form.lookup ("de")
  , te = form.lookup ("te")
  ;

puts("Script: " + self.name);

form.show();

var x = Date.now();

puts ("x:", x);

dte.dateTime (new Date(x));
puts (dte.minimum (new Date(x)));
dte.observe(self, "dateTimeChanged", function (date) {
   puts ("x:", x);
   puts ("time:", date);
});

de.dateTime (new Date(2008, 10, 12, 5, 6, 7));
de.observe(self, "dateTimeChanged", function (date) {
   puts ("de time:", date);
});

te.dateTime (new Date(2012, 12, 12));
puts(te.maximum (new Date(2012, 12, 13, 5)));
te.observe(self, "dateTimeChanged", function (date) {
   puts ("te time:", date);
});

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage (self.name, form);
   }
});

puts("Done.");
