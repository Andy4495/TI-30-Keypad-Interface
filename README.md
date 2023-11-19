# TI-30 Keypad Interface

[![Arduino Compile Sketches](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/arduino-compile-sketches.yml) [![Check Markdown Links](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml)

Sketch to simulate the keypad of an old [TI-30 calculator][1] (original model, with the bubble-lens LEDs).

I purchased a lot of "for parts or repair" TI-30 calculators on eBay. After mixing and matching some parts to make a few working units, I ended up with an extra processor/display board with a non-working keypad and a broken case, so I decided to try to use an Arduino as a keypad replacement.

## Status

As of 10-Nov-2023:

- The code works -- all rows and columns are scanned and key presses registered

To-dos:

- DONE Add a separate WAIT pseudo-keypress (in addition to NO_KEY) to program a wait between keypresses. This could be useful for calculations known to take a while (see Notes below).
- DONE Add a REPEAT pseudo-keypress. This restarts the keypress cycle from the begining of the keylist.
- DONE Add an END pseudo-keypress. This may be useful in debugging to stop the keypresses in the middle of a list without having to update the list.
- DONE Clean up code.
- Can any functionality be moved to the class instead of the sketch?
- Would it be better to put the keypad definition inside a class or in its own namespace?
- Finish documenting the "..." parts in this README.
- Add a turn-on circuit (currently use a physical NO button). Would need another digital output controlling a transistor.
- Add a voltage regulator and power switch so that the calculator and Arduino can run from a single power supply/battery.
- Create a second sketch which reads characters from serial input instead of using keylist.

Notes:

- Some operations take a while to compute (for example: sine, cosine, factorial). In these cases, the calculator chip misses keypresses during the calculation.
- The sketch gets "stuck" if the calcultor chip is off (because the sketch is expecting the row signals to sequence through HIGH and LOW levels).

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

### Connecting to Arduino

...Keypad row pins

...Keypad column pins

Keypad pin 13 has two purposes: 1) column 5 keypad keypad matrix signal and 2) power on the calculator. To power on the calculator, the pin needs to be pulled to ~9 V. That is above the maximum rating for a microcontroller pin, so there needs to be isolation between the Arduino output and the 9 V signal. I used a standard 1N4148 diode between the Arduino and keypad pin 13 (anode to Arduino, cathode to keypad pin 13). The diode will block the 9 V signal from affecting the Arduino, but allow the Arduino output pin to signal a column 5 keypress to the calculator chip. The diode forward voltage drop of ~0.6 V did not impact operation.

... Include info on voltages, diodes, resistors, voltage dividers, power needs.
... Row scans at 5.6 V --> 10K/10K divider to about 2.8 V to MSP input pin
... MSP output of about 3.24 V is sufficient for calculator chip to detect keypress (TTL logic?)
... Rows are not physically connected to columns with the simulator... it is done internal to the MSP with separate input and output pins
... Row 8 is always at Vss/9V, so needs a different way to connect to pin 13 to turn on calculator. Row 8 does not need to be monitored by MSP (since it is always high), but need a way to connect it to pin 6 (which is connected to MSP) to turn on calculator without going over MSP Vmax. 3.3V from MSP is not enough to turn on calculator.
... Need 5 output pins (colums) and 7 input pins (rows) (row 8 is always high, so not connected). May need one more pin to control a transistor to turn on calculator (need some isolation between keypad pin 13 and MSP when connected to 9V)

### Keypad and LED Scannning

The calculator chip uses the same pins to scan the keypad rows and multiplex the LED segments (except for segment a and row 8, explained below). Every LED digit is cycled during each keypad row scan.

A keypress is detected with a HIGH signal on the corresponding column pin. If a button is not pressed, then the column pin will be floating externally, but will read low because the column pins are apparently loosely pulled low internal to the chip. The calculator chip has built-in debounce functionality, but it is not the most robust and will sometime record multiple keystrokes from a single press of a key.

If an LED segment should be turned on during the row scan, then the row signal will be pulled low for **~xx** us at the same time that the corresponding digit pin is pulled high. Note that since the row signal is pulled low to turn on the LED, any keypresses at that time will not be detected.

Row 8 is tied to Vss (9V battery +), so it cannot be used to control a segment (since it is always high). Therefore, LED segment a has a dedicated control signal, **and is it high all the time except when segment a is turned on, or does it cycle on/off?**.

LED digit 9 does not display numeric digits and only segments b, g, f, and dp are present.

#### Scan Timing

The sketch included with this repo monitors when the row signal goes high. If a key in that row is to be simulated as pressed, then the corresponding column signal is set HIGH. Once the row signal goes low again, the column signal is set LOW. It takes about 10 us for the sketch to detect a level change and update the column pin (using `digitalRead()` and `digitalWrite()`).

Scanning all 8 rows takes about 5.6 ms, with rows 1 through 7 taking about 670 us each and row 8 taking about 930 us. Row 8 probably takes longer because it does not have a dedicated scan signal, so the extra time may be some sort of "guard" time to make the keypress reading more reliable.

When a button is initially pressed (i.e., column pin goes high when row pin is high), the controller chip stops the row scanning (but continues with the LED multiplexing) and holds the row pin high for about 13.5 ms (2 full row scans). This is probably related to the debounce functionality. After that initial delay, the row scanning conntinues normally.

Each `loop()` takes about 20 us on a 16 MHz MSP430 processor.

**Confirm numbers: each digit is on about 30 us, with about 5.5 ms between digits???**
**Off state is segment high, digit low??? and reversed biased or no bias??**

## Software

.... Explain how the sketch works .....
.... Setting for "keybounce" time -- essentially the number of row scans that the button remains "pressed". I have found that a value of 0 (meaning one row scan time) works most of the time with a small number of missed keypresses. A value of 1 (2 row scan times) has consistently registered correctly.

## External Libraries

... Am I using any external libraries ...

## Implementation Details

... Anything not already covered above ....

## References

- TI-30 Calculator  [reference text][1]
- Keypad, processor chip, LED pinout summary by [Sean Riddle][2]
  - This was used as a starting point for my pinout summary above. I fixed a typo (pin 4 should be row 8 not 6) and included additional information.
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
