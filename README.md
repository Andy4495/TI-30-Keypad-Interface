# TI-30 Keypad Interface

[![Arduino Compile Sketches](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/arduino-compile-sketches.yml) [![Check Markdown Links](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml)

Sketch to simulate the keypad of an old [TI-30 calculator][1] (original model, with the bubble-lens LEDs).

I purchased a lot of "for parts or repair" TI-30 calculators on eBay. After mixing and matching some parts to make a few working units, I ended up with an extra processor/display board with a non-working keypad and a broken case, so I decided to try to use an Arduino as a keypad replacement.

## Hardware

The TI-30 calculator has a surprisingly simple hardware design. The calculator processosr and LED driver functions are all contained in a single chip (TI TMC0981NL) without the need for any support circuitry: no crystal, no resistors, no capacitors. The only other components are the LEDs themselves (bonded directly to the PCB), the keypad, and a 9 V (PP3) battery clip.

### TI-30 Processor/Display Board and Keypad Pinouts

The photo below shows the orientation of the pin numbers that are used in the following tables.

![PCB][3]

#### Keypad Layout

These first two tables list the keypad matrix layout, first with respect to the keypad connector and next with respect to the TMC0981 chip.

The third table lists the correspondence between the keypad connector and the TMC0981 chip pins.

##### Pin Numbers Reference Keypad Connector

|               | Keypad Pin 1 | Keypad Pin 4 | Keypad Pin 7 | Keypad Pin 10 | Keypad Pin 13 |
| ------------- | :----------: | :----------: | :----------: | :-----------: | :-----------: |
| Keypad Pin 6  | 1/x          | x^2          | sqrt         | Off           | On/C          |
| Keypad Pin 3  | INV          | sin          | cos          | tan           | DRG           |
| Keypad Pin 9  |  K           | EE           | log          | lnx           | y^x           |
| Keypad Pin 8  | pi           |  %           |  (           |  )            |  /            |
| Keypad Pin 11 | STO          |  7           |  8           |  9            |  X            |
| Keypad Pin 12 | RCL          |  4           |  5           |  6            |  -            |
| Keypad Pin 2  | SUM          |  1           |  2           |  3            |  +            |
| Keypad Pin 5  | EXC          |  0           |  .           | +/-           |  =            |

##### Pin Numbers Reference TMC0981 Chip

|             | Chip Pin 9 | Chip Pin 7 | Chip Pin 8 | Chip Pin 6  | Chip Pin 10 |
| ----------- | :--------: | :--------: | :--------: | :---------: | :---------: |
| Chip Pin 4  | 1/x        | x^2        | sqrt       | Off         | On/C        |
| Chip Pin 12 | INV        | sin        | cos        | tan         | DRG         |
| Chip Pin 18 |  K         | EE         | log        | lnx         | y^x         |
| Chip Pin 14 | pi         |  %         |  (         |  )          |  /          |
| Chip Pin 17 | STO        |  7         |  8         |  9          |  X          |
| Chip Pin 16 | RCL        |  4         |  5         |  6          |  -          |
| Chip Pin 13 | SUM        |  1         |  2         |  3          |  +          |
| Chip Pin 11 | EXC        |  0         |  .         | +/-         |  =          |

##### Keypad Connector to TMC0981 Chip Pins

| Keypad Connector | TMC0981 Chip |
| :--------------: | :----------: |
|  1               |  9           |
|  2               | 13           |
|  3               | 12           |
|  4               |  7           |
|  5               | 11           |
|  6               |  4           |
|  7               |  8           |
|  8               | 14           |
|  9               | 18           |
| 10               |  6           |
| 11               | 17           |
| 12               | 16           |
| 13               | 10           |

#### TMC0981 Chip

In the table below, keypad columns are numbered left to right (i.e., `1/x` is in column 1), keypad rows are numbered bottom-up (i.e., `EXC` is in row 1), and LED digits are numbered right to left (the least significant digit is digit 1).

| Pin | Description |
| :-: | ----------- |
|  1  | LED Digit 7 |
|  2  | LED Digit 8 |
|  3  | LED Digit 9 |
|  4  | 9V Positive (Vss) / Keypad Row 8 / Chip Pin 20 |
|  5  | 9V Negative (Vdd) |
|  6  | Keypad Column 4 |
|  7  | Keypad Column 2 |
|  8  | Keypad Column 3 |
|  9  | Keypad Column 1 |
| 10  | Keypad Column 5 |
| 11  | Keypad Row 1 / LED Segment dp |
| 12  | Keypad Row 7 / LED Segment b  |
| 13  | Keypad Row 2 / LED Segment g  |
| 14  | Keypad Row 5 / LED Segment d  |
| 15  | LED Segment a                 |
| 16  | Keypad Row 3 / LED Segment f  |
| 17  | Keypad Row 4 / LED Segment e  |
| 18  | Keypad Row 6 / LED Segment c  |
| 19  | No connection |
| 20  | 9V Positive (Vss) / Keypad Row 8 / Chip Pin 4  |
| 21  | No connection |
| 22  | No connection |
| 23  | LED Digit 1 |
| 24  | LED Digit 2 |
| 25  | LED Digit 3 |
| 26  | LED Digit 4 |
| 27  | LED Digit 5 |
| 28  | LED Digit 6 |

##### LED Segment Labels

``` text
--- a ---
|       |
f       b
|       |
--- g ---
|       |
e       c
|       |
--- d ---  dp
```

### Interface and Signalling

The voltages and signal timings described below are dependent on the battery voltage and are probably also impacted by temperature.

#### Interface With Arduino

While the calculator runs on a 9 V battery, internally the logic appears to run on 5 V TTL levels. This also means that the high-level logic signals generated by a 3.3 V device like the MSP430 are sufficient to signal a high level to the calculator chip (since TTL only requires >2.0 V for a logic high).

The keypad row scan pins are configured as input pins on the Arduino. Only seven row pins are used (rows 1 - 7), because the row 8 pin is connected directly to +9 V and is therefore always high. The other row pins have a steady-state high level of about 5.5 V, but during state changes the ringing can reach 7 V. These signals should therefore be connected through a voltage divider before connecting to an Arduino. I use a 10K/10K divider with a 3.3 V MSP430.

The keypad column pins are configured as output pins on the Arduino. There are five column signals, and the Arduino sets a column pin high when the row scan signal corresponding to the selected key goes high.

Keypad pin 13 requires additional circuitry because it has two purposes: 1) column 5 keypad keypad matrix signal and 2) power on the calculator. To power on the calculator, the pin needs to be pulled to ~9 V. That is above the maximum rating for a microcontroller pin, so there needs to be isolation between the Arduino output and the 9 V signal. I use a standard 1N4148 diode between the Arduino and keypad pin 13 (anode to Arduino, cathode to keypad pin 13). The diode will block the 9 V signal from affecting the Arduino, but allow the Arduino output pin to signal a column 5 keypress to the calculator chip. The diode forward voltage drop of ~0.6 V did not impact operation.

Below is an example schematic showing an MSP430 controlling the TI-30. Keep in mind:

- R15 and C2 are specific to the programming interface of the MSP430; other processors (e.g. AVR) would interface differently with a programmer.
- The MSP430 is a 3.3 V device; hence the choice of 10K/10K voltage divider (to drop the ~5.5 V signal in half). When using a 5 V device, a 4.7K/15K divider may be more appropriate.
- The Arduino cannot be powered directly from the 9 V calculator battery. Either a separate supply is needed, or a voltage regulator/buck supply should be used to convert the 9 V battery to 3.3 V (or 5 V) needed by the Arduino.

![Schematic-MSP430](./extras/jpg/Schematic_TI-30%20Keypad_2023-11-22.svg)

#### Keypad and LED Scannning by the Calculator Chip

The calculator chip uses the same pins to scan the keypad rows and multiplex the LED segments (except for segment `a` and row 8, explained below).

A keypress is detected with a HIGH signal on the corresponding column pin. If a button is not pressed, then the column pin will be floating externally, but will read low to the calculator chip because the column pins are apparently loosely pulled low internal to the chip. The calculator chip has built-in debounce functionality, but it is not the most robust and will sometimes record multiple keystrokes from a single press of a key (when using a physical keypad).

The calculator chip multiplexes the LED display segments at the same time it is scanning the keypad rows. Each keypad row (except row 8) corresponds to an LED segment. When a particular row scan is active, that row signal (which is also a segment signal) goes high. For each LED display digit that needs to have that segment turned on, the calculator chip pulls the corresponding DIGIT pin low for about 18 us. Most of the time, the DIGIT pins are HIGH.

Row 8 is tied to Vss (9 V battery +), so it cannot be used to control a segment (since it is always high). Therefore, LED segment `a` has a dedicated control signal. This signal is normally low, but is set high during the row 8 scan time.

LED digit 9 does not display numeric digits and only segments `b`, `g`, `f`, and `dp` are present.

#### Row Scan Timing

Scanning all 8 rows takes about 5.6 ms, with rows 1 through 7 taking about 670 us each and row 8 taking about 930 us. Row 8 probably takes longer because it does not have a dedicated scan signal, so the extra time may be some sort of "guard" time to make the keypress reading more reliable.

When a button is initially pressed (i.e., column pin goes high when row pin is high), the controller chip stops the row scanning (but continues with the LED multiplexing) and holds the row pin high for about 13.5 ms (2 full row scans). This is probably related to the debounce functionality. After that initial delay, the row scanning conntinues normally.

## Software

The sketch is implemented as a state machine which cycles through the scan cycles and monitors when a row signal goes high. If a key in that row is to be simulated as pressed, then the corresponding column signal is set HIGH. Once the row signal goes low again, the column signal is set LOW. It takes about 10 us for the sketch to detect a level change and update the column pin (using `digitalRead()` and `digitalWrite()`).

The row 8 scanning is a little more complicated because there is no direct signal to monitor. Instead, the sketch waits for row 7 to complete at which point it assumes that the row 8 scan has started. It then assumes row 8 continues until it detects the row 1 scan has started.

The sketch has a setting called `KEYBOUNCE_CYCLES`. This indicates how many row scan cycles that the sketch holds the key press active. I have found that a setting of 0 (meaning a single row scan) sometimes results in missed keypresses. However, a setting of 1 (meaning two row scan cycles) consistently registers the keypresses correctly.

Each `loop()` takes about 20 us on a 16 MHz MSP430 processor. This means that there is no need for interrupt service routines, as the calculator signaling can be completed in plenty of time by using polling.

Notes:

- Some operations take a while to compute (for example: sine and cosine). In these cases, the calculator chip misses keypresses during the calculation.
- The sketch gets "stuck" if the calcultor chip is off (because the sketch is expecting the row signals to sequence through HIGH and LOW levels).

## References

- TI-30 Calculator [reference][1]
- Keypad, processor chip, LED pinout summary by [Sean Riddle][2]
  - This was used as a starting point for my pinout summary above.
- EdTech [article][4] about the TI-30

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: http://www.datamath.org/Sci/MAJESTIC/TI-30.htm
[2]: https://seanriddle.com/ti30.txt
[3]: ./extras/jpg/TI-30_annotated.jpg
[4]: https://edtechmagazine.com/k12/article/2016/11/ti-30-launched-electronic-calculating-math-classroom
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/Template-Repo)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
