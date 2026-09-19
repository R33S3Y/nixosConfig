#include "../utils/args.h"
#include "../utils/base64.h"
#include "../utils/nixGet.h"
#include "../utils/split.h"
#include "../utils/sslHelper.h"
#include "../utils/systemHelper.h"
#include "../utils/tarHelper.h"
#include "../utils/ttyHelper.h"
#include <algorithm>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <pwd.h>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
  vector<string> args(argv, argv + argc);

  // list flags
  map<string, args::optionIn> argsAvailable = {
      {"strict", args::optionIn{"strict", 's'}},
      {"dynamic", args::optionIn{"dynamic", 'd'}},
      {"flake", args::optionIn{"flake", 'f', true, true}},
      {"key", args::optionIn{"key", 'k', true}},
      {"keySSH", args::optionIn{.longName = "keySSH", .takesValue = true}},
      {"keySigning",
       args::optionIn{.longName = "keySigning", .takesValue = true}},
  };

  // parse user input
  map<string, args::optionOut> argsProcessed;
  try {
    argsProcessed = args::parse(args, argsAvailable);
  } catch (invalid_argument e) {
    cerr << ttyHelper::error(e.what());
    return 1;
  }

  // rebuild mode
  if (argsProcessed["strict"].invoked == true &&
      argsProcessed["dynamic"].invoked == true) {
    cerr << ttyHelper::error("--dynamic (" + ttyColour::log + "-d" +
                             ttyColour::reset + ") and --strict (" +
                             ttyColour::log + "-s" + ttyColour::reset +
                             ") are mutually exclusive");
    return 1;
  }
  bool dynamicRebuild = true;
  if (argsProcessed["strict"].invoked == true) {
    dynamicRebuild = false;
  }

  // get flake
  string tmpPath = "/tmp/deploy";
  string flakeLink = *argsProcessed["flake"].value;
  string flakePath = tmpPath + "/nixosConfig";
  filesystem::create_directories(tmpPath);
  if (filesystem::is_empty(tmpPath) == false) {
    cerr << ttyHelper::warning("flakePath (" + ttyColour::log + tmpPath +
                               ttyColour::reset +
                               ") is not empty. Deleting...");
    filesystem::remove_all(tmpPath);
  }
  filesystem::create_directories(flakePath);
  if (filesystem::is_empty(flakePath) == false) {
    cerr << ttyHelper::warning("flakePath (" + ttyColour::log + flakePath +
                               ttyColour::reset +
                               ") is not empty. Deleting...");
    filesystem::remove_all(flakePath);
  }
  systemHelper::result<string> cmdOut = systemHelper::runCommand(
      "nix flake clone " + flakeLink + " --dest " + flakePath);
  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("failed to get flake (" + ttyColour::log +
                             flakeLink + ttyColour::reset + ")");
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // get available hosts
  vector<string> hosts;
  vector<string> availableHosts = nixGet::flakeHosts(flakePath);
  if (availableHosts.size() == 0) {
    cerr << ttyHelper::error("flake does not contain any hosts or no "
                             "hosts could be found");
    filesystem::remove_all(tmpPath);
    return 1;
  }
  // compare against user input
  if (!argsProcessed["*"].value.has_value() ||
      argsProcessed["*"].value->size() == 0) {
    cerr << ttyHelper::error(
        "no hosts selected. Please enter a host or type '" + ttyColour::log +
        "man deploy" + ttyColour::reset + "' for more info.");
    filesystem::remove_all(tmpPath);
    return 1;
  }
  vector<string> userHosts =
      split::splitStrByChar(*argsProcessed["*"].value, ' ');
  if (userHosts.size() == 1 && userHosts[0] == "all") {
    hosts = availableHosts;
  } else {
    for (string userHost : userHosts) {
      if (ranges::contains(availableHosts, userHost)) {
        hosts.push_back(userHost);
      } else {
        cerr << ttyHelper::error("host (" + ttyColour::log + userHost +
                                 ttyColour::reset +
                                 ") does not exist in flake");
        filesystem::remove_all(tmpPath);
        return 1;
      }
    }
  }
  if (hosts.size() == 0) {
    cerr << ttyHelper::error(
        "no hosts selected. This error should be able to be trigged");
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // make manifest file
  string user;
  struct passwd *pw = getpwuid(getuid());
  if (!pw) {
    cerr << ttyHelper::error("couldn't find username");
    filesystem::remove_all(tmpPath);
    return 1;
  }
  user = pw->pw_name;
  nlohmann::json manifestJson = {
      {"signingUser", user},
      {
          "signingTime",
          static_cast<int64_t>(time(nullptr)),
      },
      {"dynamic", dynamicRebuild},
      {"hosts", hosts},
  };
  systemHelper::saveFileFromStr(tmpPath + "/manifest.json",
                                nlohmann::to_string(manifestJson));

  // make flake path into tarball
  const tarHelper::result<void> tarStatus =
      tarHelper::package(tmpPath + "/tarball.tar",
                         {
                             {tmpPath + "/manifest.json", "manifest.json"},
                             {flakePath, "nixosConfig"},
                         });
  if (tarStatus.exitCode != 0) {
    cerr << ttyHelper::error(*tarStatus.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // get tarball
  const systemHelper::result<vector<unsigned char>> tarballFileResult =
      systemHelper::readFile(tmpPath + "/tarball.tar");
  if (tarballFileResult.exitCode != 0) {
    cerr << ttyHelper::error(*tarballFileResult.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // make hash
  const sslHelper::result<vector<unsigned char>> sslHashStatus =
      sslHelper::getSHA512Hash(*tarballFileResult.output);
  if (sslHashStatus.exitCode != 0) {
    cerr << ttyHelper::error(*sslHashStatus.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // get signing ssh key
  string signingKeyPath;
  if (argsProcessed["keySigning"].invoked == true) {
    signingKeyPath = *argsProcessed["keySigning"].value;
  } else if (argsProcessed["key"].invoked == true) {
    signingKeyPath = *argsProcessed["key"].value;
  } else {
    cerr << ttyHelper::error(
        "no signingKey provided please set --keySigning or --key (" +
        ttyColour::log + "-k" + ttyColour::reset + ").");
    filesystem::remove_all(tmpPath);
    return 1;
  }

  const systemHelper::result<string> privateKeyFile =
      systemHelper::readFileToStr(signingKeyPath);
  if (privateKeyFile.exitCode != 0) {
    cerr << ttyHelper::error(*privateKeyFile.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  const sslHelper::result<EVP_PKEY *> privateKeyPKEY =
      sslHelper::openPrivateKey(*privateKeyFile.output);
  if (privateKeyPKEY.exitCode != 0) {
    cerr << ttyHelper::error(*privateKeyPKEY.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  // sign Sha
  const sslHelper::result<vector<unsigned char>> signatureStatus =
      sslHelper::signDataWithKey(*sslHashStatus.output, *privateKeyPKEY.output);
  if (signatureStatus.exitCode != 0) {
    cerr << ttyHelper::error(*signatureStatus.error);
    filesystem::remove_all(tmpPath);
    return 1;
  }

  cout << "signature: ";
  for (unsigned char i : *signatureStatus.output)
    cout << i;

  vector<unsigned char> test = {
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
  };

  // cout << "\nBase 64 test: " + base64::encode(test);

  // send flakePath
  // rebuild
  // done :3

  filesystem::remove_all(tmpPath);
  return 0;
}
