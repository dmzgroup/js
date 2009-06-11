var DMZ = DMZ || {};

DMZ.Epsilon = 0.000000001;

DMZ.isArray = function (value) {

   return Object.prototype.toString.apply(value) === '[object Array]';
}

DMZ.hasFunction = function (obj, name) {

   return typeof obj[name] === "function";
}
