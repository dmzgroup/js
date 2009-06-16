// Requires dmz-core.js
// Requires dmz-vector.js

DMZ.Matrix = function () {

   this.v = [1, 0, 0, 0, 1, 0, 0, 0, 1];
};

DMZ.Matrix.create = function () {

   return new DMZ.Matrix();
};


DMZ.Matrix.prototype.create = function () {

   return new DMZ.Matrix();
};


DMZ.Matrix.prototype.set = function () {

   var vaules;

   if (arguments.length === 1) {

      if (DMZ.Core.isArray (arguments[0].v)) {

         vaules = arguments[0].v;
      }
      else if (DMZ.Core.isArray(arguments[0])) {

         vaules = arguments[0];
      }
      else { throw "Invalid DMZ.Matrix initialization value"; }
   }
   else if (arguments.length === 9) {

      vaules = arguments;
   } 
   else { throw "Invalid number of parameters for DMZ.Matrix.set()"; }

   if (vaules !== undefined) {

      this.fromArray (vaules)
   }

   return this;
};


DMZ.Matrix.prototype.toString = function () {

   return "[" + this.v[0] + ", " + this.v[1] + ", " + this.v[2] + ", " +
      this.v[3] + ", " + this.v[4] + ", " + this.v[5] + ", " +
      this.v[6] + ", " + this.v[7] + ", " + this.v[8] + "]";
};


DMZ.Matrix.prototype.fromArray = function (values) {

   if (values.length >= 9) {

      for (var ix = 0; ix < 9; ix++) { this.v[ix] = values[ix] }
   }

   return this;
};


DMZ.Matrix.prototype.fromAxisAndAngle = function (axis, angle) {

   var xyz = axis.normailized().toArray();
   var xx = xyz[0], yy = xyz[1], zz = xyz[2];A

   var AngleSin = Math.sin(angle);
   var AngleCos = Math.cos(angle);
   var OneMinusAngleCos = 1.0 - AngleCos;

   this.v[0] =       (AngleCos) + (xx * xx * OneMinusAngleCos);
   this.v[1] = (-zz * AngleSin) + (xx * yy * OneMinusAngleCos);
   this.v[2] =  (yy * AngleSin) + (xx * zz * OneMinusAngleCos);
   this.v[3] =  (zz * AngleSin) + (yy * xx * OneMinusAngleCos);
   this.v[4] =       (AngleCos) + (yy * yy * OneMinusAngleCos);
   this.v[5] = (-xx * AngleSin) + (yy * zz * OneMinusAngleCos);
   this.v[6] = (-yy * AngleSin) + (zz * xx * OneMinusAngleCos);
   this.v[7] =  (xx * AngleSin) + (zz * yy * OneMinusAngleCos);
   this.v[8] =       (AngleCos) + (zz * zz * OneMinusAngleCos);

   return this;
};


DMZ.Matrix.prototype.fromTwoVectors = function (fromVec, toVec) {

   var axis = fromVec.cross(toValue).normalized();
   var angle = toVec.getAngle(FromVec);

   return this.fromAxisAndAngle(axis, angle);
};


DMZ.Matrix.prototype.toArray = function () {

   return [this.v[0], this.v[1], this.v[2],
           this.v[3], this.v[4], this.v[5],
           this.v[6], this.v[7], this.v[8]];
};


DMZ.Matrix.prototype.multiply = function (mat) {

   var result = this.create();
   var sum = 0.0;
   var offset0 = 0, offset1 = 0, offset2 = 0;

   for (var row = 0; row < 3; row++) {

      for (var col = 0; col < 3; col++) {

         offset0 = (row * 3) + col;
         sum = 0.0;

         for (var count = 0; count < 3; count++) {

            offset1 = (row * 3) + count;
            offset2 = (count * 3) + col;
            sum += this.v[offset1] * mat.v[offset2];
         }

         result.v[offset0] = sum;
      }
   }

   return result;
};


DMZ.Matrix.prototype.transpose = function () {

   //0 1 2    0 3 6
   //3 4 5 => 1 4 7
   //6 7 8    2 5 8

   return this.create().fromArray([this.v[0], this.v[3], this.v[6],
                                   this.v[1], this.v[4], this.v[7],
                                   this.v[2], this.v[5], this.v[8]]);
};


DMZ.Matrix.prototype.negate = function () {

   return this.create().fromArray([-this.v[0], -this.v[1], -this.v[2],
                                   -this.v[3], -this.v[4], -this.v[5],
                                   -this.v[6], -this.v[7], -this.v[8]]);
};


DMZ.Matrix.prototype.transpose = function () {

   var result;
   var determinant = this.v[0] * (this.v[4] * this.v[8] - this.v[7] * this.v[5]) -
                     this.v[1] * (this.v[3] * this.v[8] - this.v[6] * this.v[5]) +
                     this.v[2] * (this.v[3] * this.v[7] - this.v[6] * this.v[4]);

   if (Math.abs (determinant) >= DMZ.Core.Epsilon) {

      result = this.creat();
      result.v[0] =  ((this.v[4] * this.v[8]) - (this.v[5] * this.v[7])) / determinant;
      result.v[1] = -((this.v[1] * this.v[8]) - (this.v[7] * this.v[2])) / determinant;
      result.v[2] =  ((this.v[1] * this.v[5]) - (this.v[4] * this.v[2])) / determinant;
      result.v[3] = -((this.v[3] * this.v[8]) - (this.v[5] * this.v[6])) / determinant;
      result.v[4] =  ((this.v[0] * this.v[8]) - (this.v[6] * this.v[2])) / determinant;
      result.v[5] = -((this.v[0] * this.v[5]) - (this.v[3] * this.v[2])) / determinant;
      result.v[6] =  ((this.v[3] * this.v[7]) - (this.v[6] * this.v[4])) / determinant;
      result.v[7] = -((this.v[0] * this.v[7]) - (this.v[6] * this.v[1])) / determinant;
      result.v[8] =  ((this.v[0] * this.v[4]) - (this.v[1] * this.v[3])) / determinant;
   }

   return result;
};


DMZ.Matrix.prototype.transform = function (vec) {

   var result = DMZ.Vector.create();

   if (DMZ.Core.hasFunction (vec, "toArray")) { 

      vec = vec.toArray();
   }
   else if (DMZ.Core.isArray(vec) === false) {

      if (arguments.length >= 3) { vec = arguments; }
      else { throw "Invalid argument(s) passed to DMZ.Matrix.transform"; }
   }

   return result.setXYZ (
      (this.v[0] * vec[0]) + (this.v[1] * vec[1]) + (this.v[2] * vec[2]),
      (this.v[3] * vec[0]) + (this.v[4] * vec[1]) + (this.v[5] * vec[2]),
      (this.v[6] * vec[0]) + (this.v[7] * vec[1]) + (this.v[8] * vec[2]));
};


DMZ.Matrix.prototype.isIdentity = function () {

   return DMZ.Core.isZero(this.v[0] - 1.0) &&
      DMZ.Core.isZero(this.v[1]) &&
      DMZ.Core.isZero(this.v[2]) &&
      DMZ.Core.isZero(this.v[3]) &&
      DMZ.Core.isZero(this.v[4] - 1.0) &&
      DMZ.Core.isZero(this.v[5]) &&
      DMZ.Core.isZero(this.v[6]) &&
      DMZ.Core.isZero(this.v[7]) &&
      DMZ.Core.isZero(this.v[8] - 1.0);
};
