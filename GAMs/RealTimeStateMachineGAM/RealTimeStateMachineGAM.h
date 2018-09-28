/**
 * @file RealTimeStateMachineGAM.h
 * @brief Header file for class RealTimeStateMachineGAM
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

 * @details This header file contains the declaration of the class RealTimeStateMachineGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMS_REALTIMESTATEMACHINEGAM_REALTIMESTATEMACHINEGAM_H_
#define GAMS_REALTIMESTATEMACHINEGAM_REALTIMESTATEMACHINEGAM_H_

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
 * @brief GAM which implements the real-time state machine as per the design document.
 *
 * The PLC Abort function is registered as an RPC.
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMRTSM = {
 *     Class = RealTimeStateMachineGAM
 *     Offline = 1 //Code which describes the Offline State
 *     OnlineOff = 2 //Code which describes the Online-off State
 *     Online = 3 //Code which describes the Online State
 *     Change = 4 //Code which describes the Change State
 *     End = 5 //Code which describes the End State
 *     Fault = 15 // Code which describes the Fault State
 *
 *     PLCOnline = 1 //Code which describes the PLC online state
 *
 *     SDNRTStart = 1 //Code which describes the SDN RT START event
 *     SDNRTStop = 0 //Code which describes the SDN RT STOP event
 *
 *     SDNPowerOn = 1 //Code which describes the power on command
 *
 *     PowerSupplyTrigger = 2 //The value to write in the DIO in order trigger the power supply write
 *     InputSignals = {
 *         PLCState = { //The state as reported by the PLC
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         SDNEvent = {//The event received from the SDN (ECPC master)
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         SDNPowerCommand = {//The power command received from the SDN (ECPC master)
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *     }
 *     OutputSignals = {
 *         State = {//The current state encoded as described above.
 *              DataSource = DDB1
 *              Type = uint8 //The type shall be uint8
 *         }
 *         PORT0 = { //Outputs zero or the value of PowerSupplyTrigger
 *              DataSource = NI6368_DIO_0
 *              Type = uint32 //The type shall be uint32
 *         }
 *    }
 *}
 */
class RealTimeStateMachineGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
RealTimeStateMachineGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RealTimeStateMachineGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == 3 &&
     *  - GetNumberOfOutputSignals() == 2 &&
     *  - GetSignalType(*, *) == UnsignedInteger8Bit &&
     *  - GetSignalType(OutputSignals, 1) == UnsignedInteger32Bit
     *  @return true if the conditions above are met.
     */
    virtual bool Setup();

    /**
     * @brief The configuration data detailed in the class description
     * @return true if all the compulsory parameters are set.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Verifies if the threshold has been reach. When so corrects the time signal.
     * @return true.
     */
    virtual bool Execute();

    /**
     * @brief Requests an abort
     */
    virtual MARTe::ErrorManagement::ErrorType Abort();

private:

    /**
     * Offline state
     */
    MARTe::uint8 offlineStateCode;

    /**
     * Online state
     */
    MARTe::uint8 onlineStateCode;

    /**
     * Online-off state
     */
    MARTe::uint8 onlineOffStateCode;

    /**
     * Change state
     */
    MARTe::uint8 changeStateCode;

    /**
     * End state
     */
    MARTe::uint8 endStateCode;

    /**
     * Fault state
     */
    MARTe::uint8 faultStateCode;

    /**
     * The power supply trigger value
     */
    MARTe::uint32 powerSupplyTrigger;

    /**
     * PLC online
     */
    MARTe::uint8 plcOnline;

    /**
     * SDN RT Start
     */
    MARTe::uint8 sdnRTStart;

    /**
     * SDN Power command
     */
    MARTe::uint8 sdnPower;

    /**
     * SDN RT Stop
     */
    MARTe::uint8 sdnRTStop;

    /**
     * PLC state signal
     */
    MARTe::uint8 *plcState;

    /**
     * SDN power command signal
     */
    MARTe::uint8 *sdnPowerCommand;

    /**
     * SDN event signal
     */
    MARTe::uint8 *sdnEvent;

    /**
     * The current output state
     */
    MARTe::uint8 *outputState;

    /**
     * The trigger output value
     */
    MARTe::uint32 *trigger;

    /**
     * True when an abort from the PLC is requested.
     */
    bool abortRequested;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMS_REALTIMESTATEMACHINEGAM_REALTIMESTATEMACHINEGAM_H_ */
