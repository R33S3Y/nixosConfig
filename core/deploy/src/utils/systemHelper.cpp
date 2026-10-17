#include "systemHelper.h"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <pwd.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

systemHelper::result<string> systemHelper::runCommand(string cmd) {

  int stdout_pipe[2], stderr_pipe[2];
  if (pipe(stdout_pipe) || pipe(stderr_pipe)) {
    return {.exitCode = -1, .error = strerror(errno)};
  }

  pid_t pid = fork();
  if (pid == -1) {
    return {.exitCode = -1, .error = strerror(errno)};
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

  systemHelper::result<string> res;
  res.output = "";
  res.error = "";

  readFd(stdout_pipe[0], *res.output);
  readFd(stderr_pipe[0], *res.error);

  close(stdout_pipe[0]);
  close(stderr_pipe[0]);

  int status;
  waitpid(pid, &status, 0);
  res.exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  return res;
}
systemHelper::result<string> systemHelper::readFileToStr(const string &path) {
  ifstream file(path);
  if (!file.is_open()) {
    return {.exitCode = 1, .error = "Failed to open file: path"};
  }
  ostringstream ss;
  ss << file.rdbuf();
  return {.output = ss.str(), .exitCode = 0};
}
systemHelper::result<vector<unsigned char>>
systemHelper::readFile(const string &path) {
  ifstream file(path, ios::binary | ios::ate);
  if (!file.is_open()) {
    return {.exitCode = 1, .error = "Failed to open file: " + path};
  }

  streamsize size = file.tellg();
  file.seekg(0, ios::beg);

  vector<unsigned char> buffer(size);

  if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    return {.exitCode = 2, .error = "Failed to read file: " + path};
  }
  return {.output = buffer, .exitCode = 0};
}

systemHelper::result<void>
systemHelper::saveFileFromStr(const string &path, const string &content) {
  ofstream file(path);
  if (!file.is_open()) {
    return {.exitCode = 1, .error = "failed to open file: " + path};
  }
  file << content;
  file.close();
  return {.exitCode = 0};
}

string systemHelper::getProccessUser() { return getpwuid(getuid())->pw_name; }
