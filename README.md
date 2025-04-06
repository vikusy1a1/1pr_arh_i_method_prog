# 1pr_arh_i_method_prog
@startuml IndustrialSafetySystem

' Доменные сущности
class Sensor {
  + id: int
  + type: string
  + location: string
  + current_value: double
  + threshold: double
  + last_calibration: time_t
}

class Incident {
  + id: int
  + type: string
  + severity: string
  + status: string
  + timestamp: time_t
  + related_sensors: vector<int>
}

' Репозитории
class SensorRepository {
  - sensors: vector<Sensor>
  + add(s: Sensor): bool
  + find(id: int): Sensor*
  + update(s: Sensor): bool
  + getAll(): vector<Sensor>
}

class IncidentRepository {
  - incidents: vector<Incident>
  - next_id: int = 1
  + add(i: Incident): int
  + update(i: Incident): bool
  + getAll(): vector<Incident>
}

' Сервисный слой
class SafetyService {
  - sensor_repo: SensorRepository
  - incident_repo: IncidentRepository
  + register_sensor(s: Sensor): bool
  + check_sensor(id: int): string
  + report_incident(type: string, sensor_ids: vector<int>): int
  + getAllSensorsInfo(): string
  + getAllIncidentsInfo(): string
  - formatTime(time: time_t): string
}

' Связи между классами
SafetyService --> SensorRepository : uses
SafetyService --> IncidentRepository : uses
SensorRepository "1" *--> "0..*" Sensor : contains
IncidentRepository "1" *--> "0..*" Incident : contains
Incident --> "0..*" Sensor : related to

@enduml
