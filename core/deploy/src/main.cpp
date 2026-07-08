
#include "dynamic.h"

#include "nixEvalStatic.h"
#include "utils/args.h"
#include "utils/split.h"
#include "utils/strings.h"
#include "utils/systemHelper.h"
#include "utils/ttyHelper.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
  vector<string> args(argv, argv + argc);

  // set flags
  map<string, args::optionIn> argsAvailable = {
      {"all", args::optionIn{"all", 'a'}},
      {"dynamic", args::optionIn{"dynamic", 'd'}},
      {"flake", args::optionIn{"flake", 'f', true, true}},
  };

  // parse user input
  map<string, args::optionOut> argsProcessed;
  try {
    argsProcessed = args::parse(args, argsAvailable);
  } catch (invalid_argument e) {
    cerr << ttyHelper::error(e.what());
    return 1;
  }

  // error check
  if (argsProcessed["all"].invoked == true &&
      argsProcessed["dynamic"].invoked == true) {
    cerr << ttyHelper::error("--dynamic (\033[35m-d\033[0m) and --all "
                             "(\033[35m-a\033[0m) are mutually exclusive");
    return 1;
  }

  // get flake
  string flakeLink = *argsProcessed["flake"].value;
  string flakePath = "/tmp/nixosConfig";
  filesystem::create_directories(flakePath);
  if (filesystem::is_empty(flakePath) == false) {
    cerr << ttyHelper::error("flakePath (\033[35m" + flakePath +
                             "\033[0m) is not empty");
    return 1;
  }
  systemHelper::result cmdOut;
  cmdOut = systemHelper::runCommand("nix flake clone " + flakeLink +
                                    " --dest " + flakePath);
  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("failed to get flake (\033[35m" + flakeLink +
                             "\033[0m)");
    return 1;
  }

  // get available hosts
  vector<string> hosts;
  vector<string> availableHosts = nixEvalStatic::getFlakeHosts(flakePath);
  if (availableHosts.size() == 0) {
    cerr << ttyHelper::error("flake does not contain any hosts or no "
                             "hosts could be found");
    return 1;
  }
  // compare against user input
  vector<string> userHosts =
      split::splitStrByChar(*argsProcessed["*"].value, ' ');
  if (userHosts.size() == 1 && userHosts[0] == "*") {
    hosts = availableHosts;
  } else {
    for (string userHost : userHosts) {
      if (ranges::contains(availableHosts, userHost)) {
        hosts.push_back(userHost);
      } else {
        cerr << ttyHelper::error("host (\033[35m" + userHost +
                                 "\033[0m) does not exist in flake");
        return 1;
      }
    }
  }
  if (hosts.size() == 0) {
    cerr << ttyHelper::error("no hosts selected. Please enter a host or type "
                             "'man deploy' for more info");
  }

  // get git diff
  bool dynamicBuild = true;
  vector<string> gitDiff;
  if (dynamicBuild == true) {

    cmdOut = systemHelper::runCommand("git -C " + flakePath +
                                      " diff HEAD^ HEAD --name-only");
    if (cmdOut.exitCode != 0) {
      cerr << ttyHelper::error("git diff failed");
      return 1;
    }

    gitDiff = split::splitStrByChar(cmdOut.output, '\n');
    for (vector<string>::iterator it = gitDiff.begin(); it != gitDiff.end();
         it++) {
      *it = strings::trim(*it);

      if (it->size() == 0) {
        gitDiff.erase(it);
        it--;
        continue;
      }
      // git diff cmd gives folder/file so this just converts it into
      // /folder/file which is what we use cus it lets us just flakePath +
      // "/folder/path"
      *it = "/" + *it;
    }

    if (gitDiff.size() == 0) {
      dynamicBuild = false;
      cerr << ttyHelper::warning(
          "no changed items found. Skipping dynamic rebuild");
    }
  }

  vector<bool> rebuild;
  for (string host : hosts) {
    if (dynamicBuild == false) {
      rebuild.push_back(true);
      continue;
    }

    rebuild.push_back(dynamic::rebuild(host, flakePath, flakeLink, gitDiff));
    if (rebuild.back() == true) {
      cout << "rebuild " + host + "\n";
    } else {
      cout << "skip " + host + "\n";
    }
  }
  if (hosts.size() != rebuild.size()) {
    cerr << ttyHelper::error(
        "if rebuild list is not same length as hosts list");
    return 1;
  }
  for (int i = 0; i < hosts.size(); i++) {
    if (rebuild[i] == false) {
      hosts.erase(hosts.begin() + i);
      rebuild.erase(rebuild.begin() + i);
      i--;
    }
  }

  filesystem::remove_all(flakePath);

  return 0;
}
