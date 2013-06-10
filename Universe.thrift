namespace cpp Orion.Universe

enum AnomalyTypes {
    SHIP = 1,
    GRAVITY_WELL = 2,
}

struct Vector {
    1: double x,
    2: double y,
    3: double z
}

struct Anomaly {
    1: string id,
    2: Vector position,
    3: AnomalyTypes Type,
}

service Universe {
    map<string, Anomaly> LongRangeScan(),
    void Ping()
}
