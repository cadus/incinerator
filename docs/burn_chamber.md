# Burn chamber
The burn chamber is responsible for managing the burn process in one burn chamber, which includes triggering ignition, reading temperature and controlling gas flow via valves.

### Temperature control
The temperature is controlled with gas valves which can be ON or OFF (no gradual control). The main and afterburner chamber feature a single valve each (VALVE_MAIN,VALVE_AFT).

- When temperature sinks below T_MIN, VALVE_MAIN / VALVE_AFT is activated.
- When temperature rises above T_MAX, VALVE_MAIN / VALVE_AFT is deactivated.

### State machine
``` mermaid
stateDiagram-v2
  [*] --> start: Burn start requested
  start --> waitIgnition: Set gas valve, request ignition
  waitIgnition --> failed: Ignition failed
  failed --> [*]: Reset
  waitIgnition --> waitTemp: Ignition successful
  waitTemp --> burnHigh: T_MIN reached
  burnHigh --> burnLow: T > T_MAX
  burnLow --> waitBurnHigh: T < T_MIN, ignite again
  waitBurnHigh --> burnHigh: Ignition successful
  waitBurnHigh --> failed: Ignition failed
  burnHigh --> [*]: Reset
  burnLow --> [*]: Reset
```
