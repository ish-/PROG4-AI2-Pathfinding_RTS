#pragma once
#include <raylib.h>

class DebugInfo {
public:
  int fontSize = 30;
  int padding = 30;
  int lines = 0;

  void add (const char * txt) {
    DrawText(txt, 20, padding + (fontSize + padding) * lines, fontSize, WHITE);
    lines++;
  }

  void reset () { lines = 0; }
};
