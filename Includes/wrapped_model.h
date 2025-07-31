#pragma once

#include "wrapper.h"
#include "native_model.h"

#include <typeindex>
#include <type_traits>

#define MODEL_MESSAGE NativeModel::message::

#define CaseEntityType(type)                                                                                           \
    case MODEL_MESSAGE Entity::EntityType::##type##Entity:                                                             \
        _entities.emplace_back(new type((MODEL_MESSAGE Entity::##type##Entity*)entity->Entity));              \
        break;

#define ReleaseEntityPtr(Type)                                                                                         \
    if (entity && entity->type() == typeid(Type)) {                                                                    \
        delete (Type*)entity;                                                                                          \
        return;                                                                                                        \
    }

namespace LagrangeCore {

namespace WrappedModel {
// Todo: Implement Group, User & Friend classes.
class Group {
  public:
    Group(){};
    Group(uint64_t group_id){};
};

class User {
  public:
    User(uint64_t user_id) : _userId(user_id) {};

    operator uint64_t() const { return _userId; };

  private:
    uint64_t _userId{0};
};

class Member : public User {
  public:
    Member() : User(0) {};
    Member(const NativeModel::message::BotGroupMemeber* member) : User(member->Uin) {};
};

class Friend : public User {
  public:
    Friend(uint64_t user_id) : User(user_id) {};
};

using Stranger = User;
} // namespace WrappedModel

namespace WrappedModel::message {

struct IEntity {
    virtual std::type_index type() const = 0;
};

struct Image : public IEntity {
    std::string FileUrl;
    std::string FileName;
    std::string FileSha1;
    UINT        FileSize = 0;
    std::string FileMd5;
    FLOAT       ImageWidth  = 0.f;
    FLOAT       ImageHeight = 0.f;
    INT         SubType     = NULL;
    std::string Summary;
    UINT        RecordLength = NULL;

    std::type_index type() const { return typeid(Image); }

    Image(
        MODEL_MESSAGE Entity::ImageEntity* entity
    ) {
        FileUrl      = entity->FileUrl;
        FileName     = entity->FileName;
        FileSha1     = entity->FileSha1;
        FileSize     = entity->FileSize;
        FileMd5      = entity->FileMd5;
        ImageWidth   = entity->ImageWidth;
        ImageHeight  = entity->ImageHeight;
        SubType      = entity->SubType;
        Summary      = entity->Summary;
        RecordLength = entity->RecordLength;
    }

    // 这里没必要，因为 wrapped_model 是供使用者的，没必要做转换。
    // operator NativeModel::message::Entity::ImageEntity() const {
    //    NativeModel::message::Entity::ImageEntity entity{};
    //    entity.FileUrl      = FileUrl;
    //    entity.FileName     = FileName;
    //    entity.FileSha1     = FileSha1;
    //    entity.FileSize     = FileSize;
    //    entity.FileMd5      = FileMd5;
    //    entity.ImageWidth   = ImageWidth;
    //    entity.ImageHeight  = ImageHeight;
    //    entity.SubType      = SubType;
    //    entity.Summary      = Summary;
    //    entity.RecordLength = RecordLength;
    //    return entity;
    //}
};

struct Mention : public IEntity {
    WrappedModel::User Sender;
    std::string        Display;

    std::type_index type() const override { return typeid(Mention); }

    Mention(
        MODEL_MESSAGE Entity::MentionEntity* entity
    )
    : Sender(entity->Uin) {
        Display = entity->Display;
    }
};

struct Record : public IEntity {

    std::type_index type() const override { return typeid(Record); }

    Record(MODEL_MESSAGE Entity::RecordEntity* entity) {};
};

struct Reply : public IEntity {

    std::type_index type() const override { return typeid(Reply); }

    Reply(MODEL_MESSAGE Entity::ReplyEntity* entity) {};
};

struct Video : public IEntity {

    std::type_index type() const override { return typeid(Video); }

    Video(MODEL_MESSAGE Entity::VideoEntity* entity) {};
};

struct Text : public IEntity {
    std::string message;

    std::type_index type() const override { return typeid(Text); }

    Text(
        MODEL_MESSAGE Entity::TextEntity* entity
    ) {
        message = entity->Text;
    };
};

struct MultiMsg : public IEntity {

    std::type_index type() const override { return typeid(MultiMsg); }

    MultiMsg(MODEL_MESSAGE Entity::MultiMsgEntity* entity) {};
};

class EntitySequence {
  private:
    std::vector<IEntity*> _entities;

  public:
    EntitySequence() {};
    EntitySequence(
        NativeModel::message::BotMessage& message
    ) {
        _entities.reserve(message.EntityLength);
        for (int i = 0; i < message.EntityLength; i++) {
            auto entity = (MODEL_MESSAGE TypedEntity*)(message.Entities + i * sizeof(MODEL_MESSAGE TypedEntity));

            switch (entity->Type) {
                CaseEntityType(Image);
                CaseEntityType(Mention);
                CaseEntityType(Record);
                CaseEntityType(Reply);
                CaseEntityType(Video);
                CaseEntityType(Text);
                CaseEntityType(MultiMsg);
            default:
                spdlog::warn(
                    "[EntitySequence] Unsupported Message Type Index: {}! You may need to update the Library.",
                    (UINT)entity->Type
                );
                break;
            }
        }
    }

    ~EntitySequence() {
        for (auto entity : _entities) {
            ReleaseEntityPtr(Image);
            ReleaseEntityPtr(Mention);
            ReleaseEntityPtr(Record);
            ReleaseEntityPtr(Reply);
            ReleaseEntityPtr(Video);
            ReleaseEntityPtr(Text);
            ReleaseEntityPtr(MultiMsg);
        }

        _entities.clear();
    };

    // Generated By ChatGPT.
    template <typename... Types>
    bool ExpectSequence() {
        constexpr std::size_t N = sizeof...(Types);
        if (_entities.size() < N) {
            return false;
        }

        std::type_index expected[] = {typeid(Types)...};
        for (std::size_t i = 0; i < N; ++i) {
            if (_entities[i]->type() != expected[i]) {
                return false;
            }
        }

        return true;
    }

    auto& GetSequence() { return _entities; }
};
} // namespace WrappedModel::message


} // namespace LagrangeCore

#undef MODEL_MESSAGE
#undef CaseEntityType