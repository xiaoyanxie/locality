# CS40 Locality Project 🧠

[![Language C](https://img.shields.io/badge/language-C-00599C?logo=c&logoColor=white)](#)
[![Course COMP40](https://img.shields.io/badge/course-COMP40-1f6feb)](#)
[![Build Makefile](https://img.shields.io/badge/build-Makefile-f4a460)](#)

Cache locality and image transformation project from COMP 40.

## ✨ Highlights
- 2D array method implementations (`uarray2`, `uarray2b`)
- Traversal strategies: row-major, column-major, and blocked
- `ppmtrans` image rotations with timing support
- Performance analysis writeup and experiment results

## 📁 Project Structure
- `Makefile`: Build rules for `ppmtrans`, `a2test`, and `timing_test`
- `ppmtrans.c`: Image transformation driver
- `a2plain.c`, `a2blocked.c`: A2 methods adapters
- `uarray2.c`, `uarray2b.c`: 2D array implementations
- `cputiming.c`, `cputiming.h`: Timing support
- `a2test.c`, `timing_test.c`: Test programs
- `docs/performance-analysis.md`: Original class README/report

## 🛠️ Build
```bash
make
```

## ▶️ Run
Example usage (depends on your course environment/libraries):
```bash
./ppmtrans -rotate 90 -row-major input.ppm > out.ppm
./ppmtrans -rotate 180 -block-major -time timing.txt input.ppm > out.ppm
```

## 📊 Performance Notes
See:
- `docs/performance-analysis.md`

## 📄 License
Academic project code. Add a formal `LICENSE` if you plan to open-source publicly.
