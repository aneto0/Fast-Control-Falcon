/**
 * @file TriggerTestGAM.cpp
 * @brief Source file for class TriggerTestGAM
 * @date 01/03/2017
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class TriggerTestGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

#include "TriggerTestGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static const MARTe::uint32 SINE_WAVEFORM = 0u;
static const MARTe::uint32 COSINE_WAVEFORM = 1u;
static const MARTe::uint32 SQUARE_WAVEFORM = 2u;
static const MARTe::float64 PI = 3.14159265359;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TriggerTestGAM::TriggerTestGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    startTimes = NULL_PTR(uint64 *);
    endTimes = NULL_PTR(uint64 *);
    currentTimeWindow = 0u;
    timeInput = NULL_PTR(uint32 *);
    output = NULL_PTR(uint32 *);
    enabledOutputValue = 0u;
    disabledOutputValue = 0u;
    numberOfWindows = 0u;
    lastTimeInput = 0u;
}

TriggerTestGAM::~TriggerTestGAM() {
    using namespace MARTe;
    if (startTimes != NULL_PTR(uint64 *)) {
        delete startTimes;
    }
    if (endTimes != NULL_PTR(uint64 *)) {
        delete endTimes;
    }
}

bool TriggerTestGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 1u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() == 1u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() == 1u");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger32Bit");
        }
    }

    timeInput = static_cast<uint32 *>(GetInputSignalMemory(0u));
    output = static_cast<uint32 *>(GetOutputSignalMemory(0u));
    return ok;
}

bool TriggerTestGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.MoveRelative("TimeWindows");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeWindows section must exist");
        }
    }
    if (ok) {
        numberOfWindows = data.GetNumberOfChildren();
        startTimes = new uint64[numberOfWindows];
        endTimes = new uint64[numberOfWindows];
    }
    if (ok) {
        ok = (numberOfWindows > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfWindows must be > 0");
        }
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < numberOfWindows) && (ok); i++) {
            ok = data.MoveToChild(i);
            if (ok) {
                float64 startTime = 0;
                ok = data.Read("StartTime", startTime);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "StartTime must be specified");
                }
                startTimes[i] = startTime * 1e6;
            }
            if (ok) {
                float64 endTime = 0;
                ok = data.Read("EndTime", endTime);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "EndTime must be specified");
                }
                endTimes[i] = endTime * 1e6;
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
        if (ok) {
            ok = data.MoveToAncestor(1u);
        }
    }
    if (ok) {
        ok = data.Read("EnabledOutputValue", enabledOutputValue);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "EnabledOutputValue must be specified");
        }
    }
    if (ok) {
        ok = data.Read("DisabledOutputValue", disabledOutputValue);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "DisabledOutputValue must be specified");
        }
    }

    return ok;
}

bool TriggerTestGAM::Execute() {
    using namespace MARTe;
    uint32 currentTime = *timeInput;
    if (currentTime < lastTimeInput) {
        currentTimeWindow = 0u;
    }
    lastTimeInput = currentTime;

    bool run = ((startTimes[currentTimeWindow] <= currentTime) && (endTimes[currentTimeWindow] >= currentTime));
    if (run) {
        *output = enabledOutputValue;
    }
    else {
        *output = 0;
        if (currentTime > endTimes[currentTimeWindow]) {
            currentTimeWindow++;
            if (currentTimeWindow == numberOfWindows) {
                currentTimeWindow = disabledOutputValue;
            }
        }
    }
    return true;
}


CLASS_REGISTER(TriggerTestGAM, "1.0")

