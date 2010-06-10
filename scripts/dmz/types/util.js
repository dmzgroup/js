var ToDegrees = 180 / Math.PI
  , ToRadians = Math.PI / 180
  ;


Object.defineProperty(
   exports,
   "Epsilon",
   {
      get: function () { return 0.000000001; },
      set: function () { throw new Error("Attempting to set constant value Epsilon."); }
   }
);


Object.defineProperty(
   exports,
   "ToDegrees",
   {
      get: function () { return ToDegrees; },
      set: function () { throw new Error("Attempting to set constant value ToDegrees."); }
   }
);


Object.defineProperty(
   exports,
   "ToRadians",
   {
      get: function () { return ToRadians; },
      set: function () { throw new Error("Attempting to set constant value ToRadians."); }
   }
);

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
