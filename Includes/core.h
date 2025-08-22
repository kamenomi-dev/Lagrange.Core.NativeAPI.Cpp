#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "Utils.h"
#include "Logger.h"
#include "Compatibilities.h"
#include "Definition/NativeModel.h"
#include "Definition/InterimModel.h"
#include "DllExports.h"

namespace FileSystem = std::filesystem;

namespace Lagrange::Core::Event {
template <
    typename Type,
    typename = typename std::enable_if_t<std::is_base_of_v<Definition::NativeModel::Event::IEvent, Type>>>
struct EventArray {
  public:
    EventArray(
        CSharp_IntPtr pEventArray
    ) {
        auto* eventArray = (Definition::NativeModel::Event::EventArray*)pEventArray;

        for (size_t index = 0; index < eventArray->Count; index++) {
            _events.push_back((Type*)(eventArray->Events + index * sizeof(Type)));
        }

        DllExports::FreeMemory(pEventArray);
    }

    ~EventArray() {
        for (auto& event : _events) {
            DllExports::FreeMemory((CSharp_IntPtr)event);
        }
    }

    std::vector<Type*>& Get() { return _events; }

  private:
    std::vector<Type*> _events;
};
} // namespace Lagrange::Core::Event

namespace Lagrange::Core {
extern void Initialize() {
    Logger::Initialize();
    DllExports::Initialize();
}

extern void Uninitialize() {
    Logger::Uninitialize();
}

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
        Event::EventArray<Definition::NativeModel::Event::BotRefreshKeystoreEvent> events{
            DllExports::GetRefreshKeystoreEvent(_botContext)
        };

        for (auto* pEvent : events.Get()) {
            _keystore = pEvent->keystore;
            DumpKeystore();
        }
    }

    void ReadKeystore() {
        Definition::InterimModel::Common::BotKeystoreParser::Parse(_keystorePath.string(), _keystore);
    }

    void DumpKeystore() {
        if (!FileSystem::exists(_keystorePath.parent_path())) {
            FileSystem::create_directories(_keystorePath.parent_path());
        }

        Definition::InterimModel::Common::BotKeystoreParser::Dump(_keystorePath.string(), _keystore);
    }

    operator Definition::NativeModel::Common::BotKeystore*() { return _keystore.Empty() ? nullptr : &_keystore; }

  private:
    HCONTEXT                                     _botContext{NULL};
    Definition::NativeModel::Common::BotKeystore _keystore{};
    FileSystem::path                             _keystorePath = FileSystem::absolute("./Keystores/");
};

class EventHandler {
  public:
    EventHandler() {};

    void BindContext(
        HCONTEXT context
    ) {
        _context = context;
        _logger  = Logger::Get("Context-" + std::to_string(context));
    }

    void PollEvent() {
        PollQrCodeEvent();
        PollMessageEvent();
    }

  private:
    void PollQrCodeEvent() {
        Event::EventArray<Definition::NativeModel::Event::BotQrCodeEvent> events{DllExports::GetQrCodeEvent(_context)};

        for (auto* pEvent : events.Get()) {
            _logger->info("QRCode Verification received! Url: ", pEvent->Url.ToString());
            std::ofstream qrCodeFile{"./QRCode.png", std::ios::binary};
            qrCodeFile.write((const char*)pEvent->Image.Data, pEvent->Image.Length);
            qrCodeFile.close();
            qrCodeFile.clear();
        }
    }

    void PollMessageEvent() {
        Event::EventArray<Definition::NativeModel::Event::BotMessageEvent> events{DllExports::GetMessageEvent(_context)
        };

        for (auto* pEvent : events.Get()) {
            _logger->info(
                "New Message Event: From {} in {}, Content: Not implemented",
                pEvent->Message.Contact,
                pEvent->Message.Group.GroupUin
            );
        }
    }

  private:
    HCONTEXT                        _context{NULL};
    std::shared_ptr<spdlog::logger> _logger;
};

class MessageBuilder {
  public:
    MessageBuilder(HCONTEXT botContext)
    : _botContext(botContext),
      _instanceContext(DllExports::CreateMessageBuilder(botContext)) {};

    MessageBuilder& AddText(
        const std::string text
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{text};
        DllExports::AddText(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddImage(
        void* imageData, size_t imageSize, INT subType
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{imageSize, imageData};
        DllExports::AddImage(_botContext, _instanceContext, data, {}, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddImage(
        void* imageData, size_t imageSize, std::string summary, INT subType
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{imageSize, imageData};
        DllExports::AddImage(_botContext, _instanceContext, data, summary, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalImage(
        const std::string path, INT subType = 0
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddImage(_botContext, _instanceContext, data, {}, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalImage(
        const std::string path, const std::string summary, INT subType = 0
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddImage(_botContext, _instanceContext, data, summary, subType);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddRecord(
        void* recordData, size_t recordSize
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{recordSize, recordData};
        DllExports::AddRecord(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalRecord(
        const std::string path
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddRecord(_botContext, _instanceContext, data);
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddVideo(
        void* videoData, size_t videoSize
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{videoSize, videoData};
        DllExports::AddVideo(_botContext, _instanceContext, data, {});
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddVideo(
        void* videoData, size_t videoSize, void* thumbnailData, size_t thumbnailSize
    ) {
        Definition::NativeModel::Common::ByteArrayNative data1{videoSize, videoData};
        Definition::NativeModel::Common::ByteArrayNative data2{thumbnailSize, thumbnailData};
        DllExports::AddVideo(_botContext, _instanceContext, data1, data2);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalVideo(
        const std::string path
    ) {
        Definition::NativeModel::Common::ByteArrayNative data{path};
        DllExports::AddLocalVideo(_botContext, _instanceContext, data, {});
        data.TryRelease();

        return *this;
    }

    MessageBuilder& AddLocalVideo(
        const std::string path, const std::string thumbnailPath
    ) {
        Definition::NativeModel::Common::ByteArrayNative data1{path};
        Definition::NativeModel::Common::ByteArrayNative data2{thumbnailPath};
        DllExports::AddLocalVideo(_botContext, _instanceContext, data1, data2);
        data1.TryRelease();
        data2.TryRelease();

        return *this;
    }

    CSharp_IntPtr SendToFriend(
        LONG userId
    ) const {
        return DllExports::SendFriendMessage(_botContext, _instanceContext, userId);
    }

    CSharp_IntPtr SendToGroup(
        LONG userId
    ) const {
        return DllExports::SendGroupMessage(_botContext, _instanceContext, userId);
    }

  private:
    HCONTEXT _botContext{NULL};
    HCONTEXT _instanceContext{NULL};
};

} // namespace Lagrange::Core