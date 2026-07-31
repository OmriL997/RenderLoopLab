# RenderLoopLab – Benchmarking Guide

## What the Benchmark Measures

RenderLoopLab's benchmark mode runs a deterministic 2D sprite simulation for
a configurable number of frames and records per-frame timing breakdowns.

**Measured quantities per frame:**
- Event processing + input time (`input_ms`)
- Fixed simulation update time (`update_ms`)
- Render submission time (`render_ms`)
- Buffer swap (VSync wait) time (`swap_ms`)
- Total wall-clock frame time (`total_ms`)

**Aggregate statistics:**
- Minimum, maximum, arithmetic mean, median
- 95th and 99th percentile (nearest-rank method)
- Population standard deviation
- Average FPS (calculated from mean frame time)
- Average draw calls and average triangles per frame

---

## Warm-up Frames

The first N frames (`--warmup`, default 120) are executed but not recorded.
Warm-up allows:
- GPU shader caches and JIT to reach steady state
- SDL driver overhead to stabilise
- CPU caches to warm for the working set

Only frames after the warm-up period contribute to statistics.

---

## Measured vs. Actual Frame Count

`--frames` sets the number of **measured** frames. The total execution is
`warmup + frames` rendered frames. Benchmark output reports only the measured
portion.

---

## VSync Effects

With VSync enabled (`--no-vsync` not set), the frame time is dominated by the
display refresh period (typically 16.67 ms at 60 Hz). This artificially
compresses the measurement range and makes the benchmark less useful for
comparing GPU performance.

**Recommendation:** Always use `--no-vsync` for performance benchmarking.

```
renderlooplab --benchmark --no-vsync --frames 3000 ...
```

---

## Frame Time vs. FPS

Average FPS is calculated as `1000 / mean_frame_time_ms`. It is a derived
metric, not directly measured. FPS is a harmonic relationship with frame time:
small improvements at low FPS are more impactful than the same improvement at
high FPS.

Both mean and median are reported. Mean is sensitive to outliers (frame spikes);
median is more representative of the typical frame. Prefer median for comparing
typical performance.

---

## Percentiles and Frame Spikes

The 95th and 99th percentiles reveal frame-time spikes that would be invisible
in the mean.

| Metric | Interpretation |
|---|---|
| `mean` | Average frame budget usage |
| `median` | Typical frame performance |
| `p95` | "Worst 5%" threshold — important for perceived smoothness |
| `p99` | "Worst 1%" — captures rare OS preemptions or GC pauses |
| `max` | Absolute worst frame |

A tight `max` relative to `mean` indicates stable performance.
A large gap between `p99` and `mean` indicates occasional spikes.

---

## Percentile Method

RenderLoopLab uses the **nearest-rank** (lower-bound) method:

```
index = ceil(p / 100.0 * N) - 1   (clamped to [0, N-1])
```

For a dataset sorted ascending with N samples, the p-th percentile is the
value at the computed index. This is deterministic for any fixed sample set.

---

## Measurement Noise

Even with VSync disabled and CPU/GPU at steady state, frame times vary due to:
- OS scheduler preemptions
- Memory allocator contention
- GPU driver housekeeping
- Thermal throttling
- Cache eviction from background processes

A short benchmark (< 1000 frames) captures more noise than a long one. Use
3000+ frames for more stable statistics on typical hardware.

---

## Cross-Machine Comparison

**Results from different machines cannot be directly compared** without:
- Matching hardware (CPU, GPU, RAM speed)
- Matching OS and driver versions
- Matching background load
- Matching build configuration (Debug vs. Release, compiler, optimisation flags)

The JSON report captures GPU vendor/renderer, OS, and compiler strings to aid
in contextual comparison.

---

## How to Run a Repeatable Benchmark

1. Close background applications.
2. Use a fixed seed for a reproducible scene: `--seed 12345`
3. Use the same sprite count: `--sprites 1000`
4. Disable VSync: `--no-vsync`
5. Use a hidden window to reduce display composition overhead: `--hidden`
6. Use sufficient frames: `--frames 3000 --warmup 120`
7. Save output: `--csv results.csv --report results.json`

Example:
```bash
./renderlooplab \
  --benchmark \
  --sprites 1000 \
  --frames 3000 \
  --warmup 120 \
  --seed 12345 \
  --no-vsync \
  --hidden \
  --csv benchmark.csv \
  --report benchmark.json
```

---

## Why Benchmark Numbers Are Not Deterministic

The scene (sprite positions, velocities) is deterministic given the same seed
and update sequence. The **wall-clock time to render** each frame is not, due
to the hardware and OS factors listed above.

The benchmark deliberately records actual measured times rather than fabricated
values. Benchmark output reflects the real performance of the specific machine,
OS, GPU, and driver combination used.

---

## This Is Not a Scientific Study

RenderLoopLab is a focused portfolio demonstration. Its benchmark:
- Does not control for all hardware variables
- Does not use statistical significance testing
- Does not compare against other renderers
- Is not representative of production game performance

It is intended to demonstrate benchmark methodology and produce meaningful
relative performance data within a controlled configuration, not to make
absolute performance claims.
