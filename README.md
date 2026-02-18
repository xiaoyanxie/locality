# Locality Project 🧠

[![Language C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)](#)
[![Course COMP40](https://img.shields.io/badge/course-COMP40-1f6feb)](#)
[![Usage](https://img.shields.io/badge/usage-reference%20only-orange)](#)
[![Project Type](https://img.shields.io/badge/type-cache%20locality-ff69b4)](#)

COMP 40 HW3 implementation focused on cache-friendly image transformations.

## ⚠️ Academic Integrity Notice
This repository is for portfolio and learning reference only.
Do not copy, submit, or adapt this code for coursework or academic evaluation.

## ✨ What's Inside
- `UArray2` and blocked `UArray2b` implementations
- Row-major, column-major, and block-major traversal strategies
- `ppmtrans` rotations: `0`, `90`, `180`, `270`
- Timing-based performance experimentation

## 🗂️ Repository Layout
- `ppmtrans.c`: Image transformation driver
- `a2plain.c`, `a2blocked.c`: A2 methods adapters
- `uarray2.c`, `uarray2b.c`: 2D array implementations
- `cputiming.c`, `cputiming.h`, `cputiming_impl.h`: Timing utilities
- `a2test.c`, `timing_test.c`: Test binaries
- `docs/performance-analysis.md`: Detailed design and experiment report

## 🛠️ Build
```bash
make
```

Note: build depends on COMP 40 course libraries configured in `Makefile`.

## ▶️ Run
```bash
./ppmtrans -rotate 90 -row-major input.ppm > out.ppm
./ppmtrans -rotate 180 -block-major -time timing.txt input.ppm > out.ppm
```

## 🚀 Performance Snapshot
Source: `docs/performance-analysis.md`

| Image | Best Method Observed | Time / Pixel |
| --- | --- | --- |
| `flowers.ppm` (18,100 px) | `90_row` | `1,593 ns` |
| `mobo.ppm` (49,939,200 px) | `180_row` | `47 ns` |

Key observations:
- Small input: row-major traversal is fastest
- Large input: locality-aware strategies significantly reduce per-pixel time

## 📚 Detailed Report
- `docs/performance-analysis.md`

## 📄 Usage Terms
No open-source license is granted for reuse in coursework submissions.
All rights reserved unless explicit permission is provided by the author.
