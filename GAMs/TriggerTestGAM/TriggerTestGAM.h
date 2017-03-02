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
 * @brief TODO
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
     * @brief TODO.
     */
    virtual bool Setup();

    /**
     * @brief TODO.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief TODO.
     */
    virtual bool Execute();

private:
    /**
     * TODO
     */
    MARTe::uint64 *startTimes;

    /**
     * TODO
     */
    MARTe::uint64 *endTimes;

    /**
     * TODO
     */
    MARTe::uint32 *timeInput;

    /**
     * TODO
     */
    MARTe::uint32 *output;

    /**
     * TODO
     */
    MARTe::uint32 enabledOutputValue;

    /**
     * TODO
     */
    MARTe::uint32 disabledOutputValue;

    /**
     * TODO
     */
    MARTe::uint32 currentTimeWindow;

    /**
     * TODO
     */
    MARTe::uint32 numberOfWindows;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TRIGGERTESTGAM_H_ */

