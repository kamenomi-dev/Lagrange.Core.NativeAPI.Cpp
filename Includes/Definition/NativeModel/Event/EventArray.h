#pragma once
namespace Lagrange::Definition::NativeModel::Event {
struct EventArray {
    CSharp_IntPtr Events{NULL};
    CSharp_Int32  Count{NULL};
};
} // namespace Lagrange::Definition::NativeModel::Event