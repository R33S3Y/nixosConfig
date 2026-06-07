#include "eval.h"
#include "resolve.h"
#include "utils.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

vector<string> getFlakeInputs(string flakeLink) {
  string cmd = "nix flake show " + flakeLink + " --json";
  utils::result cmdOut = utils::runCommand(cmd);

  if (!cmdOut.ok()) {
    return {};
  }
  auto json = nlohmann::json::parse(cmdOut.output);

  vector<string> configs;
  for (auto &[key, value] : json["nixosConfigurations"].items()) {
    configs.push_back(key);
  }

  return configs;
}
vector<string> getNixFiles(string flakeLink, string host) {
  string cmd = "nix eval " + flakeLink + "#nixosConfigurations." + host +
               "._module.args.modules";

  utils::result cmdOut = utils::runCommand(cmd);

  if (!cmdOut.ok()) {
    cerr << utils::error("Failed to eval for nix files");
    return {};
  }

  vector<string> list = eval::list(cmdOut.output);

  vector<string> output;
  for (string currentStr : list) {
    if (currentStr.find(flakeLink) != string::npos) {
      currentStr = utils::replace(currentStr, flakeLink, "");
      currentStr = utils::trim(currentStr);

      if (currentStr != "") {
        output.push_back(currentStr);
      }
    }
  }

  return output;
}
vector<string> filter(vector<string> imports, vector<string> processedFiles) {
  imports.erase(remove_if(imports.begin(), imports.end(),
                          [&](const string &f) {
                            return find(processedFiles.begin(),
                                        processedFiles.end(),
                                        f) != processedFiles.end();
                          }),
                imports.end());

  return imports;
}
vector<string> merge(vector<string> imports, vector<string> unprocessedFiles) {
  set<string> pending(unprocessedFiles.begin(), unprocessedFiles.end());
  pending.insert(imports.begin(), imports.end());
  unprocessedFiles.assign(pending.begin(), pending.end());
  return unprocessedFiles;
}
int main(int argc, char const *argv[]) {

  string flakeLink = "/home/reese/Projects/nixos-config";
  string flakePath = "/tmp/currentConfig";

  filesystem::create_directories(flakePath);
  if (filesystem::is_empty(flakePath) == false) {
    cerr << utils::error("flakePath (\033[35m" + flakePath +
                         "\033[0m) is not empty");
    return 1;
  }
  utils::result cmdOut = utils::runCommand("nix flake clone " + flakeLink +
                                           " --dest " + flakePath);
  if (!cmdOut.ok()) {
    cerr << utils::error("failed to get flake (\033[35m" + flakeLink +
                         "\033[0m)");
    return 1;
  }

  vector<string> hosts = getFlakeInputs(flakePath);
  if (hosts.size() == 0) {
    cerr << utils::error("flake does not contain any hosts or no "
                         "hosts could be found");
    return 1;
  }

  for (string host : hosts) {
    cout << "\n";
    cout << host + "\n";

    // in the case of a error while getting a file paths.
    // We have to rebuild the machine no matter what seeing as we may have just
    // missed the file that changed
    bool error = false;
    vector<string> unprocessedFiles = getNixFiles(flakePath, host);

    // the only way this could happen is if getNixFiles had a error/failed
    if (unprocessedFiles.size() == 0) {
      error = true;
    }

    unprocessedFiles.push_back("/flake.nix");

    vector<string> processedFiles;
    processedFiles.push_back("/flake.lock");

    cout << "\n";
    cout << "Unprocessed files: \n";
    for (string file : unprocessedFiles) {
      cout << "\"" + file + "\"\n";
    }

    resolve r(flakePath, flakeLink, host);

    while (unprocessedFiles.size() != 0 && error == false) {
      std::string filePath = unprocessedFiles[0];
      unprocessedFiles.erase(unprocessedFiles.begin());
      processedFiles.push_back(filePath);

      r.preprocessFile(filePath);
      resolve::result imports;

      imports = r.resolveImportStatements();
      if (imports.error) {
        error = true;
        break;
      }
      if (imports.paths.size() != 0) {
        imports.paths = filter(imports.paths, processedFiles);
        unprocessedFiles = merge(imports.paths, unprocessedFiles);
      }

      imports = r.resolveImportsStatements();
      if (imports.error) {
        error = true;
        break;
      }
      if (imports.paths.size() != 0) {
        imports.paths = filter(imports.paths, processedFiles);
        unprocessedFiles = merge(imports.paths, unprocessedFiles);
      }
    }
    cout << "\n";
    cout << "Still Unprocessed files: \n";
    for (string file : unprocessedFiles) {
      cout << "\"" + file + "\"\n";
    }
    cout << "\n";
    cout << "Processed files: \n";
    for (string file : processedFiles) {
      cout << "\"" + file + "\"\n";
    }
  }

  filesystem::remove_all(flakePath);

  return 0;
}
