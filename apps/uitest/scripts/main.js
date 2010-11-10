var dmz = 
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , layout: require("dmz/ui/layout")
          , loader: require('dmz/ui/uiLoader')
          , mainWindow: require('dmz/ui/mainWindow')
          , messageBox: require('dmz/ui/messageBox')
          , widget: require("dmz/ui/widget")
          }
       }
   , _log = require('sys').puts
   , _print
   , _exports = {}
   , _table = {}
   , _form = dmz.ui.loader.load('main')
   , _list = _form.lookup('listWidget')
   , _stack = _form.lookup('stackedWidget')
   , _index = 0
   , _widget
   , _layout
   ;

_list.observe(self, 'currentRowChanged', function (row) {
   _index = row;
   _stack.currentIndex(_index);
   _print(self.name, 'currentRowChanged', _index)
   var label = _form.lookup('infoLabel');
   label.text(_index);
});

_form.observe(self, 'doneButton', 'clicked', function (button) {
   
   var mb = dmz.ui.messageBox.create(
      { type: dmz.ui.messageBox.Information
      , text: "Are you done?"
      , informativeText: "Click <b>Ok</b> to quit!"
      , standardButtons: [dmz.ui.messageBox.Ok, dmz.ui.messageBox.Cancel]
      , defaultButton: dmz.ui.messageBox.Ok
      }
      , _form
   );

   mb.open(self, function (val) {
      if (val === dmz.ui.messageBox.Ok) {

         dmz.ui.mainWindow.close();
      }
      else if (val === dmz.ui.messageBox.Cancel) {

         _print("dmz.ui.messageBox.Cancel");
      }
   });
});

_stack.currentIndex(0);

dmz.ui.mainWindow.centralWidget (_form);

dmz.module.subscribe(self, 'log', function (Mode, module) {

   if (Mode === dmz.module.Activate) {
      
      _log = module.print;
   }
});


_exports.addPage = function (name, widget) {
   
   if (name && widget) {
      
      _table[name] = 
         { name: name
         , widget: widget
         , index: _stack.add(widget)
         };

      _list.addItem(name);
      
      _print(self.name, 'addPage', name, widget);
   }
};

_print = function () {
   
   var message = ''
     , separator = ' '
     ;
   
   for(var ix = 0; ix < arguments.length; ix++) {
      
      message += arguments[ix] + separator;
   } 
   
   _log(message);
}

_exports.print = _print;

dmz.module.publish(self, _exports);
