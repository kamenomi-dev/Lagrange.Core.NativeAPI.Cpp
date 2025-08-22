#pragma once
#include <string>

namespace Lagrange::Definition::NativeModel::Common {
struct ByteArrayNative {
    CSharp_Int32  Length{NULL};
    CSharp_IntPtr Data{NULL};

  public:
    ByteArrayNative() {};
    /// <summary> Need to release self manually. </summary>
    ByteArrayNative(
        std::string str
    ) {
        Length = (CSharp_Int32)str.length();
        Data   = (CSharp_IntPtr)memcpy(new char[Length], str.data(), str.length());
    }
    /// <summary> Need to release self manually. </summary>
    ByteArrayNative(
        size_t length, void* data
    ) {
        Length = (CSharp_Int32)length;
        Data   = (CSharp_IntPtr)memcpy(new char[length], (void*)data, length);
    }

    // std::string ToBase64() const { return base64_encode((const unsigned char*)Data, Length); }

    void TryRelease() noexcept {
        try {
            if (Data) {
                ZeroMemory((void*)Data, Length);
            }
        } catch (...) {}

        Data   = NULL;
        Length = NULL;
    }

    std::string ToString() const { return std::string{(const char*)Data, (size_t)Length}; };
    operator std::string() { return ToString(); }
};
} // namespace Lagrange::Definition::NativeModel::Common