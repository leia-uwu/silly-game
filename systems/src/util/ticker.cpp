#include "util/ticker.h"

#include <algorithm>
#include <cfloat>
#include <format>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#else
#include <chrono>
#endif

void Ticker::frameStart()
{
    double now = Ticker::getTime();
    m_currentDeltaTime = std::clamp(now - m_lastFrameTime, (double)FLT_MIN, 0.125);
    m_lastFrameTime = now;
}

void Ticker::frameEnd()
{
    if (!m_profilingEnabled)
        return;

    double now = Ticker::getTime();

    m_frameTimes.push_back(now - m_lastFrameTime);
    m_deltaTimes.push_back(m_currentDeltaTime);

    m_timeSinceLastLog += m_currentDeltaTime;

    if (m_timeSinceLastLog > 10.F) {
        double avgDt = 0;
        for (auto dt : m_deltaTimes) {
            avgDt += dt;
        }
        avgDt /= m_deltaTimes.size();

        double avgFrameTime = 0;
        for (auto time : m_frameTimes) {
            avgFrameTime += time;
        }
        avgFrameTime /= m_frameTimes.size();

        std::cout << "----------------------------\n";

        std::cout << std::format("Average FPS: {0:.4f}\n", (1.0 / avgDt));

        std::cout << std::format("Average frame time: {0:.4f}ms\n", avgFrameTime * 1000.0);

        m_deltaTimes.clear();
        m_frameTimes.clear();

        m_timeSinceLastLog = 0.F;
    }
}

[[nodiscard]] float Ticker::deltaTime() const
{
    return m_currentDeltaTime;
}

[[nodiscard]] double Ticker::getTime()
{
#ifdef __EMSCRIPTEN__
    return emscripten_performance_now() / 1000.0;
#else
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(now).count() / 1000.0;
#endif
}

void Ticker::toggleProfiling(bool enabled)
{
    m_profilingEnabled = enabled;
}

[[nodiscard]] bool Ticker::profilingEnabled() const
{
    return m_profilingEnabled;
}
