#include "Debug.h"

void Debug::Print::message(const char *msg, int options) {
    for (int i = 0; i < 32; i++) {
        if (options & (1 << i)) {
            switch (i) {
                case 0:
                    // 输出 PID 判断是否存在 PID_SAMPLE
                    if (!(options & (1 << i + 1))) printf("[pid=%d]", getpid());
                    break;
                case 1:
                    // 输出 PID sample
                    printf("[%05d]", getpid());
                    break;
                case 2:
                    // 输出时间戳 判断是否存在 SAMPLE
                    if (!(options & (1 << i + 1)))
                        printf("{timestamp=%ld}", time(NULL));
                    break;
                case 3:
                    // 输出时间戳 sample
                    printf("{%ld}", time(NULL));
                    break;
                default:
                    throw std::logic_error("debug formatPrint not match option ..");
                    break;
            }
        }
    }
    if (options) printf(": ");
    printf("%s\n", msg);
    // 强制刷新缓冲区
    fflush(stdout);
}

void Debug::Print::message(const std::string &msg, int options) {
    Debug::Print::message(msg.c_str(), options);
}

void Debug::Print::messageWithIdentity(const char *identity,
                                       const char *msg,
                                       int options) {
    printf("<%s>", identity);
    message(msg, options);
}

void Debug::Print::messageWithIdentity(const std::string &identity,
                                       const std::string &msg,
                                       int options) {
    messageWithIdentity(identity.c_str(), msg.c_str(), options);
}

void Debug::Print::bytes(byte *bytes, size_t size) {
    printf("basical address: [%p]\n", bytes);

    for (size_t i = 1; i <= size; i++) {
        printf("%3u ", (unsigned char) bytes[i - 1]);
        if (i % Debug::Print::ROW_OF_BYTES_LENGTH == 0) puts("");
    }
    if (size % Debug::Print::ROW_OF_BYTES_LENGTH != 0) puts("");
}

void Debug::Print::bytes(byte_array byteArray) {
    bytes(byteArray.value, byteArray.size);
}