#include "../utils/args.h"
#include "../utils/base64.h"
#include "../utils/sslHelper.h"
#include "../utils/systemHelper.h"
#include "../utils/tarHelper.h"
#include "../utils/ttyHelper.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
  vector<string> args(argv, argv + argc);

  // list flags
  map<string, args::optionIn> argsAvailable = {
      {"signature", args::optionIn{"signature", 's', true, true}},
      {"directory", args::optionIn{"directory", 'd', true, true}},
      {"lazy", args::optionIn{"lazy", 'l'}}};

  // parse user input
  map<string, args::optionOut> argsProcessed;
  try {
    argsProcessed = args::parse(args, argsAvailable);
  } catch (invalid_argument e) {
    cerr << ttyHelper::error(e.what());
    return 1;
  }

  // get values
  const string dir = *argsProcessed["directory"].value;
  const vector<unsigned char> sig =
      base64::decode(*argsProcessed["signature"].value);

  // Check running with Sudo, user should be root
  if (systemHelper::getProccessUser() != "root") {
    cerr << ttyHelper::error("deployClient needs to be run with sudo!");
    filesystem::remove_all(dir);
    return 1;
  }

  // restrict access to --directory
  if (chown(dir.c_str(), getuid(), getgid()) != 0) {
    cerr << ttyHelper::error("Failed to chown");
    filesystem::remove_all(dir);
    return 1;
  }
  if (chmod(dir.c_str(), S_IRWXU) != 0) {
    cerr << ttyHelper::error("Failed to chmod");
    filesystem::remove_all(dir);
    return 1;
  }

  // Unpack Tarball
  const tarHelper::result<void> tarResult =
      tarHelper::unpackage(dir + "/tarball.tar", dir + "/tar/");
  if (tarResult.exitCode != 0) {
    cerr << ttyHelper::error(*tarResult.error);
    filesystem::remove_all(dir);
    return 1;
  }

  // Get Manifest Signing File

  {
    // Verify File was signed by signature
    // Throw if not
    const systemHelper::result<vector<unsigned char>> tarballFileResult =
        systemHelper::readFile(dir + "/tarball.tar");
    if (tarballFileResult.exitCode != 0) {
      cerr << ttyHelper::error(*tarballFileResult.error);
      filesystem::remove_all(dir);
      return 1;
    }
    const sslHelper::result<vector<unsigned char>> sslHashResult =
        sslHelper::getSHA512Hash(*tarballFileResult.output);
    if (sslHashResult.exitCode != 0) {
      cerr << ttyHelper::error(*sslHashResult.error);
      filesystem::remove_all(dir);
      return 1;
    }
    const sslHelper::result<bool> signatureResult =
        sslHelper::verifyDataWithSign(*sslHashResult.output, sig);
    if (signatureResult.exitCode != 0 ||
        signatureResult.output.value_or(false) != true) {
      cerr << ttyHelper::error(*signatureResult.error);
      filesystem::remove_all(dir);
      return 1;
    }
  }

  // Throw if more then 10min off now

  // Verify if this system is listed in Manifest file

  // Get Manifest user
  // Verify Signature was made by user. Should be in current nix config
  // Throw if not

  // Eval File to derivation - See nixGet.h
  // Exit If derivation is same and lazy = true

  // Complete rebuild
  // Exit
  // filesystem::remove_all(dir);
  return 0;
};
