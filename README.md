# Rubbish PRNG
This is a completely rubbish pseudorandom number generator written as a SourceMod include, which is still good enough to run on Windows & Linux, 32-bit and 64-bit, at least.

Featuring the likelihood to corrupt process memory and also providing insanely slow pseudorandom number generation, this PRNG will make you want to reconsider your life choices. Fear not, for if you take a look at the include presented, your thought process will be too over-convoluted that you will not be able to comment on its atrocities.

# CPU requirements
- Intel: Ivy Bridge and later
- AMD: Excavator and later

# How to use
Drag `scripting/include/rubbish prng.inc` into your own project, `#include` it and set the seed using `srand()` (THIS IS IMPORTANT). Then, call `rand()` as many times as you'd like.
