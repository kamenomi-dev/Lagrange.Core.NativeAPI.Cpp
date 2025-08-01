#pragma once
#include "wrapper.h"
#include "wrapped_model.h"
#include "context.h"

#include "submodule/eventpp/eventdispatcher.h"

#include <functional>

namespace LagrangeCore::EventHandler {

namespace EventTypes {
enum MessageEvents : uint8_t {
    GroupMessage = 0,
    PrivateMessage,
    StrangerMessage
};
} // namespace EventTypes

struct AnyContext {
  protected:
    ContextIndex _contextIndex{0};
    void*        _pContext{nullptr};

  public:
    AnyContext(ContextIndex contextIndex, void* context) : _contextIndex(contextIndex), _pContext(context) {};

    auto ToGroupMessage() { return (Context::GroupMessageContext*)_pContext; };
    auto ToPrivateMessage() { return (Context::PrivateMessageContext*)_pContext; };
    auto ToStrangerMessage() { return (Context::StrangerMessageContext*)_pContext; };

    auto GetContextIndex() const { return _contextIndex; };
};

class EventHandler {
    template <typename Ty, typename Ret = void>
    using EventDispatcher = eventpp::EventDispatcher<Ty, Ret(AnyContext)>;

  public:
    EventHandler() = default;

    void BindContext(
        ContextIndex contextIndex
    ) {
        _contextIndex = contextIndex;
    }

    void PollEvent() {
        HandleLogEvent();
        HandleQRCodeEvent();
        HandleMessageEvent();

        ListEventCount();
    };

    auto& RegisterMiddleware() { return *this; }

    auto& RegisterMessageHandler(
        NativeModel::Message::MessageType type, std::function<void(AnyContext)> handler
    ) {
        _messageEventDispatcher.appendListener(type, handler);
        return *this;
    }

  private:
    template <typename Target>
    void ForEachEvent(
        INT_PTR ptr, std::function<void(Target&)> callback
    ) {
        if (ptr == NULL) return;

        auto result = (NativeModel::Event::EventArray*)ptr;

        for (auto idx = 0; idx < result->count; idx++) {
            INT_PTR pCurrEvent = result->events + idx * sizeof(Target);
            auto&   currEvent  = *(Target*)pCurrEvent;

            callback(currEvent);
        }

        DllExports->FreeMemory(ptr);
    };

    void HandleLogEvent() {
        ForEachEvent<NativeModel::Event::BotLogEvent>(
            DllExports->GetBotLogEvent(_contextIndex),
            [this](NativeModel::Event::BotLogEvent& event) {
                spdlog::debug("[Core.Log] {} - {}", _contextIndex, (std::string)event.Message);
            }
        );
    }

    void HandleQRCodeEvent() {
        ForEachEvent<NativeModel::Event::BotQrCodeEvent>(
            DllExports->GetQrCodeEvent(_contextIndex),
            [this](NativeModel::Event::BotQrCodeEvent& event) {
                spdlog::info("Login via QRCode, url: {}", (std::string)event.Url);
                std::ofstream("qrcode.png", std::ios::binary) << (std::string)event.Image;
            }
        );
    }

    void HandleMessageEvent() {
        ForEachEvent<NativeModel::Message::BotMessage>(
            DllExports->GetMessageEvent(_contextIndex),
            [this](NativeModel::Message::BotMessage& event) {
                if (event.Type != NativeModel::Message::MessageType::Group) {
                    DllExports->FreeMemory(event.Contact);
                    DllExports->FreeMemory(event.Receiver);
                    return;
                }

                Context::GroupMessageContext context{};
                context.Message = std::make_unique<WrappedModel::Message::ReceivedEntitySequence>(event);
                context.member  = (NativeModel::Message::BotGroupMemeber*)event.Contact;

                _messageEventDispatcher.dispatch(NativeModel::Message::MessageType::Group, AnyContext(_contextIndex, &context));

                /*for (auto index = 0; index < event.EntityLength; index ++)
                {
                  auto entity = (NativeModel::Message::TypedEntity*)(event.Entities + index *
                sizeof(NativeModel::Message::TypedEntity)); if (entity->Type ==
                NativeModel::Message::Entity::EntityType::TextEntity) { auto textEntity =
                (NativeModel::Message::Entity::TextEntity*)entity->Entity;

                        spdlog::info("[Group({}) - Message] [{}({})] - {}",
                            event.Group.GroupUin,
                            ((NativeModel::Message::BotGroupMemeber*)event.Contact)->Nickname,
                            ((NativeModel::Message::BotGroupMemeber*)event.Contact)->Uin,
                            textEntity->Text
                        );

                        DllExports->FreeMemory(entity->Entity);
                    }
                }*/

                DllExports->FreeMemory(event.Contact);
                DllExports->FreeMemory(event.Receiver);
            }
        );
    };


    void ListEventCount() {
        auto eventCounts = (NativeModel::Event::ReverseEventCount*)DllExports->GetEventCount(_contextIndex);
        if (eventCounts == nullptr) {
            return;
        }

        static NativeModel::Event::ReverseEventCount lastEventCounts{};
        if (lastEventCounts == *eventCounts) {
            // No new events.
            DllExports->FreeMemory((INT_PTR)eventCounts);
            return;
        }

        lastEventCounts = *eventCounts;

        // Login part:
        spdlog::debug("=========================================================");
        spdlog::debug(
            "[Event Count - Login] Online:{} | Login:{}",
            eventCounts->BotOnlineEventCount,
            eventCounts->BotLoginEventCount
        );
        spdlog::debug(
            "[Event Count - Login] SMS: {} | Captcha:{} | NewDeviceVerify:{} | QrCode:{} | QrCodeQuery:{} | "
            "RefreshKeyStore:{}",
            eventCounts->BotSMSEventCount,
            eventCounts->BotCaptchaEventCount,
            eventCounts->BotNewDeviceVerifyEventCount,
            eventCounts->BotQrCodeEventCount,
            eventCounts->BotQrCodeQueryEventCount,
            eventCounts->BotRefreshKeystoreEventCount
        );

        // Others:
        spdlog::debug(
            "[Event Count - Others] Log:{} | Message:{}",
            eventCounts->BotLogEventCount,
            eventCounts->BotMessageEventCount
        );

        DllExports->FreeMemory((INT_PTR)eventCounts);
    }

  private:
    ContextIndex _contextIndex{INT_MIN};

  public:
    EventDispatcher<NativeModel::Message::MessageType> _messageEventDispatcher{};
};
} // namespace LagrangeCore::EventHandler