#pragma once

#include "native_model.h"
#include "interface_wrapper.h"

#include "submodule/nlohmann/json.hpp"
#include "submodule/cpp-base64/base64.h"

using Json = nlohmann::json;

namespace LagrangeCore {

struct BotKeystoreJson {
    int64_t     Uin = 0;
    std::string Uid;
    struct BotInfoStruct {
        int         Age    = 0;
        int         Gender = 0;
        std::string Name;
    } BotInfo;
    struct WLoginSigsStruct {
        std::string A2;
        std::string A2Key;
        std::string D2;
        std::string D2Key;
        std::string A1;
        std::string A1Key;
        std::string NoPicSig;
        std::string TgtgtKey;
        std::string Ksid;
        std::string SuperKey;
        std::string StKey;
        std::string StWeb;
        std::string St;
        std::string WtSessionTicket;
        std::string WtSessionTicketKey;
        std::string RandomKey;
        std::string SKey;
        std::string PsKey;
    } WLoginSigs;
    std::string Guid;
    std::string AndroidId;
    std::string Qimei;
    std::string DeviceName;

    // Converter
  public:
    BotKeystoreJson() = default;
    BotKeystoreJson(
        _In_ NativeModel::Common::BotKeystore& keystore
    ) {
        Uin = keystore.Uin;
        Uid = keystore.Uid;

        // BotInfo Not really available.
        WLoginSigs.A2                 = keystore.A2.SerializeToBase64();
        WLoginSigs.A2Key              = keystore.A2Key.SerializeToBase64();
        WLoginSigs.D2                 = keystore.D2.SerializeToBase64();
        WLoginSigs.D2Key              = keystore.D2Key.SerializeToBase64();
        WLoginSigs.A1                 = keystore.A1.SerializeToBase64();
        WLoginSigs.A1Key              = keystore.A1Key.SerializeToBase64();
        WLoginSigs.NoPicSig           = keystore.NoPicSig.SerializeToBase64();
        WLoginSigs.TgtgtKey           = keystore.TgtgtKey.SerializeToBase64();
        WLoginSigs.Ksid               = keystore.Ksid.SerializeToBase64();
        WLoginSigs.SuperKey           = keystore.SuperKey.SerializeToBase64();
        WLoginSigs.StKey              = keystore.StKey.SerializeToBase64();
        WLoginSigs.StWeb              = keystore.StWeb.SerializeToBase64();
        WLoginSigs.St                 = keystore.St.SerializeToBase64();
        WLoginSigs.WtSessionTicket    = keystore.WtSessionTicket.SerializeToBase64();
        WLoginSigs.WtSessionTicketKey = keystore.WtSessionTicketKey.SerializeToBase64();
        WLoginSigs.RandomKey          = keystore.RandomKey.SerializeToBase64();
        WLoginSigs.SKey               = keystore.SKey.SerializeToBase64();
        WLoginSigs.PsKey              = keystore.PsKey.SerializeToBase64();

        Guid       = keystore.Guid.SerializeToBase64();
        AndroidId  = keystore.AndroidId;
        Qimei      = keystore.Qimei;
        DeviceName = keystore.DeviceName;
    }

    // JSON Operation
  public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        BotKeystoreJson::BotInfoStruct, Age, Gender, Name
    )

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        BotKeystoreJson::WLoginSigsStruct,
        A2,
        A2Key,
        D2,
        D2Key,
        A1,
        A1Key,
        NoPicSig,
        TgtgtKey,
        Ksid,
        SuperKey,
        StKey,
        StWeb,
        St,
        WtSessionTicket,
        WtSessionTicketKey,
        RandomKey,
        SKey,
        PsKey
    )

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        BotKeystoreJson, Uin, Uid, BotInfo, WLoginSigs, Guid, AndroidId, Qimei, DeviceName
    )
};

class KeystoreHandler {
  public:
    KeystoreHandler(int64_t uin);
    ~KeystoreHandler()                                 = default;
    KeystoreHandler(const KeystoreHandler&)            = delete;
    KeystoreHandler& operator=(const KeystoreHandler&) = delete;

    bool Valid() const { return _contextIndex != -1 && !_keystore; }

    auto* Get() { return Valid() ? &_keystore : RefreshKeystore(); }

    void Bind(ContextIndex);

    void PollEvent();

    NativeModel::Common::BotKeystore* RefreshKeystore();

    operator bool() { return Valid(); }

  private:
    void DumpToLocal();

  private:
    int64_t                          _botUin{0};
    NativeModel::Common::BotKeystore _keystore;
    std::wstring                     _keystoreFilePath;
    ContextIndex                     _contextIndex{-1};
};

} // namespace LagrangeCore
