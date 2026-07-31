#include "renderlooplab/benchmark_report.hpp"
#include "renderlooplab/logging.hpp"
#include "renderlooplab/version.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace rll {

// ---------------------------------------------------------------------------
// Compiler / OS identification (compile-time)
// ---------------------------------------------------------------------------

std::string compilerIdentString()
{
#if defined(_MSC_VER)
    return "MSVC " + std::to_string(_MSC_VER);
#elif defined(__clang__)
    return std::string("Clang ") + __clang_version__;
#elif defined(__GNUC__)
    return std::string("GCC ") + std::to_string(__GNUC__) + "." +
           std::to_string(__GNUC_MINOR__) + "." +
           std::to_string(__GNUC_PATCHLEVEL__);
#else
    return "Unknown compiler";
#endif
}

std::string osIdentString()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown OS";
#endif
}

// ---------------------------------------------------------------------------
// Console summary
// ---------------------------------------------------------------------------

void printBenchmarkSummary(const BenchmarkReport& report)
{
    const auto& s  = report.stats;
    const auto& c  = report.config;
    const auto& e  = report.env;

    std::printf("\nRenderLoopLab Benchmark\n");
    std::printf("-----------------------\n");
    std::printf("Resolution:          %dx%d\n",     c.width, c.height);
    std::printf("Sprites:             %d\n",         c.sprites);
    std::printf("Seed:                %llu\n",       static_cast<unsigned long long>(c.seed));
    std::printf("Fixed update rate:   %.0f Hz\n",   c.fixedHz);
    std::printf("Warm-up frames:      %d\n",         c.warmupFrames);
    std::printf("Measured frames:     %d\n",         s.sampleCount);
    std::printf("VSync:               %s\n",         c.vsync ? "enabled" : "disabled");
    std::printf("GPU vendor:          %s\n",         e.gpuVendor.c_str());
    std::printf("GPU renderer:        %s\n",         e.gpuRenderer.c_str());
    std::printf("OpenGL version:      %s\n",         e.openglVersion.c_str());
    std::printf("\n");
    std::printf("Average frame time:  %.2f ms\n",   s.mean);
    std::printf("Median  frame time:  %.2f ms\n",   s.median);
    std::printf("95th percentile:     %.2f ms\n",   s.percentile95);
    std::printf("99th percentile:     %.2f ms\n",   s.percentile99);
    std::printf("Minimum frame time:  %.2f ms\n",   s.min);
    std::printf("Maximum frame time:  %.2f ms\n",   s.max);
    std::printf("Std deviation:       %.2f ms\n",   s.stddev);
    std::printf("Average FPS:         %.2f\n",      s.avgFps);
    std::printf("Average draw calls:  %.2f\n",      s.avgDrawCalls);
    std::printf("Average triangles:   %.0f\n",      s.avgTriangles);
    std::printf("\n");
    std::fflush(stdout);
}

// ---------------------------------------------------------------------------
// JSON report (hand-written to avoid an extra dependency)
// ---------------------------------------------------------------------------

namespace {

// Escape a string for JSON (handles backslash, double-quote, control chars)
std::string jsonEscape(const std::string& s)
{
    std::string out;
    out.reserve(s.size() + 4);
    for (unsigned char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    std::snprintf(buf, sizeof(buf), "\\u%04x", static_cast<unsigned>(c));
                    out += buf;
                } else {
                    out += static_cast<char>(c);
                }
                break;
        }
    }
    return out;
}

std::string fmtd(double v, int prec = 2)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << v;
    return ss.str();
}

} // anon

bool writeBenchmarkJson(const std::string& path, const BenchmarkReport& report)
{
    std::ofstream f(path);
    if (!f.is_open()) {
        log::error("BenchmarkReport", "Cannot open JSON file: " + path);
        return false;
    }

    const auto& s = report.stats;
    const auto& c = report.config;
    const auto& e = report.env;

    f << "{\n"
      << "  \"tool\": \"RenderLoopLab\",\n"
      << "  \"tool_version\": \"" << VERSION_STRING << "\",\n"
      << "  \"mode\": \"benchmark\",\n"
      << "  \"environment\": {\n"
      << "    \"operating_system\": \"" << jsonEscape(e.operatingSystem) << "\",\n"
      << "    \"compiler\": \""         << jsonEscape(e.compiler)        << "\",\n"
      << "    \"gpu_vendor\": \""       << jsonEscape(e.gpuVendor)       << "\",\n"
      << "    \"gpu_renderer\": \""     << jsonEscape(e.gpuRenderer)     << "\",\n"
      << "    \"opengl_version\": \""   << jsonEscape(e.openglVersion)   << "\"\n"
      << "  },\n"
      << "  \"configuration\": {\n"
      << "    \"width\": "      << c.width          << ",\n"
      << "    \"height\": "     << c.height         << ",\n"
      << "    \"sprites\": "    << c.sprites        << ",\n"
      << "    \"frames\": "     << s.sampleCount    << ",\n"
      << "    \"warmup_frames\": " << c.warmupFrames << ",\n"
      << "    \"seed\": "       << c.seed           << ",\n"
      << "    \"fixed_hz\": "   << fmtd(c.fixedHz,1)<< ",\n"
      << "    \"vsync\": "      << (c.vsync ? "true" : "false") << "\n"
      << "  },\n"
      << "  \"rendering\": {\n"
      << "    \"average_draw_calls\": " << fmtd(s.avgDrawCalls) << ",\n"
      << "    \"average_triangles\": "  << fmtd(s.avgTriangles, 0) << "\n"
      << "  },\n"
      << "  \"timing_ms\": {\n"
      << "    \"minimum\": "          << fmtd(s.min)          << ",\n"
      << "    \"maximum\": "          << fmtd(s.max)          << ",\n"
      << "    \"mean\": "             << fmtd(s.mean)         << ",\n"
      << "    \"median\": "           << fmtd(s.median)       << ",\n"
      << "    \"percentile_95\": "    << fmtd(s.percentile95) << ",\n"
      << "    \"percentile_99\": "    << fmtd(s.percentile99) << ",\n"
      << "    \"standard_deviation\": "<< fmtd(s.stddev)      << "\n"
      << "  },\n"
      << "  \"average_fps\": " << fmtd(s.avgFps) << "\n"
      << "}\n";

    if (!f) {
        log::error("BenchmarkReport", "Error writing JSON: " + path);
        return false;
    }
    return true;
}

} // namespace rll
