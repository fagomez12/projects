/**
 * @file PID.h
 * @brief Public interface for the PID module library.
 *
 * Contains all function declarations and data structures needed
 * to use the PID module library.
 *
 * @defgroup pid_module PID
 * @brief Functions and data structures related to PID control.
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

#ifndef LIB_PID
#define LIB_PID

#ifdef __cplusplus
extern "C"
{
#endif
//
// Definitions
//
#define BUFFER_SIZE (2)                   //!< @def BUFFER_SIZE @brief Defines the size of the buffer related to the number of past events to consider
#define MAX(a, b) ((a) > (b) ? (a) : (b)) //!< @def MAX(a,b) @brief Finds the maximum between the two inputs
#define MIN(a, b) ((a) < (b) ? (a) : (b)) //!< @def MIN(a,b) @brief Finds the minimum between the two inputs

    //
    //  Data Structures
    //

    /** @struct PID_CONTROL @brief This structure contains the PID controller parameters */
    typedef struct
    {
        Uint8 type_controller;    //!< @brief This variable storages the Type of Controller P = 0, PI = 1, PD = 2, PID = 3
        float32 execution_rate;   //!< @brief This variable storages the Execution Rate
        float32 freq_cutoff;      //!< @brief This variable storages the Cut-off frequency in Hz for the derivative part D(z)
        float32 kp;               //!< @brief This variable storages the proportional gain
        float32 ki;               //!< @brief This variable storages the integrative gain
        float32 kd;               //!< @brief This variable storages the derivative gain
        float32 kwp;              //!< @brief Defines the windup artificial gain
        float32 lower_saturation; //!< @brief Defines the minimum limit of integration
        float32 upper_saturation; //!< @brief Defines the maximum limit of integration
        float32 x[BUFFER_SIZE];   //!< @brief This variable storages the buffer that represents the input history x[n], x[n-1], ...  , x[n-1- @ref BUFFER_SIZE]
        float32 y[BUFFER_SIZE];   //!< @brief This variable storages the buffer that represents the output history y[n], y[n-1], ... , x[n-1- @ref BUFFER_SIZE]
    } PID_CONTROL;

    //
    // Functions
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
    float32 pid_controller(PID_CONTROL *, float32);

    float32 proportional_pid(PID_CONTROL *);
    float32 integrative_pid(PID_CONTROL *);
    float32 derivative_pid(PID_CONTROL *);

#ifdef __cplusplus
}
#endif // extern "C"
#endif
/** @} */ // end of IIR_Filter_Module
