/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// gpio.c - Driver for the gpio Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup gpio_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_PORT1_R__
#include "gpio.h"

#include <assert.h>

static const uint16_t GPIO_PORT_TO_BASE[] = {
        0x00,
#if defined(__MSP430_HAS_PORT1_R__)
        __MSP430_BASEADDRESS_PORT1_R__,
#elif defined(__MSP430_HAS_PORT1__)
        __MSP430_BASEADDRESS_PORT1__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT2_R__)
        __MSP430_BASEADDRESS_PORT2_R__,
#elif defined(__MSP430_HAS_PORT2__)
        __MSP430_BASEADDRESS_PORT2__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT3_R__)
        __MSP430_BASEADDRESS_PORT3_R__,
#elif defined(__MSP430_HAS_PORT3__)
        __MSP430_BASEADDRESS_PORT3__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT4_R__)
        __MSP430_BASEADDRESS_PORT4_R__,
#elif defined(__MSP430_HAS_PORT4__)
        __MSP430_BASEADDRESS_PORT4__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT5_R__)
        __MSP430_BASEADDRESS_PORT5_R__,
#elif defined(__MSP430_HAS_PORT5__)
        __MSP430_BASEADDRESS_PORT5__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT6_R__)
        __MSP430_BASEADDRESS_PORT6_R__,
#elif defined(__MSP430_HAS_PORT6__)
        __MSP430_BASEADDRESS_PORT6__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT7_R__)
        __MSP430_BASEADDRESS_PORT7_R__,
#elif defined(__MSP430_HAS_PORT7__)
        __MSP430_BASEADDRESS_PORT7__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT8_R__)
        __MSP430_BASEADDRESS_PORT8_R__,
#elif defined(__MSP430_HAS_PORT8__)
        __MSP430_BASEADDRESS_PORT8__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT9_R__)
        __MSP430_BASEADDRESS_PORT9_R__,
#elif defined(__MSP430_HAS_PORT9__)
        __MSP430_BASEADDRESS_PORT9__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT10_R__)
        __MSP430_BASEADDRESS_PORT10_R__,
#elif defined(__MSP430_HAS_PORT10__)
        __MSP430_BASEADDRESS_PORT10__,
#else
        0xFFFF,
#endif
#if defined(__MSP430_HAS_PORT11_R__)
        __MSP430_BASEADDRESS_PORT11_R__,
#elif defined(__MSP430_HAS_PORT11__)
        __MSP430_BASEADDRESS_PORT11__,
#else
        0xFFFF,
#endif
        0xFFFF,
#if defined(__MSP430_HAS_PORTJ_R__)
        __MSP430_BASEADDRESS_PORTJ_R__
#elif defined(__MSP430_HAS_PORTJ__)
        __MSP430_BASEADDRESS_PORTJ__
#else
        0xFFFF
#endif
};

//*****************************************************************************
//
//! \brief This function configures the selected Pin as output pin
//!
//! This function selected pins on a selected port as output pins.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register and bits of \b PxSEL register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsOutputPin(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PASEL) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PADIR) |= selectedPins;

        return;
}

//*****************************************************************************
//
//! \brief This function configures the selected Pin as input pin
//!
//! This function selected pins on a selected port as input pins.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register, bits of \b PxREN register and bits of
//! \b PxSEL register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsInputPin(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PASEL) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PADIR) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PAREN) &= ~selectedPins;
}

//*****************************************************************************
//
//! \brief This function configures the peripheral module function in the
//! output direction for the selected pin for either primary, secondary or
//! ternary module function modes
//!
//! This function configures the peripheral module function in the output
//! direction for the selected pin for either primary, secondary or ternary
//! module function modes. Accepted values for mode are
//! GPIO_PRIMARY_MODULE_FUNCTION, GPIO_SECONDARY_MODULE_FUNCTION, and
//! GPIO_TERNARY_MODULE_FUNCTION
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register and bits of \b PxSEL register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsPeripheralModuleFunctionOutputPin(uint8_t selectedPort,
                                                 uint16_t selectedPins
                                                 )
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PADIR) |= selectedPins;
        HWREG16(baseAddress + OFS_PASEL) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function configures the peripheral module function in the input
//! direction for the selected pin for either primary, secondary or ternary
//! module function modes.
//!
//! This function configures the peripheral module function in the input
//! direction for the selected pin for either primary, secondary or ternary
//! module function modes. Accepted values for mode are
//! GPIO_PRIMARY_MODULE_FUNCTION, GPIO_SECONDARY_MODULE_FUNCTION, and
//! GPIO_TERNARY_MODULE_FUNCTION
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register and bits of \b PxSEL register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsPeripheralModuleFunctionInputPin(uint8_t selectedPort,
                                                uint16_t selectedPins
                                                )
{
        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PADIR) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PASEL) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function sets output HIGH on the selected Pin
//!
//! This function sets output HIGH on the selected port's pin.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxOUT register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setOutputHighOnPin(uint8_t selectedPort,
                             uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAOUT) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function sets output LOW on the selected Pin
//!
//! This function sets output LOW on the selected port's pin.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxOUT register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setOutputLowOnPin(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAOUT) &= ~selectedPins;
}

//*****************************************************************************
//
//! \brief This function toggles the output on the selected Pin
//!
//! This function toggles the output on the selected port's pin.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxOUT register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_toggleOutputOnPin(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAOUT) ^= selectedPins;
}

//*****************************************************************************
//
//! \brief This function sets the selected Pin in input Mode with Pull Down
//! resistor
//!
//! This function sets the selected Pin in input Mode with Pull Down resistor.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register, bits of \b PxOUT register and bits of
//! \b PxREN register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsInputPinWithPullDownResistor(uint8_t selectedPort,
                                            uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PASEL) &= ~selectedPins;

        HWREG16(baseAddress + OFS_PADIR) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PAREN) |= selectedPins;
        HWREG16(baseAddress + OFS_PAOUT) &= ~selectedPins;
}

//*****************************************************************************
//
//! \brief This function sets the selected Pin in input Mode with Pull Up
//! resistor
//!
//! This function sets the selected Pin in input Mode with Pull Up resistor.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxDIR register, bits of \b PxOUT register and bits of
//! \b PxREN register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setAsInputPinWithPullUpResistor(uint8_t selectedPort,
                                          uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PASEL) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PADIR) &= ~selectedPins;
        HWREG16(baseAddress + OFS_PAREN) |= selectedPins;
        HWREG16(baseAddress + OFS_PAOUT) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function gets the input value on the selected pin
//!
//! This function gets the input value on the selected pin.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Valid values are:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! \return One of the following:
//!         - \b GPIO_INPUT_PIN_HIGH
//!         - \b GPIO_INPUT_PIN_LOW
//!         \n indicating the status of the pin
//
//*****************************************************************************
uint8_t GPIO_getInputPinValue(uint8_t selectedPort,
                              uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        uint16_t inputPinValue = HWREG16(baseAddress + OFS_PAIN) & (selectedPins);

        if (inputPinValue > 0)
                return GPIO_INPUT_PIN_HIGH;
        return GPIO_INPUT_PIN_LOW;
}

//*****************************************************************************
//
//! \brief This function enables the port interrupt on the selected pin
//!
//! This function enables the port interrupt on the selected pin. Note that
//! only Port 1,2, A have this capability.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_PA
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxIE register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_enableInterrupt(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAIE) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function disables the port interrupt on the selected pin
//!
//! This function disables the port interrupt on the selected pin. Note that
//! only Port 1,2, A have this capability.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_PA
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxIE register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_disableInterrupt(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAIE) &= ~selectedPins;
}

//*****************************************************************************
//
//! \brief This function gets the interrupt status of the selected pin
//!
//! This function gets the interrupt status of the selected pin. Note that only
//! Port 1,2, A have this capability.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_PA
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! \return Logical OR of any of the following:
//!         - \b GPIO_PIN0
//!         - \b GPIO_PIN1
//!         - \b GPIO_PIN2
//!         - \b GPIO_PIN3
//!         - \b GPIO_PIN4
//!         - \b GPIO_PIN5
//!         - \b GPIO_PIN6
//!         - \b GPIO_PIN7
//!         - \b GPIO_PIN8
//!         - \b GPIO_PIN9
//!         - \b GPIO_PIN10
//!         - \b GPIO_PIN11
//!         - \b GPIO_PIN12
//!         - \b GPIO_PIN13
//!         - \b GPIO_PIN14
//!         - \b GPIO_PIN15
//!         \n indicating the interrupt status of the selected pins [Default:
//!         0]
//
//*****************************************************************************
uint16_t GPIO_getInterruptStatus(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        return HWREG16(baseAddress + OFS_PAIFG) & selectedPins;
}

//*****************************************************************************
//
//! \brief This function clears the interrupt flag on the selected pin
//!
//! This function clears the interrupt flag on the selected pin. Note that only
//! Port 1,2,A have this capability.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_PA
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//!
//! Modified bits of \b PxIFG register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_clearInterruptFlag(uint8_t selectedPort, uint16_t selectedPins)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        HWREG16(baseAddress + OFS_PAIFG) &= ~selectedPins;
}

//*****************************************************************************
//
//! \brief This function selects on what edge the port interrupt flag should be
//! set for a transition
//!
//! This function selects on what edge the port interrupt flag should be set
//! for a transition. Values for edgeSelect should be
//! GPIO_LOW_TO_HIGH_TRANSITION or GPIO_HIGH_TO_LOW_TRANSITION.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//! \param edgeSelect specifies what transition sets the interrupt flag
//!        Valid values are:
//!        - \b GPIO_HIGH_TO_LOW_TRANSITION
//!        - \b GPIO_LOW_TO_HIGH_TRANSITION
//!
//! Modified bits of \b PxIES register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_interruptEdgeSelect(uint8_t selectedPort, uint16_t selectedPins,
                              uint8_t edgeSelect)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        if (GPIO_LOW_TO_HIGH_TRANSITION == edgeSelect)
                HWREG16(baseAddress + OFS_PAIES) &= ~selectedPins;
        else
                HWREG16(baseAddress + OFS_PAIES) |= selectedPins;
}

//*****************************************************************************
//
//! \brief This function sets the drive strength for the selected port pin.
//!
//! his function sets the drive strength for the selected port pin. Acceptable
//! values for driveStrength are GPIO_REDUCED_OUTPUT_DRIVE_STRENGTH and
//! GPIO_FULL_OUTPUT_DRIVE_STRENGTH.
//!
//! \param selectedPort is the selected port.
//!        Valid values are:
//!        - \b GPIO_PORT_P1
//!        - \b GPIO_PORT_P2
//!        - \b GPIO_PORT_P3
//!        - \b GPIO_PORT_P4
//!        - \b GPIO_PORT_P5
//!        - \b GPIO_PORT_P6
//!        - \b GPIO_PORT_P7
//!        - \b GPIO_PORT_P8
//!        - \b GPIO_PORT_P9
//!        - \b GPIO_PORT_P10
//!        - \b GPIO_PORT_P11
//!        - \b GPIO_PORT_PA
//!        - \b GPIO_PORT_PB
//!        - \b GPIO_PORT_PC
//!        - \b GPIO_PORT_PD
//!        - \b GPIO_PORT_PE
//!        - \b GPIO_PORT_PF
//!        - \b GPIO_PORT_PJ
//! \param selectedPins is the specified pin in the selected port.
//!        Mask value is the logical OR of any of the following:
//!        - \b GPIO_PIN0
//!        - \b GPIO_PIN1
//!        - \b GPIO_PIN2
//!        - \b GPIO_PIN3
//!        - \b GPIO_PIN4
//!        - \b GPIO_PIN5
//!        - \b GPIO_PIN6
//!        - \b GPIO_PIN7
//!        - \b GPIO_PIN8
//!        - \b GPIO_PIN9
//!        - \b GPIO_PIN10
//!        - \b GPIO_PIN11
//!        - \b GPIO_PIN12
//!        - \b GPIO_PIN13
//!        - \b GPIO_PIN14
//!        - \b GPIO_PIN15
//! \param driveStrength specifies the drive strength of the pin
//!        Valid values are:
//!        - \b GPIO_REDUCED_OUTPUT_DRIVE_STRENGTH
//!        - \b GPIO_FULL_OUTPUT_DRIVE_STRENGTH
//!
//! Modified bits of \b PxDS register.
//!
//! \return None
//
//*****************************************************************************
void GPIO_setDriveStrength(uint8_t selectedPort, uint16_t selectedPins,
                           uint8_t driveStrength)
{

        uint16_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    #ifndef NDEBUG
        if (baseAddress == 0xFFFF)
                return;

    #endif

        // Shift by 8 if port is even (upper 8-bits)
        if ((selectedPort & 1) ^ 1)
                selectedPins <<= 8;

        if (GPIO_REDUCED_OUTPUT_DRIVE_STRENGTH == driveStrength)
                HWREG16(baseAddress + OFS_PADS) &= ~selectedPins;
        else
                HWREG16(baseAddress + OFS_PADS) |= selectedPins;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for gpio_api
//! @}
//
//*****************************************************************************
