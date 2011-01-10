var util = require ('dmz/types/util')
,   Epsilon = util.Epsilon
,   createError = util.createError
,   vector = require ('dmz/types/vector')
,   Matrix = function () { this.v = [1, 0, 0, 0, 1, 0, 0, 0, 1]; }
;


exports.isTypeOf = function (value) {

   return Matrix.prototype.isPrototypeOf(value) ? value : undefined;
};


exports.create = function () {

   var result = new Matrix();
   if (arguments.length > 0) { result.set.apply(result, arguments); }
   return result;
};


Matrix.prototype.create = exports.create;


Matrix.prototype.copy = function () {

   return exports.create(this);
};


Matrix.prototype.set = function () {

   var values;

   if (arguments.length === 1) {

      if (Array.isArray (arguments[0].v)) {

         values = arguments[0].v;
      }
      else if (Array.isArray(arguments[0])) {

         values = arguments[0];
      }
      else { throw createError("Invalid Matrix initialization value"); }
   }
   else if (arguments.length === 9) {

      values = arguments;
   } 
   else { throw createError("Invalid number of parameters for Matrix.set()"); }

   if (values !== undefined) {

      this.fromArray (values);
   }

   return this;
};


Matrix.prototype.toString = function () {

   return "[" + this.v[0] + ", " + this.v[1] + ", " + this.v[2] + ", " +
      this.v[3] + ", " + this.v[4] + ", " + this.v[5] + ", " +
      this.v[6] + ", " + this.v[7] + ", " + this.v[8] + "]";
};


Matrix.prototype.fromArray = function (values) {

   var ix
     ;

   if (values.length >= 9) {

      for (ix = 0; ix < 9; ix++) { this.v[ix] = values[ix]; }
   }

   return this;
};


Matrix.prototype.fromAxisAndAngle = function (axis, angle) {

   var xyz = axis.normalize().toArray()
     , xx = xyz[0]
     , yy = xyz[1]
     , zz = xyz[2]
     , AngleSin = Math.sin(angle)
     , AngleCos = Math.cos(angle)
     , OneMinusAngleCos = 1.0 - AngleCos
     ;

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


Matrix.prototype.fromVector = function (vec) {

   var hvec = vec.copy()
     , pvec = vec.copy()
     , hmat = exports.create()
     , pmat = exports.create()
     ;

   hvec.y = 0;

   if (!hvec.isZero()) {

      hvec = hvec.normalize();
      hmat.fromAxisAndAngle(vector.Up, vector.Forward.getSignedAngle(hvec));
   }

   pvec = hmat.transpose().transform(pvec);

   if (!util.isZero(pvec.y)) {

      pmat.fromAxisAndAngle(vector.Right, vector.Forward.getSignedAngle(pvec));
   }

   return hmat.multiply(pmat);
};


Matrix.prototype.fromTwoVectors = function (fromVec, toVec) {

   var axis = fromVec.cross(toVec).normalize()
     , angle = toVec.getAngle(fromVec)
     ;

   return this.fromAxisAndAngle(axis, angle);
};


Matrix.prototype.fromEuler = function () {

   var hpr = [0 , 0 , 0]
     , hmat = exports.create()
     , pmat = exports.create()
     , rmat = exports.create()
     ;

   if (arguments.length === 1) {

      if (Array.isArray(arguments[0])) {

         hpr[0] = arguments[0][0];
         hpr[1] = arguments[0][1];
         hpr[2] = arguments[0][2];
      }
      else if (vector.isTypeOf(arguments[0])) {

         hpr[0] = arguments[0].x;
         hpr[1] = arguments[0].y;
         hpr[2] = arguments[0].z;
      }
   }
   else if (arguments.length === 3) {

      hpr[0] = arguments[0];
      hpr[1] = arguments[1];
      hpr[2] = arguments[2];
   }

   hmat.fromAxisAndAngle(vector.Up, hpr[0]);
   pmat.fromAxisAndAngle(vector.Right, hpr[1]);
   rmat.fromAxisAndAngle(vector.Forward, hpr[2]);

   this.set (hmat.multiply(pmat.multiply(rmat)));

   return this;
};


Matrix.prototype.toArray = function () {

   return [this.v[0], this.v[1], this.v[2],
           this.v[3], this.v[4], this.v[5],
           this.v[6], this.v[7], this.v[8]];
};


Matrix.prototype.toEuler = function () {

   var result = [0, 0, 0]
     , cmat = this.copy()
     , hvec = this.transform(vector.Forward)
     , hmat = exports.create()
     , pvec
     , pmat = exports.create()
     , rvec
     , rmat = exports.create()
     ;

   hvec.y = 0;

   if (util.isNotZero(hvec.magnitude())) {

      hvec = hvec.normalize();
      result[0] = vector.Forward.getSignedAngle(hvec);
      hmat = hmat.fromAxisAndAngle(vector.Up, result[0]);
      hmat = hmat.transpose();
      cmat = hmat.multiply(cmat);
   }

   pvec = cmat.transform(vector.Forward);
   if (util.isNotZero(pvec.y)) {

      result[1] = vector.Forward.getSignedAngle(pvec);
      pmat = pmat.fromAxisAndAngle(vector.Right, result[1]);
      pmat = pmat.transpose();
      cmat = pmat.multiply(cmat);
   }

   rvec = cmat.transform(vector.Right);
   if (util.isNotZero(rvec.x)) {

      result[2] = vector.Right.getSignedAngle(rvec);
   }

   return result;
};

Matrix.prototype.multiply = function (mat) {

   var result = exports.create();
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


Matrix.prototype.transpose = function () {

   //0 1 2    0 3 6
   //3 4 5 => 1 4 7
   //6 7 8    2 5 8

   return exports.create().fromArray(
      [this.v[0], this.v[3], this.v[6],
       this.v[1], this.v[4], this.v[7],
       this.v[2], this.v[5], this.v[8]]);
};


Matrix.prototype.negate = function () {

   return exports.create().fromArray(
      [-this.v[0], -this.v[1], -this.v[2],
       -this.v[3], -this.v[4], -this.v[5],
       -this.v[6], -this.v[7], -this.v[8]]);
};


Matrix.prototype.invert = function () {

   var result;

   var determinant =
      this.v[0] * (this.v[4] * this.v[8] - this.v[7] * this.v[5]) -
      this.v[1] * (this.v[3] * this.v[8] - this.v[6] * this.v[5]) +
      this.v[2] * (this.v[3] * this.v[7] - this.v[6] * this.v[4]);

   if (Math.abs (determinant) >= Epsilon) {

      result = exports.create();
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


Matrix.prototype.transform = function (vec) {

   var result = vector.create();

   if (util.hasFunction (vec, "toArray")) { 

      vec = vec.toArray();
   }
   else if (Array.isArray(vec) === false) {

      if (arguments.length >= 3) { vec = arguments; }
      else { throw createError("Invalid argument(s) passed to Matrix.transform"); }
   }

   return result.setXYZ (
      (this.v[0] * vec[0]) + (this.v[1] * vec[1]) + (this.v[2] * vec[2]),
      (this.v[3] * vec[0]) + (this.v[4] * vec[1]) + (this.v[5] * vec[2]),
      (this.v[6] * vec[0]) + (this.v[7] * vec[1]) + (this.v[8] * vec[2]));
};


Matrix.prototype.isIdentity = function () {

   return util.isZero(this.v[0] - 1.0) &&
      util.isZero(this.v[1]) &&
      util.isZero(this.v[2]) &&
      util.isZero(this.v[3]) &&
      util.isZero(this.v[4] - 1.0) &&
      util.isZero(this.v[5]) &&
      util.isZero(this.v[6]) &&
      util.isZero(this.v[7]) &&
      util.isZero(this.v[8] - 1.0);
};
