#pragma once
#include <Windows.h>
#include <string>
#include <memory>
#include <exception>

#ifndef _LAGRANGE_CORE_NATIVEAPI_CPP_COMMON_H_
#define _LAGRANGE_CORE_NATIVEAPI_CPP_COMMON_H_
#pragma region Common

namespace LagrangeCore {
namespace Common {
struct ByteArray {
    UINT    Length = 0;
    INT_PTR Data;

    ByteArray();
    ByteArray(
        UINT length
    ) {
        Length = length;
    }
};

using ByteArrayDict = ByteArray;
} // namespace Common


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
    INT64                 Uin = 0;
    Common::ByteArray     Uid;
    Common::ByteArray     Guid;
    Common::ByteArray     AndroidId;
    Common::ByteArray     Qimei;
    Common::ByteArray     DeviceName;
    Common::ByteArray     A2;
    Common::ByteArray     A2Key;
    Common::ByteArray     D2;
    Common::ByteArray     D2Key;
    Common::ByteArray     A1;
    Common::ByteArray     A1Key;
    Common::ByteArray     NoPicSig;
    Common::ByteArray     TgtgtKey;
    Common::ByteArray     StKey;
    Common::ByteArray     StWeb;
    Common::ByteArray     St;
    Common::ByteArray     WtSessionTicket;
    Common::ByteArray     WtSessionTicketKey;
    Common::ByteArray     RandomKey;
    Common::ByteArray     Skey;
    Common::ByteArrayDict PsKey;
};

enum StatusCode {
    Success = 0,
    UnInitialized,
    AlreadyInitialized,
    AlreadyStarted,
    InvalidIndex
};

using ContextIndex = int;
} // namespace LagrangeCore

#pragma endregion Common
#endif // _LAGRANGE_CORE_NATIVEAPI_CPP_COMMON_H_