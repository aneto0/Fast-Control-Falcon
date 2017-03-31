/**
 * @file TimeCorrectionGAM.h
 * @brief Header file for class TimeCorrectionGAM
 * @date 30/03/2017
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

 * @details This header file contains the declaration of the class TimeCorrectionGAM
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
 * @brief GAM which corrects the time vector against the time of a given analogue event.
 * @details Until this event occurs the trigger signal (from the TriggerMaskGAM) will be forced to be disabled.
 * The corrected time signal will be equal to CORRECTED_TIME[k] += NUMBER_OF_SAMPLES_ADC * ADC_PERIOD, with CORRECTED_TIME[0] = NUMBER_OF_SAMPLES_ADC * ADC_PERIOD - TIME_ANALOGUE_EVENT_MICRO_SEC and CORRECTED_TRIGGER[k] = INPUT_TRIGGER[k]
 * Before the event is detected  CORRECTED_TIME[k] = 0 and CORRECTED_TRIGGER[k] = 0
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMTC = {
 *     Class = TimeCorrectionGAM
 *     AssertCycles = 3 //Compulsory > 0. Number of consecutive cycles above which the analogue signal shall be greater (in absolute value) of the Threshold
 *     Threshold = 300 //Compulsory. Threshold above which the analogue event is considered to be detected.
 *     TimePeriod = 500e-9 //Compulsory. Time signal period.
 *     InputSignals = {
 *         ADC = {//An analogue signal where the threshold will be detected
 *              DataSource = DDB1
 *              Type = int16 //The type shall be int16
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
 *         CorrectedTrigger = { //A corrected output time signal shall be specified and shall be set in position zero.
 *             DataSource = DDB1
 *             Type = uint8 //The type shall be uint8
 *         }
 *     }
 *}
 */
class TimeCorrectionGAM: public MARTe::GAM, public MARTe::StatefulI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
TimeCorrectionGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TimeCorrectionGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 3 &&
     *  - GetNumberOfOutputSignals() == 2 &&
     *  - GetSignalType(InputSignals, 0) == UnsignedInteger32Bit &&
     *  - GetSignalType(InputSignals, 1) == SignedInteger16Bit &&
     *  - GetSignalType(InputSignals, 2) == UnsignedInteger8Bit &&
     *  - GetSignalType(OutputSignals, 0) == UnsignedInteger32Bit &&
     *  - GetSignalType(OutputSignals, 1) == UnsignedInteger8Bit &&
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
     * The corrected trigger output signal memory
     */
    MARTe::uint8 *correctedTriggerSignal;

    /**
     * The analogue input signal memory
     */
    MARTe::int16 *analogueInputSignal;

    /**
     * The number of analogue input samples
     */
    MARTe::uint32 numberOfSamples;

    /**
     * The threshold set by the user.
     */
    MARTe::int16 threshold;

    /**
     * The assert set by the user.
     */
    MARTe::uint32 assertCycles;

    /**
     * Counter that goes to zero when the threshold has been reached, i.e. when |*analogueInputSignal| >= |threshold|.
     */
    MARTe::uint32 assertCounter;

    /**
     * Counter that goes to zero when the threshold has been reached, i.e. when |*analogueInputSignal| >= |threshold|.
     */
    MARTe::float64 timeSignalPeriod;

    /**
     * Correction value to apply to the time signal
     */
    MARTe::uint32 timeCorrection;

    /**
     * The time to increment at every cycle
     */
    MARTe::uint32 cycleTimeIncrement;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMECORRECTIONGAM_H_ */

