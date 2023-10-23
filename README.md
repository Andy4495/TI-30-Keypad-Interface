# TI-30 Keypad Interface

[![Check Markdown Links](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/TI-30-Keypad-Interface/actions/workflows/CheckMarkdownLinks.yml)

Sketch to simulate the keypad of an old [TI-30 calculator][1] (original model, with the bubble-lens LEDs).

I purchased a lot of "for parts or repair" TI-30 calculators on Ebay. After mixing and matching some parts to make a couple working units, I ended up with an extra processor/display board with a non-working keypad.

## Hardware

The TI-30 calculator has a surprisingly simple hardware design. The calculator processosr and LED driver functions are all contained in a single chip (TI TMC0981NL) without the need for any additional support circuitry: no crystal, no resistors, no capacitors. The only other components are the LEDs themselves (bonded directly to the PCB), the keypad, and a 9 V (PP3) battery clip.

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
| 14  | Keypad Row 5 / LED Segment d |
| 15  | LED Segment a                 |
| 16  | Keypad Row 3 / LED Segment f  |
| 17  | Keypad Row 4 / LED Segment e |
| 18  | Keypad Row 6 / LED Segment c |
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
--- d ---
```

### Connecting to Arduino

... Include info on voltages, diodes, resistors, voltage dividers, power needs.

### Signalling (maybe put under the display board heading)

... Explain how keyboard is scanned (using actual experimental results): rate, voltages, ....

... Explain shared signals and how that may impact operation ...

## Software

.... Explain how the sketch works .....

## External Libraries

... Am I using any external libraries ...

## Implementation Details

... Anything not already covered above ....

## References

- TI-30 Calculator  [reference text][1]
- Keypad, processor chip, LED pinout summary by [Sean Riddle][2]
  - This was used as a starting point for my pinout summary above
  - There is at least one typo in Sean's summary that is corrected in my tables

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: http://www.datamath.org/Sci/MAJESTIC/TI-30.htm
[2]: https://seanriddle.com/ti30.txt
[3]: ./extras/jpg/TI-30_annotated.jpg
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/Template-Repo)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
