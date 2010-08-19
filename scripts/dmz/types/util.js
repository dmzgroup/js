var ToDegrees = 180 / Math.PI
  , ToRadians = Math.PI / 180
  ;


exports.defineConst = function (target, name, value) {

   var type = typeof value;

   if (value && ((type === 'Object') || (type === 'Function'))) {

      Object.freeze(value);
   }

   Object.defineProperty(
      target,
      name,
      { get: function () { return value; }
      , set: function () {

           return new Error("Attempting to set constant value " + name + ".");
        }
      });
};


exports.defineConst(exports, "Epsilon", 0.000000001);
exports.defineConst(exports, "ToDegrees", ToDegrees);
exports.defineConst(exports, "ToRadians", ToRadians);


exports.randomInt = function (min, max) {

   if (exports.isUndefined(max)) {

      if (exports.isUndefined(min)) { max = 99; min = 0 ; }
      else { max = min; min = 0 }
   }

   return Math.floor(Math.random() * (max - min + 1)) + min;
};


exports.radiansToDegrees = function (value) {

   var result = value * ToDegrees;
   if (result < 0) { result += 360; }
   return result;
};


exports.degreesToRadians = function (value) {

   var result = value * ToRadians;
   return result;
};


exports.isZero = function (value) {

   return Math.abs (value) < exports.Epsilon;
};


exports.isNotZero = function (value) {

   return Math.abs (value) >= exports.Epsilon;
};


exports.isDefined = function (val) { var ud; return val !== ud; };


exports.isUndefined = function (val) { var ud; return val === ud; };


exports.hasFunction = function (obj, name) {
 
   return typeof obj[name] === "function";
};


exports.createError = function (message) {

   return new Error (message);
}
