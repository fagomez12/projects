/**
 ******************************************************************************
 * @file           : lib.c
 * @brief          : Source file for lib.h module.
 * @author         : F. Gómez
 * @date           : 2025
 * @version        : 1.0.0
 * @copyright      : Copyright (c) 2025 GOIC Embedded Systems
 ******************************************************************************
 * @attention
 *
 * MIT License
 *
 * Copyright (c) 2025 GOIC Embedded Systems
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

/*******************************************************************************/
/* Includes -------------------------------------------------------------------*/
/*******************************************************************************/

#include "lib.h"

/*******************************************************************************/
/* Private Define -------------------------------------------------------------*/
/*******************************************************************************/
/**
 * @def PI value with 8 decimals after the ","
 */
#define PI_ (3.14159265f)

/*******************************************************************************/
/* Private / Public  Variables ------------------------------------------------*/
/*******************************************************************************/

/**
 * @brief Sin(x) Look-Up-Table of size @see SIN_LUT_SIZE in format Q15
 */
static q15_t sq15_Sin_LUT[SIN_LUT_SIZE] = {0};

/*******************************************************************************/
/* Private Function Implementations -------------------------------------------*/
/*******************************************************************************/
/*--None*/

/*******************************************************************************/
/* Public Function Implementations --------------------------------------------*/
/*******************************************************************************/

/**
 * @brief   Initializes the sine Look-Up-Table (LUT) in Q15 format.
 *
 * This function fills the private LUT array `sq15_Sin_LUT` with
 * precomputed sine values scaled to Q15 fixed-point representation.
 * The LUT size is defined by `SIN_LUT_SIZE`, and each entry corresponds
 * to one equally spaced angle over a full 2π cycle.
 *
 * @return  None.
 * @note    Should be called once during system initialization
 *          (e.g., in main loop or RTOS task) before using the LUT.
 */
void vfn_InitSin_LUT_q15()
{
    uint16_t u16_Idx; /**< Loop index for LUT entries. */

    /* Iterate through all LUT positions. */
    for (u16_Idx = 0; u16_Idx < SIN_LUT_SIZE; u16_Idx++)
    {
        /*
         * Compute sine value:
         * - Angle = (2π * index / LUT size)
         * - sinf() returns float in range [-1.0, +1.0]
         * - Multiply by INT16_MAX (32767) to scale into Q15 range
         * - Cast to q15_t (typically int16_t)
         */
        sq15_Sin_LUT[u16_Idx] =
            (q15_t)(INT16_MAX * 0.5 * sinf(2.0f * PI_ * u16_Idx / SIN_LUT_SIZE));
    }
}

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
q15_t xfn_GetCurrentSinVal(uint16_t u16_FreqSampl,
                           uint16_t u16_FreqSignal,
                           uint16_t u16_PhaseWrap, uint16_t *u16_Idx)
{
    //    static uint16_t u16_Idx = 0;                          /**< Persistent sample index across calls. */
    uint32_t delta_phase = 0;                             /**< Phase increment per sample. */
    uint16_t u16_LUP_Idx = 0;                             /**< Lookup table index. */
    uint16_t u16_MaxIdx = u16_FreqSampl / u16_FreqSignal; /**< Max index before wrap. */
    uint32_t phase_acc = 0;                               /**< Phase accumulator value. */
    uint16_t phase_wrap = 0;                              /**< Phase offset in LUT units. */
    uint16_t currentIdx = 0;                              /**< Placeholder for current index (unused). */

    /* Convert phase offset (degrees) into LUT index offset. */
    phase_wrap = (uint16_t)(u16_PhaseWrap * SIN_LUT_SIZE / 360);

    /* Compute phase increment: maps signal frequency to full 32-bit phase range. */
    delta_phase = (uint32_t)((uint64_t)u16_FreqSignal * UINT32_MAX / u16_FreqSampl);

    /* Accumulate phase based on current index. */
    phase_acc = delta_phase * (*u16_Idx);

    /* Derive LUT index: shift down to 12-bit resolution and add phase offset. */
    u16_LUP_Idx = ((phase_acc >> 20) & (SIN_LUT_SIZE - 1)) + phase_wrap; // 32-12=20

    /* Increment sample index and wrap around if exceeding LUT size. */
    if (++(*u16_Idx) > SIN_LUT_SIZE)
        (*u16_Idx) = 0;

    /* Wrap LUT index if exceeding table size. */
    if (u16_LUP_Idx > SIN_LUT_SIZE)
    {
        return sq15_Sin_LUT[u16_LUP_Idx - SIN_LUT_SIZE]; /**< Return wrapped sine value. */
    }
    else
    {
        return sq15_Sin_LUT[u16_LUP_Idx]; /**< Return sine value at index. */
    }
}
/* End of File */
