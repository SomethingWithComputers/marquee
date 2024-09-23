#ifndef MARQUEEWATCH_H
#define MARQUEEWATCH_H

#include <Watchy.h>

class MarqueeWatch : public Watchy {

  public:
    MarqueeWatch(const watchySettings& settings);

    void drawWatchFace() override;

  private:
    void drawBackground();
    void drawBatteryBirds();
    void drawDate();
    void drawTime();
    void drawVines();
};

#endif