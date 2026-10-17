#include "sslHelper.h"
#include <cstddef>
#include <cstdio>
#include <libssh/libssh.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/provider.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

const sslHelper::result<vector<unsigned char>>
sslHelper::getSHA512Hash(const vector<unsigned char> data) {

  const unsigned int dataLen = data.size();
  unsigned char *dataArray = new unsigned char[dataLen];
  copy(data.begin(), data.end(), dataArray);

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  unsigned char SHA256Key[EVP_MAX_MD_SIZE];
  unsigned int SHA256Size = EVP_MAX_MD_SIZE;
  if (EVP_DigestInit(ctx, EVP_sha512()) != 1) { // 1 = succeed, 0 = failed
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestInit Failed"};
  }
  if (EVP_DigestUpdate(ctx, dataArray, dataLen) != 1) {
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestUpdate Failed"};
  }
  delete[] dataArray;
  if (EVP_DigestFinal_ex(ctx, SHA256Key, &SHA256Size) != 1) {
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestFinal_ex failed"};
  }
  EVP_MD_CTX_free(ctx);

  vector<unsigned char> output(SHA256Key, SHA256Key + SHA256Size);
  return {.output = output, .exitCode = 0};
}

const sslHelper::result<EVP_PKEY *>
sslHelper::openPrivateKey(const string privateKeyFile) {
  EVP_PKEY *privateKeyPKEY = nullptr;

  string privateKeyString = privateKeyFile;
  if (privateKeyFile.find("OPENSSH PRIVATE KEY") !=
      string::npos) { // is openssh key not ssl

    ssh_key privateKeySSH = NULL;
    if (ssh_pki_import_privkey_base64(privateKeyFile.c_str(), NULL, NULL,
                                      nullptr, &privateKeySSH) != SSH_OK) {
      SSH_KEY_FREE(privateKeySSH);
      return {.exitCode = 1,
              .error = "ssh_pki_import_privkey_base64 failed while converting "
                       "the key from the ssh format to pem"};
    }
    char *privateKeyCStr = NULL;
    if (ssh_pki_export_privkey_base64_format(privateKeySSH, NULL, NULL, nullptr,
                                             &privateKeyCStr,
                                             SSH_FILE_FORMAT_PEM) != SSH_OK) {
      SSH_KEY_FREE(privateKeySSH);
      return {.exitCode = 1,
              .error = "ssh_pki_export_privkey_base64_format failed while "
                       "converting the key from ssh format to pem."};
    }
    SSH_KEY_FREE(privateKeySSH);
    privateKeyString = string(privateKeyCStr);
    free(privateKeyCStr);
  }

  BIO *bio =
      BIO_new_mem_buf(privateKeyString.c_str(), (int)privateKeyString.size());
  if (!bio) {
    return {.exitCode = 1, .error = "BIO_new_mem_buf failed"};
  }

  privateKeyPKEY = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);

  if (!privateKeyPKEY) {
    unsigned long errCode = ERR_get_error();
    char errBuf[256];
    ERR_error_string_n(errCode, errBuf, sizeof(errBuf));
    string errStr(errBuf);
    if (errStr.find("bad password") != string::npos ||
        errStr.find("bad decrypt") != string::npos) {
      return {
          .exitCode = 1,
          .error = "Key requires password. Add support for password to func or "
                   "cry :3",
      };
    }
    return {.exitCode = 1, .error = errStr};
  }
  return {.output = privateKeyPKEY, .exitCode = 0};
}

const sslHelper::result<vector<unsigned char>>
sslHelper::signDataWithKey(const vector<unsigned char> data,
                           EVP_PKEY *privateKey) {

  if (!privateKey) {
    return {.exitCode = 1, .error = "privateKey isn't valid"};
  }

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  if (EVP_DigestSignInit_ex(ctx, NULL, NULL, NULL, NULL, privateKey, NULL) !=
      1) {
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestSignInit_ex failed"};
  };
  unsigned char signature[EVP_MAX_MD_SIZE];
  size_t signatureSize = EVP_MAX_MD_SIZE;

  const unsigned int dataSize = data.size();
  unsigned char *dataArr = new unsigned char[dataSize];
  copy(data.begin(), data.end(), dataArr);

  if (EVP_DigestSign(ctx, signature, &signatureSize, dataArr, dataSize) != 1) {
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestSign failed"};
  }

  EVP_MD_CTX_free(ctx);

  vector<unsigned char> output(signature, signature + signatureSize);
  return {.output = output, .exitCode = 0};
}
const sslHelper::result<bool>
sslHelper::verifyDataWithSign(const vector<unsigned char> data,
                              const vector<unsigned char> sign) {

  EVP_MD_CTX *ctx = EVP_MD_CTX_new();

  if (EVP_DigestVerifyInit(ctx, NULL, NULL, NULL, NULL) != 1) {
    EVP_MD_CTX_free(ctx);
    return {.exitCode = 1, .error = "EVP_DigestVerifyInit failed"};
  };
  int out = EVP_DigestVerify(ctx, &sign[0], sign.size(), &data[0], data.size());
  if (out != 1) {
    EVP_MD_CTX_free(ctx);
    return {.output = false,
            .exitCode = 0,
            .error = "Signature is not valid or EVP_DigestSign failed"};
  }

  EVP_MD_CTX_free(ctx);

  return {.output = out, .exitCode = 0};
}
