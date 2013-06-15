namespace cpp Orion.Universe

struct Vector {
    1: double x,
    2: double y,
    3: double z
}

struct netPosition {
	1: Vector Position,
	2: Vector Velocity
}

struct netGravityWell {
	1: double Mass
}

struct netMesh {
	1: string name,
	2: double scale,
	3: Vector orientation,
}

struct Anomaly {
    1: string id,
    2: optional netPosition Position,
    3: optional netGravityWell GravityWell,
    4: optional netMesh Mesh,
}

service Universe {
    map<string, Anomaly> LongRangeScan(),
    map<string, Anomaly> ShortRangeScan(),

    void Ping(), // Short ping to test connectivity
    void LongWait() // A long wait for testing
}
