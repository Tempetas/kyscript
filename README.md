## KyScript

There are 6 registers: A, B, C, X, Y, Z.
All of them contain signed 64 bit integers.

Comments can be done using `;`.

The interpreter ignores any lines with text where the first word isn't an
opcode. Thus, for example this README can be executed, though I wouldn't
recommend using this quirk.

Example:
```
PRINT 123 ; Hello world!
```

The A register is often used by internal functions.

`PRINT`: prints the value
Example: PRINT Y

`SET`: set a register to a value
Example:
```
SET X, 4
SET Y, X
```

`ADD`: same syntaxis as `SET`, add a value to a register

`MUL`: same syntaxis as `ADD`, multiply a value in a register

`JUMP`: jump to a line in code
Example:
```
JUMP 32 ; Jump to line 32
```

`IFEQ`: compare values and skip the next line if they aren't equal
Example:
```
SET A, 33 ; Store 33 in A
IFEQ A, 34 ; Compare
PRINT 123 ; This will never be printed
PRINT 321 ; And this will
```

`IFMR`: same as `IFEQ` but checks whether the first parameter is bigger
