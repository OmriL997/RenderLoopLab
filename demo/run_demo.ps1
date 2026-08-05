# RenderLoopLab — Portfolio Demo Script (Windows / PowerShell)
# Demonstrates: tests, shader hot-reload, compile failure, benchmark, CSV/JSON output, CI
#
# Usage (from repo root, after a Release build):
#   powershell -ExecutionPolicy Bypass -File demo\run_demo.ps1

$ErrorActionPreference = "Stop"
$repo   = Split-Path $PSScriptRoot -Parent
$exe    = "$repo\build\bin\Release\renderlooplab.exe"
$tests  = "$repo\build\bin\Release\renderlooplab_tests.exe"
$outDir = "$repo\demo"

function Header($text) {
    Write-Host ""
    Write-Host ("=" * 60) -ForegroundColor Cyan
    Write-Host "  $text" -ForegroundColor Cyan
    Write-Host ("=" * 60) -ForegroundColor Cyan
}

# ── Prerequisite check ────────────────────────────────────────
if (-not (Test-Path $exe)) {
    Write-Host "ERROR: Executable not found at $exe" -ForegroundColor Red
    Write-Host "Build first: cmake --build build --config Release"
    exit 1
}

# ── 1. Unit test count ────────────────────────────────────────
Header "1 / 5  Unit Tests"
Write-Host "Running Catch2 test suite..." -ForegroundColor Yellow
& ctest --test-dir "$repo\build" -C Release --output-on-failure
Write-Host ""
Write-Host "Direct test runner output (summary line):" -ForegroundColor Yellow
& $tests --reporter compact 2>&1 | Select-String -Pattern "passed|failed|test cases"

# ── 2. Shader hot-reload & compile-failure instructions ───────
Header "2 / 5  Shader Hot-Reload"
Write-Host @"
Run the app interactively and press:
  F5  — toggles between 'default' (color) and 'alt (grayscale)' shaders.
        The window title reflects the active shader.
  F6  — attempts to compile a broken shader (syntax error).
        The console shows the GLSL compile log; rendering is unaffected.
        The window title briefly shows '[err: compile failed]'.

  Start app:  $exe
"@ -ForegroundColor Yellow

# ── 3. Benchmark ──────────────────────────────────────────────
Header "3 / 5  Benchmark Output"
$csvOut  = "$outDir\output_benchmark.csv"
$jsonOut = "$outDir\output_benchmark.json"

Write-Host "Running benchmark (500 sprites, 300 frames, no VSync, headless)..." -ForegroundColor Yellow
& $exe `
    --benchmark `
    --sprites 500 `
    --frames  300 `
    --warmup   60 `
    --seed  12345 `
    --no-vsync `
    --hidden `
    --csv  $csvOut `
    --report $jsonOut

# ── 4. CSV / JSON result samples ──────────────────────────────
Header "4 / 5  CSV & JSON Results"

Write-Host "--- CSV (first 6 lines of $csvOut) ---" -ForegroundColor Yellow
Get-Content $csvOut | Select-Object -First 6

Write-Host ""
Write-Host "--- JSON summary ($jsonOut) ---" -ForegroundColor Yellow
Get-Content $jsonOut

# ── 5. CI environments summary ────────────────────────────────
Header "5 / 5  CI Environments"
Write-Host @"
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
"@ -ForegroundColor Yellow

Header "Demo complete"
Write-Host "CSV  : $csvOut"
Write-Host "JSON : $jsonOut"
