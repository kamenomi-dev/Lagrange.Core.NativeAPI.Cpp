#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <exception>

#include "logger.cpp"

#include "third_party/nlohmann/json.hpp"

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_DEFINES_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_DEFINES_H_
#pragma region Common

namespace LagrangeCore {

namespace NativeTypes {
struct ByteArray {
    UINT    Length = 0;
    INT_PTR Data{};
};

using ByteArrayDict = ByteArray;

struct BotConfigStruct {
    BYTE Protocol          = 0b00000100;
    BOOL AutoReconnect     = true;
    BOOL UseIPv6Network    = false;
    BOOL GetOptimumServer  = true;
    UINT HighwayChunkSize  = 1024 * 1024;
    UINT HighwayConcurrent = 4;
    BOOL AutoReLogin       = true;
};

struct BotKeystoreStruct {
    INT64                      Uin = 0;
    NativeTypes::ByteArray     Uid;
    NativeTypes::ByteArray     Guid;
    NativeTypes::ByteArray     AndroidId;
    NativeTypes::ByteArray     Qimei;
    NativeTypes::ByteArray     DeviceName;
    NativeTypes::ByteArray     A2;
    NativeTypes::ByteArray     A2Key;
    NativeTypes::ByteArray     D2;
    NativeTypes::ByteArray     D2Key;
    NativeTypes::ByteArray     A1;
    NativeTypes::ByteArray     A1Key;
    NativeTypes::ByteArray     NoPicSig;
    NativeTypes::ByteArray     TgtgtKey;
    NativeTypes::ByteArray     StKey;
    NativeTypes::ByteArray     StWeb;
    NativeTypes::ByteArray     St;
    NativeTypes::ByteArray     WtSessionTicket;
    NativeTypes::ByteArray     WtSessionTicketKey;
    NativeTypes::ByteArray     RandomKey;
    NativeTypes::ByteArray     SKey;
    NativeTypes::ByteArrayDict PsKey;
};

enum StatusCode {
    Success = 0,
    UnInitialized,
    AlreadyInitialized,
    AlreadyStarted,
    InvalidIndex
};

using ContextIndex = int;

}; // namespace NativeTypes

namespace CoreTypes {
struct BotInfo {
    int         Age    = 0;
    int         Gender = 0;
    std::string Name;
};

struct WLoginSigs {
    std::string A2;
    std::string A2Key;
    std::string D2;
    std::string D2Key;
    std::string A1;
    std::string A1Key;
    std::string NoPicSig;
    std::string TgtgtKey;
    std::string Ksid;
    std::string SuperKey;
    std::string StKey;
    std::string StWeb;
    std::string St;
    std::string WtSessionTicket;
    std::string WtSessionTicketKey;
    std::string RandomKey;
    std::string SKey;
    std::string PsKey;
};

struct BotKeystoreStructRaw {
    int64_t     Uin = 0;
    std::string Uid;
    BotInfo     BotInfo;
    WLoginSigs  WLoginSigs;
    std::string Guid;
    std::string AndroidId;
    std::string Qimei;
    std::string DeviceName;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    BotInfo, Age, Gender, Name
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    WLoginSigs, A2, A2Key, D2, D2Key, A1, A1Key, NoPicSig, TgtgtKey, Ksid, SuperKey, StKey, StWeb, St, WtSessionTicket,
    WtSessionTicketKey, RandomKey, SKey, PsKey
)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    BotKeystoreStructRaw, Uin, Uid, BotInfo, WLoginSigs, Guid, AndroidId, Qimei, DeviceName
)

} // namespace CoreTypes
} // namespace LagrangeCore

#pragma endregion Common
#endif // _LAGRANGE_CORE_NATIVEAPI_CPP_DEFINES_H_