#include "keystore_controller.h"

using namespace LagrangeCore;
using namespace LagrangeCore::NativeModel::Event;
using namespace LagrangeCore::NativeModel::Common;

KeystoreController::KeystoreController(
    int64_t uin
) {
    _uin = uin;

    std::wstring    currentPath{};
    std::error_code err{};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.
    try {
        currentPath = std::filesystem::current_path(err).generic_wstring();
    } catch (...) {
        currentPath = L".";
    }

    if (err) {
        currentPath = L".";
    }

    currentPath += L"/keystores";

    _storedFile = std::format(L"{}/{}.keystore", currentPath, uin);
}

void KeystoreController::BindContext(
    ContextIndex index
) {
    _contextIndex = index;
}

void KeystoreController::Poll() {
    auto result = reinterpret_cast<EventArray*>(DllExports->GetRefreshKeystoreEvent(_contextIndex));
    if (result == nullptr) {
        return;
    }

    // 这里应该不会触发多次，除非多次登录什么的？
    for (auto idx = 0; idx < result->count; idx++) {
        auto currEvent =
            reinterpret_cast<BotRefreshKeystoreEvent*>(result->events + idx * sizeof(BotRefreshKeystoreEvent));
        _keystore = currEvent->Keystore;

        if (_keystore.Uin != _uin) {
            spdlog::error("[KeystoreController] Unmatched uin! {} <=!=> {}. ", _keystore.Uin, _uin);
        }

        spdlog::info("[KeystoreController] Requested to update keystore. ");
        GenerateKeystoreFile();
    }

    DllExports->FreeMemory(INT_PTR(result));
}

void KeystoreController::GenerateKeystoreFile() {
    std::wstring    currentPath{};
    std::error_code err{};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.
    try {
        currentPath = std::filesystem::current_path(err).generic_wstring();
    } catch (...) {
        currentPath = L".";
    }

    if (err) {
        currentPath = L".";
    }

    currentPath += L"/keystores";

    // currentPath = currentPath.assign(L"/keystores/").assign(std::to_wstring(_keystore.Uin)).assign(L".keystore");
    _storedFile = std::format(L"{}/{}.keystore", currentPath, _keystore.Uin);

    // Regardless of whether the folder is created successfully or not.
    std::filesystem::create_directory(currentPath);

    Json          json = PreBotKeystore(_keystore);
    std::ofstream fileOut{_storedFile, std::ios::out};
    fileOut << json.dump(4, ' ', false, nlohmann::detail::error_handler_t::ignore).c_str();
    fileOut.close();
}

BotKeystore* KeystoreController::RefreshKeystore() {

    if (Valid()) {
        return &_keystore;
    }

    std::ifstream fileIn{_storedFile, std::ios::in};
    if (!fileIn.is_open()) {
        return nullptr;
    }

    using FnHandler = void*(std::string&);

    // 这返回的 ptr 指针在 napi 会释放的，调用方不需要。
    // ByteArrayNative.cs Line28
    auto copyString = [](std::string& data) -> void* {
        auto* ptr = new char[data.size()];
        std::copy(data.c_str(), data.c_str() + data.size(), ptr);

        return ptr;
    };

    auto decodeString = [](std::string& data) -> void* {
        auto  convertedData = base64_decode(data);
        auto* ptr           = new char[convertedData.size()];
        std::copy(convertedData.c_str(), convertedData.c_str() + convertedData.size(), ptr);

        return ptr;
    };

    auto convertToByteArray = [=](std::string& data, FnHandler handler) -> ByteArrayNative {
        ByteArrayNative arr;
        arr.Length = (UINT)data.size();
        arr.Data   = (INT_PTR)handler(data);
        return arr;
    };

    PreBotKeystore preBotKeystore;
    preBotKeystore               = Json::parse(fileIn).get<PreBotKeystore>();
    _keystore.Uin                = preBotKeystore.Uin;
    _keystore.Uid                = convertToByteArray(preBotKeystore.Uid, copyString);
    _keystore.Guid               = convertToByteArray(preBotKeystore.Guid, decodeString);
    _keystore.Qimei              = convertToByteArray(preBotKeystore.Qimei, copyString);
    _keystore.AndroidId          = convertToByteArray(preBotKeystore.AndroidId, copyString);
    _keystore.DeviceName         = convertToByteArray(preBotKeystore.DeviceName, copyString);
    _keystore.A2                 = convertToByteArray(preBotKeystore.WLoginSigs.A2, decodeString);
    _keystore.A2Key              = convertToByteArray(preBotKeystore.WLoginSigs.A2Key, decodeString);
    _keystore.D2                 = convertToByteArray(preBotKeystore.WLoginSigs.D2, decodeString);
    _keystore.D2Key              = convertToByteArray(preBotKeystore.WLoginSigs.D2Key, decodeString);
    _keystore.A1                 = convertToByteArray(preBotKeystore.WLoginSigs.A1, decodeString);
    _keystore.A1Key              = convertToByteArray(preBotKeystore.WLoginSigs.A1Key, decodeString);
    _keystore.NoPicSig           = convertToByteArray(preBotKeystore.WLoginSigs.NoPicSig, decodeString);
    _keystore.TgtgtKey           = convertToByteArray(preBotKeystore.WLoginSigs.TgtgtKey, decodeString);
    _keystore.Ksid               = convertToByteArray(preBotKeystore.WLoginSigs.Ksid, decodeString);
    _keystore.SuperKey           = convertToByteArray(preBotKeystore.WLoginSigs.SuperKey, decodeString);
    _keystore.StKey              = convertToByteArray(preBotKeystore.WLoginSigs.StKey, decodeString);
    _keystore.StWeb              = convertToByteArray(preBotKeystore.WLoginSigs.StWeb, decodeString);
    _keystore.St                 = convertToByteArray(preBotKeystore.WLoginSigs.St, decodeString);
    _keystore.WtSessionTicket    = convertToByteArray(preBotKeystore.WLoginSigs.WtSessionTicket, decodeString);
    _keystore.WtSessionTicketKey = convertToByteArray(preBotKeystore.WLoginSigs.WtSessionTicketKey, decodeString);
    _keystore.RandomKey          = convertToByteArray(preBotKeystore.WLoginSigs.RandomKey, decodeString);
    _keystore.SKey               = convertToByteArray(preBotKeystore.WLoginSigs.SKey, decodeString);
    _keystore.PsKey              = convertToByteArray(preBotKeystore.WLoginSigs.PsKey, decodeString);

    fileIn.close();
    return &_keystore;
}
