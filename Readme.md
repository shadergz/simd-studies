# SIMD-Studies

A study collection of SSE technologies use-case

## Brief
The SSE (Streaming SIMD Extesions) is a feature of every CISC processor created by Intel and AMD processor manufactures. The technology invented by Intel for replace the older extension called "MMX", brings new 70 intructions that is used into graphics processing area for improve the performance of various math operaion at the decode and mixing parts.

## Linear Data Processing
The SSE enable the processing of 128:256 bits of information in only once operations by reading a vector from a aligned memory address.
> Vector: A sequence of data into a specific place with a defined sequence. Can be of the type (int, float, double).
>
From the developer pespective, we can load 8 integers (32 * 8 = 256 bits or 32 bytes of data in a only intructions, this makes a better use of the CPU caches and with the data BUS of the motherboard hardware). We can make this using the 8 new registers added (XMM0:XMM7).

## What is available into this study project

### Memory
Memory based excluvily use case for SSE instructions, see below:
- C memset implementation

### Math
Some math operations use cade for SSE intructions, see below:
- Linear Multiplication of 2 float vectors

## Strings
Related strings operations use case for SSE instructions, see below:
- C tolower implementation (Isn't fully complete)

