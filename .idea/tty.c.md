# TTY Library Documentation

## Main interface for TTY

- `char* TTY_WriteCharBuffer(char* buffer)`
  - This returns an empty string if everything is sent correct
- `int* TTY_WRITEBUFFER(int* buffer)`
- `char TTYreadChar()`
  - This returns the current char or `0x00` if timeout

## Configuration Functions

- void setLoopback(int _loopback)
- void setBaudrate(int baudrate)
- void setTermWidth(int termwidth)
- void setStopbits(float stopbit)
- void setSendMode()
  - Self-resetting

## Hardware Interface Functions

- void setTTY(int state)
- int readTTY()

## Buffer Manipulation Functions

- int* appendSymbol(int* head, int sym)
- int* appendChar(int* head, char c)
- int getBufferLength(int* head)
- int toSymbol(char c)

## TTY Communication Functions

- void TTY_WRITE(int _sym)
- int TTY_READ()
- int readSymbol()
- int* TTY_WRITEBUFFER(int* buffer)

## Timing and Control Functions

- void TTY_DELAY(int cycles)
- void TTY_Startbit()
- void TTY_Stopbit()

## Hardware Interface Functions

- setTTY
- readTTY

## Utility Functions

- int majority(Databit d)
- int* toSymbols(char* chars)

## Debug Functions

- int* TTY_FOX(int* buffer)
- void TTY_DEBUG()

## Error Handling Functions

- void setReadError()
- void clearReadError()


