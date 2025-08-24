#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {
struct RecordEntity : IEntity {
    std::string FileUrl{};
    std::string FileName{};
    std::string FileSha1{};
    int         FileSize{NULL};
    std::string FileMd5{};

    RecordEntity(
        NativeModel::Message::Entity::RecordEntity* e
    ) {
        FileUrl  = e->FileUrl;
        FileName = e->FileName;
        FileSha1 = e->FileSha1;
        FileSize = e->FileSize;
        FileMd5  = e->FileMd5;
    }
};
} // namespace Lagrange::Definition::NativeModel::Message::Entity