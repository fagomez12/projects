/**
 ******************************************************************************
 * @file           : lib.h
 * @brief          : Header for lib.c file.
 * @author         : F. Gómez
 * @date           : 2025
 * @version        : 1.0.0
 * @copyright      : Copyright (c) 2025 GOIC Embedded Systems
 ******************************************************************************
 * @attention
 *
 * MIT License
 *
 * Copyright (c) ${date} GOIC Embedded Systems
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LIB_INCLUDE_HEADER_
#define _LIB_INCLUDE_HEADER_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @defgroup lib_Module lib Module
 * @brief    Public API for lib driver.
 * @{
 */

/*******************************************************************************/
/* Includes -------------------------------------------------------------------*/
/*******************************************************************************/
#ifndef __ARM_MATH_H
#include <stdio.h>
#include <stdint.h>
#include <math.h>
    typedef int16_t q15_t;
    typedef int32_t q31_t;
    typedef int64_t q63_t;
#endif

    /*******************************************************************************/
    /* Exported Types -------------------------------------------------------------*/
    /*******************************************************************************/
    /**
     * @brief Librerie definitions
     */

    typedef enum
    {
        SIN_LUT_SIZE = 4096,
    } LibModuleDefs_t;

    /**
     * @brief Example of configuration structure.
     */
    /*
    typedef struct
    {
        uint32_t frequency;   ///< Operating frequency in Hz
        uint16_t voltage;     ///< Operating voltage in mV
    } ExampleConfig_t;
    */

    /*******************************************************************************/
    /* Exported Constants ---------------------------------------------------------*/
    /*******************************************************************************/
    /*--None*/
    /* #define EXAMPLE_DEFAULT_FREQUENCY   (1000U) */ /**< Default frequency in Hz */

    /*******************************************************************************/
    /* Exported Macros ------------------------------------------------------------*/
    /*******************************************************************************/

    /*******************************************************************************/
    /* Exported Variables ---------------------------------------------------------*/
    /*******************************************************************************/
    /**
     * @brief Sin(x) Look-Up-Table of size @see SIN_LUT_SIZE in format Q15
     */
    static q15_t sq15_Sin_LUT[SIN_LUT_SIZE];

    /*******************************************************************************/
    /* Exported Functions ---------------------------------------------------------*/
    /*******************************************************************************/

    /**
     * @brief   Get the current sine value from a Q15 lookup table.
     *
     * This function computes the sine value based on sampling frequency,
     * cutoff frequency, and phase wrap. It uses a phase accumulator and
     * lookup table indexing to generate sine wave samples in Q15 format.
     *
     * @param u16_FreqSampl   Sampling frequency (Hz).
     * @param u16_FreqSignal  Signal frequency (Hz).
     * @param u16_PhaseWrap   Phase offset in degrees (0–360).
     *
     * @return q15_t          Current sine sample in Q15 format.
     */
    extern q15_t xfn_GetCurrentSinVal(uint16_t u16_FreqSampl, uint16_t u16_FreqSignal, uint16_t phase, uint16_t *u16_Idx);

    /**
     * @brief   Initializes the sine Look-Up-Table (LUT) in Q15 format.
     *
     * This function fills the global LUT array `gq15_Sin_LUT` with
     * precomputed sine values scaled to Q15 fixed-point representation.
     * The LUT size is defined by `SIN_LUT_SIZE`, and each entry corresponds
     * to one equally spaced angle over a full 2π cycle.
     *
     * @return  None.
     * @note    Should be called once during system initialization
     *          (e.g., in main loop or RTOS task) before using the LUT.
     */
    extern void vfn_InitSin_LUT_q15();

    /**
     * @}
     */

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* _LIB_INCLUDE_HEADER_ */
/* End of File */
