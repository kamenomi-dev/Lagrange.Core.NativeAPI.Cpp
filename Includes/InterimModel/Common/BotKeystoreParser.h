#pragma once
#include <concepts>
#include <filesystem>

namespace FileSystem = std::filesystem;

#define ReadAsByteArrayNative(helper, property)                                                                        \
    property.Length = helper.ReadAsInt32();                                                                            \
    property.Data   = (CSharp_IntPtr)helper.__Unsafe_ReadAsBinaryData(property.Length);

#define WriteAsByteArrayNative(helper, property)                                                                       \
    helper.WriteAsInt32(property.Length);                                                                              \
    helper.__Unsafe_WriteAsBinaryData((const void*)property.Data, property.Length);

namespace Lagrange::InterimModel::Common {
struct BotKeystoreParser {
    static void Parse(
        const std::string file, NATIVE_MODEL Common::BotKeystore& result
    ) {
        Utils::FileBinaryHelper helper{file};
        if (!FileSystem::exists(file) || FileSystem::is_empty(file)) {
            return;
        }

        try {
            result.Uin = helper.ReadAsInt64();
            ReadAsByteArrayNative(helper, result.Uid);

            // Todo: Check BotInfo exists ???

            // WLoginSigs
            ReadAsByteArrayNative(helper, result.A2);
            ReadAsByteArrayNative(helper, result.A2Key);
            ReadAsByteArrayNative(helper, result.D2);
            ReadAsByteArrayNative(helper, result.D2Key);
            ReadAsByteArrayNative(helper, result.A1);
            ReadAsByteArrayNative(helper, result.A1Key);
            ReadAsByteArrayNative(helper, result.NoPicSig);
            ReadAsByteArrayNative(helper, result.TgtgtKey);
            ReadAsByteArrayNative(helper, result.Ksid);
            ReadAsByteArrayNative(helper, result.SuperKey);
            ReadAsByteArrayNative(helper, result.StKey);
            ReadAsByteArrayNative(helper, result.StWeb);
            ReadAsByteArrayNative(helper, result.St);
            ReadAsByteArrayNative(helper, result.WtSessionTicket);
            ReadAsByteArrayNative(helper, result.WtSessionTicketKey);
            ReadAsByteArrayNative(helper, result.RandomKey);
            ReadAsByteArrayNative(helper, result.SKey);
            ReadAsByteArrayNative(helper, result.PsKey);

            ReadAsByteArrayNative(helper, result.Guid);
            ReadAsByteArrayNative(helper, result.AndroidId);
            ReadAsByteArrayNative(helper, result.Qimei);
            ReadAsByteArrayNative(helper, result.DeviceName);
        } catch (std::exception& error) {
            result = NATIVE_MODEL Common::BotKeystore();
            Logger::error("Cannot parse keystore file correctly. It seems file has been damaged. ");
            Logger::error(error.what());
        }
    }

    static void Dump(
        const std::string file, NATIVE_MODEL Common::BotKeystore out
    ) {
        Utils::FileBinaryHelper helper{file};
        try {
            helper.WriteAsInt64(out.Uin);
            WriteAsByteArrayNative(helper, out.Uid);

            // Todo: Check BotInfo exists ???

            // WLoginSigs
            WriteAsByteArrayNative(helper, out.A2);
            WriteAsByteArrayNative(helper, out.A2Key);
            WriteAsByteArrayNative(helper, out.D2);
            WriteAsByteArrayNative(helper, out.D2Key);
            WriteAsByteArrayNative(helper, out.A1);
            WriteAsByteArrayNative(helper, out.A1Key);
            WriteAsByteArrayNative(helper, out.NoPicSig);
            WriteAsByteArrayNative(helper, out.TgtgtKey);
            WriteAsByteArrayNative(helper, out.Ksid);
            WriteAsByteArrayNative(helper, out.SuperKey);
            WriteAsByteArrayNative(helper, out.StKey);
            WriteAsByteArrayNative(helper, out.StWeb);
            WriteAsByteArrayNative(helper, out.St);
            WriteAsByteArrayNative(helper, out.WtSessionTicket);
            WriteAsByteArrayNative(helper, out.WtSessionTicketKey);
            WriteAsByteArrayNative(helper, out.RandomKey);
            WriteAsByteArrayNative(helper, out.SKey);
            WriteAsByteArrayNative(helper, out.PsKey);

            WriteAsByteArrayNative(helper, out.Guid);
            WriteAsByteArrayNative(helper, out.AndroidId);
            WriteAsByteArrayNative(helper, out.Qimei);
            WriteAsByteArrayNative(helper, out.DeviceName);
        } catch (std::exception& error) {
            Logger::error("Cannot dump bot({})'s keystore file correctly. It seems file has been damaged. ", out.Uin);
            Logger::error(error.what());
        }
    }
};
} // namespace Lagrange::InterimModel::Common

#undef ReadAsByteArrayNative
#undef WriteAsByteArrayNative