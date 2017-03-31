/**
 * @file TimeCorrectionGAM.cpp
 * @brief Source file for class TimeCorrectionGAM
 * @date 30/03/2017
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
 * the class TimeCorrectionGAM (public, protected, and private). Be aware that some 
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
#include "TimeCorrectionGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TimeCorrectionGAM::TimeCorrectionGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    correctedTimeSignal = NULL_PTR(uint32 *);
    triggerSignal = NULL_PTR(uint8 *);
    correctedTriggerSignal = NULL_PTR(uint8 *);
    analogueInputSignal = NULL_PTR(int16 *);
    threshold = 0;
    assertCycles = 0u;
    assertCounter = 0u;
    timeCorrection = 0u;
    signalPeriod = 0;
    numberOfSamples = 0u;
    cycleTimeIncrement = 0u;
}

TimeCorrectionGAM::~TimeCorrectionGAM() {
    using namespace MARTe;

}

bool TimeCorrectionGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 2u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 2u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 1u");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == SignedInteger16Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != SignedInteger16Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 1u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 1u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 0u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 1u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 1u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(InputSignals, 1u, numberOfSamples);
        cycleTimeIncrement = static_cast<uint32>(static_cast<float64>(numberOfSamples) * signalPeriod);
    }
    if (ok) {
        analogueInputSignal = static_cast<int16 *>(GetInputSignalMemory(1u));
        triggerSignal = static_cast<uint8 *>(GetInputSignalMemory(2u));
        correctedTimeSignal = static_cast<uint32 *>(GetOutputSignalMemory(0u));
        correctedTriggerSignal = static_cast<uint8 *>(GetOutputSignalMemory(1u));
    }
    return ok;
}

bool TimeCorrectionGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("AssertCycles", assertCycles);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "AssertCycles must be specified");
        }
    }
    if (ok) {
        ok = (assertCycles > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "AssertCycles must be > 0");
        }
    }
    if (ok) {
        ok = data.Read("Threshold", threshold);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Threshold must be specified");
        }
    }
    if (ok) {
        ok = data.Read("SignalPeriod", signalPeriod);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "SignalPeriod must be specified");
        }
    }
    if (ok) {
        ok = (signalPeriod > 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignalPeriod must be > 0");
        }
    }
    return ok;
}

bool TimeCorrectionGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    assertCounter = assertCycles;
    *correctedTimeSignal = 0u;
    return true;
}

bool TimeCorrectionGAM::Execute() {
    using namespace MARTe;
    if (assertCounter == 0u) {
        *correctedTimeSignal += cycleTimeIncrement;
        *correctedTriggerSignal = *triggerSignal;
    }
    else {
        uint32 s;
        for (s = 0u; (s < numberOfSamples) && (assertCounter != 0u); s++) {
            if ((analogueInputSignal[s] >= threshold) || (analogueInputSignal[s] <= -threshold)) {
                assertCounter--;
            }
            else {
                assertCounter = assertCycles;
            }
        }
        if (assertCounter == 0u) {
            //Note that the assertCounter might go to zero between two Executes
            if (s >= (assertCycles - 1u)) {
                timeCorrection = static_cast<uint32>(static_cast<float64>(s - (assertCycles - 1u)) * signalPeriod);
            }
            else {
                timeCorrection = (static_cast<uint32>(numberOfSamples - (assertCycles - s - 1u)) * signalPeriod);
            }
            timeCorrection = cycleTimeIncrement - timeCorrection;
        }
        else {
            *correctedTimeSignal = 0u;
            *correctedTriggerSignal = 0u;
        }
    }
    return true;
}

CLASS_REGISTER(TimeCorrectionGAM, "1.0")

