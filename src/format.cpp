#include "format.h"

#include <iostream>
#include <string>
using std::string;

// Add a single left zero pad to less than 10
string zero_pad(int num) {
  if (num == 0) {
    return "00";
  }
  if (num < 10) {
    return "0" + std::to_string(num);
  }
  return std::to_string(num);
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long totalSeconds) {
  std::string elapsedTime;
  int seconds = 0;
  int minutes = 0;
  int hours = 0;
  int curr = totalSeconds;

  // calc seconds
  seconds = totalSeconds % 60;
  curr = totalSeconds / 60;

  // calc minutes
  minutes = curr % 60;
  hours = curr / 60;

  return zero_pad(hours) + ":" + zero_pad(minutes) + ":" + zero_pad(seconds);
}