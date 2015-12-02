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
// timer_a.c - Driver for the timer_a Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup timer_a_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TxA7__
#include "timer_a.h"

#include <assert.h>

//*****************************************************************************
//
//! \brief Starts TIMER_A counter
//!
//! This function assumes that the timer has been previously configured using
//! TIMER_A_configureContinuousMode, TIMER_A_configureUpMode or
//! TIMER_A_configureUpDownMode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param timerMode mode to put the timer in
//!        Valid values are:
//!        - \b TIMER_A_STOP_MODE
//!        - \b TIMER_A_UP_MODE
//!        - \b TIMER_A_CONTINUOUS_MODE [Default]
//!        - \b TIMER_A_UPDOWN_MODE
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_startCounter( uint16_t baseAddress,
                           uint16_t timerMode
                           )
{
        assert(
                (TIMER_A_UPDOWN_MODE == timerMode) ||
                (TIMER_A_CONTINUOUS_MODE == timerMode) ||
                (TIMER_A_UP_MODE == timerMode)
                );

        HWREG16(baseAddress + OFS_TAxCTL) |= timerMode;
}

//*****************************************************************************
//
//! \brief Configures TIMER_A in continuous mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for continuous mode initialization.
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initContinuousMode(uint16_t baseAddress,
                                TIMER_A_initContinuousModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK == param->clockSource)
                );

        assert(
                (TIMER_A_DO_CLEAR == param->timerClear) ||
                (TIMER_A_SKIP_CLEAR == param->timerClear)
                );

        assert(
                (TIMER_A_TAIE_INTERRUPT_ENABLE == param->timerInterruptEnable_TAIE) ||
                (TIMER_A_TAIE_INTERRUPT_DISABLE == param->timerInterruptEnable_TAIE)
                );

        assert(
                (TIMER_A_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_A_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        HWREG16(baseAddress +
                OFS_TAxCTL) &= ~(TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
                                 TIMER_A_UPDOWN_MODE +
                                 TIMER_A_DO_CLEAR +
                                 TIMER_A_TAIE_INTERRUPT_ENABLE +
                                 ID__8
                                 );
        HWREG16(baseAddress + OFS_TAxEX0) &= ~TAIDEX_7;

        HWREG16(baseAddress + OFS_TAxEX0) |= param->clockSourceDivider & 0x7;
        HWREG16(baseAddress + OFS_TAxCTL) |= (param->clockSource +
                                              param->timerClear +
                                              param->timerInterruptEnable_TAIE +
                                              ((param->clockSourceDivider >> 3) << 6));

        if (param->startTimer)
                HWREG16(baseAddress + OFS_TAxCTL) |= TIMER_A_CONTINUOUS_MODE;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures TIMER_A in continuous mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_A_startCounter API.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerInterruptEnable_TAIE is to enable or disable TIMER_A interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if TIMER_A clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_configureContinuousMode( uint16_t baseAddress,
                                      uint16_t clockSource,
                                      uint16_t clockSourceDivider,
                                      uint16_t timerInterruptEnable_TAIE,
                                      uint16_t timerClear
                                      )
{
        TIMER_A_initContinuousModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.timerClear = timerClear;
        param.startTimer = false;

        TIMER_A_initContinuousMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Configures TIMER_A in up mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for up mode initialization.
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initUpMode(uint16_t baseAddress,
                        TIMER_A_initUpModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK == param->clockSource)
                );

        assert(
                (TIMER_A_DO_CLEAR == param->timerClear) ||
                (TIMER_A_SKIP_CLEAR == param->timerClear)
                );

        assert(
                (TIMER_A_DO_CLEAR == param->timerClear) ||
                (TIMER_A_SKIP_CLEAR == param->timerClear)
                );

        HWREG16(baseAddress + OFS_TAxCTL) &=
                ~(TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
                  TIMER_A_UPDOWN_MODE +
                  TIMER_A_DO_CLEAR +
                  TIMER_A_TAIE_INTERRUPT_ENABLE +
                  ID__8
                  );
        HWREG16(baseAddress + OFS_TAxEX0) &= ~TAIDEX_7;

        HWREG16(baseAddress + OFS_TAxEX0) |= param->clockSourceDivider & 0x7;
        HWREG16(baseAddress + OFS_TAxCTL) |= (param->clockSource +
                                              param->timerClear +
                                              param->timerInterruptEnable_TAIE +
                                              ((param->clockSourceDivider >> 3) << 6));

        if (param->startTimer)
                HWREG16(baseAddress + OFS_TAxCTL) |= TIMER_A_UP_MODE;

        if (TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ==
            param->captureCompareInterruptEnable_CCR0_CCIE)
                HWREG16(baseAddress + OFS_TAxCCTL0)  |= TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
        else
                HWREG16(baseAddress + OFS_TAxCCTL0)  &= ~TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;

        HWREG16(baseAddress + OFS_TAxCCR0) = param->timerPeriod;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures TIMER_A in up mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_A_startCounter API.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerPeriod is the specified TIMER_A period. This is the value that
//!        gets written into the CCR0. Limited to 16 bits[uint16_t]
//! \param timerInterruptEnable_TAIE is to enable or disable TIMER_A interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        TIMER_A CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if TIMER_A clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_configureUpMode(uint16_t baseAddress,
                             uint16_t clockSource,
                             uint16_t clockSourceDivider,
                             uint16_t timerPeriod,
                             uint16_t timerInterruptEnable_TAIE,
                             uint16_t captureCompareInterruptEnable_CCR0_CCIE,
                             uint16_t timerClear
                             )
{
        TIMER_A_initUpModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;
        param.startTimer = false;

        TIMER_A_initUpMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Configures TIMER_A in up down mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for up-down mode initialization.
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initUpDownMode(uint16_t baseAddress,
                            TIMER_A_initUpDownModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK == param->clockSource)
                );

        assert(
                (TIMER_A_DO_CLEAR == param->timerClear) ||
                (TIMER_A_SKIP_CLEAR == param->timerClear)
                );

        HWREG16(baseAddress + OFS_TAxCTL) &=
                ~(TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
                  TIMER_A_UPDOWN_MODE +
                  TIMER_A_DO_CLEAR +
                  TIMER_A_TAIE_INTERRUPT_ENABLE +
                  ID__8
                  );
        HWREG16(baseAddress + OFS_TAxEX0) &= ~TAIDEX_7;

        HWREG16(baseAddress + OFS_TAxEX0) |= param->clockSourceDivider & 0x7;
        HWREG16(baseAddress + OFS_TAxCTL) |= (param->clockSource +
                                              param->timerClear +
                                              param->timerInterruptEnable_TAIE +
                                              ((param->clockSourceDivider >> 3) << 6));

        if (param->startTimer)
                HWREG16(baseAddress + OFS_TAxCTL) |= TIMER_A_UPDOWN_MODE;

        if (TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ==
            param->captureCompareInterruptEnable_CCR0_CCIE)
                HWREG16(baseAddress + OFS_TAxCCTL0)  |= TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;
        else
                HWREG16(baseAddress + OFS_TAxCCTL0)  &= ~TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE;

        HWREG16(baseAddress + OFS_TAxCCR0)  = param->timerPeriod;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures TIMER_A in up down mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_A_startCounter API.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerPeriod is the specified TIMER_A period
//! \param timerInterruptEnable_TAIE is to enable or disable TIMER_A interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        TIMER_A CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if TIMER_A clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_configureUpDownMode(
        uint16_t baseAddress,
        uint16_t clockSource,
        uint16_t clockSourceDivider,
        uint16_t timerPeriod,
        uint16_t timerInterruptEnable_TAIE,
        uint16_t captureCompareInterruptEnable_CCR0_CCIE,
        uint16_t timerClear
        )
{
        TIMER_A_initUpDownModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;
        param.startTimer = false;

        TIMER_A_initUpDownMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief DEPRECATED - Starts timer in continuous mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerInterruptEnable_TAIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_startContinuousMode( uint16_t baseAddress,
                                  uint16_t clockSource,
                                  uint16_t clockSourceDivider,
                                  uint16_t timerInterruptEnable_TAIE,
                                  uint16_t timerClear
                                  )
{
        TIMER_A_initContinuousModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.timerClear = timerClear;
        param.startTimer = true;

        TIMER_A_initContinuousMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief DEPRECATED - Spelling Error Fixed. Starts timer in continuous mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerInterruptEnable_TAIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_startContinousMode( uint16_t baseAddress,
                                 uint16_t clockSource,
                                 uint16_t clockSourceDivider,
                                 uint16_t timerInterruptEnable_TAIE,
                                 uint16_t timerClear
                                 )
{
        TIMER_A_startContinuousMode(baseAddress,
                                    clockSource,
                                    clockSourceDivider,
                                    timerInterruptEnable_TAIE,
                                    timerClear
                                    );
}

//*****************************************************************************
//
//! \brief DEPRECATED - Replaced by TIMER_A_configureUpMode and
//! TIMER_A_startCounter API. Starts timer in up mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerPeriod is the specified timer period. This is the value that
//!        gets written into the CCR0. Limited to 16 bits[uint16_t]
//! \param timerInterruptEnable_TAIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        timer CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_startUpMode(   uint16_t baseAddress,
                            uint16_t clockSource,
                            uint16_t clockSourceDivider,
                            uint16_t timerPeriod,
                            uint16_t timerInterruptEnable_TAIE,
                            uint16_t captureCompareInterruptEnable_CCR0_CCIE,
                            uint16_t timerClear
                            )
{
        TIMER_A_initUpModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;
        param.startTimer = true;

        TIMER_A_initUpMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief DEPRECATED - Replaced by TIMER_A_configureUpMode and
//! TIMER_A_startCounter API. Starts timer in up down mode.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerPeriod is the specified timer period
//! \param timerInterruptEnable_TAIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_A_TAIE_INTERRUPT_ENABLE
//!        - \b TIMER_A_TAIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        timer CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_A_DO_CLEAR
//!        - \b TIMER_A_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register and bits
//! of \b TAxCCR0 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_startUpDownMode(
        uint16_t baseAddress,
        uint16_t clockSource,
        uint16_t clockSourceDivider,
        uint16_t timerPeriod,
        uint16_t timerInterruptEnable_TAIE,
        uint16_t captureCompareInterruptEnable_CCR0_CCIE,
        uint16_t timerClear
        )
{
        TIMER_A_initUpDownModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TAIE = timerInterruptEnable_TAIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;
        param.startTimer = true;

        TIMER_A_initUpDownMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Initializes Capture Mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for capture mode initialization.
//!
//! Modified bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initCaptureMode(uint16_t baseAddress,
                             TIMER_A_initCaptureModeParam *param)
{
        assert(param != 0);

        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == param->captureRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == param->captureRegister)
               );

        assert((TIMER_A_CAPTUREMODE_NO_CAPTURE == param->captureMode) ||
               (TIMER_A_CAPTUREMODE_RISING_EDGE == param->captureMode) ||
               (TIMER_A_CAPTUREMODE_FALLING_EDGE == param->captureMode) ||
               (TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE == param->captureMode)
               );

        assert((TIMER_A_CAPTURE_INPUTSELECT_CCIxA == param->captureInputSelect) ||
               (TIMER_A_CAPTURE_INPUTSELECT_CCIxB == param->captureInputSelect) ||
               (TIMER_A_CAPTURE_INPUTSELECT_GND == param->captureInputSelect) ||
               (TIMER_A_CAPTURE_INPUTSELECT_Vcc == param->captureInputSelect)
               );

        assert((TIMER_A_CAPTURE_ASYNCHRONOUS == param->synchronizeCaptureSource) ||
               (TIMER_A_CAPTURE_SYNCHRONOUS == param->synchronizeCaptureSource)
               );

        assert(
                (TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE == param->captureInterruptEnable) ||
                (TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE == param->captureInterruptEnable)
                );

        assert((TIMER_A_OUTPUTMODE_OUTBITVALUE == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_RESET == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET_RESET == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_SET == param->captureOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET_SET == param->captureOutputMode)
               );

        //CaptureCompare register 0 only supports certain modes
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == param->captureRegister) &&
               ((TIMER_A_OUTPUTMODE_OUTBITVALUE == param->captureOutputMode) ||
                (TIMER_A_OUTPUTMODE_SET == param->captureOutputMode) ||
                (TIMER_A_OUTPUTMODE_TOGGLE == param->captureOutputMode) ||
                (TIMER_A_OUTPUTMODE_RESET == param->captureOutputMode)));

        HWREG16(baseAddress + param->captureRegister ) |= CAP;

        HWREG16(baseAddress + param->captureRegister) &=
                ~(TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE +
                  TIMER_A_CAPTURE_INPUTSELECT_Vcc +
                  TIMER_A_CAPTURE_SYNCHRONOUS +
                  TIMER_A_DO_CLEAR +
                  TIMER_A_TAIE_INTERRUPT_ENABLE +
                  CM_3
                  );

        HWREG16(baseAddress + param->captureRegister) |= (param->captureMode +
                                                          param->captureInputSelect +
                                                          param->synchronizeCaptureSource +
                                                          param->captureInterruptEnable +
                                                          param->captureOutputMode
                                                          );
}

//*****************************************************************************
//
//! \brief DEPRECATED - Initializes Capture Mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureRegister selects the Capture register being used. Refer to
//!        datasheet to ensure the device has the capture compare register
//!        being used.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param captureMode is the capture mode selected.
//!        Valid values are:
//!        - \b TIMER_A_CAPTUREMODE_NO_CAPTURE [Default]
//!        - \b TIMER_A_CAPTUREMODE_RISING_EDGE
//!        - \b TIMER_A_CAPTUREMODE_FALLING_EDGE
//!        - \b TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE
//! \param captureInputSelect decides the Input Select
//!        Valid values are:
//!        - \b TIMER_A_CAPTURE_INPUTSELECT_CCIxA
//!        - \b TIMER_A_CAPTURE_INPUTSELECT_CCIxB
//!        - \b TIMER_A_CAPTURE_INPUTSELECT_GND
//!        - \b TIMER_A_CAPTURE_INPUTSELECT_Vcc
//! \param synchronizeCaptureSource decides if capture source should be
//!        synchronized with timer clock
//!        Valid values are:
//!        - \b TIMER_A_CAPTURE_ASYNCHRONOUS [Default]
//!        - \b TIMER_A_CAPTURE_SYNCHRONOUS
//! \param captureInterruptEnable is to enable or disable timer captureComapre
//!        interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE [Default]
//!        - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE
//! \param captureOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_A_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_A_OUTPUTMODE_SET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_A_OUTPUTMODE_SET_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE
//!        - \b TIMER_A_OUTPUTMODE_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_A_OUTPUTMODE_RESET_SET
//!
//! Modified bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initCapture(uint16_t baseAddress,
                         uint16_t captureRegister,
                         uint16_t captureMode,
                         uint16_t captureInputSelect,
                         uint16_t synchronizeCaptureSource,
                         uint16_t captureInterruptEnable,
                         uint16_t captureOutputMode
                         )
{
        TIMER_A_initCaptureModeParam param = { 0 };

        param.captureRegister = captureRegister;
        param.captureMode = captureMode;
        param.captureInputSelect = captureInputSelect;
        param.synchronizeCaptureSource = synchronizeCaptureSource;
        param.captureInterruptEnable = captureInterruptEnable;
        param.captureOutputMode = captureOutputMode;

        TIMER_A_initCaptureMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Initializes Compare Mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for compare mode initialization.
//!
//! Modified bits of \b TAxCCRn register and bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initCompareMode(uint16_t baseAddress,
                             TIMER_A_initCompareModeParam *param)
{
        assert(param != 0);

        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == param->compareRegister)
               );

        assert((TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE == param->compareInterruptEnable) ||
               (TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE == param->compareInterruptEnable)
               );

        assert((TIMER_A_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_SET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET_SET == param->compareOutputMode)
               );

        //CaptureCompare register 0 only supports certain modes
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) &&
               ((TIMER_A_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
                (TIMER_A_OUTPUTMODE_SET == param->compareOutputMode) ||
                (TIMER_A_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
                (TIMER_A_OUTPUTMODE_RESET == param->compareOutputMode)));

        HWREG16(baseAddress + param->compareRegister ) &= ~CAP;

        HWREG16(baseAddress + param->compareRegister) &=
                ~(TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE +
                  TIMER_A_OUTPUTMODE_RESET_SET
                  );

        HWREG16(baseAddress + param->compareRegister) |= (param->compareInterruptEnable +
                                                          param->compareOutputMode
                                                          );

        HWREG16(baseAddress + param->compareRegister + OFS_TAxR) = param->compareValue;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Initializes Compare Mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param compareRegister selects the Capture register being used. Refer to
//!        datasheet to ensure the device has the capture compare register
//!        being used.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param compareInterruptEnable is to enable or disable timer captureComapre
//!        interrupt.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE [Default]
//!        - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_A_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_A_OUTPUTMODE_SET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_A_OUTPUTMODE_SET_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE
//!        - \b TIMER_A_OUTPUTMODE_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_A_OUTPUTMODE_RESET_SET
//! \param compareValue is the count to be compared with in compare mode
//!
//! Modified bits of \b TAxCCRn register and bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_initCompare(  uint16_t baseAddress,
                           uint16_t compareRegister,
                           uint16_t compareInterruptEnable,
                           uint16_t compareOutputMode,
                           uint16_t compareValue
                           )
{
        TIMER_A_initCompareModeParam param = { 0 };

        param.compareRegister = compareRegister;
        param.compareInterruptEnable = compareInterruptEnable;
        param.compareOutputMode = compareOutputMode;
        param.compareValue = compareValue;

        TIMER_A_initCompareMode(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Enable timer interrupt
//!
//! Does not clear interrupt flags
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_enableInterrupt(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TAxCTL) |= TAIE;
}

//*****************************************************************************
//
//! \brief Disable timer interrupt
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_disableInterrupt(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TAxCTL) &= ~TAIE;
}

//*****************************************************************************
//
//! \brief Get timer interrupt status
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! \return One of the following:
//!         - \b TIMER_A_INTERRUPT_NOT_PENDING
//!         - \b TIMER_A_INTERRUPT_PENDING
//!         \n indicating the TIMER_A interrupt status
//
//*****************************************************************************
uint32_t TIMER_A_getInterruptStatus(uint16_t baseAddress)
{
        return HWREG16(baseAddress + OFS_TAxCTL) & TAIFG;
}

//*****************************************************************************
//
//! \brief Enable capture compare interrupt
//!
//! Does not clear interrupt flags
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_enableCaptureCompareInterrupt(uint16_t baseAddress,
                                           uint16_t captureCompareRegister
                                           )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        HWREG16(baseAddress + captureCompareRegister) |= CCIE;
}

//*****************************************************************************
//
//! \brief Disable capture compare interrupt
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_disableCaptureCompareInterrupt(uint16_t baseAddress,
                                            uint16_t captureCompareRegister
                                            )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );
        HWREG16(baseAddress + captureCompareRegister) &= ~CCIE;
}

//*****************************************************************************
//
//! \brief Return capture compare interrupt status
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param mask is the mask for the interrupt status
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_A_CAPTURE_OVERFLOW
//!        - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG
//!
//! \return Logical OR of any of the following:
//!         - \b TIMER_A_CAPTURE_OVERFLOW
//!         - \b TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************
uint32_t TIMER_A_getCaptureCompareInterruptStatus(uint16_t baseAddress,
                                                  uint16_t captureCompareRegister,
                                                  uint16_t mask
                                                  )
{
        return HWREG16(baseAddress + captureCompareRegister) & mask;
}

//*****************************************************************************
//
//! \brief Reset/Clear the timer clock divider, count direction, count
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_clear(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TAxCTL) |= TACLR;
}

//*****************************************************************************
//
//! \brief Get synchronized capturecompare input
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param synchronized
//!        Valid values are:
//!        - \b TIMER_A_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT
//!        - \b TIMER_A_READ_CAPTURE_COMPARE_INPUT
//!
//! \return One of the following:
//!         - \b TIMER_A_CAPTURECOMPARE_INPUT_HIGH
//!         - \b TIMER_A_CAPTURECOMPARE_INPUT_LOW
//
//*****************************************************************************
uint8_t TIMER_A_getSynchronizedCaptureCompareInput
        (uint16_t baseAddress,
        uint16_t captureCompareRegister,
        uint16_t synchronized
        )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        assert((TIMER_A_READ_CAPTURE_COMPARE_INPUT == synchronized) ||
               (TIMER_A_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT == synchronized)
               );

        if (HWREG16(baseAddress + captureCompareRegister) & synchronized)
                return TIMER_A_CAPTURECOMPARE_INPUT_HIGH;
        else
                return TIMER_A_CAPTURECOMPARE_INPUT_LOW;
}

//*****************************************************************************
//
//! \brief Get output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//!
//! \return One of the following:
//!         - \b TIMER_A_OUTPUTMODE_OUTBITVALUE_HIGH
//!         - \b TIMER_A_OUTPUTMODE_OUTBITVALUE_LOW
//
//*****************************************************************************
uint8_t TIMER_A_getOutputForOutputModeOutBitValue
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        if (HWREG16(baseAddress + captureCompareRegister) & OUT)
                return TIMER_A_OUTPUTMODE_OUTBITVALUE_HIGH;
        else
                return TIMER_A_OUTPUTMODE_OUTBITVALUE_LOW;
}

//*****************************************************************************
//
//! \brief Get current capturecompare count
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//!
//! \return Current count as an uint16_t
//
//*****************************************************************************
uint16_t TIMER_A_getCaptureCompareCount
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        return HWREG16(baseAddress + OFS_TAxR + captureCompareRegister);
}

//*****************************************************************************
//
//! \brief Set output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param outputModeOutBitValue is the value to be set for out bit
//!        Valid values are:
//!        - \b TIMER_A_OUTPUTMODE_OUTBITVALUE_HIGH
//!        - \b TIMER_A_OUTPUTMODE_OUTBITVALUE_LOW
//!
//! Modified bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_setOutputForOutputModeOutBitValue
        (uint16_t baseAddress,
        uint16_t captureCompareRegister,
        uint8_t outputModeOutBitValue
        )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        assert((TIMER_A_OUTPUTMODE_OUTBITVALUE_HIGH == outputModeOutBitValue) ||
               (TIMER_A_OUTPUTMODE_OUTBITVALUE_LOW == outputModeOutBitValue)
               );

        HWREG16(baseAddress + captureCompareRegister) &= ~OUT;
        HWREG16(baseAddress + captureCompareRegister) |= outputModeOutBitValue;
}

//*****************************************************************************
//
//! \brief Generate a PWM with timer running in up mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param param is the pointer to struct for PWM configuration.
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register, bits of
//! \b TAxCCR0 register and bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_outputPWM(uint16_t baseAddress, TIMER_A_outputPWMParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK == param->clockSource)
                );

        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == param->compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == param->compareRegister)
               );

        assert((TIMER_A_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_SET_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_TOGGLE_SET == param->compareOutputMode) ||
               (TIMER_A_OUTPUTMODE_RESET_SET == param->compareOutputMode)
               );

        HWREG16(baseAddress + OFS_TAxCTL)  &=
                ~( TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK +
                   TIMER_A_UPDOWN_MODE + TIMER_A_DO_CLEAR +
                   TIMER_A_TAIE_INTERRUPT_ENABLE +
                   ID__8
                   );
        HWREG16(baseAddress + OFS_TAxEX0) &= ~TAIDEX_7;

        HWREG16(baseAddress + OFS_TAxEX0) |= param->clockSourceDivider & 0x7;
        HWREG16(baseAddress + OFS_TAxCTL)  |= (param->clockSource +
                                               TIMER_A_UP_MODE +
                                               TIMER_A_DO_CLEAR +
                                               ((param->clockSourceDivider >> 3) << 6));

        HWREG16(baseAddress + OFS_TAxCCR0) = param->timerPeriod;

        HWREG16(baseAddress + OFS_TAxCCTL0)  &=
                ~(TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE +
                  TIMER_A_OUTPUTMODE_RESET_SET
                  );
        HWREG16(baseAddress + param->compareRegister) |= param->compareOutputMode;

        HWREG16(baseAddress + param->compareRegister + OFS_TAxR) = param->dutyCycle;
} //*****************************************************************************
//
//! \brief DEPRECATED - Generate a PWM with timer running in up mode
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK [Default]
//!        - \b TIMER_A_CLOCKSOURCE_ACLK
//!        - \b TIMER_A_CLOCKSOURCE_SMCLK
//!        - \b TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK
//! \param clockSourceDivider is the desired divider for the clock source
//!        Valid values are:
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_A_CLOCKSOURCE_DIVIDER_64
//! \param timerPeriod selects the desired timer period
//! \param compareRegister selects the compare register being used. Refer to
//!        datasheet to ensure the device has the capture compare register
//!        being used.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_A_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_A_OUTPUTMODE_SET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_A_OUTPUTMODE_SET_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE
//!        - \b TIMER_A_OUTPUTMODE_RESET
//!        - \b TIMER_A_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_A_OUTPUTMODE_RESET_SET
//! \param dutyCycle specifies the dutycycle for the generated waveform
//!
//! Modified bits of \b TAxCTL register, bits of \b TAxCCTL0 register, bits of
//! \b TAxCCR0 register and bits of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_generatePWM(uint16_t baseAddress,
                         uint16_t clockSource,
                         uint16_t clockSourceDivider,
                         uint16_t timerPeriod,
                         uint16_t compareRegister,
                         uint16_t compareOutputMode,
                         uint16_t dutyCycle
                         )
{
        TIMER_A_outputPWMParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.timerPeriod = timerPeriod;
        param.compareRegister = compareRegister;
        param.compareOutputMode = compareOutputMode;
        param.dutyCycle = dutyCycle;

        TIMER_A_outputPWM(baseAddress, &param);
}

//*****************************************************************************
//
//! \brief Stops the timer
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! Modified bits of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_stop( uint16_t baseAddress )
{
        HWREG16(baseAddress + OFS_TAxCTL)  &= ~MC_3;
        HWREG16(baseAddress + OFS_TAxCTL)  |= MC_0;
}

//*****************************************************************************
//
//! \brief Sets the value of the capture-compare register
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param compareRegister selects the Capture register being used. Refer to
//!        datasheet to ensure the device has the capture compare register
//!        being used.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//! \param compareValue is the count to be compared with in compare mode
//!
//! Modified bits of \b TAxCCRn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_setCompareValue(  uint16_t baseAddress,
                               uint16_t compareRegister,
                               uint16_t compareValue
                               )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == compareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == compareRegister)
               );

        HWREG16(baseAddress + compareRegister + OFS_TAxR) = compareValue;
}

//*****************************************************************************
//
//! \brief Clears the Timer TAIFG interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! Modified bits are \b TAIFG of \b TAxCTL register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_clearTimerInterruptFlag(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TAxCTL) &= ~TAIFG;
}

//*****************************************************************************
//
//! \brief Clears the capture-compare interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_A module.
//! \param captureCompareRegister selects the Capture-compare register being
//!        used.
//!        Valid values are:
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_A_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits are \b CCIFG of \b TAxCCTLn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_A_clearCaptureCompareInterruptFlag(uint16_t baseAddress,
                                              uint16_t captureCompareRegister
                                              )
{
        assert((TIMER_A_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_A_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        HWREG16(baseAddress + captureCompareRegister)  &= ~CCIFG;
}

//*****************************************************************************
//
//! \brief Reads the current timer count value
//!
//! Reads the current count value of the timer. There is a majority vote system
//! in place to confirm an accurate value is returned. The TIMER_A_THRESHOLD
//! #define in the corresponding header file can be modified so that the votes
//! must be closer together for a consensus to occur.
//!
//! \param baseAddress is the base address of the TIMER_A module.
//!
//! \return Majority vote of timer count value
//
//*****************************************************************************
uint16_t TIMER_A_getCounterValue(uint16_t baseAddress)
{
        uint16_t voteOne, voteTwo, res;

        voteTwo = HWREG16(baseAddress + OFS_TAxR);

        do {
                voteOne = voteTwo;
                voteTwo = HWREG16(baseAddress + OFS_TAxR);

                if (voteTwo > voteOne)
                        res = voteTwo - voteOne;
                else if (voteOne > voteTwo)
                        res = voteOne - voteTwo;
                else
                        res = 0;

        } while ( res > TIMER_A_THRESHOLD);

        return voteTwo;
}


#endif
//*****************************************************************************
//
//! Close the doxygen group for timer_a_api
//! @}
//
//*****************************************************************************
