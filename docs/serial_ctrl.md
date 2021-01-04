# Serial control

On the ESP32 serial port, there's a simple command line interface implemented.

## Serial commands

| Command | Argument | Description |
|---------|----------|-------------|
| `TM`    | deg. C   | Simulate main burner temperature |
| `TA`    | deg. C   | Simulate afterburner temperature |
| `SS`    |          | Create screenshot |

## Temperature simulation

The temperature of the burn chambers can be simulated to facilitate testing of the incineration logic w/o actual burning.
To enable temperature simulation, the firmware needs to be built with `-DENABLE_TEMP_SIMULATION`.

### Temperature simulation tool

The script `temp_sim.py` can be used to conduct temperature simulation while testing.
The keys `e`, `r`, `d`, `f` and `u`, `i`, `j`, `k` lower or raise the main and afterburner temperature, respectively.

## Screenshots

`SS` creates a screenshot. It prints out a base64-encoding of a PNM file containing the current screen contents.

The base64 dump can be easily converted to a PNG file using the following shell snippet (needs imagemagick to work):
```
base64 -d | convert pnm:- screenshot.png
```
