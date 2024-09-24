#include "MarqueeWatch.h"

#include "bitmaps.h"
#include "constants.h"
#include "fonts.h"

#define DARK_MODE false

#define BATTERY_OFFSET 0.00  // This offset is different for every Watchy
#define COLOR DARK_MODE ? GxEPD_WHITE : GxEPD_BLACK
#define INVERTED_COLOR DARK_MODE ? GxEPD_BLACK : GxEPD_WHITE

#define MAXIMUM_NUMBER_OF_BIRDS 6
#define MAXIMUM_NUMBER_OF_LEAFS_PER_VINE 34

MarqueeWatch::MarqueeWatch(const watchySettings& settings)
  : Watchy(settings) {}

// Overrides
void MarqueeWatch::drawWatchFace() {
  drawBackground();
  drawBatteryBirds();
  drawDate();
  drawTime();
  drawVines();
}

// Methods
void MarqueeWatch::drawBackground() {
  display.fillScreen(INVERTED_COLOR);
  display.drawBitmap(0, 0, BITMAP_BACKGROUND, DISPLAY_WIDTH, DISPLAY_HEIGHT, COLOR);
}

void MarqueeWatch::drawBatteryBirds() {
  // See how many birds we have to show
  int numberOfBirds = (int)ceil(getBatteryPercentage() / (100 / MAXIMUM_NUMBER_OF_BIRDS));

  // Clip them birds just to be safe
  numberOfBirds = min(6, max(1, numberOfBirds));

  // Check if we need to do anything
  if(6 == numberOfBirds) {
    return;
  }

  // Let's hide some birds! (bottom right 144, 57)
  int x = BATTERY_BIRD_POSITIONS_X[numberOfBirds];
  display.fillRect(x, 47, 144 - x, 10, INVERTED_COLOR);
}

void MarqueeWatch::drawDate() {
  // Draw the short name of the month
  const int* shortNameIndexes = DATE_TO_SHORTNAME_INDEXES[currentTime.Month - 1];
  display.drawBitmap(34, 106, ISOMETRIC_DOWN_GLYPH_FONT[shortNameIndexes[0]], ISOMETRIC_DOWN_FONT_WIDTH, ISOMETRIC_DOWN_FONT_HEIGHT, COLOR);
  display.drawBitmap(42, 110, ISOMETRIC_DOWN_GLYPH_FONT[shortNameIndexes[1]], ISOMETRIC_DOWN_FONT_WIDTH, ISOMETRIC_DOWN_FONT_HEIGHT, COLOR);
  display.drawBitmap(50, 114, ISOMETRIC_DOWN_GLYPH_FONT[shortNameIndexes[2]], ISOMETRIC_DOWN_FONT_WIDTH, ISOMETRIC_DOWN_FONT_HEIGHT, COLOR);

  // Draw the numbers of the date
  int day = currentTime.Day;
  display.drawBitmap(148, 113, ISOMETRIC_UP_GLYPH_FONT[getTens(day)], ISOMETRIC_UP_FONT_WIDTH, ISOMETRIC_UP_FONT_HEIGHT, COLOR);
  display.drawBitmap(156, 109, ISOMETRIC_UP_GLYPH_FONT[day % 10], ISOMETRIC_UP_FONT_WIDTH, ISOMETRIC_UP_FONT_HEIGHT, COLOR);
}

void MarqueeWatch::drawTime() {
  // Set the current hour per digit
  display.drawBitmap(64, 68, NEON_FONT[getTens(currentTime.Hour)], NEON_FONT_WIDTH, NEON_FONT_HEIGHT, COLOR);
  display.drawBitmap(80, 68, NEON_FONT[currentTime.Hour % 10], NEON_FONT_WIDTH, NEON_FONT_HEIGHT, COLOR);

  // Set the current minute per digit
  display.drawBitmap(105, 68, NEON_FONT[getTens(currentTime.Minute)], NEON_FONT_WIDTH, NEON_FONT_HEIGHT, COLOR);
  display.drawBitmap(122, 68, NEON_FONT[currentTime.Minute % 10], NEON_FONT_WIDTH, NEON_FONT_HEIGHT, COLOR);
}

void MarqueeWatch::drawVines() {
  // Reset step counter at midnight
  if (0 == currentTime.Hour && 0 == currentTime.Minute) {
    sensor.resetStepCounter();
  }

  // Get the actual amount of steps that have been taken
  int steps = (int)sensor.getCounter();
  
  // Or: Simulate it for testing purposes
  // int steps = currentTime.Minute * 567;  // Will be between 0 and (60 * 567 =) 34020
  int numberOfLeafs = (int)(steps / 500);

  // Add a nice little early out
  if (0 == numberOfLeafs) {
    // Block out the vines pretty much completely
    display.fillRect(10, 24, 4, 136, INVERTED_COLOR);
    display.fillRect(186, 24, 4, 136, INVERTED_COLOR);

    return;
  }

  // Cap it for now at a measily 34000 steps
  numberOfLeafs = min(numberOfLeafs, MAXIMUM_NUMBER_OF_LEAFS_PER_VINE * 2);

  // Check how many leafs we are above 17000 steps
  int numberOfLeafsLeft = numberOfLeafs - MAXIMUM_NUMBER_OF_LEAFS_PER_VINE;

  // Block out the vines until the point of interest
  display.fillRect(10, 24, 4, 136 - (((min(numberOfLeafs, MAXIMUM_NUMBER_OF_LEAFS_PER_VINE)) - 1) * 4), INVERTED_COLOR);
  display.fillRect(186, 24, 4, 136 - (((numberOfLeafsLeft > 0 ? numberOfLeafsLeft : numberOfLeafs) - 1) * 4), INVERTED_COLOR);

  // Draw in the leafs where needed
  for (int index = 0; index < min(numberOfLeafs, MAXIMUM_NUMBER_OF_LEAFS_PER_VINE); ++index) {
    int leafIndex = index % 4;

    // Left vine: left, right, right, left
    display.drawBitmap(LEFT_VINE_POSITIONS_X[leafIndex], 161 - (index * 4), (0 == leafIndex || 3 == leafIndex ? BITMAP_LEAF_LEFT : BITMAP_LEAF_RIGHT), 3, 3, COLOR);

    // Check if we're over 17000 steps, otherwise sync them
    // Right vine: right, left, left, right
    if (numberOfLeafsLeft <= 0 || index <= numberOfLeafsLeft) {
      display.drawBitmap(RIGHT_VINE_POSITIONS_X[leafIndex], 161 - (index * 4), (0 == leafIndex || 3 == leafIndex ? BITMAP_LEAF_RIGHT : BITMAP_LEAF_LEFT), 3, 3, COLOR);
    }
  }
}

int MarqueeWatch::getBatteryPercentage() {
  // Thanks to https://github.com/peerdavid/wos for most of this snippet of code!
  float voltage = getBatteryVoltage() + BATTERY_OFFSET;
  uint8_t percentage = 2808.3808 * pow(voltage, 4)
                       - 43560.9157 * pow(voltage, 3)
                       + 252848.5888 * pow(voltage, 2)
                       - 650767.4615 * voltage
                       + 626532.5703;
  return (int)min((uint8_t)100, max((uint8_t)0, percentage));
}

int MarqueeWatch::getTens(int value) {
  return (int)((value / 10) % 10);
}
