#pragma once
#include <tuple>
#include <array>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <initializer_list>

namespace Lagrange::Definition::InterimModel::Message {

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
    NativeModel::Message::Entity::ImageEntity,
    NativeModel::Message::Entity::MentionEntity,
    NativeModel::Message::Entity::MultiMsgEntity,
    NativeModel::Message::Entity::RecordEntity,
    NativeModel::Message::Entity::ReplyEntity,
    NativeModel::Message::Entity::TextEntity,
    NativeModel::Message::Entity::VideoEntity>;

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
        NativeModel::Message::BotMessage msg
    ) {
        for (int index = 0; index < msg.EntityLength; index++) {
            _entities.push_back(msg.Entities + index);
        }
    };

    IncomingMessage(
        InterimModel::Message::BotMessage msg
    ) {
        for (int index = 0; index < msg.Messages.EntityLength; index++) {
            _entities.push_back(msg.Messages.Entities + index);
        }
    };

    ~IncomingMessage() {
        for (auto& entity : _entities) {
            DllExports::FreeMemory(entity->Entity);
        }

        _entities.clear();
    }

    template <NativeModel::Message::Entity::EntityType ExpectedType>
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
        std::initializer_list<NativeModel::Message::Entity::EntityType> types
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

    template <NativeModel::Message::Entity::EntityType... Types>
    bool Expect() {
        if (_entities.size() < sizeof...(Types)) return false;

        size_t i = 0;
        return ((_entities[i++]->Type == Types) && ...);
    }

    // template <
    //     typename... ExpectedTypes,
    //     typename = std::enable_if_t<(std::is_base_of_v<Entity::IEntity, ExpectedTypes> && ...)>>
    // bool Expect() {
    //     static const auto typeSize = sizeof...(ExpectedTypes);
    //     if (_entities.size() < typeSize) {
    //         return false;
    //     }

    //    static const std::type_index types[] = {typeid(ExpectedTypes)...};
    //    for (size_t i = 0; i < typeSize; ++i) {
    //        if (types[i] == std::type_index(typeid(Entity::IEntity))) {
    //            continue;
    //        }

    //        if (std::type_index(typeid(*_entities[i]->Entity)) != types[i]) {
    //            return false;
    //        }
    //    }
    //    return true;
    //}

  private:
    std::vector<NativeModel::Message::TypedEntity*> _entities;
};
} // namespace Lagrange::Definition::InterimModel::Message