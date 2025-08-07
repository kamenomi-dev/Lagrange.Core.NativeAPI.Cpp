#include "keystore_controller.h"

using namespace LagrangeCore;
using namespace LagrangeCore::NativeModel::Event;
using namespace LagrangeCore::NativeModel::Common;

namespace filesystem = std::filesystem;

KeystoreHandler::KeystoreHandler(
    int64_t uin
)
: _botUin(uin) {
    std::wstring currentPath{L"."};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.
    try {
        currentPath = std::filesystem::current_path().generic_wstring();
    } catch (...) {}

    currentPath += L"/keystores";
    filesystem::create_directory(currentPath);

    _keystoreFilePath = std::format(L"{}/{}.keystore", currentPath, uin);
}

void KeystoreHandler::Bind(
    ContextIndex index
) {
    _contextIndex = index;
}

void KeystoreHandler::PollEvent() {
    auto result = (EventArray*)DllExports->GetRefreshKeystoreEvent(_contextIndex);
    if (result == nullptr) {
        return;
    }

    for (auto idx = 0; idx < result->count; idx++) {
        const auto& currEvent = *(BotRefreshKeystoreEvent*)(result->events + idx * sizeof(BotRefreshKeystoreEvent));

        _keystore = currEvent.Keystore;
        if (_keystore.Uin != _botUin) {
            spdlog::error("[KeystoreController - {}] Unmatched uin: {}. ", _botUin, _keystore.Uin);
        }

        DumpToLocal();
        spdlog::debug("[KeystoreController - {}] Keystore updated. ", _botUin);
    }

    DllExports->FreeMemory(INT_PTR(result));
}

void KeystoreHandler::DumpToLocal() {
    std::wstring currentPath{L"."};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.

    try {
        currentPath = std::filesystem::current_path().generic_wstring();
    } catch (...) {}

    currentPath += L"/keystores";
    filesystem::create_directory(currentPath);

    _keystoreFilePath = std::format(L"{}/{}.keystore", currentPath, _keystore.Uin);

    std::ofstream jsonFile{_keystoreFilePath, std::ios::out};
    jsonFile << Json(BotKeystoreJson(_keystore)).dump(4);
    jsonFile.close();
}

BotKeystore* KeystoreHandler::RefreshKeystore() {
    if (Valid()) {
        return &_keystore;
    }

    std::ifstream jsonFile{_keystoreFilePath, std::ios::in};
    if (!jsonFile.is_open()) {
        return nullptr;
    }

    BotKeystoreJson preBotKeystore = Json::parse(jsonFile).get<BotKeystoreJson>();
    jsonFile.close();

    // This ptr will be released by the deconstructor of BotKeystore.

    using FnHandler = void*(std::string&, INT&);

    auto toBytes = [](std::string& data, int& size) {
        const auto convertedData = base64_decode(data);

        auto ptr = new char[convertedData.length()];
        std::copy(convertedData.data(), convertedData.data() + convertedData.length(), ptr);

        size = (int)convertedData.length();
        return (void*)ptr;
    };

    auto toString = [](std::string& data, int& size) {
        auto ptr = new char[data.length()];
        std::copy(data.data(), data.data() + data.length(), ptr);

        size = (int)data.length();
        return (void*)ptr;
    };

    auto convertByteArray = [](std::string& data, FnHandler handler) {
        ByteArrayNative arr;
        arr.Data = (INT_PTR)handler(data, arr.Length);

        return arr;
    };

    _keystore.Uin                = preBotKeystore.Uin;
    _keystore.Uid                = convertByteArray(preBotKeystore.Uid, toString);
    _keystore.A2                 = convertByteArray(preBotKeystore.WLoginSigs.A2, toBytes);
    _keystore.A2Key              = convertByteArray(preBotKeystore.WLoginSigs.A2Key, toBytes);
    _keystore.D2                 = convertByteArray(preBotKeystore.WLoginSigs.D2, toBytes);
    _keystore.D2Key              = convertByteArray(preBotKeystore.WLoginSigs.D2Key, toBytes);
    _keystore.A1                 = convertByteArray(preBotKeystore.WLoginSigs.A1, toBytes);
    _keystore.A1Key              = convertByteArray(preBotKeystore.WLoginSigs.A1Key, toBytes);
    _keystore.NoPicSig           = convertByteArray(preBotKeystore.WLoginSigs.NoPicSig, toBytes);
    _keystore.TgtgtKey           = convertByteArray(preBotKeystore.WLoginSigs.TgtgtKey, toBytes);
    _keystore.Ksid               = convertByteArray(preBotKeystore.WLoginSigs.Ksid, toBytes);
    _keystore.SuperKey           = convertByteArray(preBotKeystore.WLoginSigs.SuperKey, toBytes);
    _keystore.StKey              = convertByteArray(preBotKeystore.WLoginSigs.StKey, toBytes);
    _keystore.StWeb              = convertByteArray(preBotKeystore.WLoginSigs.StWeb, toBytes);
    _keystore.St                 = convertByteArray(preBotKeystore.WLoginSigs.St, toBytes);
    _keystore.WtSessionTicket    = convertByteArray(preBotKeystore.WLoginSigs.WtSessionTicket, toBytes);
    _keystore.WtSessionTicketKey = convertByteArray(preBotKeystore.WLoginSigs.WtSessionTicketKey, toBytes);
    _keystore.RandomKey          = convertByteArray(preBotKeystore.WLoginSigs.RandomKey, toBytes);
    _keystore.SKey               = convertByteArray(preBotKeystore.WLoginSigs.SKey, toBytes);
    _keystore.PsKey              = convertByteArray(preBotKeystore.WLoginSigs.PsKey, toBytes);
    _keystore.Guid               = convertByteArray(preBotKeystore.Guid, toBytes);
    _keystore.AndroidId          = convertByteArray(preBotKeystore.AndroidId, toString);
    _keystore.Qimei              = convertByteArray(preBotKeystore.Qimei, toString);
    _keystore.DeviceName         = convertByteArray(preBotKeystore.DeviceName, toString);

    return &_keystore;
}
