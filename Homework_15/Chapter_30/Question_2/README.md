# Chapter 30, Question 2 - Small String Optimization

Implemented a small-string optimization for the tagged-union `Value` representation:

- Added `VAL_SMALL_STRING` in `Value`.
- Added inline storage for tiny strings (`SMALL_STRING_INLINE_CAP = 6`).
- Added helper APIs so VM string ops work with either heap strings or inline strings.
- Updated concatenation to produce inline strings when the result is short.
- Added two benchmarks:
  - `bench_small_vs_heap` for constructing many tiny strings.
  - `bench_small_concat` for concatenating tiny strings repeatedly.

## Did it help?

Yes. After fixing the small-string accessor bug and rebuilding with MSYS2 MinGW64, both benchmarks ran successfully and the inline-small path was faster.

Measured results:

- `bench_small_vs_heap`
  - heap-only: 0.128 s, 25.60 ns/op
  - inline-small: 0.085 s, 17.00 ns/op
  - speedup: 1.51x
- `bench_small_concat`
  - heap-only: 0.374 s, 124.67 ns/op
  - inline-small: 0.279 s, 93.00 ns/op
  - speedup: 1.34x

That said, this optimization should help tiny-string-heavy workloads by:

- avoiding heap object dereference for short strings,
- avoiding intern-table lookup for short runtime results,
- improving cache locality by keeping characters in the `Value` itself.

## How to run locally

```bash
cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc
cmake --build build --target bench_small_vs_heap bench_small_concat
./build/bench_small_vs_heap
./build/bench_small_concat
```

If `inline-small` is faster than `heap-only` in both benchmarks, then the optimization helped.
