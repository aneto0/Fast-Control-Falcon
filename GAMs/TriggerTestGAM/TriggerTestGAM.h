/**
 * @file TriggerTestGAM.h
 * @brief Header file for class TriggerTestGAM
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

 * @details This header file contains the declaration of the class TriggerTestGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TRIGGERTESTGAM_H_
#define TRIGGERTESTGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief The objective of this GAM is to generate time windows of triggers which can be
 * fed to the DIO of the system.
 * @details One and only one input signal with the time input vector (type shall be uint32)
 * One and only one output signal with the generated digital value (type shall be uint32).
 * See Initialise() for further details in the configuration.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +TriggerTest = {
 *     Class = TriggerTestGAM
 *     EnabledOutputValue = 2 //Compulsory. The value that the output signal shall have inside a valid time window.
 *     DisabledOutputValue = 0 //Compulsory. The value that the output signal shall have outside a valid time window.
 *     TimeWindows = { //At least one time window with a start and an end time
 *         T1 = {
 *             StartTime = 0.0
 *             EndTime = 1000.0
 *         }
 *         T2 = {
 *             StartTime = 2000.0
 *             EndTime = 3000.0
 *         }
 *         ...
 *     }
 *     InputSignals = {
 *         Time = {
 *             DataSource = DDB1
 *             Type = uint32 //Only accepted type
 *         }
 *     }
 *     OutputSignals = {
 *         PORT0 = {
 *             DataSource = NI6368_DIO_0
 *             Trigger = 1
 *             Type = uint32 //Only accepted type
 *         }
 *     }
 * }
 */
class TriggerTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
TriggerTestGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TriggerTestGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 1u &&
     *  - GetNumberOfOutputSignals() == 1u &&
     *  - GetSignalType(InputSignals, 0u) != UnsignedInteger32Bit &&
     *  - GetSignalType(OutputSignals, 0u) != UnsignedInteger32Bit
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Generates the digital output value.
     */
    virtual bool Execute();

private:
    /**
     * The trigger windows start times.
     */
    MARTe::uint64 *startTimes;

    /**
     * The trigger windows end times.
     */
    MARTe::uint64 *endTimes;

    /**
     * The time input signal.
     */
    MARTe::uint32 *timeInput;

    /**
     * Last time input value.
     */
    MARTe::uint32 lastTimeInput;

    /**
     * The trigger signal.
     */
    MARTe::uint32 *output;

    /**
     * The value to which to set the trigger when the output is enabled.
     */
    MARTe::uint32 enabledOutputValue;

    /**
     * The value to which to set the trigger when the output is disabled.
     */
    MARTe::uint32 disabledOutputValue;

    /**
     * Index with the value of the current time window.
     */
    MARTe::uint32 currentTimeWindow;

    /**
     * The number of configured time windows.
     */
    MARTe::uint32 numberOfWindows;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TRIGGERTESTGAM_H_ */

