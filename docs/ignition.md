# Ignition module
Ignition is triggered from higher level control (burn chamber).

### Ignition control
Ignition coil is activated for a configurable time `ign_pulse_w`, this is repeated after a configurable delay `ign_repeat_itvl` until temperature has risen enough that a successful ignition can be assumed.

### Protection logic
Ignition control features a simple protection logic to make sure there's no gas flow when the gas is not burning. After ignition has started, the ignition module checks if temperature has risen by a configurable delta `ign_delta`. When a configurable timeout `ign_t` has elapsed w/o temperature rising, then the ignition module indicates that ignition has failed and the gas flow should be shut down.

### State machine
``` mermaid
stateDiagram
  [*] --> setCoil: Ignition request
  setCoil --> waitPulse
  waitPulse --> resetCoil: ign_pulse_w elapsed
  resetCoil --> waitIgnition
  waitIgnition --> setCoil: ign_repeat_itvl elapsed
  waitIgnition --> success: Temp. raised
  waitIgnition --> failure: Timeout
  success --> [*]: Reset
  failure --> [*]: Reset
```
