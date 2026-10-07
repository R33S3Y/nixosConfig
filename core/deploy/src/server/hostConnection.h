#include <cstddef>
#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <optional>
#include <string>
#include <sys/types.h>
#include <vector>

using namespace std;
namespace hostConnection {
template <typename T> struct result {
  optional<T> output;
  int exitCode;
  optional<string> error;
};
template <> struct result<void> {
  int exitCode;
  optional<string> error;
};

struct input {
  string host;
  string user;
  string privateKey;
  string tmpPath;
  string fileSig;
  bool lazy;
};

result<string> hostConnection(input input);
} // namespace hostConnection
