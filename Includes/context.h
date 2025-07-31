#pragma once

#include <memory>

#include "wrapped_model.h"

namespace LagrangeCore::Context {

class BaseContext {};

class BaseMessageContext {
  public:
    std::unique_ptr<WrappedModel::message::EntitySequence> message;
};

class GroupMessageContext : public BaseMessageContext {
  public:
    WrappedModel::Group  group;
    WrappedModel::Member member;

  public:
    GroupMessageContext() {};
};

class PrivateMessageContext : public BaseMessageContext {};

class StrangerMessageContext : public BaseMessageContext {};
} // namespace LagrangeCore::Context