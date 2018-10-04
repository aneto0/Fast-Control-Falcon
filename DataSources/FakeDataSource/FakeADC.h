/**
 * @file FakeADC.h
 * @brief Header file for class FakeADC
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

 * @details This header file contains the declaration of the class FakeADC
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_FAKEDATASOURCE_FAKEADC_H_
#define DATASOURCES_FAKEDATASOURCE_FAKEADC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryDataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Fakes the data of any given DataSource.
 */
class FakeADC: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
FakeADC    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FakeADC();

    /**
     * @see DataSourceI::Initialise
     * @brief Simulates the ADC0 value = ADC0Value after ADC0Counts counter cycles.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @see DataSourceI::Synchronise
     */
    virtual bool Synchronise();

    /**
     * @return MemoryMapInputBroker, MemoryMapOutputBroker, MemoryMapSynchrinisedInputBroker or MemoryMapSynchronisedOutputBroker, depending on the Frequency and Trigger parameters
     */
    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction);

    /**
     * @brief Restarts the time counter to zero,
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName);

    /**
     * @brief Allocates the fake memory
     */
    virtual bool AllocateMemory();

    /**
     * @brief Gets the fake memory addresses
     */
    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress);
private:
    /**
     * The time to sleep in synchronise.
     */
    MARTe::float64 sleepPeriod;

    /**
     * The ADC0 signal
     */
    MARTe::int16 **adcs;

    /**
     * The number of ADCs
     */
    MARTe::uint32 numberOfADCs;

    /**
     * The number of samples per ADC
     */
    MARTe::uint32 numberOfSamples;

    /**
     * The ADC0 value to simulate
     */
    MARTe::int16 adc0Value;

    /**
     * The time signal
     */
    MARTe::uint32 timeSignal;

    /**
     * The counter signal
     */
    MARTe::uint32 counterSignal;

    /**
     * True if the ADC0 is to be simulated
     */
    bool simulateADC;

    /**
     * The ADC0 counts to wait before setting the *adc0 = to adc0Value
     */
    MARTe::uint32 adc0Counts;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_FAKEDATASOURCE_FAKEADC_H_ */
