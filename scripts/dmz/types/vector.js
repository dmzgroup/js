var util = require('dmz/types/util');
var createError = util.createError;
var Epsilon = util.Epsilon;

var Vector = function () {

   this.x = 0;
   this.y = 0;
   this.z = 0;
};


exports.isTypeOf = function (value) {

   return Vector.prototype.isPrototypeOf(value) ? value : undefined;
};


exports.create = function () {

   var result = new Vector();
   if (arguments.length > 0) { result.set.apply(result, arguments); }
   return result;
};


Vector.prototype.create = exports.create;


Vector.prototype.copy = function () {

   return this.create(this);
};


Vector.prototype.toString = function () {

   return "[" + this.x + ", " + this.y + ", " + this.z + "]";
};


Vector.prototype.fromArray = function (values) {

   this.x = values[0];
   this.y = values[1];
   this.z = values[2];

   return this;
};


Vector.prototype.toArray = function () {

   return [this.x, this.y, this.z];
};


Vector.prototype.set = function () {

   var values, arg;

   if (arguments.length === 1) {

      arg = arguments[0];

      if (Vector.prototype.isPrototypeOf(arg)) {

         values = [ arg.x, arg.y, arg.z ];
      }
      else if (Array.isArray(arg) && (arg.length === 3)) {

         values = arg;
      }
      else { throw createError("Invalid Vector initialization value"); }
   }
   else if (arguments.length === 3) {

      values = arguments;
   } 
   else { throw createError("Invalid number of parameters for Vector.set()"); }

   if (values !== undefined) {

      this.fromArray (values);
   }

   return this;
};


Vector.prototype.setXYZ = function (x, y, z) {
   
   this.x = x;
   this.y = y;
   this.z = z;

   return this;
};


Vector.prototype.magnitude = function () {

   var result = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   return result > Epsilon ? result : 0.0;
};


Vector.prototype.normalized = function () {

   var mag = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z)),
      div = 0.0;

   if (mag > Epsilon) {

      div = 1 / mag;
   }

   return this.multiplyConst (div);
};


Vector.prototype.add = function (vec) {

   return this.create().setXYZ(this.x + vec.x, this.y + vec.y, this.z + vec.z);
};


Vector.prototype.subtract = function (vec) {

   return this.create().setXYZ(this.x - vec.x, this.y - vec.y, this.z - vec.z);
};


Vector.prototype.multiplyConst = function (k) {

   return this.create().setXYZ(this.x * k, this.y * k, this.z * k);
};


Vector.prototype.dot = function (vec) {

   return (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
};


Vector.prototype.cross = function (vec) {

   return this.create().setXYZ(
      (this.y * vec.z) - (this.z * vec.y),
      (this.z * vec.x) - (this.x * vec.z),
      (this.x * vec.y) - (this.y * vec.x));
};


Vector.prototype.getAngle = function (vec) {

   return Math.atan2(this.cross(vec).magnitude(), this.dot(vec));
};

