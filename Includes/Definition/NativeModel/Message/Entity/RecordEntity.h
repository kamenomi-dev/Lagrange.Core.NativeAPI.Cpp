#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct RecordEntity : IEntity {
    Common::ByteArrayNative FileUrl{};
    Common::ByteArrayNative FileName{};
    Common::ByteArrayNative FileSha1{};
    CSharp_UInt32           FileSize{NULL};
    Common::ByteArrayNative FileMd5{};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity