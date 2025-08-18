/**
 * @file IIR.h
 * @brief Public interface for the IIR Filter module library.
 *
 * Contains all function declarations and data structures needed
 * to use the IIR Filter module library.
 *
 * @defgroup iir_filter_module IIR Filter Module
 * @brief Functions and data structures related to IIR filtering.
 * @author Felipe A. Gómez Olaya
 * @date 2025
 * @copyright
 *
 * Copyright (C) 2025 Felipe Antonio Gomez Olaya
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * GitHub Repository: https://github.com/fagomez12/projects.git
 * @{
 */
//
// Include files
//
#include "typedef_lib.h"

#ifndef LIB_IIR
#define LIB_IIR

#ifdef __cplusplus
extern "C"
{
#endif
//
// Definitions
//
#define MAX_IIR_ORD_FILTER (8) //!< @def MAX_IIR_ORD_FILTER @brief Defines the maximum order filter possible

    //
    //  Data Structures
    //

    /** @struct IIR_FILTER @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER */
    typedef struct
    {
        Uint16 freq_sampling;              //!< @brief This variable storages the Sampling frequency in kHz
        Uint16 freq_cutoff;                //!< @brief This variable storages the Cut-off frequency in Kz
        Uint8 order;                       //!< @brief This variable storages the order of the filter
        float32 a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable storages the buffer that represents the coefficients a0, a1, a2, ...
        float32 b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable storages the buffer that represents the coefficients b0, b1, b2, ...
        float32 x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable storages the buffer that represents the input history x[0], x[1], ...
        float32 y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable storages the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER;

    //
    // functions
    //

    /**
     * @brief Applies an IIR filter to the current input sample.
     *
     * This function implements a direct form I IIR filter using the difference equation:
     *
     * \f[
     * y[n] = \sum_{i=0}^{N} b[i] \cdot x[n-i] - \sum_{j=1}^{N} a[j] \cdot y[n-j]
     * \f]
     *
     * where:
     * - `x[]` is the input sample history,
     * - `y[]` is the output sample history,
     * - `b[]` are the feedforward coefficients,
     * - `a[]` are the feedback coefficients,
     * - `N` is the filter order.
     *
     * The coefficient `a[0]` is assumed to be `1.0` and is not used in the computation.
     *
     * @param f Pointer to the IIR_FILTER structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @return The filtered output sample.
     *
     * @see IIR_FILTER
     */
    float32 iir_filter(IIR_FILTER *, float32);

#ifdef __cplusplus
}
#endif // extern "C"
#endif
/** @} */ // end of IIR_Filter_Module
