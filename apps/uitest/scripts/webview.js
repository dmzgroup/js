var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , event: require("dmz/ui/event")
          , graph: require("dmz/ui/graph")
          , webview: require("dmz/ui/webView")
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
//       , util: require("dmz/types/util")
//       , graphlib: require("dmz/types/graph")
       }
   , _main
   , _exports = {}
   , xyGraph
   , puts = require('sys').puts
   , form = dmz.ui.widget.create()
   , webview = dmz.ui.webview.create(form)
   , func
   ;

form.show();

puts ("Webview script");

webview.url ("http://dev.chds.us/?dystopia:map2");

webview.contextMenuPolicy (dmz.ui.consts.NoContextMenu);

dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      _main = module
      _main.addPage ("Webview", form);
   }
});

webview.eventFilter (self, function (object, event) {

   if (event.type() == dmz.ui.event.MouseButtonPress) {

      puts ("type:", event.type());
      puts ("button:", event.button());
   }
   return false;
});

puts ("end script");
