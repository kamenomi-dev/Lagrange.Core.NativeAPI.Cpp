#pragma once

#include "native_model.h"
#include "wrapper.h"

namespace LagrangeCore {
class KeystoreController {
  public:
    KeystoreController()  = default;
    ~KeystoreController() = default;

    KeystoreController(const KeystoreController&)            = delete;
    KeystoreController& operator=(const KeystoreController&) = delete;

  public:
    void BindContext(
        ContextIndex index
    ) {
        _contextIndex = index;
    }

    void PollBotRefreshKeystoreEvent() {
        auto result = (NativeModel::Event::EventArray*)DllExports->GetRefreshKeystoreEvent(_contextIndex);
        if (result == nullptr) {
            return;
        }

        for (auto idx = 0; idx < result->count; idx++) {
              INT_PTR pCurrEvent = result->events + idx * sizeof(NativeModel::Event::BotRefreshKeystoreEvent);
              auto*   currEvent  = (NativeModel::Event::BotRefreshKeystoreEvent*)pCurrEvent;
              NativeModel::Common::BotKeystore keystore{};
              keystore.Uin = currEvent->Keystore.Uin;
        };

        DllExports->FreeMemory(INT_PTR(result));
    }

  private:
    ContextIndex _contextIndex{-1};
};
} // namespace LagrangeCore