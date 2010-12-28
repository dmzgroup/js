var dmz =
       { ui:
          { consts: require('dmz/ui/consts')
          , layout: require("dmz/ui/layout")
          , loader: require('dmz/ui/uiLoader')
          , graph: require("dmz/ui/graph")
          , widget: require("dmz/ui/widget")
          }
       , timer: require('dmz/runtime/time')
//       , util: require("dmz/types/util")
       , sys: require("sys")
       }
   , xyGraph
   , puts = dmz.sys.puts
//   , GraphForm = dmz.ui.loader.load("./scripts/GraphForm.ui")
//   , _self = dmz.sys.createSelf("GraphLib-" + dmz.sys.createUUID())
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
   , update
   , XYGraph
   , createBar
   , drawPowerLaw
   , createGraphForm


//   , mbraData = [0,95.77464788732394,85.91549295774648,79.92957746478872,74.29577464788731,70.4225352112676,65.49295774647887,64.08450704225352,61.97183098591549,61.97183098591549,61.61971830985915,61.61971830985915,61.61971830985915,61.267605633802816,61.267605633802816,61.267605633802816,60.56338028169014,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,60.2112676056338,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,59.859154929577464,58.098591549295776,51.40845070422535,45.774647887323944,42.6056338028169,38.38028169014084,32.74647887323944,26.408450704225352,23.239436619718308,18.661971830985916,16.901408450704224,15.492957746478872,14.084507042253518,13.732394366197182,13.028169014084506,11.619718309859154,9.507042253521124,9.154929577464788,8.80281690140845,8.80281690140845,8.80281690140845,8.098591549295774,7.746478873239436,7.042253521126759,5.28169014084507,4.577464788732394,3.873239436619718,3.5211267605633796,3.5211267605633796,3.5211267605633796,3.5211267605633796,3.5211267605633796,2.816901408450704,2.464788732394366,2.112676056338028,2.112676056338028,2.112676056338028,1.7605633802816902,1.7605633802816902,1.4084507042253522,1.056338028169014,1.056338028169014,1.056338028169014,1.056338028169014,1.056338028169014,0.7042253521126761,0.7042253521126761,0.7042253521126761,0.7042253521126761,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0.35211267605633806,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
   ;

puts(Object.keys(dmz.sys));

XYGraph = function
   ( GraphForm
   , startValue
   , divisions
   , xAxisLabel
   , yAxisLabel
   , barStroke
   , barFill
   , barWidth
   , barHeight
   , spaceWidth
   , yDivisions
   , showPowerLaw
   )
{

//   var GraphForm = dmz.ui.loader.load("./scripts/GraphForm.ui")
   var GraphForm = createGraphForm()
     , scene = dmz.ui.graph.createScene(-50, 50, 50, 1)
     , ix
     , offset
     , xAxis
     , yAxis
     , maxCount = 0
     , totalCount = 0
     , maxBarCount = 0
     , text
     , rect
     , line
     , result = {}
     , whitePen = dmz.ui.graph.createPen({ r: 1, g: 1, b: 1 })
     , spacer

     ;

   if (!startValue) { startValue = 0; }
   if (!divisions) { divisions = 100; }
   if (!yAxisLabel) { yAxisLabel = ""; }
   if (!xAxisLabel) { xAxisLabel = ""; }
   if (!barStroke && !(barStroke.r && barStroke.g && barStroke.b)) {

      barStroke = dmz.ui.graph.createPen({ r: 0.5, g: 0, b: 0 });
   }
   if (!barFill && !(barFill.r && barFill.g && barStroke.b)) {

      barFill = dmz.ui.graph.createBrush({ r: 0.3, g: 0.8, b: 0.3 });
   }
   if (!barWidth) { barWidth = 12.5; }
   if (!barHeight) { barHeight = 130; }
   if (!spaceWidth) { spaceWidth = 5; }
   if (!yDivisions) { yDivisions = 4; }
   if (!xLabelRotation) { xLabelRotation = 0; }
   if (showPowerLaw !== true) { showPowerLaw = false; }

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
      , startValue: startValue
      , xAxis: xAxis
      , yAxis: yAxis
      , barStroke: barStroke
      , barFill: barFill
      , spaceWidth: spaceWidth
      , xLabelRotation: xLabelRotation
      , barWidth: barWidth
      , barHeight: barHeight
      , showPowerLaw: showPowerLaw
      };

   return result;
};

exports.createGraphForm = createGraphForm;
createGraphForm = function () {

   var form = dmz.ui.widget.create()
     , layout = dmz.ui.layout.createVBoxLayout ()
     , hlayout = dmz.ui.layout.createHBoxLayout ()
     , exportButton = dmz.ui.button.createPushButton("Export")

     , exportFunction = function () { puts ("PLACEHOLDER EXPORT FUNCTION"); }
     ;

//   exportButton.observe(_self, "clicked", exportFunction);
   layout.addWidget (dmz.ui.graph.createView());
   hlayout.addWidget(exportButton);
   layout.addLayout (hlayout);
   form.layout(layout);
};

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

XYGraph.prototype.drawPowerLaw = function () {

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

   for (idx = 0; idx < this.bars.length && !foundStartIndex; idx += 1) {

      if (this.bars[idx].value != 1) {

         if (idx == 0) { startIndex = 0; }
         else { startIndex = idx - 1; }
         foundStartIndex = true;
      }
   }

   puts ("startIndex:", startIndex);

   for (idx = startIndex; idx < this.bars.length; idx += 1) {

      if (idx == startIndex) { x = 1; }
      else { x = Math.log (idx - startIndex); }

      if (this.bars[idx].height < 0) {

         y = Math.log (this.bars[idx].value * 100);
         puts ("y:", y, this.bars[idx].value);
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
   x = this.spaceWidth + (this.barWidth + this.spaceWidth) * startIndex;
   y = this.bars[startIndex].image.boundingRect().height;
   powerPath.moveTo (x + (this.barWidth / 2), y);
   for (idx = startIndex + 1; idx < this.bars.length; idx += 1) {

      x = this.spaceWidth + (this.barWidth + this.spaceWidth) * idx;
      y = local_power (p, q, idx - startIndex + this.startValue) / scaleHeightMax * this.barHeight;
      powerPath.lineTo(x + (this.barWidth / 2), y);
   }

   if (powerPath.count() > 1) {

      this.powerLawPath = dmz.ui.graph.createPathItem (powerPath);
      this.powerLawPath.z(2);
      this.powerLawExponentLabel = dmz.ui.graph.createTextItem("Exponent = " + (-q));
      this.powerLawExponentLabel.pos(260, -this.barHeight - 60);
      this.scene.addItem(this.powerLawExponentLabel);
      this.scene.addItem(this.powerLawPath);
   }
};

XYGraph.prototype.update = function (values) {

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

   for (idx = this.startValue; idx < values.length; idx += 1) {

      if (values[idx] > maxValue) { maxValue = values[idx]; }
   }

   while (this.bars && this.bars.length) {

      bar = this.bars.pop();
//      this.scene.removeItem(bar.valueLabel);
//      this.scene.removeItem(bar.label);
      this.scene.removeItem(bar.image);
   }
   if (this.powerLawPath) { this.scene.removeItem (this.powerLawPath); }
   if (this.powerLawExponentLabel) { this.scene.removeItem (this.powerLawExponentLabel); }

   delete this.bars;
   this.bars = [];

   for (idx = this.startValue; idx < values.length; idx += 1) {

      xpos = this.spaceWidth + (this.barWidth + this.spaceWidth) * (idx - this.startValue);
      ypos = 0;
      value = values[idx] / maxValue;
      label = idx.toString();
      bar = createBar
         ( value
         , label
         , xpos
         , ypos
         , this.xAxis
         , this.barWidth
         , this.barHeight
         , this.xLabelRotation
         , this.barStroke
         , this.barFill
         );

      this.bars.push(bar);
   }

   while (Math.round(this.bars[this.bars.length - 1].value * 100) == 0) {

      this.scene.removeItem (this.bars.pop().image);
   }

   if (this.showPowerLaw) { this.drawPowerLaw(); }


   var rect = this.xAxis.childrenBoundingRect();
   var line = this.xAxis.line();
   this.xAxis.line(line.x1, line.y2, rect.width, line.y2);
}

// Following code is a start on conversion to a library

exports.createHLine = createHLine;
exports.createVLine = createVLine;

exports.create = function () { return new XYGraph (arguments); }
exports.isTypeOf = function (value) {

   return XYGraph.prototype.isPrototypeOf(value) ? value : undefined;
};

XYGraph.prototype.create = exports.create;
XYGraph.prototype.copy = function () {

   return this.create
      ( this.GraphForm
      , this.startValue
      , this.divisions
      , this.xAxisLabel
      , this.yAxisLabel
      , this.barStroke
      , this.barFill
      , this.barWidth
      , this.barHeight
      , this.spaceWidth
      , this.yDivisions
      , this.showPowerLaw
      );
}

XYGraph.prototype.toString = function () {

   return "[ XYGraph:" +
      " X: " + this.xAxisLabel +
      " Y: " + this.yAxisLabel +
      " start: " + this.startValue +
      " divs:" + this.divisions +
      " barHeight: " + this.barHeight +
      " barWidth: " + this.barWidth +
      " showPowerLaw: " + this.showPowerLaw +
      " X Axis Label: " + this.xAxisLabel +
      " Y Axis Label: " + this.yAxisLabel +
      " Bar Stroke: "  + this.barStroke.color() +
      " Bar Fill: " + this.barFill.color() +
      " Label Rotation: " + this.xLabelRotation +
      " ]";
}

puts ("Done");
