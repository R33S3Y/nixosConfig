#include "../utils/args.h"
#include "../utils/nixGet.h"
#include "../utils/split.h"
#include "../utils/systemHelper.h"
#include "../utils/ttyHelper.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
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
    cerr << ttyHelper::warning("flakePath (\033[35m" + flakePath +
                               "\033[0m) is not empty. Deleting...");
    filesystem::remove_all(flakePath);
  }
  systemHelper::result cmdOut;
  cmdOut = systemHelper::runCommand("nix flake clone " + flakeLink +
                                    " --dest " + flakePath);
  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("failed to get flake (\033[35m" + flakeLink +
                             "\033[0m)");
    filesystem::remove_all(flakePath);
    return 1;
  }

  // get available hosts
  vector<string> hosts;
  vector<string> availableHosts = nixGet::flakeHosts(flakePath);
  if (availableHosts.size() == 0) {
    cerr << ttyHelper::error("flake does not contain any hosts or no "
                             "hosts could be found");
    filesystem::remove_all(flakePath);
    return 1;
  }
  // compare against user input
  if (!argsProcessed["*"].value.has_value() ||
      argsProcessed["*"].value->size() == 0) {
    cerr << ttyHelper::error("no hosts selected. Please enter a host or type "
                             "'man deploy' for more info");
    filesystem::remove_all(flakePath);
    return 1;
  }
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
        filesystem::remove_all(flakePath);
        return 1;
      }
    }
  }
  if (hosts.size() == 0) {
    cerr << ttyHelper::error(
        "no hosts selected. This error should be able to be trigged");
    filesystem::remove_all(flakePath);
    return 1;
  }

  // make manifest file
  // tarball flakepath
  // send flakepath
  // rebuild
  // done

  filesystem::remove_all(flakePath);
  return 0;
}
