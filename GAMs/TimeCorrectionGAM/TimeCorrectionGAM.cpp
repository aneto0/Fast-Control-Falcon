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
    timeSignal = NULL_PTR(uint32 *);
    correctedTimeSignal = NULL_PTR(uint32 *);
    triggerSignal = NULL_PTR(uint8 *);
    correctedTriggerSignal = NULL_PTR(uint8 *);
    analogueInputSignal = NULL_PTR(int16 *);
    threshold = 0;
    assertCycles = 0u;
    assertCounter = 0u;
    timeCorrection = 0u;
    timeSignalPeriod = 0;
}

TimeCorrectionGAM::~TimeCorrectionGAM() {
    using namespace MARTe;

}

bool TimeCorrectionGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 3u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 3u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 1u");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 1u) == SignedInteger16Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 1u) != SignedInteger16Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 2u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 2u) != UnsignedInteger8Bit");
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
        timeSignal = static_cast<uint32 *>(GetInputSignalMemory(0u));
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
        ok = data.Read("TimeSignalPeriod", timeSignalPeriod);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignalPeriod must be specified");
        }
    }
    if (ok) {
        ok = (timeSignalPeriod > 0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimeSignalPeriod must be > 0");
        }
    }
    return ok;
}

bool TimeCorrectionGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    assertCounter = assertCycles;
    return true;
}

bool TimeCorrectionGAM::Execute() {
    using namespace MARTe;
    if (assertCounter != 0u) {
        if ((*analogueInputSignal >= threshold) || (*analogueInputSignal <= -threshold)){
            assertCounter--;
        }
        else {
            assertCounter = assertCycles;
        }
    }
    if (assertCounter == 0u) {
        *correctedTimeSignal = ((*timeSignal) - timeCorrection);
        *correctedTriggerSignal = *triggerSignal;
    }
    else {
        *correctedTimeSignal = 0u;
        *correctedTriggerSignal = 0u;
    }
    return true;
}

CLASS_REGISTER(TimeCorrectionGAM, "1.0")

