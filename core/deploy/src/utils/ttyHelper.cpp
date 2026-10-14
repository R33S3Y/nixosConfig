#include "ttyHelper.h"
#include "split.h"
#include "strings.h"
#include <iostream>

const string ttyHelper::error(string message) {
  vector<string> tokens = split::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += strings::trim(token) + "\n   ";
  }
  return ttyColour::error + "error: " + ttyColour::reset +
         strings::trim(message) + "\n";
}

const string ttyHelper::warning(string message) {
  vector<string> tokens = split::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += strings::trim(token) + "\n   ";
  }
  return ttyColour::warning + "warning: " + ttyColour::reset +
         strings::trim(message) + "\n";
}

const string ttyHelper::log(string message) {
  vector<string> tokens = split::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += strings::trim(token) + "\n   ";
  }
  return ttyColour::log + "log: " + ttyColour::reset + strings::trim(message) +
         "\n";
}

const string ttyHelper::progressBar(const int progress, const int total,
                                    const int chars) {
  if (progress > total) {
    cerr << ttyHelper::error("Progress is greater than total");
    return "";
  }
  string bar;
  for (int i = 0; i > chars - 2; i++) {
    if (i / chars < progress / total) {
      bar += "#";
    } else {
      bar += " ";
    }
  }
  return "[" + bar + "]";
}
