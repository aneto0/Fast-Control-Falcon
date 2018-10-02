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
#include "CLASSMETHODREGISTER.h"
#include "RealTimeStateMachineGAM.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
RealTimeStateMachineGAM::RealTimeStateMachineGAM() :
        GAM(), MessageI() {
    using namespace MARTe;
    offlineStateCode = 0u;
    onlineStateCode = 0u;
    onlineOffStateCode = 0u;
    changeStateCode = 0u;
    endStateCode = 0u;
    faultStateCode = 0u;
    powerSupplyTrigger = 0u;
    onlineMainStateMachine = "";
    sdnRTStart = 0u;
    sdnRTStop = 0u;
    sdnPower = 0u;

    mainStateMachineIsOnline = false;
    abortRequested = false;
    resumeRequested = false;
    suspendRequested = false;

    sdnEvent = NULL_PTR(MARTe::uint8 *);
    sdnPowerCommand = NULL_PTR(MARTe::uint8 *);
    outputState = NULL_PTR(MARTe::uint8 *);
    trigger = NULL_PTR(MARTe::uint32 *);

    ReferenceT<RegisteredMethodsMessageFilter> filter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

}

RealTimeStateMachineGAM::~RealTimeStateMachineGAM() {
}

bool RealTimeStateMachineGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 2u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 2u");
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
        sdnEvent = reinterpret_cast<uint8 *>(GetInputSignalMemory(0u));
        sdnPowerCommand = reinterpret_cast<uint8 *>(GetInputSignalMemory(1u));
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
        ok = data.Read("OnlineMainStateMachine", onlineMainStateMachine);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The OnlineMainStateMachine value shall be specified");
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
    bool hasToAbort = abortRequested;
    bool hasToResume = resumeRequested;
    bool hasToSuspend = suspendRequested;
    if (hasToAbort) {
        abortRequested = false;
    }
    if (hasToResume) {
        resumeRequested = false;
    }
    if (hasToSuspend) {
        suspendRequested = false;
    }

    if (*outputState == offlineStateCode) {
        if ((mainStateMachineIsOnline) && (*sdnEvent == sdnRTStart)) {
            *outputState = onlineOffStateCode;
        }
    }
    else if (*outputState == onlineOffStateCode) {
        if (mainStateMachineIsOnline) {
            if (hasToAbort) {
                *outputState = endStateCode;
            }
            else if (hasToSuspend) {
                *outputState = faultStateCode;
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
            REPORT_ERROR(ErrorManagement::FatalError, "outputState == onlineOffStateCode && !mainStateMachineIsOnline");
        }
    }
    else if (*outputState == onlineStateCode) {
        if (mainStateMachineIsOnline) {
            if (hasToAbort) {
                *outputState = endStateCode;
            }
            else if (hasToSuspend) {
                *outputState = faultStateCode;
            }
            else if (*sdnEvent == sdnRTStop) {
                *outputState = endStateCode;
            }
            else if (*sdnEvent == sdnRTStart) {
                if (*sdnPowerCommand == sdnPower) {
                    *trigger = powerSupplyTrigger;
                }
                else {
                    *outputState = onlineOffStateCode;
                }
            }
        }
        else {
            *outputState = faultStateCode;
            REPORT_ERROR(ErrorManagement::FatalError, "outputState == onlineStateCode && !mainStateMachineIsOnline");
        }
    }
    else if (*outputState == endStateCode) {
        //Can only get out through PrepareNextState
    }
    else if (*outputState == faultStateCode) {
        if (hasToAbort) {
            *outputState = endStateCode;
        }
        else if (mainStateMachineIsOnline) {
            if (*sdnEvent == sdnRTStop) {
                *outputState = endStateCode;
            }
            else if (hasToResume) {
                *outputState = onlineOffStateCode;
            }
        }
    }

    return ok;
}

MARTe::ErrorManagement::ErrorType RealTimeStateMachineGAM::Abort() {
    abortRequested = true;
    REPORT_ERROR(MARTe::ErrorManagement::Information, "Abort requested!");
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType RealTimeStateMachineGAM::Resume() {
    resumeRequested = true;
    REPORT_ERROR(MARTe::ErrorManagement::Information, "Resume requested!");
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType RealTimeStateMachineGAM::Suspend() {
    suspendRequested = true;
    REPORT_ERROR(MARTe::ErrorManagement::Information, "Suspend requested!");
    return MARTe::ErrorManagement::NoError;
}

bool RealTimeStateMachineGAM::PrepareNextState(const MARTe::char8* const currentStateName, const MARTe::char8* const nextStateName) {
    *outputState = offlineStateCode;
    mainStateMachineIsOnline = (onlineMainStateMachine == nextStateName);
    if (mainStateMachineIsOnline) {
        REPORT_ERROR(MARTe::ErrorManagement::Information, "Main state machine is detected to be online");
    }
    return true;
}

CLASS_REGISTER(RealTimeStateMachineGAM, "1.0")
CLASS_METHOD_REGISTER(RealTimeStateMachineGAM, Abort)
CLASS_METHOD_REGISTER(RealTimeStateMachineGAM, Resume)
