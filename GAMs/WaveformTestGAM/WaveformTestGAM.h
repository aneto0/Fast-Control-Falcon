/**
 * @file WaveformTestGAM.h
 * @brief Header file for class WaveformTestGAM
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

 * @details This header file contains the declaration of the class WaveformTestGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMTESTGAM_H_
#define WAVEFORMTESTGAM_H_

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
 * @brief The objective of this GAM is to generate time windows of data which can be
 * fed to the DAC of the system.
 * @details One and only one input signal with the time input vector (type shall be uint32)
 * One and only one output signal with the generated signal (type shall be float32).
 * See Initialise() for further details in the configuration.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +WaveformTestGAM = {
 *     Class = WaveformTestGAM
 *     WaveformType = Square //Compulsory. The type of the waveform (Cosine, Sine or Square)
 *     Frequency = 10 //Compulsory. The frequency of the waveform
 *     Amplitude = 1 //Compulsory. The amplitude of the waveform
 *     SamplingPeriod = 500e-9 //Compulsory. The sampling period at which the signal should be generated.
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
 *         Sine = {
 *             DataSource = DDB1
 *             Type = float32 //Only accepted type
 *         }
 *     }
 * }
 */
class WaveformTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
WaveformTestGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~WaveformTestGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 1u &&
     *  - GetNumberOfOutputSignals() == 1u &&
     *  - GetSignalType(InputSignals, 0u) != UnsignedInteger32Bit &&
     *  - GetSignalType(OutputSignals, 0u) != Float32Bit
     */
    virtual bool Setup();

    /**
     * @brief The following configuration data is required:
     * WaveformType = Square //The type of the waveform (Cosine, Sine or Square)
     * Frequency = 10 //The frequency of the waveform
     * Amplitude = 1 //The amplitude of the waveform
     * SamplingPeriod = 500e-9 //The sampling period at which the signal should be generated.
     * TimeWindows = { //At least one time window with a start and an end time
     *     T1 = {
     *         StartTime = 0.0
     *         EndTime = 1000.0
     *     }
     *     T2 = {
     *         StartTime = 2000.0
     *         EndTime = 3000.0
     *     }
     *     ...
     * }
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Generates the waveform.
     */
    virtual bool Execute();

private:
    /**
     * The waveform time windows start times.
     */
    MARTe::uint64 *startTimes;

    /**
     * The waveform time windows end times.
     */
    MARTe::uint64 *endTimes;

    /**
     * The number of elements of the waveform.
     */
    MARTe::uint32 numberOfElements;

    /**
     * The amplitude of the waveform.
     */
    MARTe::float32 amplitude;

    /**
     * The frequency of the waveform.
     */
    MARTe::float32 frequency;

    /**
     * The sampling period of the waveform.
     */
    MARTe::float64 samplingPeriod;

    /**
     * The type of waveform (sine, cosine, square).
     */
    MARTe::uint32 wavetype;

    /**
     * The time input signal.
     */
    MARTe::uint32 *timeInput;

    /**
     * Last time input value.
     */
    MARTe::uint32 lastTimeInput;

    /**
     * The output signal.
     */
    MARTe::float32 *waveOutput;

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

#endif /* WAVEFORMTESTGAM_H_ */

