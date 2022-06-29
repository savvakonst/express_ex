#include "FolderIoInterface.h"

#include <QDir>
#include <QFile>

FolderIoInterface::~FolderIoInterface() { FolderIoInterface::close(); }

bool FolderIoInterface::create(const char* path) {
    if (!QDir(path).exists())
        if (!QDir().mkdir(path)) return false;

    // TODO: might add some error code returning

    folder_name_ = std::string(path);
    rw_ = true;
    return true;
}

bool FolderIoInterface::open(const char* path, bool rw) {
    if (!QDir(path).exists()) return false;


    folder_name_ = QDir(path).absolutePath().toStdString();  // std::string(path);

    rw_ = rw;
    return true;
}

bool FolderIoInterface::close() {
    if (datasets_.empty()) return false;

    for (auto i : datasets_)
        if (i) {
            if (i->isOpen()) i->close();
            delete i;
        }

    datasets_.clear();

    return true;
}

bool FolderIoInterface::datasetExists(const char* name) { return QFileInfo::exists(name); }

DatasetsStorage_ifs::id_t FolderIoInterface::createDataset(const char* name) {
    // TODO: add check for relative path

    auto path = folder_name_ + "/" + std::string(name);

    for (auto i : datasets_) {
        if (getNameByPtr(i) == path) return false;
    }
    auto file = new QFile();

    file->setFileName(path.c_str());

    if (!file->open(QIODevice::ReadWrite)) {
        delete file;
        return -1;
    }

    datasets_.push_back(file);
    return id_t(datasets_.size()) - 1;
}

DatasetsStorage_ifs::id_t FolderIoInterface::openDataset(const char* name) {
    auto path = folder_name_ + "/" + std::string(name);

    if (rw_)
        for (auto i : datasets_) {
            if (getNameByPtr(i) == path) return false;
        }
    auto file = new QFile();

    file->setFileName(path.c_str());

    QFile::OpenMode mode = rw_ ? QIODevice::ReadWrite : QIODevice::ReadOnly;
    if (!file->open(mode)) {
        delete file;
        return -1;
    }

    datasets_.push_back(file);
    return id_t(datasets_.size()) - 1;
}

bool FolderIoInterface::closeDataset(id_t index) {
    if (id_t(datasets_.size()) > index) {
        auto i = datasets_[index];
        i->close();
        datasets_[index] = nullptr;
        return true;
    }
    return false;
}

bool FolderIoInterface::removeDataset(const char* name) {
    for (auto& i : datasets_) {
        if (i != nullptr)
            if (getNameByPtr(i) == std::string(name)) return false;
    }

    auto path = folder_name_ + "/" + std::string(name);
    return std::remove(path.c_str()) == 0;
}

DatasetsStorage_ifs::ds_ssize_t FolderIoInterface::getDatasetSize(id_t index) {
    if (ds_ssize_t(datasets_.size()) < index) return -1;

    if (auto file = datasets_.at(index)) return file->size();
    return -1;
}

bool FolderIoInterface::seek(DatasetsStorage_ifs::id_t index, DatasetsStorage_ifs::ds_usize_t pos) {
    if (ds_ssize_t(datasets_.size()) < index) return false;

    if (auto file = datasets_.at(index)) return file->seek(pos);
    return false;
}

std::ptrdiff_t FolderIoInterface::writeToDataset(id_t index, const char* data, std::size_t count) {
    // TODO:unsafe add limits check
    auto file = datasets_.at(index);
    return file->write(data, qint64(count));
}

std::ptrdiff_t FolderIoInterface::readFromDataset(id_t index, char* data, std::size_t count) {
    auto file = datasets_.at(index);
    return file->read(data, qint64(count));
}

std::string FolderIoInterface::getNameByPtr(const QFile* obj_id) { return obj_id->fileName().toStdString(); }
