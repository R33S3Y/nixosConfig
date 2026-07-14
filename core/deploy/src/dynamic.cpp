#include "dynamic.h"
#include "nix/resolve.h"
#include "nix/staticGet.h"
#include "utils/strings.h"
#include "utils/systemHelper.h"
#include "utils/ttyHelper.h"
#include <algorithm>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <vector>

vector<string> dynamic::getNixFiles(const string &flakePath,
                                    const string host) {
  string cmd = "nix eval " + flakePath + "#nixosConfigurations." + host +
               "._module.args.modules";

  systemHelper::result cmdOut = systemHelper::runCommand(cmd);

  if (cmdOut.exitCode != 0) {
    cerr << ttyHelper::error("Failed to eval for nix files");
    return {};
  }

  vector<string> list = staticGet::listItems(cmdOut.output);

  vector<string> output;
  for (string currentStr : list) {
    if (currentStr.find(flakePath) != string::npos) {
      currentStr = strings::replace(currentStr, flakePath, "");
      currentStr = strings::trim(currentStr);

      if (currentStr != "") {
        output.push_back(currentStr);
      }
    }
  }

  return output;
}
vector<string> dynamic::filter(vector<string> imports,
                               vector<string> processedFiles) {
  imports.erase(remove_if(imports.begin(), imports.end(),
                          [&](const string &f) {
                            return find(processedFiles.begin(),
                                        processedFiles.end(),
                                        f) != processedFiles.end();
                          }),
                imports.end());

  return imports;
}
vector<string> dynamic::merge(vector<string> imports,
                              vector<string> unprocessedFiles) {
  set<string> pending(unprocessedFiles.begin(), unprocessedFiles.end());
  pending.insert(imports.begin(), imports.end());
  unprocessedFiles.assign(pending.begin(), pending.end());
  return unprocessedFiles;
}

bool dynamic::rebuild(const string &host, const string &flakePath,
                      const string &flakeLink, const vector<string> &gitDiff) {

  vector<string> skippableFiles = {"LICENSE", "README"};

  for (int i = 0; i < gitDiff.size(); i++) {

    if (gitDiff[i].ends_with(".nix") == true) {
      continue;
    }
    for (int j = 0; j < skippableFiles.size(); j++) {
      if (gitDiff[i].ends_with(skippableFiles[j]) == true) {
        continue;
      }
    }
    return true;
  }

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
      return true;
    }
    if (imports.paths.size() != 0) {
      imports.paths = filter(imports.paths, processedFiles);
      unprocessedFiles = merge(imports.paths, unprocessedFiles);
    }
  }
  cout << "files: \n";
  for (string file : processedFiles)
    cout << "  " + file + "\n";
  cout << "git diff: \n";
  for (string path : gitDiff) {
    cout << "  " + path + "\n";
  }
  for (string file : processedFiles) {
    if (ranges::contains(gitDiff, file)) {
      return true;
    }
  }

  return false;
}
