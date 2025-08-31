#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <filesystem>

#include "Utils.h"
#include "Logger.h"
#include "Compatibilities.h"
#include "Definition/NativeModel.h"
#include "DllExports.h"

#include "InterimModel.h"

#include "SubModule/eventpp/eventdispatcher.h"

namespace FileSystem = std::filesystem;

namespace Lagrange {
enum class Events : unsigned char {
    Online,
    OnlineLogin,
    OnlineReconnect,
    GroupNudge,
    GroupMessage,
};
} // namespace Lagrange

namespace Lagrange::Core::Events {
namespace NativeModel  = Lagrange::Definition::NativeModel;
namespace InterimModel = Lagrange::InterimModel;

struct IEvent {
  public:
    HCONTEXT Context{NULL};
    void*    Reserved{nullptr};
};

struct Online : public IEvent {
  public:
    NativeModel::Event::BotOnlineEvent::Reasons Reason;
};

struct OnlineLogin : public IEvent {};

struct OnlineReconnect : public IEvent {};

struct GroupNudge : public IEvent {
  public:
    int64_t       Group;
    int64_t       SenderUin;
    int64_t       TargetUin;
    std::u8string Action;
    std::u8string ActionImgUrl;
    std::u8string Suffix;
};

struct GroupMessage : public IEvent, public InterimModel::Message::BotGroupMessage {};

}; // namespace Lagrange::Core::Events

namespace Lagrange::Core {

extern void Initialize() {
    Logger::Initialize();
    DllExports::Initialize();
}

extern void Uninitialize() {
    Logger::Uninitialize();
}

class MessageBuilder {
  public:
    MessageBuilder(HCONTEXT botContext)
    : _botContext(botContext),
      _instanceContext(DllExports::CreateMessageBuilder(botContext)) {};

    MessageBuilder& AddText(
        const std::u8string text
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{text};
        DllExports::AddText(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddImage(
        void* imageData, size_t imageSize, INT subType
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{imageSize, imageData};
        DllExports::AddImage(_botContext, _instanceContext, data, {}, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddImage(
        void* imageData, size_t imageSize, const std::u8string summary, INT subType
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data1{imageSize, imageData};
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data2{summary};
        DllExports::AddImage(_botContext, _instanceContext, data1, summary, subType);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalImage(
        const std::u8string path, INT subType = 0
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddImage(_botContext, _instanceContext, data, {}, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalImage(
        const std::u8string path, const std::u8string summary, INT subType = 0
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data1{path};
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data2{summary};
        DllExports::AddImage(_botContext, _instanceContext, data1, data2, subType);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    MessageBuilder& AddMention(
        CSharp_Int64 uin, std::u8string display = u8""
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data1{display};
        DllExports::AddMention(_botContext, _instanceContext, uin, data1);
        data1.TryRelease();

        return *this;
    }

    MessageBuilder& AddRecord(
        void* recordData, size_t recordSize
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{recordSize, recordData};
        DllExports::AddRecord(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalRecord(
        const std::u8string path
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddRecord(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddVideo(
        void* videoData, size_t videoSize
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{videoSize, videoData};
        DllExports::AddVideo(_botContext, _instanceContext, data, {});
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddVideo(
        void* videoData, size_t videoSize, void* thumbnailData, size_t thumbnailSize
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data1{videoSize, videoData};
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data2{thumbnailSize, thumbnailData};
        DllExports::AddVideo(_botContext, _instanceContext, data1, data2);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalVideo(
        const std::u8string path
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddLocalVideo(_botContext, _instanceContext, data, {});
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalVideo(
        const std::u8string path, const std::u8string thumbnailPath
    ) {
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data1{path};
        Lagrange::Definition::NativeModel::Common::ByteArrayNative data2{thumbnailPath};
        DllExports::AddLocalVideo(_botContext, _instanceContext, data1, data2);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    void SendToFriendNoReturn(
        CSharp_Int64 userId
    ) const {
        DllExports::FreeMemory(DllExports::SendFriendMessage(_botContext, _instanceContext, userId));
    }

    void SendToGroupNoReturn(
        CSharp_Int64 userId
    ) const {
        DllExports::FreeMemory(DllExports::SendGroupMessage(_botContext, _instanceContext, userId));
    }

    Lagrange::Definition::NativeModel::Message::BotMessage* SendToFriend(
        CSharp_Int64 userId
    ) const {
        return DllExports::SendFriendMessage(_botContext, _instanceContext, userId);
    }

    Lagrange::Definition::NativeModel::Message::BotMessage* SendToGroup(
        CSharp_Int64 userId
    ) const {
        return DllExports::SendGroupMessage(_botContext, _instanceContext, userId);
    }

  private:
    HCONTEXT _botContext{NULL};
    HCONTEXT _instanceContext{NULL};
};

template <
    typename Type,
    typename = typename std::enable_if_t<std::is_base_of_v<Lagrange::Definition::NativeModel::Event::IEvent, Type>>>
class EventArray {
  public:
    EventArray(
        void* pEventArray
    ) {
        auto* eventArray = (Lagrange::Definition::NativeModel::Event::EventArray*)pEventArray;

        for (size_t index = 0; index < eventArray->Count; index++) {
            _events.push_back((Type*)(eventArray->Events) + index);
        }

        DllExports::FreeMemory(pEventArray);
    }

    ~EventArray() {
        if (_events.empty()) {
            return;
        }

        DllExports::FreeMemory(_events[0]);
    }

    std::vector<Type*>& Get() { return _events; }

  private:
    std::vector<Type*> _events;
};

class Keystore {
  public:
    Keystore(
        LONG uin
    ) {
        _keystorePath = _keystorePath.append(std::to_string(uin)).replace_extension(".keystore");
        ReadKeystore();
    }

    bool Empty() const { return _keystore.Empty(); }

    int64_t GetUin() const { return _keystore.Uin; }

    void BindBotContext(
        HCONTEXT context
    ) {
        _botContext = context;
    }

    void PollEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotRefreshKeystoreEvent> events{
            DllExports::GetRefreshKeystoreEvent(_botContext)
        };

        for (auto* pEvent : events.Get()) {
            _keystore = pEvent->keystore;
            DumpKeystore();
        }
    }

    void ReadKeystore() { InterimModel::Common::BotKeystoreParser::Parse(_keystorePath.string(), _keystore); }

    void DumpKeystore() {
        if (!FileSystem::exists(_keystorePath.parent_path())) {
            FileSystem::create_directories(_keystorePath.parent_path());
        }

        InterimModel::Common::BotKeystoreParser::Dump(_keystorePath.string(), _keystore);
    }

    operator Lagrange::Definition::NativeModel::Common::BotKeystore*() {
        return _keystore.Empty() ? nullptr : &_keystore;
    }

  private:
    HCONTEXT                                               _botContext{NULL};
    Lagrange::Definition::NativeModel::Common::BotKeystore _keystore{};
    FileSystem::path                                       _keystorePath = FileSystem::absolute("./Keystores/");
};

class EventHandler {

    using EventDispatcher = eventpp::EventDispatcher<Lagrange::Events, void(const Events::IEvent*)>;

  public:
    EventHandler() {};

    void BindContext(
        HCONTEXT context
    ) {
        _context = context;
        _logger  = Logger::Get("Context-" + std::to_string(context));
    }

    auto On(
        const Lagrange::Events ev, std::function<void(const Lagrange::Core::Events::IEvent*)> cb
    ) {
        return _eventEmitter.appendListener(ev, cb);
    }

    void Remove(
        const Lagrange::Events ev, const EventDispatcher::Handle& cb
    ) {
        _eventEmitter.removeListener(ev, cb);
    }

    void PollEvent() {
        PollBotFriendRequestEvent();
        PollBotGroupReactionEvent();
        PollBotLogEvent();
        PollCaptchaEvent();
        PollGroupInviteNotificationEvent();
        PollGroupJoinNotificationEvent();
        PollGroupMemberDecreaseEvent();
        PollGroupNudgeEvent();
        PollLoginEvent();
        PollMessageEvent();
        PollNewDeviceVerifyEvent();
        PollOnlineEvent();
        PollQrCodeEvent();
        PollQrCodeQueryEvent();
        GetSMSEvent();
    }

  private:
    void PollBotFriendRequestEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotFriendRequestEvent> events{
            DllExports::GetBotFriendRequestEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info(
                "A stranger({}) sent a request to you with message: {}",
                event->InitiatorUin,
                (std::string)event->Message
            );
        }
    };

    void PollBotGroupReactionEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotGroupReactionEvent> events{
            DllExports::GetBotGroupReactionEvent(_context)
        };

        for (auto* event : events.Get()) {
            // _logger->info("Someone({}) sent a reaction to the another({})", pEvent->);
            // 不想写这个回应，太懒了
        }
    };

    void PollBotLogEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotLogEvent> events{DllExports::GetBotLogEvent(_context)};
        using LogLevel = Lagrange::Definition::NativeModel::Event::BotLogEvent::LogLevel;

        for (auto* event : events.Get()) {
            // It is not a ANSI string, but UTF-8 string
            const std::string tag     = event->Tag;
            const std::string message = event->Message;

            switch (event->Level) {
            case LogLevel::Trace:
                _logger->trace("[{}] - {}", tag, message);
                break;
            case LogLevel::Debug:
                _logger->debug("[{}] - {}", tag, message);
                break;
            case LogLevel::Information:
                _logger->info("[{}] - {}", tag, message);
                break;
            case LogLevel::Warning:
                _logger->warn("[{}] - {}", tag, message);
                break;
            case LogLevel::Error:
                _logger->error("[{}] - {}", tag, message);
                break;
            case LogLevel::Critical:
                _logger->critical("[{}] - {}", tag, message);
                break;
            default:
                _logger->info("[{}] [{}] - {}", tag, event->LogLevelToEnumTag(), message);
                break;
            }
        }
    };

    void PollCaptchaEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotCaptchaEvent> events{
            DllExports::GetCaptchaEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info("QQ has sent a Captcha verification to you, Url: {}", (std::string)event->CaptchaUrl);
        }
    };

    void PollEventCount() { throw new std::runtime_error("Not necessary to implement this function. "); };

    void PollGroupInviteNotificationEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotGroupInviteNotificationEvent> events{
            DllExports::GetGroupInviteNotificationEvent(_context)
        };

        for (auto* event : events.Get()) {}
    };

    void PollGroupJoinNotificationEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotGroupJoinNotificationEvent> events{
            DllExports::GetGroupJoinNotificationEvent(_context)
        };

        for (auto* event : events.Get()) {}
    };

    void PollGroupMemberDecreaseEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotGroupMemberDecreaseEvent> events{
            DllExports::GetGroupMemberDecreaseEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info(
                "Someone({}) has been left by Operator({}) in {}", event->UserUin, event->OperatorUin, event->GroupUin
            );
        }
    };

    void PollGroupNudgeEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotGroupNudgeEvent> events{
            DllExports::GetGroupNudgeEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info(
                "{} {} {} {} in {}",
                event->OperatorUin,
                (std::string)event->Action,
                event->TargetUin,
                (std::string)event->Suffix,
                event->GroupUin
            );
            Events::GroupNudge nudge{
                _context, event, event->GroupUin, event->OperatorUin, event->TargetUin, event->Action, event->ActionImgUrl, event->Suffix
            };
            _eventEmitter.dispatch(Lagrange::Events::GroupNudge, &nudge);
        }
    };

    void PollLoginEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotLoginEvent> events{DllExports::GetLoginEvent(_context)};

        for (auto* event : events.Get()) {
            const std::string tag     = event->Tag;
            const std::string message = event->Message;

            if (event->IsSuccessful()) {
                _logger->info("[{}] [Code: 0] - {}", tag, message);
                continue;
            }

            _logger->error("[{}] [Code: {}] - {}", tag, event->State, message);
            _logger->error("It seems like login unsuccessfully; Deleting keystore maybe fine. ");
        }
    };

    void PollMessageEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotMessageEvent> events{
            DllExports::GetMessageEvent(_context)
        };

        for (auto* event : events.Get()) {
            if (event->Message.Type == Definition::NativeModel::Message::MessageType::Group) {
                InterimModel::Message::BotGroupMessage message{event->Message};
                _logger->info("{} Sent a message in {}", message.Sender.Uin, message.Group.GroupUin);

                // Oh it's stupid.
                Events::GroupMessage groupMessage{_context, event, message};
                message.Messages.GetEntities().clear();
                message.Messages.GetEntities().resize(0);
                _eventEmitter.dispatch(Lagrange::Events::GroupMessage, &groupMessage);
            }
        }
    }

    void PollNewDeviceVerifyEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotNewDeviceVerifyEvent> events{
            DllExports::GetNewDeviceVerifyEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info("Received new-device verification with Url: {}", (std::string)event->Url);
        }
    };

    void PollOnlineEvent() {
        using Reasons = Definition::NativeModel::Event::BotOnlineEvent::Reasons;

        EventArray<Definition::NativeModel::Event::BotOnlineEvent> events{DllExports::GetOnlineEvent(_context)};

        for (auto* event : events.Get()) {
            Events::Online online{_context, event, event->Reason};
            _eventEmitter.dispatch(Lagrange::Events::Online, &online);

            if (event->Reason == Reasons::Login) {
                _logger->info("Bot successfully login ed! ");

                Events::OnlineLogin onlineLogin{_context, event};
                _eventEmitter.dispatch(Lagrange::Events::OnlineLogin, &onlineLogin);
            }

            if (event->Reason == Reasons::Reconnect) {
                _logger->info("Bot successfully reconnected! ");

                Events::OnlineReconnect onlineReconnect{_context, event};
                _eventEmitter.dispatch(Lagrange::Events::OnlineReconnect, &onlineReconnect);
            }
        }
    };

    void PollQrCodeEvent() {
        EventArray<Lagrange::Definition::NativeModel::Event::BotQrCodeEvent> events{DllExports::GetQrCodeEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info("QRCode Verification received! Url: ", (const char*)event->Url.ToUTF8String().c_str());

            std::error_code erro{};
            auto            path = FileSystem::absolute("./QRCode.png", erro).generic_string();
            if (erro) {
                path = "./QRCode.png";
            }

            std::ofstream qrCodeFile{path, std::ios::binary};
            qrCodeFile.write((const char*)event->Image.Data, event->Image.Length);
            qrCodeFile.close();
            qrCodeFile.clear();

            _logger->info("QRCode was output to {}.", path);
        }
    }

    void PollQrCodeQueryEvent() {
        EventArray<Definition::NativeModel::Event::BotQrCodeQueryEvent> events{DllExports::GetQrCodeQueryEvent(_context)
        };

        for (auto* event : events.Get()) {
            _logger->info(
                "QRCode queried state: {}, origin: {}", event->TransEmpStateToDetail(), event->TransEmpStateToEnumTag()
            );
        }
    };

    void GetRefreshKeystoreEvent() { throw new std::runtime_error("Not call this function, a placeholder. "); };

    void GetSMSEvent() {
        EventArray<Definition::NativeModel::Event::BotSMSEvent> events{DllExports::GetSMSEvent(_context)};

        for (auto* event : events.Get()) {
            _logger->info(
                "QQ has sent a SMS verification to phone({}), Url: {}",
                (std::string)event->Phone,
                (std::string)event->Url
            );
        }
    };

  private:
    HCONTEXT                        _context{NULL};
    std::shared_ptr<spdlog::logger> _logger;
    EventDispatcher                 _eventEmitter{};
};
} // namespace Lagrange::Core

#undef NATIVE_MODEL