#include "nixEvalSupport.h"
#include "../utils/systemHelper.h"
#include "../utils/ttyHelper.h"
#include <iostream>
#include <string>

string nixEvalSupport::initWorker(string cmd) {

  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    ostringstream oss;
    oss << ttyHelper::error("Failed to eval attrs of: (\033[35m" + key +
                            "\033[0m)");
    cerr << oss.str();

    return "";
  }

  return cmdOut.output;
}
