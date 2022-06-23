#include "printer.h"
#include "ifs/SyncParameter.h"



bool g_ansi_escape_codes = false;

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const ExColors &arg) {
    if (((int)arg) <= ((int)ExColors::RESET))
        if (g_ansi_escape_codes) {
            if (arg == ExColors::RESET)
                OS << "\u001b[0m";
            else
                OS << "\u001b[3" + std::to_string((int)arg) + "m";
        }
        else
            OS << (llvm::raw_ostream::Colors)(arg);
    else {
        if (arg == ExColors::AnsiRESET)
            OS << "\u001b[0m";
        else
            OS << "\u001b[3" + std::to_string((int)arg - (int)ExColors::AnsiBLACK) + "m";
    }
    return OS;
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const Delimiter &arg) {
    OS.SetUnbuffered();
    OS << (ExColors)(arg) << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << (((int)arg <= (int)ExColors::RESET) ? ExColors::RESET : ExColors::AnsiRESET) << "\n";
    return OS;
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const PrmTypesEn &arg) {
    OS << toString(arg);
    return OS;
}
