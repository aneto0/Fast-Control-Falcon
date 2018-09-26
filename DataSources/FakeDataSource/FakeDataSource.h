/**
 * @file FakeDataSource.h
 * @brief Header file for class FakeDataSource
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

 * @details This header file contains the declaration of the class FakeDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_FAKEDATASOURCE_FAKEDATASOURCE_H_
#define DATASOURCES_FAKEDATASOURCE_FAKEDATASOURCE_H_

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
class FakeDataSource: public MARTe::MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
FakeDataSource    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~FakeDataSource();

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

private:
    /**
     * The time to sleep in synchronise.
     */
    MARTe::float64 sleepPeriod;


    /**
     * The counter incremented at every synchronise.
     */
    MARTe::uint32 counter;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_FAKEDATASOURCE_FAKEDATASOURCE_H_ */
