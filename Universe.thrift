namespace cpp Orion.Universe

struct netVector {
    1: double x,
    2: double y,
    3: double z
}

struct netPosition {
	1: netVector Position,
	2: netVector Velocity
}

struct netGravityWell {
	1: double Mass
}

struct netMesh {
	1: string MeshName,
	2: double Scale,
	3: netVector Orientation,
}

struct netAnomaly {
    1: string id,
    2: optional netPosition Position,
    3: optional netGravityWell GravityWell,
    4: optional netMesh Mesh,
}

service Universe {
    /**
     * Scan for data using the long range scanners
     */
    map<string, netAnomaly> LongRangeScan(),

    /**
     * Scan for data using the short range scanners
     */
    map<string, netAnomaly> ShortRangeScan(),

    /**
     * Set the thrust vector and magnitude
     */
    oneway void SetEngineVector(1:netVector Direction, 2:double Magnitude),


    void Ping(), // Short ping to test connectivity
    void LongWait() // A long wait for testing
}
