
exports.constants = {

   get Epsilon() { return  0.000000001; },
   set Epsilon() { throw "Attempting to set constant value Epsilon."; }

};

exports.isZero = function (value) {

   return Math.abs (value) < exports.Epsilon;
};


exports.hasFunction = function (obj, name) {
 
   return typeof obj[name] === "function";
};

