#include "keystore_controller.h"

using namespace LagrangeCore;
using namespace LagrangeCore::NativeModel::Event;
using namespace LagrangeCore::NativeModel::Common;

namespace filesystem = std::filesystem;

KeystoreController::KeystoreController(
    int64_t uin
) {
    _uin = uin;

    std::wstring    currentPath{L"."};
    std::error_code err{};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.
    try {
        currentPath = std::filesystem::current_path(err).generic_wstring();
    } catch (...) {
    }

    currentPath += L"/keystores";
    filesystem::create_directory(currentPath);

    _storedFile = std::format(L"{}/{}.keystore", currentPath, uin);
}

void KeystoreController::BindContext(
    ContextIndex index
) {
    _contextIndex = index;
}

void KeystoreController::Poll() {
    auto result = (EventArray*)DllExports->GetRefreshKeystoreEvent(_contextIndex);
    if (result == nullptr) {
        return;
    }

    // 这里应该不会触发多次，除非多次登录什么的？
    for (auto idx = 0; idx < result->count; idx++) {
        const auto currEvent = (BotRefreshKeystoreEvent*)(result->events + idx * sizeof(BotRefreshKeystoreEvent));

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
    std::wstring    currentPath{L"."};
    std::error_code err{};

    // https://en.cppreference.com/w/cpp/filesystem/current_path.html#Exceptions
    // It may cause to throw std::bad_alloc.
    try {
        currentPath = std::filesystem::current_path(err).generic_wstring();
    } catch (...) {
    }

    currentPath += L"/keystores";
    filesystem::create_directory(currentPath);

    _storedFile = std::format(L"{}/{}.keystore", currentPath, _keystore.Uin);

    std::ofstream fileOut{_storedFile, std::ios::out};
    fileOut << Json(PreBotKeystore(_keystore)).dump(4);
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

    // This ptr will be released by the deconstructor of BotKeystore.

    const auto toString = [](std::string& data) -> void* {
        auto* ptr = new char[data.length()];
        std::copy(data.data(), data.data() + data.length(), ptr);

        return ptr;
    };

    const auto toBytes = [](std::string& data) -> void* {
        auto  convertedData = base64_decode(data);
        auto* ptr           = new char[convertedData.length()];
        std::copy(convertedData.data(), convertedData.data() + convertedData.length(), ptr);

        return ptr;
    };

    using FnHandler = void*(std::string&);

    auto convertByteArray = [=](std::string& data, FnHandler handler) -> ByteArrayNative {
        ByteArrayNative arr;
        arr.Length = (UINT)data.size();
        arr.Data   = (INT_PTR)handler(data);
        return arr;
    };

    PreBotKeystore preBotKeystore;
    preBotKeystore               = Json::parse(fileIn).get<PreBotKeystore>();
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

    fileIn.close();
    return &_keystore;
}
