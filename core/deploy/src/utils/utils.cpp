#include "utils.h"
#include "split.h"
#include "strings.h"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <vector>

using namespace std;

utils::result utils::runCommand(string cmd) {
  result res;

  int stdout_pipe[2], stderr_pipe[2];
  if (pipe(stdout_pipe) || pipe(stderr_pipe)) {
    res.exitCode = -1;
    res.error = strerror(errno);
    return res;
  }

  pid_t pid = fork();
  if (pid == -1) {
    res.exitCode = -1;
    res.error = strerror(errno);
    return res;
  }

  if (pid == 0) {
    // Child: wire up pipes
    dup2(stdout_pipe[1], STDOUT_FILENO);
    dup2(stderr_pipe[1], STDERR_FILENO);
    close(stdout_pipe[0]);
    close(stdout_pipe[1]);
    close(stderr_pipe[0]);
    close(stderr_pipe[1]);

    execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
    _exit(127);
  }

  // Parent: close write ends
  close(stdout_pipe[1]);
  close(stderr_pipe[1]);

  // Read stdout and stderr
  char buffer[256];
  auto readFd = [&](int fd, string &into) {
    ssize_t n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0)
      into.append(buffer, n);
  };

  readFd(stdout_pipe[0], res.output);
  readFd(stderr_pipe[0], res.error);

  close(stdout_pipe[0]);
  close(stderr_pipe[0]);

  int status;
  waitpid(pid, &status, 0);
  res.exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  return res;
}
string utils::readFile(const string &path) {
  ifstream file(path);
  if (!file.is_open())
    throw runtime_error("Failed to open file: " + path);
  ostringstream ss;
  ss << file.rdbuf();
  return ss.str();
}
