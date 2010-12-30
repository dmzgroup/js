var dmz =
       { module: require('dmz/runtime/module')
       , ui:
          { consts: require('dmz/ui/consts')
          , loader: require('dmz/ui/uiLoader')
          , graph: require("dmz/ui/graph")
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
   , GraphForm = dmz.ui.loader.load("./scripts/GraphForm.ui")
   , rect
   , path
   , text
   , line
   , view
   , scene
   , brush
   , pen
   , painterPath

   , values
   , values2
   , idx

   , createXYGraph
   , createHLine
   , createVLine
   , updateXYGraphBars
   , XYGraph
   , createBar
   , drawPowerLaw


   , mbraData = [0,95.77464788732394,85.91549295774648,79.92957746478872,74.29577464788731,70.4225352112676,65.49295774647887,64.08450704225352,61.97183098591549,61.97183098591549,61.61971830985915,61.61971830985915,61.61971830985915,61.267605633802816,61.267605633802816,61.267605633802816,60.56338028169014,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,58.098591549295776,51.40845070422535,45.774647887323944,42.6056338028169,38.38028169014084,32.74647887323944,26.408450704225352,23.239436619718308,18.661971830985916,16.901408450704224,15.492957746478872,14.084507042253518,13.732394366197182,13.028169014084506,11.619718309859154,9.507042253521124,9.154929577464788,8.80281690140845,8.80281690140845,8.80281690140845,8.098591549295774,7.746478873239436,7.042253521126759,5.28169014084507,4.577464788732394,3.873239436619718,3.5211267605633796,3.5211267605633796,3.5211267605633796,3.5211267605633796,3.5211267605633796,2.816901408450704,2.464788732394366,2.112676056338028,2.112676056338028,2.112676056338028,1.7605633802816902,1.7605633802816902,1.4084507042253522,1.056338028169014,1.056338028169014,1.056338028169014,1.056338028169014,1.056338028169014,0.7042253521126761,0.7042253521126761,0.7042253521126761,0.7042253521126761,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
   ;

puts("Script: " + self.name);


createHLine = function (x, y, len) { return dmz.ui.graph.createLineItem(x, y, x + len, y); }
createVLine = function (x, y, height) { return dmz.ui.graph.createLineItem(x, y, x, -(-y + height)); }
createBar = function (Value, XLabel, XPos, YPos, XAxis, Width, BarHeight, LabelRotation, Pen, Brush) {

   var bar
     , label
     , valueLabel
     , rect
     , height
     , result = {}
     ;

   if (!Width) { Width = 20; }
   if (!BarHeight) { BarHeight = 130; }
   if (!LabelRotation) { LabelRotation = 0; }
   else if (LabelRotation > 90) { LabelRotation = 90; }
   else if (LabelRotation < -45) { LabelRotation = -45; }
   if (!Pen) { Pen = dmz.ui.graph.createPen({ r: 0.5, g: 0, b: 0 }); }
   if (!Brush) { Brush = dmz.ui.graph.createBrush({ r: 0.3, g: 0.8, b: 0.3 }); }

   height = -(BarHeight * Value);

   bar = dmz.ui.graph.createRectItem (XPos, YPos, Width, height, XAxis);
   bar.pen(Pen);
   bar.brush(Brush);
   bar.z(0);

   label = dmz.ui.graph.createTextItem (XLabel, bar);
   rect = label.boundingRect();
   label.pos (XPos + ((rect.width * Math.cos(90 - LabelRotation)) + (rect.height * Math.sin(LabelRotation))) / 2, 20);
   label.rotation(LabelRotation);

   valueLabel = dmz.ui.graph.createTextItem (Math.round(Value * 100).toString(), bar);
   rect = valueLabel.boundingRect();
   valueLabel.pos(XPos + (Width - rect.width) / 2, -rect.height + height);

   result.image = bar;
   result.height = height;
   result.value = Value;
   result.label = label;
   result.valueLabel = valueLabel;

   return result;
};

drawPowerLaw = function (xyGraph) {

   var idx = 0
     , startIndex
     , powerPath
     , p = 0
     , q = 0
     , nonZeroCount = 0
     , x = 0
     , y = 0
     , sumX = 0
     , sumY = 0
     , sumXY = 0
     , sumX2 = 0
     , pct
     , foundStartIndex = false
     , scaleHeightMax
     , local_power = function (p, q, x) { return (x <= 0) ? p : p * Math.pow(x, q); }
     ;

   for (idx = 0; idx < xyGraph.bars.length && !foundStartIndex; idx += 1) {

      if (xyGraph.bars[idx].value != 1) {

         if (idx == 0) { startIndex = 0; }
         else { startIndex = idx - 1; }
         foundStartIndex = true;
      }
   }

   puts ("startIndex:", startIndex);

   for (idx = startIndex; idx < xyGraph.bars.length; idx += 1) {

      if (idx == startIndex) { x = 1; }
      else { x = Math.log (idx - startIndex); }

      if (xyGraph.bars[idx].height < 0) {

         y = Math.log (xyGraph.bars[idx].value * 100);
         puts ("y:", y, xyGraph.bars[idx].value);
         sumY += y;
         sumX += x;
         sumXY += x * y;
         sumX2 += x * x;
         nonZeroCount += 1;
      }
   }

   if (nonZeroCount > 0) {

      q = ((nonZeroCount * sumXY) - (sumX * sumY)) /
            ((nonZeroCount * sumX2) - (sumX * sumX));

      p = Math.exp ((sumY - (q * sumX)) / nonZeroCount);
   }

   puts
      ( "nZC:", nonZeroCount
      , "sumY:", sumY
      , "sumX", sumX
      , "sumXY", sumXY
      , "sumX2:", sumX2
      , "q:", q
      , "p:", p
      )

   powerPath = dmz.ui.graph.createPainterPath();
   scaleHeightMax = -local_power (p, q, startIndex);

   puts ("startIndex:", startIndex);
   x = xyGraph.spaceWidth + (xyGraph.barWidth + xyGraph.spaceWidth) * startIndex;
   y = xyGraph.bars[startIndex].image.boundingRect().height;
   powerPath.moveTo (x + (xyGraph.barWidth / 2), y);
   for (idx = startIndex + 1; idx < xyGraph.bars.length; idx += 1) {

      x = xyGraph.spaceWidth + (xyGraph.barWidth + xyGraph.spaceWidth) * idx;
      y = local_power (p, q, idx - startIndex + xyGraph.startValue) / scaleHeightMax * xyGraph.barHeight;
      powerPath.lineTo(x + (xyGraph.barWidth / 2), y);
   }

   if (powerPath.count() > 1) {

      xyGraph.powerLawPath = dmz.ui.graph.createPathItem (powerPath);
      xyGraph.powerLawPath.z(2);
      xyGraph.powerLawExponentLabel = dmz.ui.graph.createTextItem("Exponent = " + (-q));
      xyGraph.powerLawExponentLabel.pos(260, -xyGraph.barHeight - 60);
      xyGraph.scene.addItem(xyGraph.powerLawExponentLabel);
      xyGraph.scene.addItem(xyGraph.powerLawPath);
   }
};

updateXYGraphBars = function (xyGraph, values) {

   var lastBar
     , bar
     , maxValue = 0
     , value
     , idx
     , startIndex
     , xpos
     , ypos
     , label
     , length
     , powerPath
     ;

   while (values[values.length - 1] == 0) { values.pop(); }

   for (idx = xyGraph.startValue; idx < values.length; idx += 1) {

      if (values[idx] > maxValue) { maxValue = values[idx]; }
   }

   while (xyGraph.bars && xyGraph.bars.length) {

      bar = xyGraph.bars.pop();
//      xyGraph.scene.removeItem(bar.valueLabel);
//      xyGraph.scene.removeItem(bar.label);
      xyGraph.scene.removeItem(bar.image);
      xyGraph.scene.addItem(bar.image);
   }
   if (xyGraph.powerLawPath) { xyGraph.scene.removeItem (xyGraph.powerLawPath); }
   if (xyGraph.powerLawExponentLabel) { xyGraph.scene.removeItem (xyGraph.powerLawExponentLabel); }

   delete xyGraph.bars;
   xyGraph.bars = [];

   for (idx = xyGraph.startValue; idx < values.length; idx += 1) {

      xpos = xyGraph.spaceWidth + (xyGraph.barWidth + xyGraph.spaceWidth) * (idx - xyGraph.startValue);
      ypos = 0;
      value = values[idx] / maxValue;
      label = idx.toString();
      bar = createBar
         ( value
         , label
         , xpos
         , ypos
         , xyGraph.xAxis
         , xyGraph.barWidth
         , xyGraph.barHeight
         , xyGraph.xLabelRotation
         , xyGraph.barStroke
         , xyGraph.barFill
         );

      xyGraph.bars.push(bar);
   }

   while (Math.round(xyGraph.bars[xyGraph.bars.length - 1].value * 100) == 0) {

      xyGraph.scene.removeItem (xyGraph.bars.pop().image);
   }

   if (xyGraph.drawPowerLaw) { drawPowerLaw(xyGraph); }


   var rect = xyGraph.xAxis.childrenBoundingRect();
   var line = xyGraph.xAxis.line();
   xyGraph.xAxis.line(line.x1, line.y2, rect.width, line.y2);
}


createXYGraph = function () {

   var GraphForm = dmz.ui.loader.load("./scripts/GraphForm.ui")
     , scene = dmz.ui.graph.createScene(-50, 50, 50, 1)
     , startValue = 1
     , drawPowerLaw = true
     , divisions = 100
     , yLabels = []
     , xAxisLabel = ""
     , yAxisLabel = ""
     , steps
     , barStroke = dmz.ui.graph.createPen({ r: 0.5, g: 0, b: 0 })
     , barFill = dmz.ui.graph.createBrush({ r: 0.3, g: 0.8, b: 0.3 })
     , ix
     , offset
     , xAxis
     , yAxis
     , maxCount = 0
     , totalCount = 0
     , maxBarCount = 0
     , ascendingOrder = true
     , barWidth = 12.5
     , barHeight = 130
     , spaceWidth = 5
     , yDivisions = 4
     , xLabelRotation = 90
     , text
     , rect
     , line
     , result = {}
     , whitePen = dmz.ui.graph.createPen({ r: 1, g: 1, b: 1 })
     , spacer

     ;


   result.form = GraphForm;
   result.scene = scene;

   GraphForm.lookup("graphicsView").scene (scene);

   for (ix = 0; ix < yDivisions; ix += 1) {

      offset = -barHeight * (ix + 1) / yDivisions;
      text = dmz.ui.graph.createTextItem((100 * (ix + 1) / yDivisions) + "%");
      text.z (1);

      rect = text.boundingRect();
      text.pos(-rect.width - 10, offset - (rect.height * 0.5));
      scene.addItem(text)
      line = createHLine (0, offset, -10);
      line.z(1);
      scene.addItem(line);
      yLabels.push(text);
   }

   xAxisLabel = dmz.ui.graph.createTextItem("X Axis");
   xAxisLabel.pos (0, 0);
   scene.addItem (xAxisLabel);

   yAxisLabel = dmz.ui.graph.createTextItem("Y Axis");
   yAxisLabel.pos(0, -barHeight - 40);
   scene.addItem(yAxisLabel);

   result.xAxisLabel = xAxisLabel;
   result.yAxisLabel = yAxisLabel;
   result.yLabels = yLabels;

   yAxis = createVLine(0, 0, barHeight);
   yAxis.z (1);
   scene.addItem (yAxis);

   xAxis = createHLine(0, 0, 40);
   xAxis.z (1);
   scene.addItem(xAxis);

   result =
      { form: GraphForm
      , scene: scene
      , xAxisLabel: xAxisLabel
      , yAxisLabel: yAxisLabel
      , yLabels: yLabels
      , steps: steps
      , startValue: startValue
      , xAxis: xAxis
      , yAxis: yAxis
      , barStroke: barStroke
      , barFill: barFill
      , ascendingOrder: ascendingOrder
      , spaceWidth: spaceWidth
      , xLabelRotation: xLabelRotation
      , barWidth: barWidth
      , barHeight: barHeight
      , drawPowerLaw: drawPowerLaw
      };
//   var bar = createBar(1, "1", spaceWidth, 0, xAxis, barWidth, barHeight, -45, barStroke, barFill);
//   bar = createBar(.9, "2", barWidth + spaceWidth + spaceWidth, 0, xAxis, barWidth, barHeight, -45, barStroke, barFill);
//   bar = createBar(.8, "3", (barWidth + spaceWidth) * 2 + spaceWidth, 0, xAxis, barWidth, barHeight, -45, barStroke, barFill);
//   bar = createBar(.7, "4", (barWidth + spaceWidth) * 3 + spaceWidth, 0, xAxis, barWidth, barHeight, -45, barStroke, barFill);
//   bar = createBar(.6, "5", (barWidth + spaceWidth) * 4 + spaceWidth, 0, xAxis, barWidth, barHeight, -45, barStroke, barFill);


   return result;
}



dmz.module.subscribe(self, "main", function (Mode, module) {

   if (Mode === dmz.module.Activate) {

      xyGraph = createXYGraph();
      _main = module
      _main.addPage (self.name, xyGraph.form);

      idx = 0;
      values = [];
      values2 = [];

      for (idx = 0; idx < 100; idx += 1) { values[idx] = idx; }
//      for (idx = 0; idx < 5; idx += 1) { values[idx] = idx; }
      for (idx = 99; idx >= 0; idx -= 1) { values2[idx] = 99 - idx; }
      dmz.timer.setTimer(self, 0, function () { updateXYGraphBars(xyGraph, values); });
      dmz.timer.setTimer(self, 5, function () { updateXYGraphBars(xyGraph, mbraData); });
//      updateXYGraphBars(xyGraph, mbraData);
   }
});

// Following code is a start on conversion to a library

//exports.createHLine = createHLine;
//exports.createVLine = createVLine;

//XYGraph = function
//   ( GraphForm
//   , startValue
//   , divisions
//   , xAxisLabel
//   , yAxisLabel
//   , barStrokeColor
//   , barFillColor
//   , barWidth
//   , barHeight
//   , spaceWidth
//   , yDivisions
//   , drawPowerLaw
//   )
//{

//   if (!bars) { bars = []; }



//};

//exports.create = function
//   ( GraphForm
//   , startValue
//   , divisions
//   , xAxisLabel
//   , yAxisLabel
//   , barStrokeColor
//   , barFillColor
//   , barWidth
//   , barHeight
//   , spaceWidth
//   , yDivisions
//   , drawPowerLaw
//   )
//{

//   var result = new XYGraph (arguments);
//   return result;


//};
//exports.isTypeOf = function (value) {

//   return XYGraph.prototype.isPrototypeOf(value) ? value : undefined;
//};

//XYGraph.prototype.create = exports.create;
//XYGraph.prototype.copy = function () {


//   return this.create
//      ( this.GraphForm
//      , this.startValue
//      , this.divisions
//      , this.xAxisLabel
//      , this.yAxisLabel
//      , this.barStrokeColor
//      , this.barFillColor
//      , this.barWidth
//      , this.barHeight
//      , this.spaceWidth
//      , this.yDivisions
//      , this.drawPowerLaw
//      );
//}

//XYGraph.prototype.toString = function () {

//   return "[ XYGraph:" +
//      " X: " + this.xAxisLabel +
//      " Y: " + this.yAxisLabel +
//      " start: " + this.startValue +
//      " divs:" + this.divisions +
//      " barHeight: " + this.barHeight +
//      " barWidth: " + this.barWidth +
//      " drawPowerLaw: " + this.drawPowerLaw +
//      " ]";
//}

puts ("Done");
