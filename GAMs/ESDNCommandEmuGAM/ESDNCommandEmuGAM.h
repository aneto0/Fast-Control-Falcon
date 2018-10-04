/**
 * @file ESDNCommandEmuGAM.h
 * @brief Header file for class ESDNCommandEmuGAM
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

 * @details This header file contains the declaration of the class ESDNCommandEmuGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ESDNCOMMANDEMUGAM_H_
#define ESDNCOMMANDEMUGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "MessageI.h"
#include "StatefulI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which emulates the ESDN commands (in particular the power on command, and the RT_START and the RT_STOP events).
 *
 * @details The RT_START will be latched to one until the RT_STOP is received.
 *
 * The total power duration will be computed only when the Load RPC is called and if the RTState is Offline.
 *
 * If the SelectedGyrotron is EU then the ESDNCommand will be latched to one, otherwise:
 *
 * If the SelectedGyrotron is RU and If PulseMode is MANUAL the ESDNCommand will be set to one when the time ellapsed from the last power request is > PowerDelayTime and < then the total power duration
 *
 * If the SelectedGyrotron is RU and If PulseMode is PRE-PROGRAMMED the ESDNCommand will be set to one when the time ellapsed from zero is > PowerDelayTime and < then the total power duration
 *
 * The following functions are registered as RPCs: Load, PutManualPower and StopPower
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMESDNEMU = {
 *     Class = ESDNCommandEmuGAM
 *     PowerDelayTime = 100000 //Compulsory. Delay in micro-seconds before putting power on the ESDNCommand.
 *     Offline = 8 //Code which identifies the RT Offline state.
 *     OnlineOff = 240 //Code which describes the Online-off State
 *     RTStartEvent = 1 //ID which identifies the RT_START event.
 *     RTStopEvent = 0 //ID which identifies the RT_STOP event.
 *     RTStopTime = 3600000 //Time in micro-seconds at which to stop the pulse.
 *     PowerOnCommand = 1 //ID which identifies the Power ON command.
 *     PowerOffCommand = 0 //ID which identifies the Power OFF command.
 *     InputSignals = {
 *         Time = {//Time signal in micro-seconds
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *         Trigger = { //Signal which emulates the RTStart
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         RTState = {//The RT state as computed by the RealTimeStateMachineGAM
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint32
 *         }
 *         PulseDurationMillis = {//The expected pulse duration in milli-seconds
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *         PulseDurationSeconds = {//The expected pulse duration in seconds
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *         PulseDurationMinutes = {//The expected pulse duration in minutes
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *         SelectedGyrotron = {//The selected gyrotron (EU vs RU)
 *              DataSource = EPICSCAInput
 *              Type = uint32
 *         }
 *         PulseMode = {//The pulse mode can be either MANUAL or PRE-PROGRAMMED
 *              DataSource = EPICSCAInput
 *              Type = uint32
 *         }
 *     }
 *     OutputSignals = {
 *         ESDNEvent = {//The simulated ESDN event
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         ESDNCommand = { //The command (power on/off)
 *             DataSource = DDB1
 *             Type = uint8 //The type shall be uint8
 *         }
 *    }
 *}
 */
class ESDNCommandEmuGAM: public MARTe::GAM, public MARTe::MessageI, public MARTe::StatefulI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
ESDNCommandEmuGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ESDNCommandEmuGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 8 &&
     *  - GetNumberOfOutputSignals() == 2 &&
     *  - GetSignalType(InputSignals, [0, 3, 4, 5, 6, 7]) == UnsignedInteger32Bit &&
     *  - GetSignalType(InputSignals, [1, 2]) == UnsignedInteger8Bit &&
     *  - GetSignalType(OutputSignals, *) == UnsignedInteger8Bit
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

    /**
     * @brief To be called when a new pulse duration is to be loaded.
     */
    virtual MARTe::ErrorManagement::ErrorType Load();

    /**
     * @brief In manual mode trigger a power request
     */
    virtual MARTe::ErrorManagement::ErrorType PutManualPower();

    /**
     * @brief Forces to terminate the power.
     */
    virtual MARTe::ErrorManagement::ErrorType StopPower();

private:
    /**
     * The input time signal
     */
    MARTe::uint32 *timeSignal;

    /**
     * The current trigger signal
     */
    MARTe::uint8 *triggerSignal;

    /**
     * The current RTState signal
     */
    MARTe::uint8 *rtState;

    /**
     * The milli-seconds pulse duration signal
     */
    MARTe::uint32 *pulseDurationMillis;

    /**
     * The seconds pulse duration signal
     */
    MARTe::uint32 *pulseDurationSeconds;

    /**
     * The minutes pulse duration signal
     */
    MARTe::uint32 *pulseDurationMinutes;

    /**
     * The selected gyrotron signal
     */
    MARTe::uint32 *selectedGyrotronSignal;

    /**
     * The gyrotron operation mode signal
     */
    MARTe::uint32 *selectedModeSignal;

    /**
     * The selected gyrotron
     */
    MARTe::uint32 selectedGyrotron;

    /**
     * The selected operation mode
     */
    MARTe::uint32 selectedMode;

    /**
     * The simulated ESDN event signal
     */
    MARTe::uint8 *esdnEvent;

    /**
     * The simulated ESDN command signal
     */
    MARTe::uint8 *esdnCommand;

    /**
     * Time in us to delay the power on command.
     */
    MARTe::uint64 powerDelayTime;

    /**
     * Time in us to put power
     */
    MARTe::uint64 powerTotalTime;

    /**
     * True when a load is requested.
     */
    bool loadRequested;

    /**
     * Value which identifies the RT_START.
     */
    MARTe::uint8 rtStartEvent;

    /**
     * Value which identifies the RT_STOP.
     */
    MARTe::uint8 rtStopEvent;

    /**
     * Value which identifies the request to put power.
     */
    MARTe::uint8 powerOnCommand;

    /**
     * Value which identifies the request to put power.
     */
    MARTe::uint8 powerOffCommand;

    /**
     * Value which identifies the offline state.
     */
    MARTe::uint8 offlineState;

    /**
     * Value which identifies the onlineOff state.
     */
    MARTe::uint8 onlineOffState;

    /**
     * Time at which to stop the experiment.
     */
    MARTe::uint64 rtStopTime;

    /**
     * Time of the stop time w.r.t. to when the trigger was set to 1 for the first time
     */
    MARTe::uint64 rtAbsoluteStopTime;

    /**
     * Time at which the last power request was received.
     */
    MARTe::uint64 lastPowerRequestTime;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ESDNCOMMANDEMUGAM_H_ */

