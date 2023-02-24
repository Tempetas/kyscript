# Kyscript

To run a file in debugging mode, pass a `--debug` flag to the executable.

There are 26 * 26 "registers" with id's like AA, AB, AC, BC, etc.
All of them can contain signed 64 bit integers.

Script command arguments are loaded into the registers starting with the A register.

A line of source code cannot not be longer than 128 characters.
The source file cannot contain more than 1024 lines.

Comments can be made using `;`, they must always come after a space.

Example:
```
PRINT 123 ; This is a comment
```

![Screenshot](https://github.com/Tempetas/kyscript/blob/main/screenshot.png?raw=true)

## Opcodes

`PRINT`: prints the value, supports strings
```
PRINT "Among us\n"
PRINT Y
```

`INPUT`: store user input in a register
```
INPUT X
PRINT X
```

`SET`: set a register to a value
```
SET X, 4
SET Y, X
```

`ADD`: same syntaxis as `SET`, add a value to a register

`MUL`: same syntaxis as `SET`, multiply a value in a register

`DIV`: same syntaxis as `SET`, divide a value in a register

`MOD`: perform a modulo operation on a register
```
SET X, 5
MOD X, 2
PRINT X ; Outputs 1, as it is the result of 5 % 2
```

`JUMP`: jump to a line in code, using + or - makes the jump relative to the
current line
```
.loop ; Label
JUMP 32 ; Jump to line 32
JUMP +1 ; Jump 2 lines further
JUMP -4 ; Jump 4 lines back
JUMP loop ; Jump to the loop label
```

`IFEQ`: compare values and skip the next line if they aren't equal
```
SET A, 33 ; Store 33 in A
IFEQ A, 34 ; Compare
PRINT 123 ; This will never be printed
PRINT 321 ; And this will
```

`IFMR`: same as `IFEQ` but checks whether the first parameter is bigger

`PARAM`: change the according interpreter setting
```
PARAM PRINT_NUM_NEWLINE 0 ; Prevents `PRINT` from appending a newline to each integer output
PARAM DEBUG 1 ; Makes the interpreter print debugging information
```
