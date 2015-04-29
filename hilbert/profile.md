# Hilbert Curve Algorithm Performance

Both `hil_s_from_xy()` and `hil_xy_from_s()` have *O(n)* time complexity (*n* being order of the Hilbert curve).

Ran on my 2.5G, late 2013 MBP.

Compiler:

    Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)
    Target: x86_64-apple-darwin13.3.0

Options: 

        -Os -std=gnu99

Result:
     
    Take 1
    1) Testing uint32 hilbert ...ok, 4194304 samples in 0.318s.
    2) Testing uint64 hilbert ...ok, 4194304 samples in 0.309s.
    3) Testing uint64 hilbert (z=20) ...ok, 4297185 samples in 0.580s.
    4) Testing tile coordinate to serial...ok, 3129702 samples in 0.453s.
    Take 2
    1) Testing uint32 hilbert ...ok, 4194304 samples in 0.338s.
    2) Testing uint64 hilbert ...ok, 4194304 samples in 0.314s.
    3) Testing uint64 hilbert (z=20) ...ok, 4297185 samples in 0.589s.
    4) Testing tile coordinate to serial...ok, 3129702 samples in 0.446s.
    Take 3
    1) Testing uint32 hilbert ...ok, 4194304 samples in 0.312s.
    2) Testing uint64 hilbert ...ok, 4194304 samples in 0.327s.
    3) Testing uint64 hilbert (z=20) ...ok, 4297185 samples in 0.574s.
    4) Testing tile coordinate to serial...ok, 3129702 samples in 0.451s.
     
Thats about 14 million `x,y->s` per second at level 20, which is probably fast enough for most applications.
     
(Profile shows `hil_s_from_xy` and `hil_xy_from_s` takes roughly same time).