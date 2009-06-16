var DMZ = DMZ || {};

DMZ.Core = DMZ.Core || {};

DMZ.Core.Epsilon = 0.000000001;


DMZ.Core.isZero = function (value) {

   return Math.abs (value) < DMZ.Core.Epsilon;
};


DMZ.Core.isArray = function (value) {

   return Object.prototype.toString.apply(value) === '[object Array]';
};


DMZ.Core.hasFunction = function (obj, name) {

   return typeof obj[name] === "function";
};
