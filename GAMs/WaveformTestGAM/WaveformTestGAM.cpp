/**
 * @file WaveformTestGAM.cpp
 * @brief Source file for class WaveformTestGAM
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
 * the class WaveformTestGAM (public, protected, and private). Be aware that some 
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
#include "WaveformTestGAM.h"

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

WaveformTestGAM::WaveformTestGAM() :
        MARTe::GAM() {
    using namespace MARTe;
    startTimes = NULL_PTR(uint64 *);
    endTimes = NULL_PTR(uint64 *);
    frequency = 0;
    amplitude = 0;
    wavetype = 0;
    numberOfElements = 0;
    currentTimeWindow = 0u;
    timeInput = NULL_PTR(uint32 *);
    waveOutput = NULL_PTR(float32 *);
    numberOfWindows = 0u;
}

WaveformTestGAM::~WaveformTestGAM() {
    using namespace MARTe;
    if (startTimes != NULL_PTR(uint64 *)) {
        delete startTimes;
    }
    if (endTimes != NULL_PTR(uint64 *)) {
        delete endTimes;
    }
}

bool WaveformTestGAM::Setup() {
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
        ok = (GetSignalType(OutputSignals, 0u) == Float32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType(InputSignals, 0u) != Float32Bit");
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(OutputSignals, 0u, numberOfElements);
    }

    timeInput = static_cast<uint32 *>(GetInputSignalMemory(0u));
    waveOutput = static_cast<float32 *>(GetOutputSignalMemory(0u));
    return ok;
}

bool WaveformTestGAM::Initialise(MARTe::StructuredDataI & data) {
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
        StreamString waveform;
        ok = data.Read("WaveformType", waveform);
        waveform.Seek(0LLU);
        if (waveform == "Sine") {
            wavetype = SINE_WAVEFORM;
        }
        else if (waveform == "Cosine") {
            wavetype = COSINE_WAVEFORM;
        }
        else if (waveform == "Square") {
            wavetype = SQUARE_WAVEFORM;
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported WaveformType specified");
            ok = false;
        }
    }
    if (ok) {
        ok = data.Read("Amplitude", amplitude);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Amplitude must be specified");
        }
    }
    if (ok) {
        ok = data.Read("Frequency", frequency);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Frequency must be specified");
        }
    }
    return ok;
}

bool WaveformTestGAM::Execute() {
    using namespace MARTe;
    uint32 i;
    uint32 currentTime = *timeInput;
    bool run = ((startTimes[currentTimeWindow] <= currentTime) && (endTimes[currentTimeWindow] >= currentTime));
    if (run) {
        for (i = 0u; i < numberOfElements; i++) {
            if (wavetype == SINE_WAVEFORM) {
                waveOutput[i] = amplitude * sin(2 * PI * frequency * currentTime * 1e-6 * i);
            }
            else if (wavetype == COSINE_WAVEFORM) {
                waveOutput[i] = amplitude * cos(2 * PI * frequency * currentTime * 1e-6 * i);
            }
            else if (wavetype == SQUARE_WAVEFORM) {
                float64 currentSign = sin(2 * PI * frequency * currentTime * 1e-6 * i);
                if (currentSign >= 0) {
                    waveOutput[i] = amplitude;
                }
                else {
                    waveOutput[i] = -amplitude;
                }
            }
        }
    }
    else {
        if (currentTime > endTimes[currentTimeWindow]) {
            currentTimeWindow++;
            if (currentTimeWindow == numberOfWindows) {
                currentTimeWindow = 0;
            }
        }
    }
    return true;
}

CLASS_REGISTER(WaveformTestGAM, "1.0")

