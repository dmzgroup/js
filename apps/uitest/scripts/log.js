var dmz = 
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , mainWindow: require('dmz/ui/mainWindow')
          }
       }
   , _exports = {}
   , _form = dmz.ui.loader.load('log')
   , _dock = dmz.ui.mainWindow.createDock
       (_form.title()
       , { area: dmz.ui.consts.BottomToolBarArea
         , visible: true
         }
       , _form
       )
   , _log = _form.lookup("textEdit")
   ;

_exports.print = function (message) {

   _log.append(message);
};


dmz.module.publish(self, _exports);
