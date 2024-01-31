#include <stdexcept>

class NetworkFailure : public std::exception {};
class DiskFailure : public std::exception {};
class InternalStatusFailure : public std::exception {};

void NetworkCheck() {
    throw NetworkFailure();
}

void DiskCheck() {
    throw DiskFailure();
}

void InternalStatusCheck() {
    throw InternalStatusFailure();
}
