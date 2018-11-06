/**
 * @file ESDNCommandEmuGAM.cpp
 * @brief Source file for class ESDNCommandEmuGAM
 * @date 28/09/2018
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
 * the class ESDNCommandEmuGAM (public, protected, and private). Be aware that some
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
#include "ESDNCommandEmuGAM.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
const MARTe::uint32 RU_GYROTRON = 0u;
const MARTe::uint32 EU_GYROTRON = 1u;
const MARTe::uint32 GYROTRON_MODE_PREPROG = 0u;
const MARTe::uint32 GYROTRON_MODE_MANUAL = 1u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ESDNCommandEmuGAM::ESDNCommandEmuGAM() :
        MARTe::GAM(), MARTe::MessageI() {
    using namespace MARTe;

    timeSignal = NULL_PTR(MARTe::uint32 *);
    rtState = NULL_PTR(MARTe::uint8 *);
    pulseDurationMillis = NULL_PTR(MARTe::uint32 *);
    pulseDurationSeconds = NULL_PTR(MARTe::uint32 *);
    pulseDurationMinutes = NULL_PTR(MARTe::uint32 *);
    selectedGyrotronSignal = NULL_PTR(MARTe::uint32 *);
    selectedModeSignal = NULL_PTR(MARTe::uint32 *);
    esdnEvent = NULL_PTR(MARTe::uint8 *);
    esdnCommand = NULL_PTR(MARTe::uint8 *);
    powerDelayTime = 0u;
    powerTotalTime = 0u;
    rtStartEvent = 0u;
    rtStopEvent = 0u;
    powerOnCommand = 0u;
    powerOffCommand = 0u;
    offlineState = 0u;
    onlineOffState = 0u;
    rtStopTime = 0u;
    lastPowerRequestTime = 0u;
    selectedGyrotron = 0u;
    selectedMode = 0u;
    rtAbsoluteStopTime = 0u;
    loadRequested = false;

    ReferenceT<RegisteredMethodsMessageFilter> filter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

ESDNCommandEmuGAM::~ESDNCommandEmuGAM() {
    using namespace MARTe;

}

bool ESDNCommandEmuGAM::Setup() {
    using namespace MARTe;
    bool ok = (GetNumberOfInputSignals() == 8u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfInputSignals() != 8u");
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetNumberOfOutputSignals() != 2u");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == UnsignedInteger32Bit);
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
        ok = (GetSignalType(InputSignals, 2u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 2u) != UnsignedInteger8Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 3u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 3u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 4u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 4u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 5u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 5u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 6u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 6u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 7u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 7u) != UnsignedInteger32Bit");
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(OutputSignals, 0u) != UnsignedInteger8Bit");
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
        triggerSignal = static_cast<uint8 *>(GetInputSignalMemory(1u));
        rtState = static_cast<uint8 *>(GetInputSignalMemory(2u));
        pulseDurationMillis = static_cast<uint32 *>(GetInputSignalMemory(3u));
        pulseDurationSeconds = static_cast<uint32 *>(GetInputSignalMemory(4u));
        pulseDurationMinutes = static_cast<uint32 *>(GetInputSignalMemory(5u));
        selectedGyrotronSignal = static_cast<uint32 *>(GetInputSignalMemory(6u));
        selectedModeSignal = static_cast<uint32 *>(GetInputSignalMemory(7u));
        esdnEvent = static_cast<uint8 *>(GetOutputSignalMemory(0u));
        esdnCommand = static_cast<uint8 *>(GetOutputSignalMemory(1u));
    }
    return ok;
}

bool ESDNCommandEmuGAM::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("PowerDelayTime", powerDelayTime);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PowerDelayTime must be specified");
        }
    }
    if (ok) {
        ok = data.Read("RTStartEvent", rtStartEvent);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "RTStartEvent must be specified");
        }
    }
    if (ok) {
        ok = data.Read("RTStopEvent", rtStopEvent);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "RTStopEvent must be specified");
        }
    }
    if (ok) {
        ok = data.Read("RTStopTime", rtStopTime);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "rtStopTime must be specified");
        }
    }
    if (ok) {
        ok = data.Read("PowerOnCommand", powerOnCommand);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PowerOnCommand must be specified");
        }
    }
    if (ok) {
        ok = data.Read("PowerOffCommand", powerOffCommand);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PowerOffCommand must be specified");
        }
    }
    if (ok) {
        ok = data.Read("Offline", offlineState);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Offline must be specified");
        }
    }
    if (ok) {
        ok = data.Read("OnlineOff", onlineOffState);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "OnlineOff must be specified");
        }
    }
    return ok;
}

bool ESDNCommandEmuGAM::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    if (selectedMode == GYROTRON_MODE_MANUAL) {
        lastPowerRequestTime = 0xEFFFFFFFFFFFFFFF;
    }
    else {
        lastPowerRequestTime = 0;
    }
    rtAbsoluteStopTime = 0;
    return true;
}

bool ESDNCommandEmuGAM::Execute() {
    using namespace MARTe;
    bool wasLoadRequested = loadRequested;
    loadRequested = false;

    uint64 absoluteTime = *timeSignal;
    uint64 relativeTime = 0xEFFFFFFFFFFFFFFF;

    *esdnCommand = powerOffCommand;
    *esdnEvent = rtStopEvent;

    if (absoluteTime > lastPowerRequestTime) {
        relativeTime = (absoluteTime - lastPowerRequestTime);
    }

    if (*triggerSignal == 1u) {
        if (rtAbsoluteStopTime == 0) {
            //First time the trigger was detected. Shift the stop time accordingly.
            rtAbsoluteStopTime = (absoluteTime + rtStopTime);
        }
        if (absoluteTime < rtAbsoluteStopTime) {
            *esdnEvent = rtStartEvent;
        }
    }
    if (*esdnEvent == rtStartEvent) {
        if (selectedGyrotron == RU_GYROTRON) {
            if ((relativeTime > powerDelayTime) && (relativeTime <= powerTotalTime)) {
                *esdnCommand = powerOnCommand;
            }
        }
        else {
            *esdnCommand = powerOnCommand;
        }
    }

    bool ret = true;
    if (wasLoadRequested) {
        if (*rtState == offlineState) {
            selectedMode = *selectedModeSignal;
            selectedGyrotron = *selectedGyrotronSignal;
            if (selectedGyrotron == RU_GYROTRON) {
                REPORT_ERROR(ErrorManagement::Information, "RU Gyrotron selected");
            }
            else if (selectedGyrotron == EU_GYROTRON) {
                REPORT_ERROR(ErrorManagement::Information, "EU Gyrotron selected");
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "UNKNOWN Gyrotron selected");
                ret = false;
            }


            powerTotalTime = powerDelayTime;
            *pulseDurationMillis += 1u;//IMPORTANT - 06/11/2018 I need to add 1 ms to get 1 ms in the output
            powerTotalTime += (*pulseDurationMillis * 1000u);
            powerTotalTime += (*pulseDurationSeconds * 1000000u);
            powerTotalTime += (*pulseDurationMinutes * 60u * 1000000u);
            REPORT_ERROR(ErrorManagement::Information, "Power delay time = %u Loaded power total time = %u (including one extra fake ms)", powerDelayTime, (powerTotalTime - powerDelayTime));
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Refused to Load with a RTState != Offline");
        }
    }
    return ret;
}

MARTe::ErrorManagement::ErrorType ESDNCommandEmuGAM::Load() {
    loadRequested = true;
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType ESDNCommandEmuGAM::PutManualPower() {
    using namespace MARTe;
    if (selectedMode == GYROTRON_MODE_MANUAL) {
        if (*rtState == onlineOffState) {
            lastPowerRequestTime = *timeSignal;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Refused to PutManualPower with a RTState != OnlineOff");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Refused to PutManualPower with a SelectedMode != GYROTRON_MODE_MANUAL");
    }
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType ESDNCommandEmuGAM::StopPower() {
    using namespace MARTe;
    lastPowerRequestTime = 0xEFFFFFFFFFFFFFFF;
    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(ESDNCommandEmuGAM, "1.0")
CLASS_METHOD_REGISTER(ESDNCommandEmuGAM, Load)
CLASS_METHOD_REGISTER(ESDNCommandEmuGAM, PutManualPower)
CLASS_METHOD_REGISTER(ESDNCommandEmuGAM, StopPower)

