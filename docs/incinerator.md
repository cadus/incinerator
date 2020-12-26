# Incinerator
The incinerator is responsible for managing the overall burn process.

### Burn sequence
- Start burn process in afterburner to pre-heat it
- When afterburner has reached operating temperature, enable air pump and main burner.
- When main burner has reached operating temp., wait until burn time is elapsed.
- Switch off gas and wait until system has cooled down.

### State machine
``` mermaid
stateDiagram-v2
  [*] --> start: Burn start requested
  start --> waitAfterburner: Start afterburner
  waitAfterburner --> failureCoolDown: Aft. failed
  failureCoolDown --> [*]: System cooled down
  waitAfterburner --> startMain: Aft. temp. reached
  startMain --> waitMain: Start main burner
  waitMain --> burnActive: Main temp. reached
  waitMain --> failureCoolDown: Main failed
  burnActive --> coolDown: Burn time elapsed
  coolDown --> [*]: System cooled down
```
