#pragma once

#include <vector>

class Ticker
{
public:
    void frameStart();

    void frameEnd();

    [[nodiscard]] float deltaTime() const;

    [[nodiscard]] static double getTime();

    void toggleProfiling(bool enabled);

    [[nodiscard]] bool profilingEnabled() const;

private:
    double m_lastFrameTime = Ticker::getTime();
    float m_currentDeltaTime;

    bool m_profilingEnabled = false;

    float m_timeSinceLastLog = 0;
    std::vector<float> m_frameTimes;
    std::vector<float> m_deltaTimes;
};
