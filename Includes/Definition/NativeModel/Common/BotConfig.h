#pragma once
namespace Lagrange::Definition::NativeModel::Common {
struct BotConfig {
    CSharp_Byte    Protocol          = 0b00000100;
    CSharp_Boolean AutoReconnect     = true;
    CSharp_Boolean UseIPv6Network    = false;
    CSharp_Boolean GetOptimumServer  = true;
    CSharp_UInt32  HighwayChunkSize  = 1024 * 1024;
    CSharp_UInt32  HighwayConcurrent = 4;
    CSharp_Boolean AutoReLogin       = true;
};
} // namespace Lagrange::Definition::NativeModel::Common