# CS40 HW3 Locality 🧠⚡

[![Language C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)](#)
[![Course COMP40](https://img.shields.io/badge/course-COMP40-1f6feb)](#)
[![Topic Cache Locality](https://img.shields.io/badge/topic-cache%20locality-ff69b4)](#)
[![Status Clean History](https://img.shields.io/badge/history-clean-2ea44f)](#)

A standalone, clean-history repository for the COMP 40 HW3 Locality project.

## Why This Repo Exists
This project was migrated out of the original course repository so the commit history here reflects real project work only.

## ✨ Highlights
- Implemented `UArray2` and blocked `UArray2b` data structures
- Supports row-major, column-major, and block-major traversal
- `ppmtrans` supports rotations: `0`, `90`, `180`, `270`
- Built-in timing support for performance experiments

## 🚀 Benchmark Snapshot
Source: `docs/performance-analysis.md`

| Image | Best Method Observed | Time / Pixel |
| --- | --- | --- |
| `flowers.ppm` (18,100 px) | `90_row` | `1,593 ns` |
| `mobo.ppm` (49,939,200 px) | `180_row` | `47 ns` |

Key observation:
- Small input: row-major tends to win
- Large input: blocked traversal improves cache behavior significantly

## 📁 Project Structure
- `ppmtrans.c`: Image transformation driver
- `a2plain.c`, `a2blocked.c`: A2 methods adapters
- `uarray2.c`, `uarray2b.c`: 2D array implementations
- `cputiming.c`, `cputiming.h`: Timing utilities
- `a2test.c`, `timing_test.c`: Test binaries
- `docs/performance-analysis.md`: Original detailed design + experiment report

## 🛠️ Build
```bash
make
```

## ▶️ Run
```bash
./ppmtrans -rotate 90 -row-major input.ppm > out.ppm
./ppmtrans -rotate 180 -block-major -time timing.txt input.ppm > out.ppm
```

Note: build and runtime depend on the COMP 40 course libraries configured in `Makefile`.

## 📚 Detailed Report
- `docs/performance-analysis.md`
- `docs/legacy-readme.md`
