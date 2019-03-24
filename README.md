# Multi-threading-demo-with-PPM

## Overview

This repository implemented ppm(a kind of image format) class and use multi-threading techniques to accelerate Gaussian Blur.

![asyousee.png](https://github.com/ganler/Multi-threading-demo-with-PPM/blob/master/asyousee.png?raw=true)

**The codes are totally programed with standard `C++14/17`. No other dependency, your new-version compiler is the only dependency.**

#### You'll learn

- How to build a template(actually not necessary) PPM from scratch.
- How to use multi-threading with `std::thread` provided by the standard library to accelerate your program.
- How to implement Gaussian Blur.

Here is the result of mine:

```bash
➜  cat /proc/cpuinfo | grep name | cut -f2 -d:  | uniq -c
	12  Genuine Intel(R) CPU 0000 @ 2.90GHz
➜   ./a.out                          
	Multi-threading: 104 ms
	Non-multi-threading: 638 ms
```

## Quick Start

```bash
g++ main.cpp -std=c++17 -lpthread -O2
./a.out
```

> Of course, my code is also compatible with c++14.
