function listen (event, es, ed, data) {

   if (event == V8.Debug.DebugEvent.Exception) {

      print ("*** exception");
      for (var ix = 0; ix < es.frameCount (); ix++) {

         //print ("file " + es.frame(0).func().script().name ());
         //print ("line " + es.frame(0).sourceLine ());
         print ("code: " + es.frame(ix).sourceLineText ());
         print (ix + ":" + es.frame(ix).sourceAndPositionText ());
      }
   }
}

V8.Debug.setListener(listen);
V8.Debug.setBreakOnException();

vec1 = DMZ.Vector.create().fromArray([1, 2, 3]);
vec2 = DMZ.Vector.create().fromArray([4, 5, 6]);

v = [0, 0, 1, 0, 1, 0, 1, 0, 0];

mat1 = DMZ.Matrix.create().set(v);
mat2 = DMZ.Matrix.create().set(1, 2, 3, 4, 5, 6, 7, 8, 9);
mat3 = DMZ.Matrix.create();
mat4 = DMZ.Matrix.create().set (mat2);

print(mat3.isIdentity());

//print("hello world!", vec1.add(vec2), mat1, mat3.multiply(mat1), mat3.multiply(mat2));
//print(mat3.transform(vec1))
try {
print(mat3.transform(1))
//for (ix in V8) { print (ix); }
}
catch (e) { print (e); } // throw e; }

print(mat3.transform(1))
//print ("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
//for (ix in V8.Debug) { print (ix); }
/*
print ("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
for (ix in V8.Debug.DebugEvent) { print (ix); }
*/
//for (ix in new V8.FrameDetails) { print (ix); }
