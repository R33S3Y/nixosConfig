#include "tarHelper.h"
#include "ttyHelper.h"
#include <fcntl.h>
#include <filesystem>
#include <libtar.h>
#include <string>
#include <vector>

using namespace std;

tarHelper::result<void> tarHelper::package(string tarPath,
                                           vector<tarHelper::tarItem> items) {
  TAR *tarball = nullptr;
  if (tar_open(&tarball, tarPath.c_str(), nullptr, O_WRONLY | O_CREAT, 0644,
               TAR_GNU) != 0) {
    return {1, "tar_open failed"};
  }

  for (tarHelper::tarItem item : items) {
    if (filesystem::is_regular_file(item.realPath)) {
      if (tar_append_file(tarball, item.realPath.c_str(),
                          item.tarPath.c_str()) != 0) {
        tar_close(tarball);
        return {1, "failed to file (" + ttyColour::log + item.realPath +
                       ttyColour::reset + ") to tarball"};
      }
      continue;
    }
    if (filesystem::is_directory(item.realPath)) {
      if (tar_append_tree(tarball, const_cast<char *>(item.realPath.c_str()),
                          const_cast<char *>(item.tarPath.c_str())) != 0) {
        tar_close(tarball);
        return {1, "failed to folder (" + ttyColour::log + item.realPath +
                       ttyColour::reset + ") to tarball"};
      }
      continue;
    }
    tar_close(tarball);
    return {1, "failed to identify (" + ttyColour::log + item.realPath +
                   ttyColour::reset + ")"};
  }

  if (tar_append_eof(tarball) != 0) {
    return {1, "tar_append_eof failed"};
  }
  if (tar_close(tarball) != 0) {
    return {1, "tar_close failed"};
  }

  return {0};
}

tarHelper::result<vector<tarHelper::tarItem>>
tarHelper::unpackage(string tarPath, string tarItemsSaveDir) {
  return {.exitCode = 1, .error = "todo"};
}
