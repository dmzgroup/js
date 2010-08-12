var util = require('dmz/types/util')
  , createError = util.createError
  , Epsilon = util.Epsilon
  , Vector = function () { this.x = 0; this.y = 0; this.z = 0; }
  ;

// NOTE: Constants are defined at the end of the file.

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

   var  length = arguments.length
   ;

   if (length === 1) {

      if (Vector.prototype.isPrototypeOf(arguments[0])) {

         this.x = arguments[0].x;
         this.y = arguments[0].y;
         this.z = arguments[0].z;
      }
      else if (Array.isArray(arguments[0]) && (arguments[0].length === 3)) {

         this.x = arguments[0][0];
         this.y = arguments[0][1];
         this.z = arguments[0][2];
      }
      else { throw createError("Invalid Vector initialization value: " + typeof arg); }
   }
   else if (length === 3) {

      this.x = arguments[0];
      this.y = arguments[1];
      this.z = arguments[2];
   } 

   return this;
};


Vector.prototype.setXYZ = function (x, y, z) {
   
   this.x = x;
   this.y = y;
   this.z = z;

   return this;
};


Vector.prototype.magnitudeSquared = function () {

   var result = (this.x * this.x) + (this.y * this.y) + (this.z * this.z);
   return result > Epsilon ? result : 0.0;
};


Vector.prototype.magnitude = function () {

   var result = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z));
   return result > Epsilon ? result : 0.0;
};


Vector.prototype.normalize = function () {

   var mag = Math.sqrt((this.x * this.x) + (this.y * this.y) + (this.z * this.z)),
      div = 0.0;

   if (mag > Epsilon) {

      div = 1 / mag;
   }

   return this.multiply (div);
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


Vector.prototype.multiply = function (k) {

   return this.create().setXYZ(this.x * k, this.y * k, this.z * k);
};


Vector.prototype.multiplyConst = Vector.prototype.multiply;


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


Vector.prototype.getSignedAngle = function (vec) {

   var result =  Math.atan2(this.cross(vec).magnitude(), this.dot(vec))
   ,   cross = this.cross(vec)
   ;

   if (util.isZero(cross.y)) {

      if (util.isZero(cross.x)) {

         if (cross.z > 0) { result = -result; }
      }
      else if (cross.x < 0) { result = -result; }
   }
   else if (cross.y < 0) { result = -result; }

   return result;
};


Vector.prototype.isZero = function () {

   return util.isZero(this.magnitude());
};


util.defineConst(exports, "Forward", exports.create(0, 0, -1));
util.defineConst(exports, "Right", exports.create(1, 0, 0));
util.defineConst(exports, "Up", exports.create(0, 1, 0));

