/**
 * @file TriggerMaskGAM.cpp
 * @brief Source file for class TriggerMaskGAM
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
 * the class TriggerMaskGAM (public, protected, and private). Be aware that some
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

#include "TriggerMaskGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TriggerMaskGAM::TriggerMaskGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    input = NULL_PTR(uint32 *);
    output = NULL_PTR(uint8 *);
    mask = 0u;
}

TriggerMaskGAM::~TriggerMaskGAM() {
    using namespace MARTe;

}

bool TriggerMaskGAM::Setup() {
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
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        input = static_cast<uint32 *>(GetInputSignalMemory(0u));
        output = static_cast<uint8 *>(GetOutputSignalMemory(0u));
    }
    return ok;
}

bool TriggerMaskGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("Mask", mask);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Mask must be specified");
        }
    }

    return ok;
}

bool TriggerMaskGAM::Execute() {
    using namespace MARTe;
    uint32 currentInput = *input;
    if ((currentInput & mask) != 0u){
        *output = 1u;
    }
    else {
        *output = 0u;
    }
    return true;
}


CLASS_REGISTER(TriggerMaskGAM, "1.0")

