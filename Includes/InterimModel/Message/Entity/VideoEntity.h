#pragma once
#include "IEntity.h"

namespace Lagrange::InterimModel::Message::Entity {
struct VideoEntity : IEntity {
    std::u8string FileUrl{};
    std::u8string FileName{};
    std::u8string FileSha1{};
    int           FileSize{NULL};
    std::u8string FileMd5{};

    VideoEntity(
        NATIVE_MODEL Message::Entity::VideoEntity* e
    ) {
        FileUrl  = e->FileUrl;
        FileName = e->FileName;
        FileSha1 = e->FileSha1;
        FileSize = e->FileSize;
        FileMd5  = e->FileMd5;
    }
};
} // namespace Lagrange::InterimModel::Message::Entity