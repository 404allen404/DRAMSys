/*
 * Copyright (c) 2019, RPTU Kaiserslautern-Landau
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:
 *    Lukas Steiner
 *    Derek Christ
 */

#include "MemSpecDDR3.h"

#include "DRAMSys/common/utils.h"

#include <iostream>

using namespace sc_core;
using namespace tlm;

namespace DRAMSys
{

MemSpecDDR3::MemSpecDDR3(const DRAMSys::Config::MemSpec &memSpec)
    : MemSpec(memSpec, MemoryType::DDR3,
      memSpec.memarchitecturespec.entries.at("nbrOfChannels"),
      1,
      memSpec.memarchitecturespec.entries.at("nbrOfRanks"),
      memSpec.memarchitecturespec.entries.at("nbrOfBanks"),
      1,
      memSpec.memarchitecturespec.entries.at("nbrOfBanks"),
      memSpec.memarchitecturespec.entries.at("nbrOfBanks")
          * memSpec.memarchitecturespec.entries.at("nbrOfRanks"),
      memSpec.memarchitecturespec.entries.at("nbrOfRanks"),
      memSpec.memarchitecturespec.entries.at("nbrOfDevices")),
      tCKE     (tCK * memSpec.memtimingspec.entries.at("CKE")),
      tPD      (tCKE),
      tCKESR   (tCK * memSpec.memtimingspec.entries.at("CKESR")),
      tDQSCK   (tCK * memSpec.memtimingspec.entries.at("DQSCK")),
      tRAS     (tCK * memSpec.memtimingspec.entries.at("RAS")),
      tRC      (tCK * memSpec.memtimingspec.entries.at("RC")),
      tRCD     (tCK * memSpec.memtimingspec.entries.at("RCD")),
      tRL      (tCK * memSpec.memtimingspec.entries.at("RL")),
      tRTP     (tCK * memSpec.memtimingspec.entries.at("RTP")),
      tWL      (tCK * memSpec.memtimingspec.entries.at("WL")),
      tWR      (tCK * memSpec.memtimingspec.entries.at("WR")),
      tXP      (tCK * memSpec.memtimingspec.entries.at("XP")),
      tXS      (tCK * memSpec.memtimingspec.entries.at("XS")),
      tCCD     (tCK * memSpec.memtimingspec.entries.at("CCD")),
      tFAW     (tCK * memSpec.memtimingspec.entries.at("FAW")),
      tREFI    (tCK * memSpec.memtimingspec.entries.at("REFI")),
      tRFC     (tCK * memSpec.memtimingspec.entries.at("RFC")),
      tRP      (tCK * memSpec.memtimingspec.entries.at("RP")),
      tRRD     (tCK * memSpec.memtimingspec.entries.at("RRD")),
      tWTR     (tCK * memSpec.memtimingspec.entries.at("WTR")),
      tAL      (tCK * memSpec.memtimingspec.entries.at("AL")),
      tXPDLL   (tCK * memSpec.memtimingspec.entries.at("XPDLL")),
      tXSDLL   (tCK * memSpec.memtimingspec.entries.at("XSDLL")),
      tACTPDEN (tCK * memSpec.memtimingspec.entries.at("ACTPDEN")),
      tPRPDEN  (tCK * memSpec.memtimingspec.entries.at("PRPDEN")),
      tREFPDEN (tCK * memSpec.memtimingspec.entries.at("REFPDEN")),
      tRTRS    (tCK * memSpec.memtimingspec.entries.at("RTRS")),
      iDD0     (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd0") : 0),
      iDD2N    (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd2n") : 0),
      iDD3N    (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd3n") : 0),
      iDD4R    (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd4r") : 0),
      iDD4W    (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd4w") : 0),
      iDD5     (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd5") : 0),
      iDD6     (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd6") : 0),
      vDD      (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("vdd") : 0),
      iDD2P0   (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd2p0") : 0),
      iDD2P1   (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd2p1") : 0),
      iDD3P0   (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd3p0") : 0),
      iDD3P1   (memSpec.mempowerspec.has_value() ? memSpec.mempowerspec.value().entries.at("idd3p1") : 0)
{
    uint64_t deviceSizeBits = static_cast<uint64_t>(banksPerRank) * rowsPerBank * columnsPerRow * bitWidth;
    uint64_t deviceSizeBytes = deviceSizeBits / 8;
    memorySizeBytes = deviceSizeBytes * devicesPerRank * ranksPerChannel * numberOfChannels;

    if (!memSpec.mempowerspec.has_value())
        SC_REPORT_FATAL("MemSpec", "No power spec defined!");

    std::cout << headline << std::endl;
    std::cout << "Memory Configuration:" << std::endl << std::endl;
    std::cout << " Memory type:           " << "DDR3"           << std::endl;
    std::cout << " Memory size in bytes:  " << memorySizeBytes  << std::endl;
    std::cout << " Channels:              " << numberOfChannels << std::endl;
    std::cout << " Ranks per channel:     " << ranksPerChannel << std::endl;
    std::cout << " Banks per rank:        " << banksPerRank     << std::endl;
    std::cout << " Rows per bank:         " << rowsPerBank << std::endl;
    std::cout << " Columns per row:       " << columnsPerRow << std::endl;
    std::cout << " Device width in bits:  " << bitWidth         << std::endl;
    std::cout << " Device size in bits:   " << deviceSizeBits   << std::endl;
    std::cout << " Device size in bytes:  " << deviceSizeBytes  << std::endl;
    std::cout << " Devices per rank:      " << devicesPerRank << std::endl;
    std::cout << std::endl;
}

sc_time MemSpecDDR3::getRefreshIntervalAB() const
{
    return tREFI;
}

// Returns the execution time for commands that have a fixed execution time
sc_time MemSpecDDR3::getExecutionTime(Command command, const tlm_generic_payload &) const
{
    if (command == Command::PREPB || command == Command::PREAB)
        return tRP;
    else if (command == Command::ACT)
        return tRCD;
    else if (command == Command::RD)
        return tRL + burstDuration;
    else if (command == Command::RDA)
        return tRTP + tRP;
    else if (command == Command::WR)
        return tWL + burstDuration;
    else if (command == Command::WRA)
        return tWL + burstDuration + tWR + tRP;
    else if (command == Command::REFAB)
        return tRFC;
    else
    {
        SC_REPORT_FATAL("getExecutionTime",
                        "command not known or command doesn't have a fixed execution time");
        return SC_ZERO_TIME;
    }
}

TimeInterval MemSpecDDR3::getIntervalOnDataStrobe(Command command, const tlm_generic_payload &) const
{
    if (command == Command::RD || command == Command::RDA)
        return {tRL, tRL + burstDuration};
    else if (command == Command::WR || command == Command::WRA)
        return {tWL, tWL + burstDuration};
    else
    {
        SC_REPORT_FATAL("MemSpec", "Method was called with invalid argument");
        return {};
    }
}

} // namespace DRAMSys
