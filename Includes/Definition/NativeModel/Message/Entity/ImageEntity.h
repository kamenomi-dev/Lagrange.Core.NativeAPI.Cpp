#pragma once
namespace Lagrange::Definition::NativeModel::Message::Entity {
struct ImageEntity : IEntity {
    Common::ByteArrayNative FileUrl{};
    Common::ByteArrayNative FileName{};
    Common::ByteArrayNative FileSha1{};
    CSharp_UInt32           FileSize{NULL};
    Common::ByteArrayNative FileMd5{};
    CSharp_Single           ImageWidth{0.f};
    CSharp_Single           ImageHeight{0.f};
    CSharp_Int32            SubType{NULL};
    Common::ByteArrayNative Summary{};
    CSharp_UInt32           RecordLength{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity