#ifndef CONSTANTS_H
#define CONSTANTS_H

const int BATTERY_BIRD_POSITIONS_X[6] = {
  52,
  76,
  87,
  107,
  128,
  136
};

// a: 0, b: 1, c: 2, d: 3, e: 4, f: 5, g: 6, h: 7, j: 8, l: 9, m: 10, n: 11, o: 12, p: 13, r: 14, s: 15, t: 16, u: 17, v: 18, y: 19
const int DATE_TO_SHORTNAME_INDEXES[12][3] = {
  { 8, 0, 11 },    // jan
  { 5, 4, 1 },     // feb
  { 10, 0, 14 },   // mar
  { 0, 13, 14 },   // apr
  { 10, 0, 19 },   // may
  { 8, 17, 11 },   // jun
  { 8, 17, 9 },    // jul
  { 0, 17, 6 },    // aug
  { 15, 4, 13 },   // sep
  { 12, 2, 16 },   // oct
  { 11, 12, 18 },  // nov
  { 3, 4, 2 }      // dec
};

const int LEFT_VINE_POSITIONS_X[4] = {
  9, 14, 12, 7
};

const int RIGHT_VINE_POSITIONS_X[4] = {
  188, 183, 185, 190
};

#endif