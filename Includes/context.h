#pragma once

#include "object_wrapper.h"

namespace LagrangeCore::Context {

class BaseContext {};

class GroupMessageContext : public BaseContext {};

class PrivateMessageContext : public BaseContext {};

class StrangerMessageContext : public BaseContext {};
} // namespace LagrangeCore::Context