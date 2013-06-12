namespace cpp Orion.Universe

struct Vector {
    1: double x,
    2: double y,
    3: double z
}

struct TPosition {
	1: Vector Position,
	2: Vector Velocity
}

struct TGravityWell {
	1: double Mass
}

struct TMesh {
	1: string name,
	2: double scale,
	3: Vector orientation,
}

struct Anomaly {
    1: string id,
    2: optional TPosition Position,
    3: optional TGravityWell GravityWell,
    4: optional TMesh Mesh,
}

service Universe {
    map<string, Anomaly> LongRangeScan(),

    void Ping(), // Short ping to test connectivity
    void LongWait() // A long wait for testing
}
