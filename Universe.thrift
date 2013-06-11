namespace cpp Orion.Universe

struct Vector {
    1: double x,
    2: double y,
    3: double z
}

struct PositionType {
	1: Vector Position,
	2: Vector Velocity
}

struct GravityWellType {
	1: double mass
}

struct MeshType {
	1: string name,
	2: double scale,
	3: Vector orientation,
}

struct Anomaly {
    1: string id,
    2: optional PositionType Position,
    3: optional GravityWellType GravityWell,
    4: optional MeshType Mesh,
}

service Universe {
    map<string, Anomaly> LongRangeScan(),
    void Ping(),
    void LongWait()
}
