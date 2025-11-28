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
 *  MIT License
 *
 * Copyright (c) 2025 FELIPE ANTONIO GÓMEZ OLAYA
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
 *
 * GitHub: https://github.com/fagomez12
 */

#ifndef LIB_IIR_H
#define LIB_IIR_H

#ifdef __cplusplus
extern "C"
{
#endif
//
// Include
//
#ifndef __ARM_MATH_H
#include <stdint.h>
    typedef int16_t q15_t;
    typedef int32_t q31_t;
    typedef int64_t q63_t;
#endif

//
// Definitions
//
#define MAX_IIR_ORD_FILTER (8u) //!< @def MAX_IIR_ORD_FILTER @brief Defines the maximum order filter possible

    //
    //  Type Definitions
    //

    //
    // Type Definitions
    //
    /** @def Filter Types */
    typedef enum
    {
        IIR_TYPE_Q15 = 0,
        IIR_TYPE_Q31,
        IIR_TYPE_F32
    } IIR_Type_t;

    /** @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER in format float*/
    typedef struct
    {
        uint16_t freq_sampling;          //!< @brief This variable store the Sampling frequency in kHz
        uint16_t freq_cutoff;            //!< @brief This variable store the Cut-off frequency in Kz
        uint8_t order;                   //!< @brief This variable store the order of the filter
        float a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients a0, a1, a2, ...
        float b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients b0, b1, b2, ...
        float x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the input history x[0], x[1], ...
        float y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER_f32_t;

    /** @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER in format int16_t*/
    typedef struct
    {
        uint16_t freq_sampling;            //!< @brief This variable store the Sampling frequency in kHz
        uint16_t freq_cutoff;              //!< @brief This variable store the Cut-off frequency in Kz
        uint8_t order;                     //!< @brief This variable store the order of the filter
        int16_t a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients a0, a1, a2, ...
        int16_t b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients b0, b1, b2, ...
        int16_t x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the input history x[0], x[1], ...
        int16_t y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER_int16_t;

    /** @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER in format int32_t*/
    typedef struct
    {
        uint16_t freq_sampling;            //!< @brief This variable store the Sampling frequency in kHz
        uint16_t freq_cutoff;              //!< @brief This variable store the Cut-off frequency in Kz
        uint8_t order;                     //!< @brief This variable store the order of the filter
        int32_t a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients a0, a1, a2, ...
        int32_t b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients b0, b1, b2, ...
        int32_t x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the input history x[0], x[1], ...
        int32_t y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER_int32_t;

    /** @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER in format Q15*/
    typedef struct
    {
        uint16_t freq_sampling;          //!< @brief This variable store the Sampling frequency in kHz
        uint16_t freq_cutoff;            //!< @brief This variable store the Cut-off frequency in Kz
        uint8_t order;                   //!< @brief This variable store the order of the filter
        q15_t a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients a0, a1, a2, ...
        q15_t b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients b0, b1, b2, ...
        q15_t x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the input history x[0], x[1], ...
        q15_t y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER_q15_t;

    /** @brief This structure contains the IIR parameters for a maximum order filter of @see MAX_IIR_ORD_FILTER in format Q31*/
    typedef struct
    {
        uint16_t freq_sampling;          //!< @brief This variable store the Sampling frequency in kHz
        uint16_t freq_cutoff;            //!< @brief This variable store the Cut-off frequency in Kz
        uint8_t order;                   //!< @brief This variable store the order of the filter
        q31_t a[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients a0, a1, a2, ...
        q31_t b[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the coefficients b0, b1, b2, ...
        q31_t x[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the input history x[0], x[1], ...
        q31_t y[MAX_IIR_ORD_FILTER + 1]; //!< @brief This variable store the buffer that represents the output history y[0], y[1], ...
    } IIR_FILTER_q31_t;

    //
    // functions
    //

    /**
     * @brief Applies an IIR filter to the current input sample in format float.
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
     * @param f Pointer to the IIR_FILTER_f32_t structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @return The filtered output sample.
     *
     * @see     float xfn_IrrFilter_f32(IIR_FILTER_f32_t *, float);

     */
    float xfn_IrrFilter_f32(IIR_FILTER_f32_t *, float);

    /**
     * @brief Applies an IIR filter to the current input sample in format Q15.
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
     * @param f Pointer to the IIR_FILTER_q15_t structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @return The filtered output sample.
     *
     * @see     q15_t xfn_IrrFilter_q15(IIR_FILTER_q15_t *, q15_t);

     */
    q15_t xfn_IrrFilter_q15(IIR_FILTER_q15_t *, q15_t);

    /**
     * @brief Applies an IIR filter to the current input sample in format Q31.
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
     * @param f Pointer to the IIR_FILTER_q31_t structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @return The filtered output sample.
     *
     * @see     q31_t xfn_IrrFilter_q31(IIR_FILTER_q31_t *, q31_t);

     */
    q31_t xfn_IrrFilter_q31(IIR_FILTER_q31_t *, q31_t);

    /**
     * @brief Applies an IIR filter to the current input sample in format int16_t.
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
     * The coefficient `a[0]` is assumed to be `1` and is not used in the computation.
     *
     * @param f Pointer to the IIR_FILTER_int16_t structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @param coeff_scale The scale used for the a[n] and b[n] coefficients
     * @return The filtered output sample.
     *
     * @see     int16_t xfn_IrrFilter_int16(IIR_FILTER_int16_t *f, int16_t new_input, int16_t coeff_scale);

     */
    int16_t xfn_IrrFilter_int16(IIR_FILTER_int16_t *f, int16_t new_input, int16_t coeff_scale);

    /**
     * @brief Applies an IIR filter to the current input sample in format int32_t.
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
     * The coefficient `a[0]` is assumed to be `1` and is not used in the computation.
     *
     * @param f Pointer to the  IIR_FILTER_int32_t structure containing coefficients and state.
     * @param new_input The new input sample to be filtered.
     * @param coeff_scale The scale used for the a[n] and b[n] coefficients
     * @return The filtered output sample.
     *
     * @see     int32_t xfn_IrrFilter_int32(IIR_FILTER_int32_t *f, int32_t new_input, int32_t coeff_scale);

     */
    int32_t xfn_IrrFilter_int32(IIR_FILTER_int32_t *f, int32_t new_input, int32_t coeff_scale);

    /**
     * @brief Saturates a 32-bit signed integer to int16_t range.
     *
     * This function takes a 32-bit signed integer (`int32_t`) and
     * clamps it to the range representable by Q15 (`int16_t`):
     * - Maximum: 0x7FFF
     * - Minimum: 0x8000
     *
     * @param val Input value of type int32_t to be saturated.
     * @return Saturated value as int16_t.
     *
     */
    static inline int16_t __IRRSATINT16(int32_t val)
    {
        if (val > 0x7FFF)
            return 0x7FFF;
        else if (val < 0x8000)
            return 0x8000;
        else
            return (int16_t)val;
    }

    /**
     * @brief Saturates a 64-bit signed integer to int32_t range.
     *
     * This function takes a 64-bit signed integer (`int64_t`) and
     * clamps it to the range representable by Q15 (`int32_t`):
     * - Maximum: 0x7FFFFFFF
     * - Minimum: 0x80000000
     *
     * @param val Input value of type int64_t to be saturated.
     * @return Saturated value as int32_t.
     *
     */
    static inline int32_t __IRRSATINT32(int64_t val)
    {
        if (val > 0x7FFFFFFF)
            return 0x7FFFFFFF;
        else if (val < 0x80000000)
            return 0x80000000;
        else
            return (int16_t)val;
    }

    /**
     * @brief Saturates a 32-bit signed integer to Q15 range.
     *
     * This function takes a 32-bit signed integer (`q31_t`) and
     * clamps it to the range representable by Q15 (`q15_t`):
     * - Maximum: 0x7FFF
     * - Minimum: 0x8000
     *
     * @param val Input value of type q31_t to be saturated.
     * @return Saturated value as q15_t.
     *
     */
    static inline q15_t __IRRSATQ15(q31_t val)
    {
        if (val > 0x7FFF)
            return 0x7FFF;
        else if (val < 0x8000)
            return 0x8000;
        else
            return (q15_t)val;
    }

    /**
     * @brief Saturates a 64-bit signed integer to Q31 range.
     *
     * This function takes a 64-bit signed integer (`q63_t`) and
     * clamps it to the range representable by Q15 (`q31_t`):
     * - Maximum: 0x7FFFFFFF
     * - Minimum: 0x80000000
     *
     * @param val Input value of type q63_t to be saturated.
     * @return Saturated value as q31_t.
     *
     */
    static inline q31_t __IRRSATQ31(q63_t val)
    {
        if (val > 0x7FFFFFFF)
            return 0x7FFFFFFF;
        else if (val < 0x80000000)
            return 0x80000000;
        else
            return (q31_t)val;
    }

    /**
     * @brief Saturates a double to the range of float (32-bit IEEE 754).
     *
     * This function takes a double-precision floating-point value and
     * clamps it to the maximum and minimum representable float values
     * using their IEEE 754 bit patterns:
     * - Maximum: 0x7F7FFFFF
     * - Minimum: 0xFF7FFFFF
     *
     * @param val Input value of type double to be saturated.
     * @return Saturated value as float.
     *
     */
    static inline float __IRRSATF32(double val)
    {
        union
        {
            uint32_t u;
            float f;
        } max = {.u = 0x7F7FFFFF};
        union
        {
            uint32_t u;
            float f;
        } min = {.u = 0xFF7FFFFF};

        if (val > max.f)
            return max.f;
        else if (val <= min.f)
            return min.f;
        else
            return (float)val;
    }
#ifdef __cplusplus
}
#endif // extern "C"
#endif
// End of File
