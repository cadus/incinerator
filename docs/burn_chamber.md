# Burn chamber
The burn chamber is responsible for managing the burn process in one burn chamber, which includes triggering ignition, reading temperature and controlling gas flow via valves.

### Temperature control
The temperature is controlled with gas valves which can be ON or OFF (no gradual control). The main chamber features a single valve (VALVE_MAIN), the afterburner chamber features two valves (VALVE_AFT_HI, VALVE_AFT_LO) which create a high or low gas flow, respectively.
- When temperature sinks below T_MIN, VALVE_MAIN / VALVE_AFT_HI is activated.
- When temperature rises above T_MAX, VALVE_MAIN / VALVE_AFT_HI is deactivated, and in case of the afterburner chamber, VALVE_AFT_LO is activated instead.

### State machine
``` mermaid
stateDiagram
  [*] --> start: Burn start requested
  start --> waitIgnition: Set gas valve, request ignition
  waitIgnition --> failed: Ignition failed
  failed --> [*]: Reset
  waitIgnition --> waitTemp: Ignition successful
  waitTemp --> burnHigh: T_MIN reached
  burnHigh --> burnLow: T > T_MAX
  burnLow --> burnHigh: T < T_MIN
  burnHigh --> [*]: Reset
  burnLow --> [*]: Reset
```
