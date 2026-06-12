#include "utils.h"
#include <algorithm>
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

string utils::error(string message) {
  vector<string> tokens = utils::splitStrByChar(message, '\n');
  message = "";
  for (string token : tokens) {
    message += utils::trim(token) + "\n  ";
  }
  return "\n\033[31merror:\033[0m " + utils::trim(message) + "\n";
}

vector<string> utils::splitStrByChar(string inputStr, char inputChar) {
  vector<string> output;
  string currentStr;

  for (char currentChar : inputStr) {
    if (currentChar == inputChar) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += currentChar;
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> utils::splitStrByChars(string inputStr,
                                      vector<char> inputChars) {
  vector<string> output;
  string currentStr;

  for (char currentChar : inputStr) {
    if (find(inputChars.begin(), inputChars.end(), currentChar) !=
        inputChars.end()) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += currentChar;
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> utils::splitStrByCharByFilterStr(string inputStr,
                                                string filterStr,
                                                char inputChar) {

  if (inputStr.size() != filterStr.size()) {
    cerr << utils::error(
        "utils::splitStrByCharByFilterStr : inputStr and fileStr "
        "does not match");
    return {};
  }

  vector<string> output;
  string currentStr;

  for (int i = 0; i < filterStr.size(); i++) {
    if (filterStr[i] == inputChar) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += inputStr[i];
    }
  }
  output.push_back(currentStr);
  return output;
}
vector<string> utils::splitStrByCharsByFilterStr(string inputStr,
                                                 string filterStr,
                                                 vector<char> inputChars) {

  if (inputStr.size() != filterStr.size()) {
    cerr << utils::error(
        "utils::splitStrByCharByFilterStr : inputStr and fileStr "
        "does not match");
    return {};
  }

  vector<string> output;
  string currentStr;

  for (int i = 0; i < filterStr.size(); i++) {
    if (find(inputChars.begin(), inputChars.end(), filterStr[i]) !=
        inputChars.end()) {
      output.push_back(currentStr);
      currentStr.clear();
    } else {
      currentStr += inputStr[i];
    }
  }
  output.push_back(currentStr);
  return output;
}

string utils::replace(string s, string from, string to) {
  size_t pos = s.find(from);
  if (pos != string::npos) {
    s.replace(pos, from.size(), to);
  }
  return s;
}
string utils::rReplace(string s, string from, string to) {
  size_t pos = s.rfind(from);
  if (pos != string::npos) {
    s.replace(pos, from.size(), to);
  }
  return s;
}
string utils::replaceAll(string s, string from, string to) {
  while (s.find(from) != string::npos) {
    size_t pos = s.find(from);
    if (pos != string::npos) {
      s.replace(pos, from.size(), to);
    }
  }
  return s;
}

string utils::trim(string s) {
  s.erase(0, s.find_first_not_of(" \t\n\r"));
  s.erase(s.find_last_not_of(" \t\n\r") + 1);
  return s;
}

string utils::blankWithinTokens(string fileStr, string startToken,
                                string endToken, char blankChar) {

  if (endToken == "") {
    endToken = startToken;
  }

  string holdStr;

  while (fileStr.size() > 0 && fileStr.find(startToken) != string::npos) {
    size_t startLeft = fileStr.find(startToken);
    size_t startRight = startLeft + startToken.size();
    size_t endLeft = fileStr.find(endToken, startRight);
    if (endLeft == string::npos) {
      break;
    }
    size_t endRight = endLeft + endToken.size();

    for (size_t i = startRight; i < endLeft; i++) {
      if (fileStr[i] != '\n') {
        fileStr[i] = blankChar;
      }
    }
    holdStr += fileStr.substr(0, endRight);
    fileStr = fileStr.substr(endRight);
  }
  holdStr += fileStr;
  return holdStr;
}
