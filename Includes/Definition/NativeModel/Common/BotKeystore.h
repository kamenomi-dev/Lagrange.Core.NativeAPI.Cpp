#pragma once

namespace Lagrange::Definition::NativeModel::Common {

struct BotKeystore {
    CSharp_Int64         Uin = 0;
    ByteArrayNative     Uid{};
    ByteArrayNative     Guid{};
    ByteArrayNative     AndroidId{};
    ByteArrayNative     Qimei{};
    ByteArrayNative     DeviceName{};
    ByteArrayNative     A2{};
    ByteArrayNative     A2Key{};
    ByteArrayNative     D2{};
    ByteArrayNative     D2Key{};
    ByteArrayNative     A1{};
    ByteArrayNative     A1Key{};
    ByteArrayNative     NoPicSig{};
    ByteArrayNative     TgtgtKey{};
    ByteArrayNative     Ksid{};
    ByteArrayNative     SuperKey{};
    ByteArrayNative     StKey{};
    ByteArrayNative     StWeb{};
    ByteArrayNative     St{};
    ByteArrayNative     WtSessionTicket{};
    ByteArrayNative     WtSessionTicketKey{};
    ByteArrayNative     RandomKey{};
    ByteArrayNative     SKey{};
    ByteArrayDictNative PsKey{};

  public:
    ~BotKeystore() { Clear(); }

    void Clear() {
        Uin = NULL;
        ZeroMemory((void*)Uid.Data, Uid.Length);
        ZeroMemory((void*)Guid.Data, Guid.Length);
        ZeroMemory((void*)AndroidId.Data, AndroidId.Length);
        ZeroMemory((void*)Qimei.Data, Qimei.Length);
        ZeroMemory((void*)DeviceName.Data, DeviceName.Length);
        ZeroMemory((void*)A2.Data, A2.Length);
        ZeroMemory((void*)A2Key.Data, A2Key.Length);
        ZeroMemory((void*)D2.Data, D2.Length);
        ZeroMemory((void*)D2Key.Data, D2Key.Length);
        ZeroMemory((void*)A1.Data, A1.Length);
        ZeroMemory((void*)A1Key.Data, A1Key.Length);
        ZeroMemory((void*)NoPicSig.Data, NoPicSig.Length);
        ZeroMemory((void*)TgtgtKey.Data, TgtgtKey.Length);
        ZeroMemory((void*)Ksid.Data, Ksid.Length);
        ZeroMemory((void*)SuperKey.Data, SuperKey.Length);
        ZeroMemory((void*)StKey.Data, StKey.Length);
        ZeroMemory((void*)StWeb.Data, StWeb.Length);
        ZeroMemory((void*)St.Data, St.Length);
        ZeroMemory((void*)WtSessionTicket.Data, WtSessionTicket.Length);
        ZeroMemory((void*)WtSessionTicketKey.Data, WtSessionTicketKey.Length);
        ZeroMemory((void*)RandomKey.Data, RandomKey.Length);
        ZeroMemory((void*)SKey.Data, SKey.Length);
        ZeroMemory((void*)PsKey.Data, PsKey.Length);
    }

    bool Empty() const {
        return Uin == 0 && Uid.Data == NULL && Guid.Data == NULL && AndroidId.Data == NULL && Qimei.Data == NULL
            && DeviceName.Data == NULL && A2.Data == NULL && A2Key.Data == NULL && D2.Data == NULL && D2Key.Data == NULL
            && A1.Data == NULL && A1Key.Data == NULL && NoPicSig.Data == NULL && TgtgtKey.Data == NULL
            && StKey.Data == NULL && StWeb.Data == NULL && St.Data == NULL && WtSessionTicket.Data == NULL
            && WtSessionTicketKey.Data == NULL && RandomKey.Data == NULL && SKey.Data == NULL;
    }
};
} // namespace Lagrange::Definition::NativeModel::Common