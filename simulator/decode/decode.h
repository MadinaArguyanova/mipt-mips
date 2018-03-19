/**
 * decode.h - simulation of decode stage
 * Copyright 2015-2018 MIPT-MIPS
 */


#ifndef DECODE_H
#define DECODE_H


#include <infra/ports/ports.h>
#include <core/perf_instr.h>
#include <bypass/data_bypass.h>
#include <func_sim/rf/rf.h>


template <typename ISA>
class Decode : public Log
{
    using FuncInstr = typename ISA::FuncInstr;
    using Instr = PerfInstr<FuncInstr>;
    using BypassingUnit = DataBypass<ISA>;

    private:
        RF<ISA>* rf = nullptr;
        std::unique_ptr<BypassingUnit> bypassing_unit = nullptr;

        std::unique_ptr<WritePort<Instr>> wp_datapath = nullptr;
        std::unique_ptr<ReadPort<Instr>> rp_datapath = nullptr;

        std::unique_ptr<WritePort<Instr>> wp_stall_datapath = nullptr;
        std::unique_ptr<ReadPort<Instr>> rp_stall_datapath = nullptr;

        std::unique_ptr<WritePort<bool>> wp_stall = nullptr;

        std::unique_ptr<ReadPort<bool>> rp_flush = nullptr;
        
        static constexpr const uint8 SRC_REGISTERS_NUM = 2;
        static constexpr const uint8 BYPASSING_UNIT_FLUSH_NOTIFIERS_NUM = 2;

        std::array<std::unique_ptr<WritePort<typename BypassingUnit::BypassCommand>>, SRC_REGISTERS_NUM>
            wps_command;
        
        std::unique_ptr<WritePort<Instr>> wp_bypassing_unit_notify = nullptr;
        std::unique_ptr<ReadPort<Instr>> rp_bypassing_unit_notify = nullptr;

        std::array<std::unique_ptr<ReadPort<Instr>>, BYPASSING_UNIT_FLUSH_NOTIFIERS_NUM> 
            rps_bypassing_unit_flush_notify;
        
        Instr read_instr( Cycle cycle);

    public:
        explicit Decode( bool log);
        void clock( Cycle cycle);
        void set_RF( RF<ISA>* value) { rf = value;}
};


#endif // DECODE_H