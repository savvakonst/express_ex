#include "Hdf5IoInterface.h"


Hdf5IoInterface::~Hdf5IoInterface() { Hdf5IoInterface::close(); }

bool Hdf5IoInterface::create(const char *path) {
    file_id_ = ::H5Fcreate(path, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    if (file_id_ <= 0) return false;
    rw_ = true;

    if (initVariables()) return true;

    close();

    std::remove(path);
    return false;
}

bool Hdf5IoInterface::open(const char *path, bool rw) {
    rw_ = rw;
    file_id_ = ::H5Fopen(path, (rw ? H5F_ACC_RDWR : H5F_ACC_RDONLY), H5P_DEFAULT);  // read only
    if (file_id_ < 0) return false;

    if (initVariables()) return true;

    close();
    return false;
}


bool Hdf5IoInterface::close() {
    rw_ = false;
    if (file_id_ < 0) return false;
    closeHandles();
    return true;
}

bool Hdf5IoInterface::datasetExists(const char *name) {
    herr_t err = ::H5Lexists(file_id_, name, H5P_DEFAULT);
    return err > 0;    //return err > -1;
}

DatasetsStorage_ifs::id_t Hdf5IoInterface::createDataset(const char *name) {
    // TODO:is it necessary check existence, maybe only H5Dcreate1 do it?
    hid_t id = ::H5Dcreate(file_id_, name, H5T_NATIVE_UCHAR, file_space_id_, H5P_DEFAULT, prop_list_id_, H5P_DEFAULT);

    if (id < 0) return -1;

    id_t ret = id_t(datasets_.size());
    datasets_.push_back({id, 0});
    return ret;
}

DatasetsStorage_ifs::id_t Hdf5IoInterface::openDataset(const char *name) {
    id_t ret = 0;
    /*
    //TODO: it is wrong way since... what if I want to have two separated interfaces to read one file
    for(const auto &i: datasets_){
        if((getNameByHid(i.id_) == std::string(name)) && (i.id_>-1))
            return (id_t)i.id_;

        ret++;
    }
    */
    hid_t id = kDefaultId;
    if (datasetExists(name)) {
        id = H5Dopen1(file_id_, name);
        if (rw_ && (id < 0))  // when dataset was already exists but it is empty H5Dopen1(file_id_, name); returns -1;
            id = ::H5Dcreate(file_id_, name, H5T_NATIVE_UCHAR, file_space_id_, H5P_DEFAULT, prop_list_id_, H5P_DEFAULT);
    } else if (rw_)
        id = ::H5Dcreate(file_id_, name, H5T_NATIVE_UCHAR, file_space_id_, H5P_DEFAULT, prop_list_id_, H5P_DEFAULT);

    if (id < 0) return id;

    ret = id_t(datasets_.size());
    datasets_.push_back({id, 0});

    // its dirty hack
    getDatasetSize(ret);
    return ret;
}

bool Hdf5IoInterface::closeDataset(id_t index) {
    if ((kDefaultId < index) && (id_t(datasets_.size()) > index)) {
        auto status = H5Dclose(datasets_[index].id_);
        datasets_[index].id_ = -1;
        return true;
    }
    return false;
}

bool Hdf5IoInterface::removeDataset(const char *name) {
    for (auto &i : datasets_) {
        if (i.id_ > -1)
            if (getNameByHid(i.id_) == std::string(name)) return false;
    }

    herr_t err = ::H5Ldelete(file_id_, name, H5P_DEFAULT);
    return err > -1;
}

DatasetsStorage_ifs::ds_ssize_t Hdf5IoInterface::getDatasetSize(id_t index) {
    if (ds_ssize_t(datasets_.size()) < index) return -1;

    auto &ds = datasets_[index];
    auto dataset_id = ds.id_;
    if (dataset_id < 0) return -1;

    // TODO:  maybe need to extract code above to inline function


    auto dataspace_id = ::H5Dget_space(dataset_id);
    if (dataspace_id < 0) return -1;


    hsize_t dims = 0;
    ::H5Sget_simple_extent_dims(dataspace_id, &dims, nullptr);
    ::H5Sclose(dataspace_id);

    ds.size_ = dims;
    return ds_ssize_t(dims);
}

bool Hdf5IoInterface::seek(id_t index, DatasetsStorage_ifs::ds_usize_t pos) {
    // TODO:unsafe add limits check
    auto &ds = datasets_[index];

    if (pos >= ds.size_) return false;

    ds.offset_ = pos;

    return true;
}

std::ptrdiff_t Hdf5IoInterface::writeToDataset(id_t index, const char *data, std::size_t count) {
    // TODO: add limits check
    auto &ds = datasets_[index];


    auto dims = hsize_t(count);
    auto max_dims = H5S_UNLIMITED;

    auto hs_start = hsize_t(ds.offset_);
    auto hs_count = hsize_t(count);

    herr_t status = ::H5Sset_extent_simple(mem_space_id_, 1, &dims, &max_dims);
    if (status < 0) return -1;


    ds_usize_t next_pos = ds.offset_ + count;


    if (next_pos > ds.size_) {
        dims = hsize_t(next_pos);

        status = ::H5Sset_extent_simple(file_space_id_, 1, &dims, &max_dims);
        if (status < 0) return -2;


        status = ::H5Dset_extent(ds.id_, &dims);
        if (status < 0) return -3;
    } else {
        dims = hsize_t(ds.size_);

        status = ::H5Sset_extent_simple(file_space_id_, 1, &dims, &max_dims);
        if (status < 0) return -2;
    }


    status = ::H5Sselect_hyperslab(file_space_id_, H5S_SELECT_SET, &hs_start, nullptr, &hs_count, nullptr);
    if (status < 0) return -4;

    status = ::H5Dwrite(ds.id_, H5T_NATIVE_UCHAR, mem_space_id_, file_space_id_, H5P_DEFAULT, data);
    if (status < 0) return -5;


    ds.size_ = dims;
    ds.offset_ = next_pos;

    return std::ptrdiff_t(count);
}

std::ptrdiff_t Hdf5IoInterface::readFromDataset(id_t index, char *data, std::size_t count) {
    // TODO: add limits check
    auto &ds = datasets_[index];


    if (ds_usize_t diff = ds.size_ - ds.offset_; diff < count) {
        count = diff;
        if (diff == 0) return 0;
    }


    auto dims = hsize_t(count);
    hsize_t max_dims = H5S_UNLIMITED;

    auto hs_start = hsize_t(ds.offset_);
    auto hs_count = hsize_t(count);


    herr_t status = ::H5Sset_extent_simple(mem_space_id_, 1, &dims, &max_dims);
    if (status < 0) return -2;

    auto filespace_id = ::H5Dget_space(ds.id_);
    if (filespace_id < 0) return -3;


    status = ::H5Sselect_hyperslab(filespace_id, H5S_SELECT_SET, &hs_start, nullptr, &hs_count, nullptr);
    if (status < 0) {
        ::H5Sclose(filespace_id);
        return -4;
    }

    status = ::H5Dread(ds.id_, H5T_NATIVE_UCHAR, mem_space_id_, filespace_id, H5P_DEFAULT, data);

    ::H5Sclose(filespace_id);
    if (status < 0) return -5;


    ds.offset_ += count;

    return ptrdiff_t(count);
}

bool Hdf5IoInterface::initVariables() {
    hsize_t dims = 0;
    hsize_t max_dims = H5S_UNLIMITED;

    file_space_id_ = ::H5Screate_simple(1, &dims, &max_dims);
    if (file_space_id_ < 0) return false;

    mem_space_id_ = ::H5Screate_simple(1, &dims, &max_dims);
    if (mem_space_id_ < 0) return false;

    prop_list_id_ = ::H5Pcreate(H5P_DATASET_CREATE);
    if (prop_list_id_ < 0) return false;

    hsize_t chunks = 0x10000;


    if (::H5Pset_chunk(prop_list_id_, 1, &chunks) < 0) return false;

    return true;
}


void Hdf5IoInterface::closeHandles() {
    for (auto i : datasets_)
        if (i.id_ > -1) ::H5Dclose(i.id_);

    datasets_.clear();

    if (prop_list_id_ > 0) {
        ::H5Pclose(prop_list_id_);
        prop_list_id_ = H5I_BADID;
    }

    if (mem_space_id_ > 0) {
        ::H5Sclose(mem_space_id_);
        mem_space_id_ = H5I_BADID;
    }

    if (file_space_id_ > 0) {
        ::H5Sclose(file_space_id_);
        file_space_id_ = H5I_BADID;
    }

    if (file_id_ > 0) {
        ::H5Fclose(file_id_);
        file_id_ = H5I_BADID;
    }
}

std::string Hdf5IoInterface::getNameByHid(hid_t obj_id) {
    size_t size = 256;
    auto name = new char[size];
    size = H5Iget_name(obj_id, name, size);

    std::string ret(name, size);
    delete[] name;
    return ret;
}
