#ifndef SYNC_PARAMETER_H
#define SYNC_PARAMETER_H

#include <string>
#include <vector>

#include "ifs/ParameterIfs.h"

class DLL_EXPORT SyncParameter : public ParameterIfs {
   public:
    SyncParameter(const SyncParameter& prm);
    SyncParameter(std::string name, const std::vector<ExDataInterval>& interval_list, bool save_file_names);
    SyncParameter(std::string name, const ExTimeInterval& time_interval,
                  const std::vector<ExDataInterval>& interval_list, bool save_file_names = true);

    ~SyncParameter() override;

    bool isAsync() override { return false; }


    bool open(bool open_to_write = false) override;
    bool close() override;
    bool seek(int64_t point_umber);
    uint64_t write(char* data_buffer_ptr, uint64_t point_number) override;
    uint64_t read(char* data_buffer_ptr, uint64_t point_number) override;

    uint64_t getVirtualSize() const override;

    ParameterIfs* intersection(ParameterIfs* prm, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                               const std::string& name = "") override;
    SyncParameter* enlargeFrequency(int64_t freq_factor, PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN,
                                    const std::string& name = "") const;
    ParameterIfs* retyping(PrmTypesEn target_ty = PrmTypesEn::PRM_TYPE_UNKNOWN, const std::string& name = "") override;

    ParameterIfs* newParameter() override;

    bool opened_ = false;

   protected:
    bool calcExtendedInfo();

    int64_t type_size_ = 0;

    BareChunk* chunk_ = nullptr;
    BareChunk* current_chunk_ = nullptr;
    int64_t frequency_ = -1;
};

#endif