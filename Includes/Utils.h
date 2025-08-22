#pragma once
#include <ctime>
#include <chrono>
#include <string>
#include <fstream>
#include <filesystem>

namespace FileSystem = std::filesystem;

namespace Lagrange::Utils {
/// @return Current local time like "20250820T173800"
extern std::string GetCurrentLocalTimeAsISO8601() {
    time_t currentTime = std::time(nullptr);
    tm     localTime{};
    localtime_s(&localTime, &currentTime);

    std::stringstream result;
    result << std::put_time(&localTime, "%Y%m%dT%H%M%S");
    return result.str();
}

class FileBinaryHelper {
  public:
    FileBinaryHelper(
        std::string file
    ) {
        _fileStream = std::fstream{file, std::ios::app};
        _fileStream.close();
        _fileStream.clear();

        _fileStream = std::fstream{file, std::ios::in | std::ios::out | std::ios::binary};
        _fileStream.seekp(0);
        _fileStream.seekg(0);
    };

    ~FileBinaryHelper() {
        _fileStream.close();
        _fileStream.clear();
    }

    std::fstream& GetStream() { return _fileStream; }

    int32_t ReadAsInt32() {
        int32_t value = 0;
        _fileStream.read((char*)&value, sizeof(int32_t));
        if (_fileStream.fail()) {
            throw std::exception("Unexpected error while reading Int32.");
        }
        return value;
    };

    int64_t ReadAsInt64() {
        int64_t value = 0;
        _fileStream.read((char*)&value, sizeof(int64_t));
        if (_fileStream.fail()) {
            throw std::exception("Unexpected error while reading Int64.");
        }
        return value;
    };

    void* __Unsafe_ReadAsBinaryData(
        size_t length
    ) {
        void* buffer = new char[length];
        _fileStream.read((char*)buffer, length);

        if (_fileStream.fail()) {
            delete[] (char*)buffer;
            throw std::exception("Unexpected error while reading binary data.");
        }

        return buffer;
    }

    void WriteAsInt32(
        int32_t data
    ) {
        _fileStream.write((const char*)&data, sizeof(int32_t));
        if (_fileStream.fail()) {
            throw std::exception("Unexpected error while writing Int32.");
        }
    }

    void WriteAsInt64(
        int64_t data
    ) {
        _fileStream.write((const char*)&data, sizeof(int64_t));
        if (_fileStream.fail()) {
            throw std::exception("Unexpected error while writing Int64.");
        }
    }

    void __Unsafe_WriteAsBinaryData(
        const void* data, size_t length
    ) {
        _fileStream.write((const char*)data, length);
        if (_fileStream.fail()) {
            throw std::exception("Unexpected error while writing binary data.");
        }
    }

  private:
    std::fstream _fileStream;
};
} // namespace Lagrange::Utils