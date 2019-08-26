# crccheck

In theory, CRC is supposed to be much faster than any cryptographic-grade hash algorithm. But in practice, I can't find any package in Debian/Ubuntu's repository that provides a shell-scriptable command line program that actually gives you a hash with CRC quicker than it takes `md5sum` to run.

There was a library called `libcrcutil` that was supposed to be pretty fast (especially with SSE 4.2), but it was just a library without an accompanying command-line program.

So, here's that program. It takes input on stdin and prints the checksum to stdout. No command line arguments are acknowledged. If nothing else, it is fast.

This bundles the library source, so no need to install the library or its headers. Just build & install with

```bash
g++ -static -O3 -no-pie -march=native -fno-stack-protector -flto -fpermissive *.cc
sudo mv a.out /usr/local/bin/crccheck
```

The `-static` flag actually makes a huge difference. It actually couldn't beat the standard `md5sum` utility until I tried it. Apparently C++ is slow at dynamic linking due to too many symbols, and this really weighs it down when the program is being invoked repeatedly in a Bash loop (which is this program's intended use case). It has no such problem when it's statically built, though.

Although it beats `md5sum`, it's only about ~25% faster, which is not as much as I hoped TBH. For many purposes it may not be worth messing with...

## Disclaimer.
This was adapted from the example code bundled in Debian/Ubuntu's `libcrcutil-doc` package. To get the program to build I had to replace the weird non-standard `AlignedAlloc` (not to be confused with C11 `aligned_alloc`) allocator they were using, which doesn't seem to exist anywhere. It crashed when I tried to use `posix_memalign` or `aligned_alloc`, but the always-page-aligned `valloc` (which is supposed to be old and deprecated) seemed to work.

I made no effort to ensure output is compatible with any particular other CRC implementation besides itself. The algorithm is provided by a good library, but I might be using weird settings or something IDRK.

This code is probably overall very bad and if you use it for anything reckless it might eat your baby. Caveat emptor.
