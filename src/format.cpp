#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int Hours, Minutes, Seconds;
  Hours = (int)seconds/3600;
  seconds -= 3600*Hours;
  Minutes = (int)seconds/60;
  seconds -= 60*Minutes;
  Seconds = seconds;
  return std::to_string(Hours) + ":"+ std::to_string(Minutes) + ":" +
         std::to_string(Seconds);
}
