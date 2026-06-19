#include "ttyHelper.h"
#include "split.h"
#include "strings.h"
#include <iostream>

string ttyHelper::error(string message) {
  vector<string> tokens = split::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += strings::trim(token) + "\n  ";
  }
  return "\n\033[31merror:\033[0m " + strings::trim(message) + "\n";
}

string ttyHelper::progressBar(const int progress, const int total,
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
