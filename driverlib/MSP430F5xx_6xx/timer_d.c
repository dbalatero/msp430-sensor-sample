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
// timer_d.c - Driver for the timer_d Module.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup timer_d_api
//! @{
//
//*****************************************************************************

#include "inc/hw_regaccess.h"
#include "inc/hw_memmap.h"

#ifdef __MSP430_HAS_TxD7__
#include "timer_d.h"

#include <assert.h>

//*****************************************************************************
//
//! \brief Starts TIMER_D counter
//!
//! NOTE: This function assumes that the timer has been previously configured
//! using TIMER_D_configureContinuousMode, TIMER_D_configureUpMode or
//! TIMER_D_configureUpDownMode.
//!
//! \param baseAddress is the base address of the TIMER_DA module.
//! \param timerMode selects the mode of the timer
//!        Valid values are:
//!        - \b TIMER_D_STOP_MODE
//!        - \b TIMER_D_UP_MODE
//!        - \b TIMER_D_CONTINUOUS_MODE [Default]
//!        - \b TIMER_D_UPDOWN_MODE
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************
#include "tlv.h"
void TIMER_D_startCounter( uint16_t baseAddress,
                           uint16_t timerMode
                           )
{
        assert(
                (TIMER_D_UPDOWN_MODE == timerMode) ||
                (TIMER_D_CONTINUOUS_MODE == timerMode) ||
                (TIMER_D_UP_MODE == timerMode)
                );

        HWREG16(baseAddress + OFS_TDxCTL0) |= timerMode;
}
//*****************************************************************************
//
//! \brief Configures timer in continuous mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for continuous mode initialization.
//!
//! Modified bits of \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_initContinuousMode(uint16_t baseAddress,
                                TIMER_D_initContinuousModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_D_DO_CLEAR == param->timerClear) ||
                (TIMER_D_SKIP_CLEAR == param->timerClear)
                );

        assert(
                (TIMER_D_TDIE_INTERRUPT_ENABLE == param->timerInterruptEnable_TDIE) ||
                (TIMER_D_TDIE_INTERRUPT_DISABLE == param->timerInterruptEnable_TDIE)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        HWREG16(baseAddress +
                OFS_TDxCTL0) &= ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
                                  TIMER_D_UPDOWN_MODE +
                                  TIMER_D_DO_CLEAR +
                                  TIMER_D_TDIE_INTERRUPT_ENABLE +
                                  ID__8
                                  );
        HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));

        HWREG16(baseAddress + OFS_TDxCTL0) |= (param->timerClear +
                                               param->timerInterruptEnable_TDIE +
                                               ((param->clockSourceDivider >> 3) << 6));
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures timer in continuous mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param timerInterruptEnable_TDIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_D_TDIE_INTERRUPT_ENABLE
//!        - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_D_DO_CLEAR
//!        - \b TIMER_D_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_configureContinuousMode( uint16_t baseAddress,
                                      uint16_t clockSource,
                                      uint16_t clockSourceDivider,
                                      uint16_t clockingMode,
                                      uint16_t timerInterruptEnable_TDIE,
                                      uint16_t timerClear
                                      )
{
        TIMER_D_initContinuousModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.timerInterruptEnable_TDIE = timerInterruptEnable_TDIE;
        param.timerClear = timerClear;

        TIMER_D_initContinuousMode(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Configures timer in up mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for up mode initialization.
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_initUpMode(uint16_t baseAddress, TIMER_D_initUpModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_D_DO_CLEAR == param->timerClear) ||
                (TIMER_D_SKIP_CLEAR == param->timerClear)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        HWREG16(baseAddress + OFS_TDxCTL0) &=
                ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
                  TIMER_D_UPDOWN_MODE +
                  TIMER_D_DO_CLEAR +
                  TIMER_D_TDIE_INTERRUPT_ENABLE +
                  ID__8
                  );
        HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));

        HWREG16(baseAddress + OFS_TDxCTL0) |= (TIMER_D_STOP_MODE +
                                               param->timerClear +
                                               param->timerInterruptEnable_TDIE +
                                               ((param->clockSourceDivider >> 3) << 6));

        if (TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE ==
            param->captureCompareInterruptEnable_CCR0_CCIE)
                HWREG16(baseAddress + OFS_TDxCCTL0)  |= TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
        else
                HWREG16(baseAddress + OFS_TDxCCTL0)  &= ~TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;

        HWREG16(baseAddress + OFS_TDxCCR0) = param->timerPeriod;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures timer in up mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param timerPeriod is the specified timer period. This is the value that
//!        gets written into the CCR0. Limited to 16 bits [uint16_t]
//! \param timerInterruptEnable_TDIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_D_TDIE_INTERRUPT_ENABLE
//!        - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        timer CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_D_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_D_DO_CLEAR
//!        - \b TIMER_D_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_configureUpMode(   uint16_t baseAddress,
                                uint16_t clockSource,
                                uint16_t clockSourceDivider,
                                uint16_t clockingMode,
                                uint16_t timerPeriod,
                                uint16_t timerInterruptEnable_TDIE,
                                uint16_t captureCompareInterruptEnable_CCR0_CCIE,
                                uint16_t timerClear
                                )
{
        TIMER_D_initUpModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TDIE = timerInterruptEnable_TDIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;

        TIMER_D_initUpMode(baseAddress, &param);

}
//*****************************************************************************
//
//! \brief Configures timer in up down mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for up-down mode initialization.
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_initUpDownMode(uint16_t baseAddress,
                            TIMER_D_initUpDownModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_D_DO_CLEAR == param->timerClear) ||
                (TIMER_D_SKIP_CLEAR == param->timerClear)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        HWREG16(baseAddress + OFS_TDxCTL0) &=
                ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
                  TIMER_D_UPDOWN_MODE +
                  TIMER_D_DO_CLEAR +
                  TIMER_D_TDIE_INTERRUPT_ENABLE +
                  ID__8
                  );
        HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));

        HWREG16(baseAddress + OFS_TDxCTL0)  |= (TIMER_D_STOP_MODE +
                                                param->timerClear +
                                                param->timerInterruptEnable_TDIE +
                                                ((param->clockSourceDivider >> 3) << 6));

        if (TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE ==
            param->captureCompareInterruptEnable_CCR0_CCIE)
                HWREG16(baseAddress + OFS_TDxCCTL0)  |= TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;
        else
                HWREG16(baseAddress + OFS_TDxCCTL0)  &= ~TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE;

        HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures timer in up down mode.
//!
//! This API does not start the timer. Timer needs to be started when required
//! using the TIMER_D_start API.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param timerPeriod is the specified timer period
//! \param timerInterruptEnable_TDIE is to enable or disable timer interrupt
//!        Valid values are:
//!        - \b TIMER_D_TDIE_INTERRUPT_ENABLE
//!        - \b TIMER_D_TDIE_INTERRUPT_DISABLE [Default]
//! \param captureCompareInterruptEnable_CCR0_CCIE is to enable or disable
//!        timer CCR0 captureComapre interrupt.
//!        Valid values are:
//!        - \b TIMER_D_CCIE_CCR0_INTERRUPT_ENABLE
//!        - \b TIMER_D_CCIE_CCR0_INTERRUPT_DISABLE [Default]
//! \param timerClear decides if timer clock divider, count direction, count
//!        need to be reset.
//!        Valid values are:
//!        - \b TIMER_D_DO_CLEAR
//!        - \b TIMER_D_SKIP_CLEAR [Default]
//!
//! Modified bits of \b TDxCCR0 register, bits of \b TDxCCTL0 register, bits of
//! \b TDxCTL0 register and bits of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_configureUpDownMode(
        uint16_t baseAddress,
        uint16_t clockSource,
        uint16_t clockSourceDivider,
        uint16_t clockingMode,
        uint16_t timerPeriod,
        uint16_t timerInterruptEnable_TDIE,
        uint16_t captureCompareInterruptEnable_CCR0_CCIE,
        uint16_t timerClear
        )
{
        TIMER_D_initUpDownModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.timerPeriod = timerPeriod;
        param.timerInterruptEnable_TDIE = timerInterruptEnable_TDIE;
        param.captureCompareInterruptEnable_CCR0_CCIE =
                captureCompareInterruptEnable_CCR0_CCIE;
        param.timerClear = timerClear;

        TIMER_D_initUpDownMode(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Initializes Capture Mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for capture mode initialization.
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCTL2 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_initCaptureMode(uint16_t baseAddress,
                             TIMER_D_initCaptureModeParam *param)
{
        assert(param != 0);

        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == param->captureRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == param->captureRegister)
               );

        assert((TIMER_D_CAPTUREMODE_NO_CAPTURE == param->captureMode) ||
               (TIMER_D_CAPTUREMODE_RISING_EDGE == param->captureMode) ||
               (TIMER_D_CAPTUREMODE_FALLING_EDGE == param->captureMode) ||
               (TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE == param->captureMode)
               );

        assert((TIMER_D_CAPTURE_INPUTSELECT_CCIxA == param->captureInputSelect) ||
               (TIMER_D_CAPTURE_INPUTSELECT_CCIxB == param->captureInputSelect) ||
               (TIMER_D_CAPTURE_INPUTSELECT_GND == param->captureInputSelect) ||
               (TIMER_D_CAPTURE_INPUTSELECT_Vcc == param->captureInputSelect)
               );

        assert((TIMER_D_CAPTURE_ASYNCHRONOUS == param->synchronizeCaptureSource) ||
               (TIMER_D_CAPTURE_SYNCHRONOUS == param->synchronizeCaptureSource)
               );

        assert(
                (TIMER_D_CAPTURECOMPARE_INTERRUPT_DISABLE == param->captureInterruptEnable) ||
                (TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE == param->captureInterruptEnable)
                );

        assert((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_RESET == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET_RESET == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_SET == param->captureOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET_SET == param->captureOutputMode)
               );

        assert((TIMER_D_SINGLE_CAPTURE_MODE == param->channelCaptureMode) ||
               (TIMER_D_DUAL_CAPTURE_MODE == param->channelCaptureMode)
               );

        //CaptureCompare register 0 only supports certain modes
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == param->captureRegister) &&
               ((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->captureOutputMode) ||
                (TIMER_D_OUTPUTMODE_SET == param->captureOutputMode) ||
                (TIMER_D_OUTPUTMODE_TOGGLE == param->captureOutputMode) ||
                (TIMER_D_OUTPUTMODE_RESET == param->captureOutputMode)));

        HWREG16(baseAddress + param->captureRegister ) |= CAP;

        HWREG8(baseAddress + OFS_TDxCTL2) |=
                (param->channelCaptureMode << ((param->captureRegister - TIMER_D_CAPTURECOMPARE_REGISTER_0) / 6));

        HWREG16(baseAddress + param->captureRegister) &=
                ~(TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE +
                  TIMER_D_CAPTURE_INPUTSELECT_Vcc +
                  TIMER_D_CAPTURE_SYNCHRONOUS +
                  TIMER_D_DO_CLEAR +
                  TIMER_D_TDIE_INTERRUPT_ENABLE +
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
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureRegister selects the Capture register being used. Refer to
//!        datasheet to ensure the device has the capture compare register
//!        being used
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param captureMode is the capture mode selected.
//!        Valid values are:
//!        - \b TIMER_D_CAPTUREMODE_NO_CAPTURE [Default]
//!        - \b TIMER_D_CAPTUREMODE_RISING_EDGE
//!        - \b TIMER_D_CAPTUREMODE_FALLING_EDGE
//!        - \b TIMER_D_CAPTUREMODE_RISING_AND_FALLING_EDGE
//! \param captureInputSelect decides the Input Select
//!        Valid values are:
//!        - \b TIMER_D_CAPTURE_INPUTSELECT_CCIxA [Default]
//!        - \b TIMER_D_CAPTURE_INPUTSELECT_CCIxB
//!        - \b TIMER_D_CAPTURE_INPUTSELECT_GND
//!        - \b TIMER_D_CAPTURE_INPUTSELECT_Vcc
//! \param synchronizeCaptureSource decides if capture source should be
//!        synchronized with timer clock
//!        Valid values are:
//!        - \b TIMER_D_CAPTURE_ASYNCHRONOUS [Default]
//!        - \b TIMER_D_CAPTURE_SYNCHRONOUS
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCTL2 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_initCapture(uint16_t baseAddress,
                         uint16_t captureRegister,
                         uint16_t captureMode,
                         uint16_t captureInputSelect,
                         uint16_t synchronizeCaptureSource,
                         uint16_t captureInterruptEnable,
                         uint16_t captureOutputMode,
                         uint8_t channelCaptureMode
                         )
{
        TIMER_D_initCaptureModeParam param = { 0 };

        param.captureRegister = captureRegister;
        param.captureMode = captureMode;
        param.captureInputSelect = captureInputSelect;
        param.synchronizeCaptureSource = synchronizeCaptureSource;
        param.captureInterruptEnable = captureInterruptEnable;
        param.captureOutputMode = captureOutputMode;
        param.channelCaptureMode = channelCaptureMode;

        TIMER_D_initCaptureMode(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Initializes Compare Mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for compare mode initialization.
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCCRn register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_initCompareMode(uint16_t baseAddress,
                             TIMER_D_initCompareModeParam *param)
{
        assert(param != 0);

        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == param->compareRegister)
               );

        assert((TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE == param->compareInterruptEnable) ||
               (TIMER_D_CAPTURECOMPARE_INTERRUPT_DISABLE == param->compareInterruptEnable)
               );

        assert((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET_SET == param->compareOutputMode)
               );

        //CaptureCompare register 0 only supports certain modes
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) &&
               ((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
                (TIMER_D_OUTPUTMODE_SET == param->compareOutputMode) ||
                (TIMER_D_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
                (TIMER_D_OUTPUTMODE_RESET == param->compareOutputMode)));

        HWREG16(baseAddress + param->compareRegister ) &= ~CAP;

        HWREG16(baseAddress + param->compareRegister) &=
                ~(TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE +
                  TIMER_D_OUTPUTMODE_RESET_SET
                  );

        HWREG16(baseAddress + param->compareRegister) |= (param->compareInterruptEnable +
                                                          param->compareOutputMode
                                                          );

        HWREG16(baseAddress + param->compareRegister + 2) = param->compareValue;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Initializes Compare Mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareInterruptEnable is to enable or disable timer captureComapre
//!        interrupt.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE
//!        - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_DISABLE [Default]
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_D_OUTPUTMODE_SET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_D_OUTPUTMODE_SET_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE
//!        - \b TIMER_D_OUTPUTMODE_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_D_OUTPUTMODE_RESET_SET
//! \param compareValue is the count to be compared with in compare mode
//!
//! Modified bits of \b TDxCCTLn register and bits of \b TDxCCRn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_initCompare(  uint16_t baseAddress,
                           uint16_t compareRegister,
                           uint16_t compareInterruptEnable,
                           uint16_t compareOutputMode,
                           uint16_t compareValue
                           )
{
        TIMER_D_initCompareModeParam param = { 0 };

        param.compareRegister = compareRegister;
        param.compareInterruptEnable = compareInterruptEnable;
        param.compareOutputMode = compareOutputMode;
        param.compareValue = compareValue;

        TIMER_D_initCompareMode(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Enable timer interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_enableTimerInterrupt(uint16_t baseAddress)
{
        HWREG8(baseAddress + OFS_TDxCTL0) &=  ~TDIFG;
        HWREG8(baseAddress + OFS_TDxCTL0) |= TDIE;
}
//*****************************************************************************
//
//! \brief Enable High Resolution interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask of interrupts to enable
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_enableHighResInterrupt(uint16_t baseAddress,
                                    uint16_t mask)
{
        HWREG16(baseAddress + OFS_TDxHINT) &=  ~(mask >> 8);
        HWREG16(baseAddress + OFS_TDxHINT) |= mask;
}
//*****************************************************************************
//
//! \brief Disable timer interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_disableTimerInterrupt(uint16_t baseAddress)
{
        HWREG8(baseAddress + OFS_TDxCTL0) &= ~TDIE;
}
//*****************************************************************************
//
//! \brief Disable High Resolution interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask of interrupts to disable
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_disableHighResInterrupt(uint16_t baseAddress,
                                     uint16_t mask)
{
        HWREG16(baseAddress + OFS_TDxHINT) &= ~mask;
}
//*****************************************************************************
//
//! \brief Get timer interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! \return One of the following:
//!         - \b TIMER_D_INTERRUPT_NOT_PENDING
//!         - \b TIMER_D_INTERRUPT_PENDING
//!         \n indicating the timer interrupt status
//
//*****************************************************************************

uint32_t TIMER_D_getTimerInterruptStatus(uint16_t baseAddress)
{
        return HWREG8(baseAddress + OFS_TDxCTL0) & TDIFG;
}
//*****************************************************************************
//
//! \brief Enable capture compare interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_enableCaptureCompareInterrupt(uint16_t baseAddress,
                                           uint16_t captureCompareRegister
                                           )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        HWREG8(baseAddress + captureCompareRegister) &= ~CCIFG;
        HWREG16(baseAddress + captureCompareRegister) |= CCIE;
}
//*****************************************************************************
//
//! \brief Disable capture compare interrupt
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_disableCaptureCompareInterrupt(uint16_t baseAddress,
                                            uint16_t captureCompareRegister
                                            )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );
        HWREG16(baseAddress + captureCompareRegister) &= ~CCIE;
}
//*****************************************************************************
//
//! \brief Return capture compare interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister is the selected capture compare register
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param mask is the mask for the interrupt status
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_CAPTURE_OVERFLOW
//!        - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_FLAG
//!
//! \return Logical OR of any of the following:
//!         - \b TIMER_D_CAPTURE_OVERFLOW
//!         - \b TIMER_D_CAPTURECOMPARE_INTERRUPT_FLAG
//!         \n indicating the status of the masked flags
//
//*****************************************************************************

uint32_t TIMER_D_getCaptureCompareInterruptStatus(uint16_t baseAddress,
                                                  uint16_t captureCompareRegister,
                                                  uint16_t mask
                                                  )
{
        return HWREG16(baseAddress + captureCompareRegister) & mask;
}
//*****************************************************************************
//
//! \brief Returns High Resolution interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask for the interrupt status
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return Logical OR of any of the following:
//!         - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!         - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!         - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!         - \b TIMER_D_HIGH_RES_FAIL_LOW
//!         \n indicating the status of the masked interrupts
//
//*****************************************************************************

uint16_t TIMER_D_getHighResInterruptStatus(uint16_t baseAddress,
                                           uint16_t mask)
{
        mask = (mask >> 8);
        return (HWREG16(baseAddress + OFS_TDxHINT) & mask) << 8;
}
//*****************************************************************************
//
//! \brief Reset/Clear the timer clock divider, count direction, count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_clear(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxCTL0) |= TDCLR;
}
//*****************************************************************************
//
//! \brief Clears High Resolution interrupt status
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param mask is the mask for the interrupts to clear
//!        Mask value is the logical OR of any of the following:
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_UNLOCK
//!        - \b TIMER_D_HIGH_RES_FREQUENCY_LOCK
//!        - \b TIMER_D_HIGH_RES_FAIL_HIGH
//!        - \b TIMER_D_HIGH_RES_FAIL_LOW
//!
//! Modified bits of \b TDxHINT register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_clearHighResInterruptStatus(uint16_t baseAddress,
                                         uint16_t mask)
{
        mask = (mask >> 8);
        HWREG16(baseAddress + OFS_TDxHINT) &= ~mask;
}
//*****************************************************************************
//
//! \brief Get synchronized capturecompare input
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param synchronized is to select type of capture compare input.
//!        Valid values are:
//!        - \b TIMER_D_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT
//!        - \b TIMER_D_READ_CAPTURE_COMPARE_INPUT
//!
//! \return One of the following:
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT_HIGH
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT_LOW
//
//*****************************************************************************

uint8_t TIMER_D_getSynchronizedCaptureCompareInput
        (uint16_t baseAddress,
        uint16_t captureCompareRegister,
        uint16_t synchronized
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        assert((TIMER_D_READ_CAPTURE_COMPARE_INPUT == synchronized) ||
               (TIMER_D_READ_SYNCHRONIZED_CAPTURECOMPAREINPUT == synchronized)
               );

        if (HWREG16(baseAddress + captureCompareRegister) & synchronized)
                return TIMER_D_CAPTURECOMPARE_INPUT_HIGH;
        else
                return TIMER_D_CAPTURECOMPARE_INPUT_LOW;
}
//*****************************************************************************
//
//! \brief Get output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return One of the following:
//!         - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH
//!         - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW
//
//*****************************************************************************

uint8_t TIMER_D_getOutputForOutputModeOutBitValue
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        if (HWREG16(baseAddress + captureCompareRegister) & OUT)
                return TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH;
        else
                return TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW;
}
//*****************************************************************************
//
//! \brief Get current capturecompare count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return current count as uint16_t
//
//*****************************************************************************

uint16_t TIMER_D_getCaptureCompareCount
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        return HWREG16(baseAddress + captureCompareRegister + 2);
}
//*****************************************************************************
//
//! \brief Get current capture compare latch register count
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return current count as uint16_t
//
//*****************************************************************************

uint16_t TIMER_D_getCaptureCompareLatchCount
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        return HWREG16(baseAddress + captureCompareRegister + 4);
}
//*****************************************************************************
//
//! \brief Get current capturecompare input signal
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! \return One of the following:
//!         - \b TIMER_D_CAPTURECOMPARE_INPUT
//!         - \b 0x00
//!         \n indicating the current input signal
//
//*****************************************************************************

uint8_t TIMER_D_getCaptureCompareInputSignal
        (uint16_t baseAddress,
        uint16_t captureCompareRegister
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        return (HWREG8(baseAddress + captureCompareRegister) & CCI);
}
//*****************************************************************************
//
//! \brief Set output bit for output mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param outputModeOutBitValue the value to be set for out bit
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW
//!
//! Modified bits of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_setOutputForOutputModeOutBitValue
        (uint16_t baseAddress,
        uint16_t captureCompareRegister,
        uint8_t outputModeOutBitValue
        )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        assert((TIMER_D_OUTPUTMODE_OUTBITVALUE_HIGH == outputModeOutBitValue) ||
               (TIMER_D_OUTPUTMODE_OUTBITVALUE_LOW == outputModeOutBitValue)
               );

        HWREG16(baseAddress + captureCompareRegister) &= ~OUT;
        HWREG16(baseAddress + captureCompareRegister) |= outputModeOutBitValue;
}
//*****************************************************************************
//
//! \brief Generate a PWM with timer running in up mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for PWM configuration.
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_outputPWM(uint16_t baseAddress, TIMER_D_outputPWMParam *param)
{
        assert(param != 0);

        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == param->compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == param->compareRegister)
               );

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        assert((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET_SET == param->compareOutputMode)
               );

        HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0)  &=
                ~(TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK +
                  TIMER_D_UPDOWN_MODE + TIMER_D_DO_CLEAR +
                  TIMER_D_TDIE_INTERRUPT_ENABLE +
                  ID__8
                  );

        HWREG16(baseAddress + OFS_TDxCTL0) |= param->clockSource;
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));

        HWREG16(baseAddress + OFS_TDxCTL0) |= (TIMER_D_UP_MODE +
                                               TIMER_D_DO_CLEAR +
                                               ((param->clockSourceDivider >> 3) << 6));

        HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;

        HWREG16(baseAddress + OFS_TDxCCTL0)  &=
                ~(TIMER_D_CAPTURECOMPARE_INTERRUPT_ENABLE +
                  TIMER_D_OUTPUTMODE_RESET_SET
                  );
        HWREG16(baseAddress + param->compareRegister) |= param->compareOutputMode;

        HWREG16(baseAddress + param->compareRegister + 2) = param->dutyCycle;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Generate a PWM with timer running in up mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param timerPeriod is the specified timer period
//! \param compareRegister selects the compare register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_D_OUTPUTMODE_SET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_D_OUTPUTMODE_SET_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE
//!        - \b TIMER_D_OUTPUTMODE_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_D_OUTPUTMODE_RESET_SET
//! \param dutyCycle specifies the dutycycle for the generated waveform
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_generatePWM(  uint16_t baseAddress,
                           uint16_t clockSource,
                           uint16_t clockSourceDivider,
                           uint16_t clockingMode,
                           uint16_t timerPeriod,
                           uint16_t compareRegister,
                           uint16_t compareOutputMode,
                           uint16_t dutyCycle
                           )
{
        TIMER_D_outputPWMParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.timerPeriod = timerPeriod;
        param.compareRegister = compareRegister;
        param.compareOutputMode = compareOutputMode;
        param.dutyCycle = dutyCycle;

        TIMER_D_outputPWM(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Stops the timer
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_stop( uint16_t baseAddress )
{
        HWREG16(baseAddress + OFS_TDxCTL0)  &= ~MC_3;
        HWREG16(baseAddress + OFS_TDxCTL0)  |= MC_0;
}
//*****************************************************************************
//
//! \brief Sets the value of the capture-compare register
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the Capture register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareValue is the count to be compared with in compare mode
//!
//! Modified bits of \b TDxCCRn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_setCompareValue(  uint16_t baseAddress,
                               uint16_t compareRegister,
                               uint16_t compareValue
                               )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == compareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == compareRegister)
               );

        HWREG16(baseAddress + compareRegister + 0x02) = compareValue;
}
//*****************************************************************************
//
//! \brief Clears the Timer TDIFG interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDIFG of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_clearTimerInterruptFlag(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxCTL0) &= ~TDIFG;
}
//*****************************************************************************
//
//! \brief Clears the capture-compare interrupt flag
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param captureCompareRegister selects the Capture-compare register being
//!        used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//!
//! Modified bits are \b CCIFG of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_clearCaptureCompareInterruptFlag(uint16_t baseAddress,
                                              uint16_t captureCompareRegister
                                              )
{
        assert((TIMER_D_CAPTURECOMPARE_REGISTER_0 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_1 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_2 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_3 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_4 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_5 == captureCompareRegister) ||
               (TIMER_D_CAPTURECOMPARE_REGISTER_6 == captureCompareRegister)
               );

        HWREG16(baseAddress + captureCompareRegister)  &= ~CCIFG;
}
//*****************************************************************************
//
//! \brief Configures TIMER_D in free running mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param desiredHighResFrequency selects the desired High Resolution
//!        frequency used.
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_64MHZ
//!        - \b TIMER_D_HIGHRES_128MHZ
//!        - \b TIMER_D_HIGHRES_200MHZ
//!        - \b TIMER_D_HIGHRES_256MHZ
//!
//! Modified bits of \b TDxHCTL1 register, bits of \b TDxHCTL0 register and
//! bits of \b TDxCTL1 register.
//!
//! \return STATUS_SUCCESS or STATUS_FAIL
//
//*****************************************************************************

uint8_t TIMER_D_initHighResGeneratorInFreeRunningMode
        (uint16_t baseAddress,
        uint8_t desiredHighResFrequency
        )
{
        struct s_TLV_Timer_D_Cal_Data * pTD0CAL;
        uint8_t TD0CAL_bytes;

        assert((TIMER_D_HIGHRES_64MHZ == desiredHighResFrequency) ||
               (TIMER_D_HIGHRES_128MHZ == desiredHighResFrequency) ||
               (TIMER_D_HIGHRES_200MHZ == desiredHighResFrequency) ||
               (TIMER_D_HIGHRES_256MHZ == desiredHighResFrequency)
               );

        // Read the TimerD TLV Data
        TLV_getInfo(TLV_TAG_TIMER_D_CAL,
                    0,
                    &TD0CAL_bytes,
                    (uint16_t**)&pTD0CAL
                    );

        if (0x00 == TD0CAL_bytes)
                // No TimerD free running cal data found
                return STATUS_FAIL;

        HWREG16(baseAddress + OFS_TDxHCTL1) = TDHCLKTRIM6;
        HWREG16(baseAddress + OFS_TDxCTL1) = 0x00;
        HWREG16(baseAddress + OFS_TDxHCTL0) = 0x00;

        switch ( desiredHighResFrequency ) {
        case TIMER_D_HIGHRES_64MHZ:
                HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_64;
                break;

        case TIMER_D_HIGHRES_128MHZ:
                HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_128;
                break;

        case TIMER_D_HIGHRES_200MHZ:
                HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_200;
                break;

        case TIMER_D_HIGHRES_256MHZ:
                HWREG16(baseAddress + OFS_TDxHCTL1) = pTD0CAL->TDH0CTL1_256;
                break;
        }

        // Select Hi-res local clock
        HWREG16(baseAddress + OFS_TDxCTL1) |= TDCLKM_1;

        // CALEN=0 => free running mode; enable Hi-res mode
        if (TIMER_D_HIGHRES_256MHZ == desiredHighResFrequency)
                HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHM_1;

        HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHEN;

        return STATUS_SUCCESS;

}
//*****************************************************************************
//
//! \brief Configures TIMER_D in Regulated mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for high resolution generator in
//!        regulated mode.
//!
//! Modified bits of \b TDxHCTL0 register, bits of \b TDxCTL0 register and bits
//! of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_initHighResGeneratorInRegulatedMode(uint16_t baseAddress,
                                                 TIMER_D_initHighResGeneratorInRegulatedModeParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        assert((TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_8x == param->highResClockMultiplyFactor) ||
               (TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_16x == param->highResClockMultiplyFactor)
               );

        assert((TIMER_D_HIGHRES_CLK_DIVIDER_1 == param->highResClockDivider) ||
               (TIMER_D_HIGHRES_CLK_DIVIDER_2 == param->highResClockDivider) ||
               (TIMER_D_HIGHRES_CLK_DIVIDER_4 == param->highResClockDivider) ||
               (TIMER_D_HIGHRES_CLK_DIVIDER_8 == param->highResClockDivider)
               );

        /**********how abt MCx and TDCLGRPx and CNTLx*/
        HWREG16(baseAddress + OFS_TDxCTL0) &= ~(TDSSEL_3 + TDHD_3 + TDCLR + ID__8);
        HWREG16(baseAddress + OFS_TDxCTL1) &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0) |= (param->clockSource +
                                               ((param->clockSourceDivider >> 3) << 6));
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));

        // Select Hi-res local clock
        // Calibration and Hi-res mode enable
        HWREG16(baseAddress + OFS_TDxCTL1) |= TDCLKM_1;
        // Select Hi-res local clock
        HWREG16(baseAddress + OFS_TDxHCTL0) =  TDHREGEN + TDHEN;
        HWREG16(baseAddress + OFS_TDxHCTL0) |= param->highResClockMultiplyFactor +
                                               param->highResClockDivider;
}

//*****************************************************************************
//
//! \brief DEPRECATED - Configures TIMER_D in Regulated mode
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param highResClockMultiplyFactor selects the high resolution multiply
//!        factor.
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_8x
//!        - \b TIMER_D_HIGHRES_CLK_MULTIPLY_FACTOR_16x
//! \param highResClockDivider selects the high resolution divider.
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_CLK_DIVIDER_1
//!        - \b TIMER_D_HIGHRES_CLK_DIVIDER_2
//!        - \b TIMER_D_HIGHRES_CLK_DIVIDER_4
//!        - \b TIMER_D_HIGHRES_CLK_DIVIDER_8
//!
//! Modified bits of \b TDxHCTL0 register, bits of \b TDxCTL0 register and bits
//! of \b TDxCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_configureHighResGeneratorInRegulatedMode(uint16_t baseAddress,
                                                      uint16_t clockSource,
                                                      uint16_t clockSourceDivider,
                                                      uint16_t clockingMode,
                                                      uint8_t highResClockMultiplyFactor,
                                                      uint8_t highResClockDivider
                                                      )
{
        TIMER_D_initHighResGeneratorInRegulatedModeParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.highResClockMultiplyFactor = highResClockMultiplyFactor;
        param.highResClockDivider = highResClockDivider;

        TIMER_D_initHighResGeneratorInRegulatedMode(baseAddress, &param);

}
//*****************************************************************************
//
//! \brief Combine TDCCR to get PWM
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param param is the pointer to struct for PWM generation using two CCRs.
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************
void TIMER_D_combineTDCCRToOutputPWM(uint16_t baseAddress,
                                     TIMER_D_combineTDCCRToOutputPWMParam *param)
{
        assert(param != 0);

        assert(
                (TIMER_D_COMBINE_CCR1_CCR2 == param->combineCCRRegistersCombination) ||
                (TIMER_D_COMBINE_CCR3_CCR4 == param->combineCCRRegistersCombination) ||
                (TIMER_D_COMBINE_CCR5_CCR6 == param->combineCCRRegistersCombination)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_ACLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_SMCLK == param->clockSource) ||
                (TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK == param->clockSource)
                );

        assert(
                (TIMER_D_CLOCKSOURCE_DIVIDER_1 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_2 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_4 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_8 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_3 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_5 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_6 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_7 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_10 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_12 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_14 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_16 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_20 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_24 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_28 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_32 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_40 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_48 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_56 == param->clockSourceDivider) ||
                (TIMER_D_CLOCKSOURCE_DIVIDER_64 == param->clockSourceDivider)
                );

        assert(
                (TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK == param->clockingMode) ||
                (TIMER_D_CLOCKINGMODE_AUXILIARY_CLK == param->clockingMode)
                );

        assert((TIMER_D_OUTPUTMODE_OUTBITVALUE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_SET_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_TOGGLE_SET == param->compareOutputMode) ||
               (TIMER_D_OUTPUTMODE_RESET_SET == param->compareOutputMode)
               );

        HWREG16(baseAddress + OFS_TDxCCTL2) &= ~OUTMOD_7;
        HWREG16(baseAddress + OFS_TDxCCTL2)  |= param->compareOutputMode;

        HWREG16(baseAddress + OFS_TDxCCR0)  = param->timerPeriod;

        HWREG16(baseAddress + OFS_TDxCCR1 + (0x05 *
                                             (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2))) = param->dutyCycle1;
        HWREG16(baseAddress + OFS_TDxCCR2 + (0x05 *
                                             (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2))) = param->dutyCycle2;

        HWREG16(baseAddress + OFS_TDxCTL0)  &= ~ID__8;
        HWREG16(baseAddress + OFS_TDxCTL1)  &= ~(TDCLKM0 + TDCLKM1 + TDIDEX_7);

        HWREG16(baseAddress + OFS_TDxCTL0) |= (param->clockSource +
                                               ((param->clockSourceDivider >> 3) << 6));
        HWREG16(baseAddress + OFS_TDxCTL1) |= (param->clockingMode +
                                               ((param->clockSourceDivider & 0x7) << 8));
        HWREG16(baseAddress + OFS_TDxCTL1) |=
                (TD2CMB << (param->combineCCRRegistersCombination - TIMER_D_COMBINE_CCR1_CCR2));
} //*****************************************************************************
//
//! \brief DEPRECATED - Combine TDCCR to get PWM
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param clockSource selects Clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_EXTERNAL_TDCLK [Default]
//!        - \b TIMER_D_CLOCKSOURCE_ACLK
//!        - \b TIMER_D_CLOCKSOURCE_SMCLK
//!        - \b TIMER_D_CLOCKSOURCE_INVERTED_EXTERNAL_TDCLK
//! \param clockSourceDivider is the divider for clock source.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_1 [Default]
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_2
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_3
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_4
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_5
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_6
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_7
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_8
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_10
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_12
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_14
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_16
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_20
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_24
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_28
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_32
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_40
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_48
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_56
//!        - \b TIMER_D_CLOCKSOURCE_DIVIDER_64
//! \param clockingMode is the selected clock mode register values.
//!        Valid values are:
//!        - \b TIMER_D_CLOCKINGMODE_EXTERNAL_CLOCK [Default]
//!        - \b TIMER_D_CLOCKINGMODE_HIRES_LOCAL_CLOCK
//!        - \b TIMER_D_CLOCKINGMODE_AUXILIARY_CLK
//! \param timerPeriod is the specified timer period
//! \param combineCCRRegistersCombination selects desired CCR registers to
//!        combine
//!        Valid values are:
//!        - \b TIMER_D_COMBINE_CCR1_CCR2
//!        - \b TIMER_D_COMBINE_CCR3_CCR4 - (available on TIMER_D5, TIMER_D7)
//!        - \b TIMER_D_COMBINE_CCR5_CCR6 - (available only on TIMER_D7)
//! \param compareOutputMode specifies the output mode.
//!        Valid values are:
//!        - \b TIMER_D_OUTPUTMODE_OUTBITVALUE [Default]
//!        - \b TIMER_D_OUTPUTMODE_SET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_RESET
//!        - \b TIMER_D_OUTPUTMODE_SET_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE
//!        - \b TIMER_D_OUTPUTMODE_RESET
//!        - \b TIMER_D_OUTPUTMODE_TOGGLE_SET
//!        - \b TIMER_D_OUTPUTMODE_RESET_SET
//! \param dutyCycle1 specifies the dutycycle for the generated waveform
//! \param dutyCycle2 specifies the dutycycle for the generated waveform
//!
//! Modified bits of \b TDxCCTLn register, bits of \b TDxCCR0 register, bits of
//! \b TDxCCTL0 register, bits of \b TDxCTL0 register and bits of \b TDxCTL1
//! register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_combineTDCCRToGeneratePWM(  uint16_t baseAddress,
                                         uint16_t clockSource,
                                         uint16_t clockSourceDivider,
                                         uint16_t clockingMode,
                                         uint16_t timerPeriod,
                                         uint16_t combineCCRRegistersCombination,
                                         uint16_t compareOutputMode,
                                         uint16_t dutyCycle1,
                                         uint16_t dutyCycle2
                                         )
{
        TIMER_D_combineTDCCRToOutputPWMParam param = { 0 };

        param.clockSource = clockSource;
        param.clockSourceDivider = clockSourceDivider;
        param.clockingMode = clockingMode;
        param.timerPeriod = timerPeriod;
        param.combineCCRRegistersCombination = combineCCRRegistersCombination;
        param.compareOutputMode = compareOutputMode;
        param.dutyCycle1 = dutyCycle1;
        param.dutyCycle2 = dutyCycle2;

        TIMER_D_combineTDCCRToOutputPWM(baseAddress, &param);
}
//*****************************************************************************
//
//! \brief Selects TIMER_D Latching Group
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param groupLatch selects the group latch
//!        Valid values are:
//!        - \b TIMER_D_GROUP_NONE [Default]
//!        - \b TIMER_D_GROUP_CL12_CL23_CL56
//!        - \b TIMER_D_GROUP_CL123_CL456
//!        - \b TIMER_D_GROUP_ALL
//!
//! Modified bits are \b TDCLGRP of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_selectLatchingGroup(uint16_t baseAddress,
                                 uint16_t groupLatch)
{
        assert((TIMER_D_GROUP_NONE  == groupLatch) ||
               (TIMER_D_GROUP_CL12_CL23_CL56 == groupLatch) ||
               (TIMER_D_GROUP_CL123_CL456 == groupLatch) ||
               (TIMER_D_GROUP_ALL == groupLatch)
               );

        HWREG16(baseAddress + OFS_TDxCTL0) &= ~TDCLGRP_3;
        HWREG16(baseAddress + OFS_TDxCTL0) |= groupLatch;
}
//*****************************************************************************
//
//! \brief Selects TIMER_D counter length
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param counterLength selects the value of counter length.
//!        Valid values are:
//!        - \b TIMER_D_COUNTER_16BIT [Default]
//!        - \b TIMER_D_COUNTER_12BIT
//!        - \b TIMER_D_COUNTER_10BIT
//!        - \b TIMER_D_COUNTER_8BIT
//!
//! Modified bits are \b CNTL of \b TDxCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_selectCounterLength(uint16_t baseAddress,
                                 uint16_t counterLength
                                 )
{
        assert((TIMER_D_COUNTER_8BIT == counterLength) ||
               (TIMER_D_COUNTER_10BIT == counterLength) ||
               (TIMER_D_COUNTER_12BIT == counterLength) ||
               (TIMER_D_COUNTER_16BIT == counterLength)
               );

        HWREG16(baseAddress + OFS_TDxCTL0) &= ~CNTL_3;
        HWREG16(baseAddress + OFS_TDxCTL0) |= counterLength;
}
//*****************************************************************************
//
//! \brief Selects Compare Latch Load Event
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param compareRegister selects the compare register being used.
//!        Valid values are:
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_0
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_1
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_2
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_3
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_4
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_5
//!        - \b TIMER_D_CAPTURECOMPARE_REGISTER_6
//! \param compareLatchLoadEvent selects the latch load event
//!        Valid values are:
//!        - \b TIMER_D_LATCH_ON_WRITE_TO_TDxCCRn_COMPARE_REGISTER [Default]
//!        - \b TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UP_OR_CONT_MODE
//!        - \b TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UPDOWN_MODE
//!        - \b
//!           TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_CURRENT_COMPARE_LATCH_VALUE
//!
//! Modified bits are \b CLLD of \b TDxCCTLn register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_initCompareLatchLoadEvent(uint16_t baseAddress,
                                       uint16_t compareRegister,
                                       uint16_t compareLatchLoadEvent
                                       )
{
        assert((TIMER_D_LATCH_ON_WRITE_TO_TDxCCRn_COMPARE_REGISTER  == compareLatchLoadEvent) ||
               (TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UP_OR_CONT_MODE == compareLatchLoadEvent) ||
               (TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_0_IN_UPDOWN_MODE == compareLatchLoadEvent) ||
               (TIMER_D_LATCH_WHEN_COUNTER_COUNTS_TO_CURRENT_COMPARE_LATCH_VALUE
                == compareLatchLoadEvent)
               );

        HWREG16(baseAddress + compareRegister)  &= ~CLLD_3;
        HWREG16(baseAddress + compareRegister)  |= compareLatchLoadEvent;
}
//*****************************************************************************
//
//! \brief Disable High Resolution fast wakeup
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHFW of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_disableHighResFastWakeup(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHFW;
}
//*****************************************************************************
//
//! \brief Enable High Resolution fast wakeup
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHFW of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_enableHighResFastWakeup(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHFW;
}
//*****************************************************************************
//
//! \brief Disable High Resolution Clock Enhanced Accuracy
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHEAEN of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_disableHighResClockEnhancedAccuracy(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHEAEN;
}
//*****************************************************************************
//
//! \brief Enable High Resolution Clock Enhanced Accuracy
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHEAEN of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_enableHighResClockEnhancedAccuracy(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHEAEN;
}
//*****************************************************************************
//
//! \brief Disable High Resolution Clock Enhanced Accuracy
//!
//! High-resolution generator is on if the TIMER_D counter
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHRON of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_DisableHighResGeneratorForceON(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) &= ~TDHRON;
}
//*****************************************************************************
//
//! \brief Enable High Resolution Clock Enhanced Accuracy
//!
//! High-resolution generator is on in all TIMER_D MCx modes. The PMM remains
//! in high-current mode.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! Modified bits are \b TDHRON of \b TDxHCTL0 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_EnableHighResGeneratorForceON(uint16_t baseAddress)
{
        HWREG16(baseAddress + OFS_TDxHCTL0) |= TDHRON;
}
//*****************************************************************************
//
//! \brief Select High Resolution Coarse Clock Range
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param highResCoarseClockRange selects the High Resolution Coarse Clock
//!        Range
//!        Valid values are:
//!        - \b TIMER_D_HIGHRES_BELOW_15MHz [Default]
//!        - \b TIMER_D_HIGHRES_ABOVE_15MHz
//!
//! Modified bits are \b TDHCLKCR of \b TDxHCTL1 register.
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_selectHighResCoarseClockRange(uint16_t baseAddress,
                                           uint16_t highResCoarseClockRange
                                           )
{
        assert((TIMER_D_HIGHRES_BELOW_15MHz  == highResCoarseClockRange) ||
               (TIMER_D_HIGHRES_ABOVE_15MHz == highResCoarseClockRange)
               );
        HWREG16(baseAddress + OFS_TDxHCTL1) &= ~TDHCLKCR;
        HWREG16(baseAddress + OFS_TDxHCTL1) |= highResCoarseClockRange;
}
//*****************************************************************************
//
//! \brief Select High Resolution Clock Range Selection
//!
//! \param baseAddress is the base address of the TIMER_D module.
//! \param highResClockRange selects the High Resolution Clock Range. Refer to
//!        datasheet for frequency details
//!        Valid values are:
//!        - \b TIMER_D_CLOCK_RANGE0 [Default]
//!        - \b TIMER_D_CLOCK_RANGE1
//!        - \b TIMER_D_CLOCK_RANGE2
//!
//! \return None
//
//*****************************************************************************

void TIMER_D_selectHighResClockRange(uint16_t baseAddress,
                                     uint16_t highResClockRange
                                     )
{
        assert((TIMER_D_CLOCK_RANGE0  == highResClockRange) ||
               (TIMER_D_CLOCK_RANGE1 == highResClockRange)   ||
               (TIMER_D_CLOCK_RANGE2 == highResClockRange)
               );
        HWREG16(baseAddress + OFS_TDxHCTL1) &= ~TDHCLKCR;
        HWREG16(baseAddress + OFS_TDxHCTL1) |= highResClockRange;
}
//*****************************************************************************
//
//! \brief Reads the current timer count value
//!
//! Reads the current count value of the timer. There is a majority vote system
//! in place to confirm an accurate value is returned. The TIMER_D_THRESHOLD
//! #define in the corresponding header file can be modified so that the votes
//! must be closer together for a consensus to occur.
//!
//! \param baseAddress is the base address of the TIMER_D module.
//!
//! \return Majority vote of timer count value
//
//*****************************************************************************

uint16_t TIMER_D_getCounterValue(uint16_t baseAddress)
{
        uint16_t voteOne, voteTwo, res;

        voteTwo = HWREG16(baseAddress + OFS_TDxR);

        do {
                voteOne = voteTwo;
                voteTwo = HWREG16(baseAddress + OFS_TDxR);

                if (voteTwo > voteOne)
                        res = voteTwo - voteOne;
                else if (voteOne > voteTwo)
                        res = voteOne - voteTwo;
                else
                        res = 0;

        } while ( res > TIMER_D_THRESHOLD);

        return voteTwo;
}

#endif
//*****************************************************************************
//
//! Close the doxygen group for timer_d_api
//! @}
//
//*****************************************************************************
