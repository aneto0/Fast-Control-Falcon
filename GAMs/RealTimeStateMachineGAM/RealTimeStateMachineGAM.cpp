/**
 * @file RealTimeStateMachineGAM.cpp
 * @brief Source file for class RealTimeStateMachineGAM
 * @date 26/09/2018
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
 * the class RealTimeStateMachineGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "RealTimeStateMachineGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
RealTimeStateMachineGAM::RealTimeStateMachineGAM() :
        GAM() {
    offlineStateCode = 0u;
    onlineStateCode = 0u;
    onlineOffStateCode = 0u;
    changeStateCode = 0u;
    endStateCode = 0u;
    faultStateCode = 0u;
    powerSupplyTrigger = 0u;
    plcOnline = 0u;
    plcAbort = 1u;
    sdnRTStart = 0u;
    sdnRTStop = 0u;
    sdnPower = 0u;

    plcState = NULL_PTR(MARTe::uint8 *);
    plcAbortCommand = NULL_PTR(MARTe::uint8 *);
    sdnEvent = NULL_PTR(MARTe::uint8 *);
    sdnPowerCommand = NULL_PTR(MARTe::uint8 *);
    outputState = NULL_PTR(MARTe::uint8 *);
    trigger = NULL_PTR(MARTe::uint32 *);
}

RealTimeStateMachineGAM::~RealTimeStateMachineGAM() {
}

bool RealTimeStateMachineGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 4u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 4u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 2u");
        }
    }
    if (ok) {
        uint32 i;
        for (i = 0u; (i < GetNumberOfInputSignals()) && (ok); i++) {
            ok = (GetSignalType(InputSignals, i) == UnsignedInteger8Bit);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, %d)() != UnsignedInteger8Bit", i);
            }
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 0u)() != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 1u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 0u)() != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        plcState = reinterpret_cast<uint8 *>(GetInputSignalMemory(0u));
        plcAbortCommand = reinterpret_cast<uint8 *>(GetInputSignalMemory(1u));
        sdnEvent = reinterpret_cast<uint8 *>(GetInputSignalMemory(2u));
        sdnPowerCommand = reinterpret_cast<uint8 *>(GetInputSignalMemory(3u));
        outputState = reinterpret_cast<uint8 *>(GetOutputSignalMemory(0u));
        trigger = reinterpret_cast<uint32 *>(GetOutputSignalMemory(1u));
        *outputState = offlineStateCode;
    }
    return ok;
}

bool RealTimeStateMachineGAM::Initialise(MARTe::StructuredDataI& data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("Offline", offlineStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Offline state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Online", onlineStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Online state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("OnlineOff", onlineOffStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The OnlineOff state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Change", changeStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Change state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("End", endStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The End state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("Fault", faultStateCode);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Fault state code shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("PowerSupplyTrigger", powerSupplyTrigger);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The PowerSupplyTrigger value shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("PLCOnline", plcOnline);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The PLCOnline value shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("PLCAbort", plcAbort);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The PLCAbort value shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("SDNRTStart", sdnRTStart);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SDNRTStart value shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("SDNRTStop", sdnRTStop);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SDNRTStop value shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("SDNPowerOn", sdnPower);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The SDNPowerOn value shall be specified");
        }
    }



    return ok;
}

bool RealTimeStateMachineGAM::Execute() {
    bool ok = true;
    using namespace MARTe;
    *trigger = 0u;
    if (*outputState == offlineStateCode) {
        if ((*plcState == plcOnline) && (*sdnEvent == sdnRTStart)) {
            *outputState = onlineOffStateCode;
        }
    }
    else if (*outputState == onlineOffStateCode) {
        if (*plcState == plcOnline) {
            if (*plcAbortCommand == plcAbort) {
                *outputState = offlineStateCode;
            }
            else if (*sdnEvent == sdnRTStop) {
                *outputState = endStateCode;
            }
            else if (*sdnPowerCommand == sdnPower) {
                *outputState = onlineStateCode;
            }
        }
        else {
            *outputState = faultStateCode;
            REPORT_ERROR(ErrorManagement::FatalError, "outputState == onlineOffStateCode && plcState != plcOnline");
        }
    }
    else if (*outputState == onlineStateCode) {
        if (*plcState == plcOnline) {
            if (*plcAbortCommand == plcAbort) {
                *outputState = offlineStateCode;
            }
            else if (*sdnEvent == sdnRTStop) {
                *outputState = endStateCode;
            }
            else if (*sdnPowerCommand == sdnPower) {
                *trigger = powerSupplyTrigger;
            }
        }
        else {
            *outputState = faultStateCode;
            REPORT_ERROR(ErrorManagement::FatalError, "outputState == onlineStateCode && plcState != plcOnline");
        }
    }
    else if (*outputState == endStateCode) {
        if (*plcState == plcOnline) {
            if (*plcAbortCommand == plcAbort) {
                *outputState = offlineStateCode;
            }
        }
        else {
            *outputState = faultStateCode;
            REPORT_ERROR(ErrorManagement::FatalError, "outputState == endStateCode && plcState != plcOnline");
        }
    }
    else if (*outputState == faultStateCode) {
        if (*plcAbortCommand == plcAbort) {
            *outputState = offlineStateCode;
        }
        else if (*plcState == plcOnline) {
            if (*sdnEvent == sdnRTStop) {
                *outputState = endStateCode;
            }
        }
    }
    return ok;
}

CLASS_REGISTER(RealTimeStateMachineGAM, "1.0")
