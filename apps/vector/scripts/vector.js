var ctor = require("dmz/types/vector").create
,   v1 = ctor()
,   v2 = ctor(1, 2, 3)
,   v3 = ctor([4, 5, 6])
;

self.log.error(v1, v2, v3, v2.add(v3));

v1.x = 10;
v1.y = 11;
v1.z = 12;

self.log.error(v1);
