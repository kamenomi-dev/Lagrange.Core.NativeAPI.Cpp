#pragma once

#include "native_model.h"
#include "wrapper.h"

#include "submodule/nlohmann/json.hpp"

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
        WLoginSigs.A2                 = keystore.A2.ToString();
        WLoginSigs.A2Key              = keystore.A2Key.ToString();
        WLoginSigs.D2                 = keystore.D2.ToString();
        WLoginSigs.D2Key              = keystore.D2Key.ToString();
        WLoginSigs.A1                 = keystore.A1.ToString();
        WLoginSigs.A1Key              = keystore.A1Key.ToString();
        WLoginSigs.NoPicSig           = keystore.NoPicSig.ToString();
        WLoginSigs.TgtgtKey           = keystore.TgtgtKey.ToString();
        WLoginSigs.Ksid               = keystore.Ksid.ToString();
        WLoginSigs.SuperKey           = keystore.SuperKey.ToString();
        WLoginSigs.StKey              = keystore.StKey.ToString();
        WLoginSigs.StWeb              = keystore.StWeb.ToString();
        WLoginSigs.St                 = keystore.St.ToString();
        WLoginSigs.WtSessionTicket    = keystore.WtSessionTicket.ToString();
        WLoginSigs.WtSessionTicketKey = keystore.WtSessionTicketKey.ToString();
        WLoginSigs.RandomKey          = keystore.RandomKey.ToString();
        WLoginSigs.SKey               = keystore.SKey.ToString();
        WLoginSigs.PsKey              = keystore.PsKey.ToString();

        Guid       = keystore.Guid.ToString();
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
