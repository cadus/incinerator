# Ignition module
Ignition is triggered from higher level control (burn chamber).

### Ignition control
Ignition coil is activated for a configurable time `ign_pulse_w`, this is repeated after a configurable delay `ign_repeat_itvl` until temperature has risen enough that a successful ignition can be assumed.

### Protection logic
Ignition control features a simple protection logic to make sure there's no gas flow when the gas is not burning. After ignition has started, the ignition module checks if temperature has risen by a configurable delta `ign_delta`. When a configurable timeout `ign_t` has elapsed w/o temperature rising, then the ignition module indicates that ignition has failed and the gas flow should be shut down.

### State machine
``` mermaid
graph TD
  I(idle)
  S("Set coil")
  WC("Wait ign_pulse_w")
  R("Reset coil")
  WI("Wait ign_repeat_itvl")
  SUCC("Ignition successful")
  FAIL("Ignition failed")
  I-- Ignition requested -->S
  S-->WC
  WC-- ign_pulse_w elapsed -->R
  R-->WI
  WI-- ign_repeat_itvl elapsed -->S
  WI-- Temp. raised by ign_delta -->SUCC
  WI-- ign_t elapsed -->FAIL
```
