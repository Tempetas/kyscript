## KyScript

There are 26*26 "registers" with id's like AA, AB, AC, BC, etc.
All of them can contain signed 64 bit integers.

Comments can be made using `;`.

Example:
```
PRINT 123 ; Hello world!
```

The interpreter ignores any lines with text where the first word isn't an
opcode. Thus, for example this README can be executed, though I wouldn't
recommend using this quirk.

`PRINT`: prints the value
```
PRINT Y
```

`SET`: set a register to a value
```
SET X, 4
SET Y, X
```

`ADD`: same syntaxis as `SET`, add a value to a register

`MUL`: same syntaxis as `ADD`, multiply a value in a register

`JUMP`: jump to a line in code
```
JUMP 32 ; Jump to line 32
```

`IFEQ`: compare values and skip the next line if they aren't equal
```
SET A, 33 ; Store 33 in A
IFEQ A, 34 ; Compare
PRINT 123 ; This will never be printed
PRINT 321 ; And this will
```

`IFMR`: same as `IFEQ` but checks whether the first parameter is bigger
