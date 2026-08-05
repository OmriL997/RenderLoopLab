#!/usr/bin/env bash
# RenderLoopLab — Portfolio Demo Script (Linux / macOS)
# Demonstrates: tests, shader hot-reload, compile failure, benchmark, CSV/JSON output, CI
#
# Usage (from repo root, after a Release build):
#   bash demo/run_demo.sh

set -euo pipefail

REPO="$(cd "$(dirname "$0")/.." && pwd)"
EXE="$REPO/build/bin/renderlooplab"
TESTS="$REPO/build/bin/renderlooplab_tests"
OUT_DIR="$REPO/demo"

header() {
    echo ""
    printf '%0.s=' {1..60}; echo
    echo "  $1"
    printf '%0.s=' {1..60}; echo
}

# ── Prerequisite check ────────────────────────────────────────
if [ ! -x "$EXE" ]; then
    echo "ERROR: Executable not found at $EXE" >&2
    echo "Build first: cmake --build build && make -j" >&2
    exit 1
fi

# Headless GL environment (Mesa software rasterizer)
export SDL_VIDEODRIVER=offscreen
export LIBGL_ALWAYS_SOFTWARE=1
export MESA_LOADER_DRIVER_OVERRIDE=swrast

# ── 1. Unit test count ────────────────────────────────────────
header "1 / 5  Unit Tests"
echo "Running Catch2 test suite..."
ctest --test-dir "$REPO/build" --output-on-failure

echo ""
echo "Direct runner summary:"
"$TESTS" --reporter compact 2>&1 | grep -E "passed|failed|test cases" || true

# ── 2. Shader hot-reload & compile-failure instructions ───────
header "2 / 5  Shader Hot-Reload"
cat <<'EOF'
Run the app interactively and press:
  F5  — toggles between 'default' (color) and 'alt (grayscale)' shaders.
        The window title reflects the active shader.
  F6  — attempts to compile a broken shader (syntax error).
        The console shows the GLSL compile log; rendering is unaffected.
        The window title briefly shows '[err: compile failed]'.
EOF

# ── 3. Benchmark ──────────────────────────────────────────────
header "3 / 5  Benchmark Output"
CSV_OUT="$OUT_DIR/output_benchmark.csv"
JSON_OUT="$OUT_DIR/output_benchmark.json"

echo "Running benchmark (500 sprites, 300 frames, no VSync, headless)..."
"$EXE" \
    --benchmark \
    --sprites 500 \
    --frames  300 \
    --warmup   60 \
    --seed  12345 \
    --no-vsync \
    --hidden \
    --csv  "$CSV_OUT" \
    --report "$JSON_OUT"

# ── 4. CSV / JSON result samples ──────────────────────────────
header "4 / 5  CSV & JSON Results"

echo "--- CSV (first 6 lines) ---"
head -6 "$CSV_OUT"

echo ""
echo "--- JSON summary ---"
cat "$JSON_OUT"

# ── 5. CI environments summary ────────────────────────────────
header "5 / 5  CI Environments"
cat <<'EOF'
GitHub Actions (.github/workflows/ci.yml) runs on every push/PR:

  ubuntu-gcc          Ubuntu (GCC, Release)
                      configure → build → asset check → CPU tests
                      → headless smoke test → short benchmark
                      → upload benchmark.csv + benchmark.json as artifacts

  ubuntu-sanitizers   Ubuntu (GCC, ASan + UBSan, RelWithDebInfo)
                      CPU tests only (AddressSanitizer + UndefinedBehaviourSanitizer)

  windows-msvc        Windows (MSVC, Release)
                      configure → build → asset check → CPU tests

Badge:
  https://github.com/OmriL997/RenderLoopLab/actions/workflows/ci.yml
EOF

header "Demo complete"
echo "CSV  : $CSV_OUT"
echo "JSON : $JSON_OUT"
