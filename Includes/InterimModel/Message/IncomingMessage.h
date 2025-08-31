#pragma once
#include <tuple>
#include <array>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <initializer_list>

namespace Lagrange::InterimModel::Message {

namespace Entity {
struct ImageEntity;
struct MentionEntity;
struct MultiMsgEntity;
struct RecordEntity;
struct ReplyEntity;
struct Text;
struct VideoEntity;
} // namespace Entity

using NativeEntityTypes = std::tuple<
    NATIVE_MODEL Message::Entity::ImageEntity,
    NATIVE_MODEL Message::Entity::MentionEntity,
    NATIVE_MODEL Message::Entity::MultiMsgEntity,
    NATIVE_MODEL Message::Entity::RecordEntity,
    NATIVE_MODEL Message::Entity::ReplyEntity,
    NATIVE_MODEL Message::Entity::TextEntity,
    NATIVE_MODEL Message::Entity::VideoEntity>;

using InterimEntityTypes = std::tuple<
    Entity::ImageEntity,
    Entity::MentionEntity,
    Entity::MultiMsgEntity,
    Entity::RecordEntity,
    Entity::ReplyEntity,
    Entity::TextEntity,
    Entity::VideoEntity>;

struct IncomingMessage {
    IncomingMessage(
        NATIVE_MODEL Message::BotMessage msg
    ) {
        for (int index = 0; index < msg.EntityLength; index++) {
            _entities.push_back(msg.Entities + index);
        }
    };

    IncomingMessage(
        NATIVE_MODEL Message::BotMessage* msg
    ) {
        for (int index = 0; index < msg->EntityLength; index++) {
            _entities.push_back(msg->Entities + index);
        }
    };

    ~IncomingMessage() {
        if (_entities.empty()) {
            return;
        }

        DllExports::FreeMemory(_entities[0]->Entity);
    }

    auto& GetEntities() { return _entities; }

    template <NATIVE_MODEL Message::Entity::EntityType ExpectedType>
    auto Pick(
        size_t position
    ) -> typename std::tuple_element<(int)ExpectedType, InterimEntityTypes>::type {
        using NativeType  = typename std::tuple_element<(int)ExpectedType, NativeEntityTypes>::type;
        using InterimType = typename std::tuple_element<(int)ExpectedType, InterimEntityTypes>::type;

        if (position >= _entities.size()) {
            throw std::out_of_range("Invalid entity position");
        }

        const auto entity = _entities[position];
        if (entity->Type != ExpectedType) {
            throw std::invalid_argument("Mismatched entity");
        }

        return InterimType((NativeType*)entity->Entity);
    }

    bool Expect(
        std::initializer_list<NATIVE_MODEL Message::Entity::EntityType> types
    ) {
        if (_entities.size() < types.size()) {
            return false;
        }

        for (size_t index = 0; index < types.size(); index++) {
            if (types.begin()[index] != _entities[index]->Type) {
                return false;
            }
        }

        return true;
    }

    template <NATIVE_MODEL Message::Entity::EntityType... Types>
    bool Expect() {
        if (_entities.size() < sizeof...(Types)) return false;

        size_t i = 0;
        return ((_entities[i++]->Type == Types) && ...);
    }

  private:
    std::vector<NATIVE_MODEL Message::TypedEntity*> _entities;
};
} // namespace Lagrange::InterimModel::Message