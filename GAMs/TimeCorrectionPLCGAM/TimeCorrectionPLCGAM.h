/**
 * @file TimeCorrectionPLCGAM.h
 * @brief Header file for class TimeCorrectionPLCGAM
 * @date 27/09/2018
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

 * @details This header file contains the declaration of the class TimeCorrectionPLCGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMECORRECTIONGAM_H_
#define TIMECORRECTIONGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "StatefulI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which corrects the time vector against the time on which the system goes for the first time to the OnlineOff state (this is Reset every time the main state machine cycles).
 * @details Until this event occurs the trigger signal (from the TriggerMaskGAM) will be forced to be disabled. The trigger signal for slow mdsplus will also be disabled.
 * The corrected time signal will be equal to CORRECTED_TIME[k] += TimePeriod, with CORRECTED_TIME[0] = 0 and CORRECTED_TRIGGER[k] = INPUT_TRIGGER[k] and CORRECTED_TRIGGER_SLOW[k] = 1
 * Before the event is detected  CORRECTED_TIME[k] = 0 and CORRECTED_TRIGGER[k] = 0 and CORRECTED_TRIGGER_SLOW[k] = 0
 *
 * Note that if the RTState == End, the outputs will also be disabled.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMTC = {
 *     Class = TimeCorrectionPLCGAM
 *     TimePeriod = 1000 //Compulsory. Time signal period in micro-seconds.
 *     OnlineOff = 1 //ID which identifies the OnlineOff RT state.
 *     End = 2 //ID which identifies the End RT state.
 *     InputSignals = {
 *         RTState = {//Signal describing the RT state
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         Trigger = {//A trigger signal shall be specified and shall be set in position one.
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *     }
 *     OutputSignals = {
 *         CorrectedTime = {//A corrected output time signal shall be specified and shall be set in position zero.
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *         CorrectedTrigger = { //A corrected output trigger signal shall be specified and shall be set in position one
 *             DataSource = DDB1
 *             Type = uint8 //The type shall be uint8
 *         }
 *         CorrectedTriggerSlow = { //A corrected output slow trigger signal shall be specified and shall be set in position two
 *             DataSource = DDB1
 *             Type = uint8 //The type shall be uint8
 *         }
 *    }
 *}
 */
class TimeCorrectionPLCGAM: public MARTe::GAM, public MARTe::StatefulI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
TimeCorrectionPLCGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TimeCorrectionPLCGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 2 &&
     *  - GetNumberOfOutputSignals() == 3 &&
     *  - GetSignalType(InputSignals, 0) == UnsignedInteger8Bit &&
     *  - GetSignalType(InputSignals, 1) == UnsignedInteger8Bit &&
     *  - GetSignalType(OutputSignals, 0) == UnsignedInteger32Bit &&
     *  - GetSignalType(OutputSignals, 1) == UnsignedInteger8Bit &&
     *  - GetSignalType(OutputSignals, 2) == UnsignedInteger8Bit
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief see PrepareNextState
     */
    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName);

    /**
     * @brief Verifies if the threshold has been reach. When so corrects the time signal.
     * @return true.
     */
    virtual bool Execute();

private:
    /**
     * The corrected time output signal memory
     */
    MARTe::uint32 *correctedTimeSignal;

    /**
     * The trigger input signal memory
     */
    MARTe::uint8 *triggerSignal;

    /**
     * The RT state signal
     */
    MARTe::uint8 *rtStateSignal;

    /**
     * The corrected trigger output signal memory
     */
    MARTe::uint8 *correctedTriggerSignal;

    /**
     * The corrected trigger output signal for the slow signal
     */
    MARTe::uint8 *correctedTriggerSignalSlow;

    /**
     * The state number which identifies the OnlineOff state
     */
    MARTe::uint8 onlineOffCode;

    /**
     * The state number which identifies the End state
     */
    MARTe::uint8 endCode;

    /**
     * The time to increment at every cycle
     */
    MARTe::uint32 timePeriod;

    /**
     * True when the start event was detected.
     */
    bool startDetected;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMECORRECTIONGAM_H_ */

