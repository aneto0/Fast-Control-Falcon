/**
 * @file TriggerMaskGAM.h
 * @brief Header file for class TriggerMaskGAM
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

 * @details This header file contains the declaration of the class TriggerMaskGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TRIGGERMASKGAM_H_
#define TRIGGERMASKGAM_H_

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
 * @brief GAM which masks a digital input triggering signal. If the mask & the input signal is not zero,
 *  a value of 1 will be output, 0 otherwise.
 * The configuration syntax is (names and signal quantities are only given as an example):
 * +GAMTRMask = {
 *     Class = TriggerMaskGAM
 *     Mask = 0x1 //Compulsory. Mask which will be applied to the input signal
 *     InputSignals = {
 *         PORT0 = {//One and only one input signal shall be specified.
 *              DataSource = DDB1
 *              Type = uint32 //The type shall be uint32
 *         }
 *     }
 *     OutputSignals = {
 *         Trigger = { //One and only one output signal shall be specified.
 *             DataSource = DDB1
 *             Type = uint8 //The type shall be uint8
 *         }
 *     }
 *}
 */
class TriggerMaskGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
TriggerMaskGAM    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TriggerMaskGAM();

    /**
     * @brief Verifies that:
     *  - GetNumberOfInputSignals() == GetNumberOfOutputSignals() &&
     *  - GetNumberOfInputSignals() == 1 &&
     *  - GetSignalType(InputSignals, *) == UnsignedInteger32Bit &&
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
     * @brief Applies the mask to the input signal and if the mask & input is not zero a value of 1 will be written to the output signal, zero otherwise.
     * @return true.
     */
    virtual bool Execute();

private:
    /**
     * The input signal memory
     */
    MARTe::uint32 *input;

    /**
     * The mask to apply
     */
    MARTe::uint32 mask;

    /**
     * The output signal memory
     */
    MARTe::uint8 *output;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TRIGGERMASKGAM_H_ */

