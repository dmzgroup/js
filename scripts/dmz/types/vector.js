var util = require('dmz/types/util')
,   createError = util.createError
,   Epsilon = util.Epsilon
,   Vector = function () { this.x = 0; this.y = 0; this.z = 0; }
;


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
      else { throw createError("Invalid Vector initialization value: " + typeof arg); }
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

   var result;

   if (Vector.prototype.isPrototypeOf(vec)) {

      result = this.create().setXYZ(this.x + vec.x, this.y + vec.y, this.z + vec.z);
   }
   else if (Array.isArray (vec) && (vec.length >= 3)) {

      result = this.create().setXYZ(this.x + vec[0], this.y + vec[1], this.z + vec[2]);
   }
   else {

      throw createError("Invalid parameter for Vector.add() " + JSON.stringify(vec));
   }

   return result;
};


Vector.prototype.subtract = function (vec) {

   var result;

   if (Vector.prototype.isPrototypeOf(vec)) {

      result = this.create().setXYZ(this.x - vec.x, this.y - vec.y, this.z - vec.z);
   }
   else if (Array.isArray (vec) && (vec.length >= 3)) {

      result = this.create().setXYZ(this.x - vec[0], this.y - vec[1], this.z - vec[2]);
   }
   else {

      throw createError("Invalid parameter for Vector.subtract() " + JSON.stringify(vec));
   }

   return result;
};


Vector.prototype.multiplyConst = function (k) {

   return this.create().setXYZ(this.x * k, this.y * k, this.z * k);
};


Vector.prototype.dot = function (vec) {

   var result;

   if (Vector.prototype.isPrototypeOf(vec)) {

      result = (this.x * vec.x) + (this.y * vec.y) + (this.z * vec.z);
   }
   else if (Array.isArray (vec) && (vec.length >= 3)) {

      result = (this.x * vec[0]) + (this.y * vec[1]) + (this.z * vec[2]);
   }
   else {

      throw createError("Invalid parameter for Vector.dot() " + JSON.stringify(vec));
   }

   return result;
};


Vector.prototype.cross = function (vec) {

   var result;

   if (Vector.prototype.isPrototypeOf(vec)) {

      result = this.create().setXYZ(
         (this.y * vec.z) - (this.z * vec.y),
         (this.z * vec.x) - (this.x * vec.z),
         (this.x * vec.y) - (this.y * vec.x));
   }
   else if (Array.isArray (vec) && (vec.length >= 3)) {

      result = (this.x * vec[0]) + (this.y * vec[1]) + (this.z * vec[2]);

      result = this.create().setXYZ(
         (this.y * vec[2]) - (this.z * vec[1]),
         (this.z * vec[0]) - (this.x * vec[2]),
         (this.x * vec[1]) - (this.y * vec[0]));
   }
   else {

      throw createError("Invalid parameter for Vector.cross() " + JSON.stringify(vec));
   }

   return result;
};


Vector.prototype.getAngle = function (vec) {

   return Math.atan2(this.cross(vec).magnitude(), this.dot(vec));
};

