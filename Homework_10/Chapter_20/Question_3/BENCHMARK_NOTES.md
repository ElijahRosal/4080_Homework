# Hash Table Benchmark Suite (Question 3)

This folder now includes four benchmark programs that target different stress patterns:

- `bench_bulk`: bulk inserts + lookups for unique keys (throughput baseline).
- `bench_delete_churn`: random insert/delete/get mix to stress tombstones.
- `bench_collision`: compares normal keys against forced same-hash keys.
- `bench_scale`: reports per-op cost as table size increases.

## Why these test cases

- **Bulk baseline**: isolates best-case average behavior when hash distribution is good.
- **Delete-heavy churn**: validates behavior when tombstones accumulate and probe chains grow.
- **Forced collisions**: approximates adversarial/poor hash distribution to expose worst-case probing.
- **Scaling**: checks whether cache effects and resizing behavior change cost at larger sizes.

These four cover the main axes called out in the prompt: keyset sensitivity, size sensitivity, and operation-mix sensitivity.

## Expected performance variation

- `bench_bulk` should be the fastest overall.
- `bench_delete_churn` is usually slower than bulk due to tombstones and mixed write traffic.
- `bench_collision` should be significantly slower than normal-key runs because probing degenerates toward long clusters.
- `bench_scale` usually shows lower ns/op at tiny sizes (cache friendly), then a gradual increase as working set and probe distances grow.

## Build and run

From this directory:

```powershell
cmake -S . -B build
cmake --build build
./build/bench_bulk 100000
./build/bench_delete_churn 100000 300000
./build/bench_collision 50000
./build/bench_scale
```
