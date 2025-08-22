#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct BotMessage {
    CSharp_IntPtr           Contact{NULL};
    CSharp_IntPtr           Receiver{NULL};
    BotGroup                Group{};
    CSharp_Int32            Type{NULL};
    Common::ByteArrayNative Time{};
    CSharp_IntPtr           Entities{NULL};
    CSharp_Int32            EntityLength{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Message