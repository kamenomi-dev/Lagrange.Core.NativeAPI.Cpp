#pragma once
#include <cstdint>
#include <cstddef>
#include <cuchar>

typedef uint8_t  CSharp_Byte;
typedef int8_t   CSharp_SByte;
typedef int16_t  CSharp_Int16;
typedef uint16_t CSharp_UInt16;
typedef int32_t  CSharp_Int32;
typedef uint32_t CSharp_UInt32;
typedef int64_t  CSharp_Int64;
typedef uint64_t CSharp_UInt64;

typedef char16_t CSharp_Char;
typedef int32_t  CSharp_Boolean;

typedef float_t CSharp_Single;

typedef intptr_t  CSharp_IntPtr;
typedef uintptr_t CSharp_UIntPtr;

typedef void* VOIDPTR;

#ifndef ZeroMemory
#define ZeroMemory(dst, sz) memset((dst), 0, (sz))
#endif
