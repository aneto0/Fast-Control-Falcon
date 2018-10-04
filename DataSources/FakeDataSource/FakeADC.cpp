/**
 * @file FakeADC.cpp
 * @brief Source file for class FakeADC
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
 * the class FakeADC (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FakeADC.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FakeADC::FakeADC() :
        DataSourceI() {
    sleepPeriod = 0.;
    adc0Counts = 0u;
    adc0Value = 0u;
    numberOfSamples = 2000u;
    numberOfADCs = 16u;
    adcs = NULL_PTR(MARTe::int16 **);
    timeSignal = 0u;
    counterSignal = 0u;
    simulateADC = false;
}

FakeADC::~FakeADC() {
    using namespace MARTe;

    if (adcs != NULL_PTR(MARTe::int16 **)) {
        uint32 n;
        for (n = 0; n < numberOfADCs; n++) {
            delete adcs[n];
        }
        delete[] adcs;
    }
}

bool FakeADC::Initialise(MARTe::StructuredDataI &data) {
    bool ok = DataSourceI::Initialise(data);
    if (ok) {
        simulateADC = data.Read("ADC0Counts", adc0Counts);
        if (simulateADC) {
            simulateADC = data.Read("ADC0Value", adc0Value);
        }
    }

    return ok;
}

bool FakeADC::AllocateMemory() {
    using namespace MARTe;
    uint32 n;
    adcs = new int16*[numberOfADCs];
    for (n = 0; n < numberOfADCs; n++) {
        adcs[n] = new int16[numberOfSamples];
        uint32 z;
        for (z = 0; z < numberOfSamples; z++) {
            adcs[n][z] = 0u;
        }

    }

    return true;
}

bool FakeADC::Synchronise() {
    using namespace MARTe;
    if (sleepPeriod > 0.) {
        Sleep::Sec(sleepPeriod);
        uint32 sleepPeriodUS = static_cast<uint32>(sleepPeriod * 1e6);
        timeSignal = (counterSignal * sleepPeriodUS);
        counterSignal++;
    }
    if (simulateADC) {
        uint32 z;
        if (counterSignal > adc0Counts) {
            for (z = 0; z < numberOfSamples; z++) {
                adcs[0][z] = adc0Value;
            }

        }
        else {
            for (z = 0; z < numberOfSamples; z++) {
                adcs[0][z] = 0;
            }
        }
    }
    return true;
}

const MARTe::char8* FakeADC::GetBrokerName(MARTe::StructuredDataI& data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    float64 frequency = 0.;
    const char8 *brokerName = "MemoryMapInputBroker";
    if (direction == InputSignals) {
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                sleepPeriod = 1 / frequency;
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
    }
    else {
        uint32 trigger;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    return brokerName;

}

bool FakeADC::PrepareNextState(const MARTe::char8* const currentStateName, const MARTe::char8* const nextStateName) {
    counterSignal = 0u;
    return true;
}

bool FakeADC::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    if (signalIdx == 0u) {
        signalAddress = &counterSignal;
    }
    else if (signalIdx == 1u) {
        signalAddress = &timeSignal;
    }
    else {
        signalAddress = adcs[signalIdx - 2];
    }
    return true;
}

CLASS_REGISTER(FakeADC, "1.0")
