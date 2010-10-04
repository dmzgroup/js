var puts = require('sys').puts
  , ui = require('dmz/components/ui')
  , mainForm = ui.load('./scripts/main.ui')
  , listWidget = mainForm.lookup('listWidget')
  , stackedWidget = mainForm.lookup('stackedWidget')
  , messageBox = require('dmz/components/ui/messageBox')
  , index = 0
  ;

puts('Script: ' + self.name);

function dump (obj) {
   puts('-d-u-m-p----------------------');
   puts('objectName: ' + obj.objectName ());
   puts('className: ' + obj.className ());
   puts('------------------------------');
}

function add_script (script) {
   var form = ui.load('./scripts/' + script + '.ui');
   form.objectName(script);
   listWidget.addItem(script);
   stackedWidget.add(form);
}

listWidget.observe(self, 'currentRowChanged', function (row) {
   index = row;
   // puts('row: ' + index);
   stackedWidget.currentIndex(index);
   
   var label = mainForm.lookup('infoLabel');
   if (label) { label.text(index);}
});

mainForm.observe(self, 'doneButton', 'clicked', function (button) {
   button.parent().close();
});

// mainForm.observe(self, 'prevButton', 'clicked', function (btn) {
// });

mainForm.observe(self, 'nextButton', 'clicked', function (button) {
   var mb = messageBox.create(
      button.parent(),
      {
         type: messageBox.Information,
         text: "Some Text",
         informativeText: "This is some informative Text",
         standardButtons: [messageBox.Ok, messageBox.Cancel, messageBox.Save],
         defaultButton: messageBox.Cancel
      }
   );
   
   mb.open(self, function (val) {
      if (val === messageBox.Ok) {
         
         puts("mb: OK pressed");
      }
      else if (val === messageBox.Save) {
         
         puts("mb: Save pressed");
      }
      else if (val === messageBox.Cancel) {
         
         puts("mb: Cancel pressed");
      }
   });
});

dump(mainForm);
dump(listWidget);
dump(stackedWidget);

add_script('Slider');
add_script('Widgets');


mainForm.observe(self, 'comboBox', 'currentIndexChanged', function (value, comboBox) {
   
   // puts('comboBox: ' + value);
   var comboBoxLabel = mainForm.lookup ('comboBoxLabel');
   if (comboBoxLabel) { comboBoxLabel.text (comboBox.itemText(value)); }
});

mainForm.observe(self, 'lineEdit', 'textChanged', function (value) {
   
   // puts('lineEdit: ' + value);
   var lineEditLabel = mainForm.lookup ('lineEditLabel');
   if (lineEditLabel) { lineEditLabel.text (value); }
});

mainForm.observe(self, 'spinBox', 'valueChanged', function (value) {
   
   // puts('spinBox: ' + value);
   var spinBoxLabel = mainForm.lookup ('spinBoxLabel');
   if (spinBoxLabel) { spinBoxLabel.text (value); }
});

mainForm.observe(self, 'doubleSpinBox', 'valueChanged', function (value) {
   
   // puts('doubleSpinBox: ' + value);
   var doubleSpinBoxLabel = mainForm.lookup ('doubleSpinBoxLabel');
   if (doubleSpinBoxLabel) { doubleSpinBoxLabel.text (value); }
});

function sliderUpdate (val) {
   
   // puts('slider update: ' + val);

   var lcd = mainForm.lookup ('lcd');
   if (lcd) { lcd.property ('value', val); }
   
   var progressBar = mainForm.lookup ('progressBar');
   if (progressBar) { 
      // progressBar.value (val);
      progressBar.property ('value', val);
   }

   var dial = mainForm.lookup ('dial');
   if (dial) { dial.value (val); }

   var slider = mainForm.lookup ('slider');
   if (slider) { slider.value (val); }
}

mainForm.observe(self, 'slider', 'valueChanged', sliderUpdate);
mainForm.observe(self, 'dial', 'valueChanged', sliderUpdate);

stackedWidget.currentIndex(0);

mainForm.show();

puts('Done with: ' + self.name);
