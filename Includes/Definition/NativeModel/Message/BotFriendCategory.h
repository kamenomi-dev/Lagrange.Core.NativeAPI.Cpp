#pragma once

namespace Lagrange::Definition::NativeModel::Common {
struct BotFriendCategory {
    CSharp_Int32    Id = 0;
    ByteArrayNative Name{};
    CSharp_Int32    Count  = 0;
    CSharp_Int32    SortId = 0;
};
} // namespace Lagrange::Definition::NativeModel::Common