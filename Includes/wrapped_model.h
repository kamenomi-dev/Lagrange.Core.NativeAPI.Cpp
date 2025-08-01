#pragma once

#include "wrapper.h"
#include "native_model.h"

#include <typeindex>
#include <type_traits>

#define MODEL_MESSAGE NativeModel::Message::

#define CaseEntityType(type)                                                                                           \
    case MODEL_MESSAGE Entity::EntityType::##type##Entity:                                                             \
        _entities.emplace_back(new type##((MODEL_MESSAGE Entity::##type##Entity*)entity->Entity));                     \
        break;

#define ReleaseEntityPtr(Type)                                                                                         \
    if (entity && entity->type() == typeid(##Type##)) {                                                                \
        delete (##Type##*)entity;                                                                                      \
        return;                                                                                                        \
    }

namespace LagrangeCore {

namespace WrappedModel {
// Todo: Implement Group, User & Friend classes.
class Group {
  public:
    Group() = default;
    Group(uint64_t group_id) : _groupId(group_id) {};

    operator uint64_t() const { return _groupId; };

  private:
    uint64_t _groupId{0};
};

class User {
  public:
    User() = default;
    User(uint64_t user_id) : _userId(user_id) {};

    operator uint64_t() const { return _userId; };

  private:
    uint64_t _userId{0};
};

class Member : public User {
  public:
    Member() : User(0) {};
    Member(const NativeModel::Message::BotGroupMemeber* member) : User(member->Uin) {};
};

class Friend : public User {
  public:
    Friend(uint64_t user_id) : User(user_id) {};
};

using Stranger = User;
} // namespace WrappedModel

namespace WrappedModel::Message {

// Todo: 完善剩余消息结构。

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
    // operator NativeModel::Message::Entity::ImageEntity() const {
    //    NativeModel::Message::Entity::ImageEntity entity{};
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
    std::string Message;

    std::type_index type() const override { return typeid(Text); }

    Text(
        MODEL_MESSAGE Entity::TextEntity* entity
    ) {
        Message = entity->Text;
    };
};

struct MultiMsg : public IEntity {

    std::type_index type() const override { return typeid(MultiMsg); }

    MultiMsg(MODEL_MESSAGE Entity::MultiMsgEntity* entity) {};
};

class ReceivedEntitySequence {
  private:
    std::vector<IEntity*> _entities;

  public:
    ReceivedEntitySequence() {};
    ReceivedEntitySequence(
        NativeModel::Message::BotMessage& Message
    ) {
        _entities.reserve(Message.EntityLength);
        for (int i = 0; i < Message.EntityLength; i++) {
            auto entity = (MODEL_MESSAGE TypedEntity*)(Message.Entities + i * sizeof(MODEL_MESSAGE TypedEntity));

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

    ~ReceivedEntitySequence() {
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
    bool Expect() {
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

    template <typename Ty>
    Ty* Pick(
        size_t position
    ) {
        if (_entities.empty()) {
            spdlog::trace("No entities available to pick. ");
            abort();
        }

        try {
            auto entity = _entities[position];
        } catch (...) {
            spdlog::trace("No entity existent to pick. ");
            abort();
        }

        if (typeid(Ty) != _entities[position]->type()) {
            spdlog::trace("Not entity matched to pick. ");
            abort();
        }

        return (Ty*)_entities[position];
    }

    auto& GetSequence() { return _entities; }
};

class SendableEntitySequence {
  public:
    enum class SendingType : char {
        Group  = 0,
        Friend = 1,
    };

  public:
    SendableEntitySequence(
        ContextIndex ctxIdx
    )
    : _contextIndex(ctxIdx),
      _builderIndex(DllExports->CreateMessageBuilder(ctxIdx)) {}

    auto& Text(
        std::string text
    ) {
        NativeModel::Common::ByteArrayNative bytes{text};
        DllExports->AddText(_contextIndex, _builderIndex, bytes);
        bytes.TryRelease();
        return *this;
    }

    template <SendingType Ty = SendingType::Group>
    NativeModel::Message::BotMessage* Send(
        uint64_t uin
    ) {
        if constexpr (Ty == SendingType::Group) {
            auto retPtr = DllExports->SendGroupMessage(_contextIndex, _builderIndex, uin);
            return (NativeModel::Message::BotMessage*)retPtr;
        };

        // Todo: Implement friend message.
        abort();
    }

  private:
    ContextIndex _contextIndex{0};
    INT          _builderIndex{0};
};

} // namespace WrappedModel::Message


} // namespace LagrangeCore

#undef MODEL_MESSAGE
#undef CaseEntityType