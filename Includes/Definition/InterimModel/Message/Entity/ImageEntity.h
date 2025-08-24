#pragma once
#include "IEntity.h"

namespace Lagrange::Definition::InterimModel::Message::Entity {
struct ImageEntity : IEntity {
    std::string  FileUrl;
    std::string  FileName;
    std::string  FileSha1;
    unsigned int FileSize;
    std::string  FileMd5;
    float        ImageWidth;
    float        ImageHeight;
    int          SubType;
    std::string  Summary;
    unsigned int RecordLength;

    ImageEntity(
        NativeModel::Message::Entity::ImageEntity* e
    ) {
        FileUrl      = e->FileUrl;
        FileName     = e->FileName;
        FileSha1     = e->FileSha1;
        FileSize     = e->FileSize;
        FileMd5      = e->FileMd5;
        ImageWidth   = e->ImageWidth;
        ImageHeight  = e->ImageHeight;
        SubType      = e->SubType;
        Summary      = e->Summary;
        RecordLength = e->RecordLength;
    }
};
} // namespace Lagrange::Definition::InterimModel::Message::Entity