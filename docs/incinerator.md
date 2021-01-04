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
  waitAfterburner --> startMain: Aft. temp. reached
  waitAfterburner --> abortCoolDown: User abort
  startMain --> waitMain: Start main burner
  waitMain --> burnActive: Main temp. reached
  waitMain --> failureCoolDown: Main failed
  waitMain --> abortCoolDown: User abort
  burnActive --> coolDown: Burn time elapsed
  burnActive --> failureCoolDown: Burner failed
  burnActive --> abortCoolDown: User abort
  coolDown --> [*]: System cooled down
  failureCoolDown --> [*]: System cooled down
  abortCoolDown --> [*]: System cooled down
```
