#pragma once

#include "wrapper.h"

namespace LagrangeCore::ObjectWrapper {

// Todo: Implement Group, User & Friend classes.
class Group {
  public:
    Group(uint64_t group_id);
};

class User {
  public:
    User(uint64_t user_id);
};

class Friend : public User {
  public:
    Friend(uint64_t user_id) : User(user_id) {};
};

using Stranger = User;
} // namespace LagrangeCore::ObjectWrapper