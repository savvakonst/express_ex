#ifndef SYNC_PARAMETER_H
#define SYNC_PARAMETER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ParameterIfs.h"

class DLL_EXPORT SyncParameter : public ParameterIfs {
   public:
    SyncParameter() = default;
    SyncParameter(std::string name, const std::vector<DataInterval>& interval_list, bool save_file_names);
    SyncParameter(std::string name, const TimeInterval& time_interval, const std::vector<DataInterval>& interval_list,
                  bool save_file_names = true);

    ~SyncParameter() override;

    bool isAsync() override { return false; }
    // is not supported yet
    std::vector<int64_t> read_dots(Dot* dot_buffer, size_t max_point_number, double from, double to) override;

    // is not supported yet
    std::vector<int64_t> read_dots(double* top_buffer_ptr, double* bottom_buffer_ptr, double* time_buffer_ptr,
                                   double from, double to, size_t max_point_number_to_read) override;

    bool open(bool open_to_write = false) override;
    bool close() override;
    bool seek(int64_t point_umber);
    uint64_t write(char* data_buffer_ptr, uint64_t point_number) override;
    uint64_t read(char* data_buffer_ptr, uint64_t point_number) override;

    uint64_t getVirtualSize() const override;

    ParameterIfs* intersection(ParameterIfs* prm, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                               const std::string& name = "") override;
    SyncParameter* enlargeFrequency(int64_t arg, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                    const std::string& name = "");
    ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN, const std::string& name = "") override;

    virtual ParameterIfs* newParameter() override;

    friend void readParametersList(std::string databaseFName, std::vector<ParameterIfs>& parameterList);
    friend class ParametersDB;

   protected:
    bool calcExtendedInfo();

    inline const TimeInterval& getTimeInterval(int64_t interval_index) const {
        return interval_list_[(size_t)interval_index].time_interval;
    }

    inline const DataInterval& getCurrentInterval() { return interval_list_[(size_t)current_interval_index_]; }

    void openNewInterval(double di_index);

    BareChunk* chunk_         = nullptr;
    BareChunk* current_chunk_ = nullptr;
    double frequency_         = -1;
};

#endif