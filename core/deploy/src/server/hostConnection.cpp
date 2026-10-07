#include "hostConnection.h"
#include "../utils/sshHelper.h"
#include <libssh/sftp.h>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

hostConnection::result<string>
hostConnection::hostConnection(hostConnection::input input) {
  sshHelper::result<ssh_session> sessionResult =
      sshHelper::connectTo(input.host, "22", input.user, "", input.privateKey);
  if (sessionResult.exitCode != 0) {
    return {.exitCode = 1,
            .error = "Failed to connect to host (" + input.host +
                     ") with error: \n" + *sessionResult.error};
  }
  ssh_session sessionSsh = *sessionResult.output;

  sshHelper::result<sftp_session> sftpResult =
      sshHelper::getsftpSession(sessionSsh);
  if (sftpResult.exitCode != 0) {
    sshHelper::disconnect(sessionSsh);
    return {.exitCode = 1,
            .error = "Failed to setup SFTP link with (" + input.host +
                     ") with error: \n" + *sftpResult.error};
  }
  sftp_session sessionSftp = *sftpResult.output;

  sshHelper::result<void> dirResult =
      sshHelper::sftpMkDir(sessionSftp, input.tmpPath, S_IWUSR | S_IRGRP);
  if (dirResult.exitCode != 0) {
    return {.exitCode = 1, .error = "Failed to make dir on host"};
  };
  sshHelper::result<void> fileResult = sshHelper::sftpMoveFileTo(
      sessionSftp, input.tmpPath + "/tarball.tar",
      input.tmpPath + "/tarball.tar", S_IWUSR | S_IRGRP);
  if (fileResult.exitCode != 0) {
    return {.exitCode = 1,
            .error = "Failed to copy file (tarball.tar) to host"};
  }
  sftp_free(sessionSftp);

  string cmd = "sudo deployClient -d " + input.tmpPath + " -s " + input.fileSig;
  if (input.lazy == true)
    cmd += " -l";

  sshHelper::result<string> cmdResult =
      sshHelper::runCommandOn(sessionSsh, cmd);

  sshHelper::disconnect(sessionSsh);

  return {
      .output = *cmdResult.output,
      .exitCode = cmdResult.exitCode,
      .error = *cmdResult.error,
  };
}
