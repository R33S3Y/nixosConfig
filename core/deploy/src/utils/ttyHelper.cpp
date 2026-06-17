#include "ttyHelper.h"
#include "split.h"
#include "strings.h"

string ttyHelper::error(string message) {
  vector<string> tokens = split::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += strings::trim(token) + "\n  ";
  }
  return "\n\033[31merror:\033[0m " + strings::trim(message) + "\n";
}
