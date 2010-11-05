var dmz = 
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          }
       }
   , _exports = {}
   , _form = dmz.ui.loader.load('button')
   , _main
   , _clicked
   , _toggled
   ;

_clicked = function (btn) { _main.print(self.name, 'clicked:', btn); }

_form.observe(self, "pushButton", "clicked", _clicked);
_form.observe(self, "toolButton", "clicked", _clicked);
_form.observe(self, "commandLinkButton", "clicked", _clicked);
_form.observe(self, "radioButton", "clicked", _clicked);
_form.observe(self, "radioButton_2", "clicked", _clicked);
_form.observe(self, "radioButton_3", "clicked", _clicked);
_form.observe(self, "radioButton_4", "clicked", _clicked);
_form.observe(self, "checkBox", "clicked", _clicked);
_form.observe(self, "checkBox_2", "clicked", _clicked);

_toggled = function (val, btn) { _main.print(self.name, 'toggled', val, btn); }

_form.observe(self, "checkBox", "toggled", _toggled);
_form.observe(self, "checkBox_2", "toggled", _toggled);

dmz.module.subscribe(self, "main", function (Mode, module) {
   
   if (Mode === dmz.module.Activate) {
      
      _main = module
      _main.addPage (_form.title(), _form);
   }
});
