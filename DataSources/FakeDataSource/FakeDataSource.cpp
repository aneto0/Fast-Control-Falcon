/**
 * @file FakeDataSource.cpp
 * @brief Source file for class FakeDataSource
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
 * the class FakeDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FakeDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FakeDataSource::FakeDataSource() :
        MemoryDataSourceI() {
    sleepPeriod = 0.;
    counter = 0u;
}

FakeDataSource::~FakeDataSource() {

}

bool FakeDataSource::Synchronise() {
    using namespace MARTe;
    if (sleepPeriod > 0.) {
        Sleep::Sec(sleepPeriod);
        *reinterpret_cast<uint32 *>(memory) = counter;
        uint32 sleepPeriodUS = static_cast<uint32>(sleepPeriod * 1e6);
        *reinterpret_cast<uint32 *>(&memory[4u]) = (counter * sleepPeriodUS);
        counter++;
    }
    return true;
}

const MARTe::char8* FakeDataSource::GetBrokerName(MARTe::StructuredDataI& data, const MARTe::SignalDirection direction) {
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

bool FakeDataSource::PrepareNextState(const MARTe::char8* const currentStateName, const MARTe::char8* const nextStateName) {
    counter = 0u;
    return true;
}

CLASS_REGISTER(FakeDataSource, "1.0")
