/**
 * @file TimeCorrectionPLCGAM.cpp
 * @brief Source file for class TimeCorrectionPLCGAM
 * @date 27/09/2018
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
 * the class TimeCorrectionPLCGAM (public, protected, and private). Be aware that some
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

#include "TimeCorrectionPLCGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TimeCorrectionPLCGAM::TimeCorrectionPLCGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    correctedTimeSignal = NULL_PTR(uint32 *);
    triggerSignal = NULL_PTR(uint8 *);
    correctedTriggerSignal = NULL_PTR(uint8 *);
    correctedTriggerSignalSlow = NULL_PTR(uint8 *);
    rtStateSignal = NULL_PTR(uint8 *);
    onlineOffCode = 0u;
    endCode = 0u;
    timePeriod = 0u;
    startDetected = false;
}

TimeCorrectionPLCGAM::~TimeCorrectionPLCGAM() {
    using namespace MARTe;

}

bool TimeCorrectionPLCGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 2u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 2u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 3u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 3u");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger8Bit");
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
        ok = (GetSignalType(OutputSignals, 2u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 2u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        rtStateSignal = static_cast<uint8 *>(GetInputSignalMemory(0u));
        triggerSignal = static_cast<uint8 *>(GetInputSignalMemory(1u));
        correctedTimeSignal = static_cast<uint32 *>(GetOutputSignalMemory(0u));
        correctedTriggerSignal = static_cast<uint8 *>(GetOutputSignalMemory(1u));
        correctedTriggerSignalSlow = static_cast<uint8 *>(GetOutputSignalMemory(2u));
    }
    return ok;
}

bool TimeCorrectionPLCGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("TimePeriod", timePeriod);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "TimePeriod must be specified");
        }
    }
    if (ok) {
        ok = data.Read("OnlineOff", onlineOffCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "OnlineOff must be specified");
        }
    }
    if (ok) {
        ok = data.Read("End", endCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "End must be specified");
        }
    }
    return ok;
}

bool TimeCorrectionPLCGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    startDetected = false;
    *correctedTimeSignal = 0u;
    return true;
}

bool TimeCorrectionPLCGAM::Execute() {
    using namespace MARTe;
    if (startDetected) {
        *correctedTimeSignal += timePeriod;
        if (*rtStateSignal != endCode) {
            *correctedTriggerSignal = *triggerSignal;
            *correctedTriggerSignalSlow = 1;
        }
    }
    else {
        if (!startDetected) {
            startDetected = ((*rtStateSignal == onlineOffCode));
            *correctedTimeSignal = 0u;
            *correctedTriggerSignal = 0u;
            *correctedTriggerSignalSlow = 0u;
        }
    }
    return true;
}

CLASS_REGISTER(TimeCorrectionPLCGAM, "1.0")

