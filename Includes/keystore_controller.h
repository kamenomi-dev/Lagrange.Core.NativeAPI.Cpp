#pragma once

#include "native_model.h"
#include "wrapper.h"

#include "submodule/nlohmann/json.hpp"
#include "submodule/cpp-base64/base64.h"

using Json = nlohmann::json;

namespace LagrangeCore {

struct PreBotKeystore {
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
    PreBotKeystore() = default;
    PreBotKeystore(
        _In_ NativeModel::Common::BotKeystore& keystore
    ) {
        Uin = keystore.Uin;
        Uid = keystore.Uid.ToString();

        // BotInfo Not really available.
        WLoginSigs.A2                 = keystore.A2.ToBase64();
        WLoginSigs.A2Key              = keystore.A2Key.ToBase64();
        WLoginSigs.D2                 = keystore.D2.ToBase64();
        WLoginSigs.D2Key              = keystore.D2Key.ToBase64();
        WLoginSigs.A1                 = keystore.A1.ToBase64();
        WLoginSigs.A1Key              = keystore.A1Key.ToBase64();
        WLoginSigs.NoPicSig           = keystore.NoPicSig.ToBase64();
        WLoginSigs.TgtgtKey           = keystore.TgtgtKey.ToBase64();
        WLoginSigs.Ksid               = keystore.Ksid.ToBase64();
        WLoginSigs.SuperKey           = keystore.SuperKey.ToBase64();
        WLoginSigs.StKey              = keystore.StKey.ToBase64();
        WLoginSigs.StWeb              = keystore.StWeb.ToBase64();
        WLoginSigs.St                 = keystore.St.ToBase64();
        WLoginSigs.WtSessionTicket    = keystore.WtSessionTicket.ToBase64();
        WLoginSigs.WtSessionTicketKey = keystore.WtSessionTicketKey.ToBase64();
        WLoginSigs.RandomKey          = keystore.RandomKey.ToBase64();
        WLoginSigs.SKey               = keystore.SKey.ToBase64();
        WLoginSigs.PsKey              = keystore.PsKey.ToBase64();

        Guid       = keystore.Guid.ToBase64();
        AndroidId  = keystore.AndroidId.ToString();
        Qimei      = keystore.Qimei.ToString();
        DeviceName = keystore.DeviceName.ToString();
    }

    // JSON Operation
  public:
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        PreBotKeystore::BotInfoStruct, Age, Gender, Name
    )

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        PreBotKeystore::WLoginSigsStruct, A2, A2Key, D2, D2Key, A1, A1Key, NoPicSig, TgtgtKey, Ksid, SuperKey, StKey,
        StWeb, St, WtSessionTicket, WtSessionTicketKey, RandomKey, SKey, PsKey
    )

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        PreBotKeystore, Uin, Uid, BotInfo, WLoginSigs, Guid, AndroidId, Qimei, DeviceName
    )
};

class KeystoreController {
  public:
    KeystoreController(int64_t uin);
    ~KeystoreController()                                    = default;
    KeystoreController(const KeystoreController&)            = delete;
    KeystoreController& operator=(const KeystoreController&) = delete;

    bool Valid() const { return _contextIndex != -1 && !_keystore.Empty(); }

    NativeModel::Common::BotKeystore* Get() { return Valid() ? &_keystore : RefreshKeystore(); }

    void BindContext(ContextIndex);

    void Poll();

    void GenerateKeystoreFile();

    NativeModel::Common::BotKeystore* RefreshKeystore();

  private:
    int64_t                          _uin{0};
    NativeModel::Common::BotKeystore _keystore;
    std::wstring                     _storedFile;
    ContextIndex                     _contextIndex{-1};
};

} // namespace LagrangeCore
