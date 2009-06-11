function listen (event, es, ed, data) {

   print ("In here!");

   if (event == debug.Debug.DebugEvent.Exception) {

      print (es.frameCount ());
      print ("*** exception");
      print ("Frame " + es.frame(0).func().script().name ());
      print ("Frame " + es.frame(0).sourceLine ());
      print ("Frame " + es.frame(0).sourceLineText ());
      print ("Frame " + es.frame(0).sourceAndPositionText ());
      print ("-=-=-=-=-=-=-=-=-=-=")
      print ("Frame " + es.frame(1).func().script().name ());
      print ("Frame " + es.frame(1).sourceLine ());
      print ("Frame " + es.frame(1).sourceLineText ());
      print ("Frame " + es.frame(1).sourceAndPositionText ());
      print ("-=-=-=-=-=-=-=-=-=-=")
      for (var ix in es.frame(0)) { print (ix); }
   }
}

debug.Debug.setListener(listen);
debug.Debug.setBreakOnException();

vec1 = DMZ.Vector.create().fromArray([1, 2, 3]);
vec2 = DMZ.Vector.create().fromArray([4, 5, 6]);

v = [0, 0, 1, 0, 1, 0, 1, 0, 0];

mat1 = DMZ.Matrix.create().set(v);
mat2 = DMZ.Matrix.create().set(1, 2, 3, 4, 5, 6, 7, 8, 9);
mat3 = DMZ.Matrix.create();
mat4 = DMZ.Matrix.create().set (mat2);

//print("hello world!", vec1.add(vec2), mat1, mat3.multiply(mat1), mat3.multiply(mat2));
//print(mat3.transform(vec1))
try {
print(mat3.transform(1))
//for (ix in debug) { print (ix); }
}
catch (e) { print (e); } // throw e; }
//print ("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
//for (ix in debug.Debug) { print (ix); }
/*
print ("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
for (ix in debug.Debug.DebugEvent) { print (ix); }
*/
//for (ix in new debug.FrameDetails) { print (ix); }
