#pragma once
#include "wrapper.h"
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

class EventHandler {
    struct AnyContext {
      protected:
        void* _pContext{nullptr};

      public:
        AnyContext(void* context) : _pContext(context) {};

        auto& ToGroupMessage() { return *(Context::GroupMessageContext*)_pContext; };
        auto& ToPrivateMessage() { return *(Context::PrivateMessageContext*)_pContext; };
        auto& ToStrangerMessage() { return *(Context::StrangerMessageContext*)_pContext; };
    };

    template <typename Ty = void>
    using CallbackWithType = std::function<Ty(AnyContext&)>;
    template <typename Ty, typename Ret = void>
    using EventDispatcher = eventpp::EventDispatcher<Ty, CallbackWithType<Ret>>;

  protected:
    EventHandler() = default;

    void BindContext(
        ContextIndex contextIndex
    ) {
        if (_contextIndex) {
            throw "虽然但是，你写的什么代码？！";
        }
        _contextIndex = contextIndex;
    }

    void PollEvent();

    auto& RegisterMiddleware() { return *this; }

    auto& RegisterEventHandler() { return *this; }

  private:
    template <typename Target>
    void ForEachEvent(INT_PTR eventsArray, std::function<void(const Target&)> callback);

    void HandleMessageEvent() { ForEachEvent(DllExports->GetMessageEvent(_contextIndex), [](const auto& event) {
        // Handle message event
    }); };

    void HandleLogEvent() {
        auto result = (NativeModel::Event::EventArray*)DllExports->GetBotLogEvent(_contextIndex);
        if (result == nullptr) {
            return;
        }

        for (auto idx = 0; idx < result->count; idx++) {
            INT_PTR pCurrEvent = result->events + idx * sizeof(NativeModel::Event::BotLogEvent);
            auto*   currEvent  = (NativeModel::Event::BotLogEvent*)pCurrEvent;

            spdlog::debug("[Core.Log] {} - {}", _uin, currEvent->Message.ToString());
        }

        DllExports->FreeMemory((INT_PTR)result);
    }

    void HandleQRCodeEvent() {
        auto result = (NativeModel::Event::EventArray*)DllExports->GetQrCodeEvent(_contextIndex);
        if (result == nullptr) {
            return;
        }

        for (auto idx = 0; idx < result->count; idx++) {
            INT_PTR pCurrEvent = result->events + idx * sizeof(NativeModel::Event::BotQrCodeEvent);
            auto*   currEvent  = (NativeModel::Event::BotQrCodeEvent*)pCurrEvent;

            spdlog::info("Login via QRCode, url: {}", currEvent->Url.ToString());
            std::ofstream("qrcode.png", std::ios::binary) << currEvent->Image.ToString();
        }

        DllExports->FreeMemory((INT_PTR)result);
    }

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
};
} // namespace LagrangeCore::EventHandler