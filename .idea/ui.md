# UI

All functions should be very sparse in their output (any TTY is quite
Slow)

The Program uses two major buffers:

`writeBuffer[]`	for every ASCII-Chars
`tty_sybols[]` 	for every Baudot-Symbol (int 0-31)

Every char or symbol gets appended to buffer and written in "one go".


## BooTY

BooTY is the boot routine for any teletype. On Power-On (PON), the TTY
gets initialized and greets the user.

### TTY init

Due to mechanical complications, I figured that 5 "neutral" character
should be enough to get the TTY in a known state.

So for initialization the TTY gets 5x LTRS (0b11111 or int(31)).

### User Greeting

The User gets greeted by a single Question Mark `?`.
In order to print the Question Mark we need to shift the TTY into
Symbols-Mode.

So `tty_symbols[]` currently contains this:

```c

	-1,	// Terminator
	19,	// Question Mark
	27,	// FIGS
// ----------------------
	31,	
	31,
	31,
	31,
	31	// HEAD

```

### Main "shell"

The user currently have 3 different options to choose

`n` prints after "Startup" the current network config of the ESP8266  
`b` sets the baudrate
`w` sets the terminal width
`.` terminates every shell command

options can be concaternated with a single `,`.

Examples:


`?50b,72w,n.` sets baudrate to 50, terminal width to 72 and prints the
network status.

`?100b.` only sets the baudrate to 100 and leave the rest default.

`?.` just takes default values.

### Illegal options / Parsing errors

When an error has occured the teletype prints a `CRLF?` so the user
gets another chance.

### POST-Init

After a valid input has been made, the teletype just prints a `CRLF`
and exits to the main program.

The main program continues the User Interface-Initialization, both
MODE-LEDs get cleared and `mode` is set to 0.

System is initialized in LOCAL (Loopback) mode.

## Default values

This project was created to be compatible with a Siemens T37 Teletype.
This Teletype uses (by default) a **baudrate of 50** and the platen 
has a **width of 72 characaters**. This are the reasons for the default
values.

The network-parameter `n` is optional, so the TTY just prints the
summary if needed and not by default (plus I know my IP-Adress).

## Settings

The option `b` overwrites `tty.baud` with the given value  
The option `w` overwrites `tty.width` with the given value  
The option `n` writes down current network configuration like this:
`?n.192.168.1.233:80/24CRLF`

Please note figs/ltrs chars will be added, this was omitted to improve
readablity.


