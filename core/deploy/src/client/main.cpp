#include "../utils/args.h"
#include "../utils/ttyHelper.h"
#include <iostream>
#include <string>

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

  cerr << "error";
  cout << "info";
  return 0;
  // Check running with Sudo - See main.cpp for how to get the current user,
  // which should be root

  // restrict access to --directory

  // Verify File was signed by signature
  // Throw if not

  // Unpack Tarball - See tarHelper.h

  // Get Manifest user
  // Verify Signature was made by user. Should be in current nix config
  // Throw if not

  // Get Manifest Signing time
  // Throw if more then 10min off now

  // Verify if this system is listed in Manifest file

  // Eval File to derivation - See nixGet.h
  // Exit If derivation is same and lazy = true

  // Complete rebuild
  // Exit
};
