# crccheck

In theory, CRC is supposed to be much faster than any cryptographic-grade hash algorithm. But in practice, I can't find any package in Debian/Ubuntu's repository that provides a shell-scriptable command line program that actually gives you a hash with CRC quicker than it takes `md5sum` to run.

There was a library called `libcrcutil` that was supposed to be pretty fast (especially with SSE 4.2), but it was just a library without an accompanying command-line program.

So, here's that program. It takes input on stdin and prints the checksum to stdout. No command line arguments are acknowledged. If nothing else, it is fast.

make sure the library and its headers are installed, then build & install with
```
g++ -O3 -march=native -flto '/tmp/examples/usage.cc' '/tmp/examples/interface.cc' -lcrcutil
sudo mv a.out /usr/local/bin/crccheck
```

## Disclaimer.
This was adapted from the example code bundled in Debian/Ubuntu's `libcrcutil-doc` package. To get the program to run I had to fix the broken header include statements replace the weird non-standard `AlignedAlloc` (not to be confused with C11 `aligned_alloc`) allocator they were using, which doesn't seem to exist anywhere. It crashed when I tried to use `posix_memalign` and `aligned_alloc`, but `valloc` (which is supposed to be old and deprecated) seemed to work.

This code is probably very bad and if you use it for anything reckless it might eat your baby. Caveat emptor.
