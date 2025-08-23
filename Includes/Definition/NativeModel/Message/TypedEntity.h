#pragma once
namespace Lagrange::Definition::NativeModel::Message {
struct TypedEntity {
    CSharp_IntPtr Entity{NULL};
    CSharp_Int32  Type{NULL};
};
}