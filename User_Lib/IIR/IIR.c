/**
 * @file IIR.c
 * @brief Implementation of the IIR Filter module routines.
 *
 * This file contains the internal logic and algorithms behind
 * the public API declared in IIR.h.
 *
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

//
// Include Files
//
#include "IIR.h"

//
// Functions
//

//
// xfn_IrrFilter_f32(): Applies an IIR filter to the current input sample in format float.
//
float xfn_IrrFilter_f32(IIR_FILTER_f32_t *f, float new_input)
{
    uint8_t index1;
    double output;
    output = 0.0f;
    // Error order
    if (f->order < 1 || f->order > MAX_IIR_ORD_FILTER)
        return 0;
    // History data
    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }
    // Get new value
    f->x[0] = new_input;
    // Apply SUM(x[n]*b[n])
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]);
    }
    // Apply (-1)*SUM(y[n-1]*b[n])
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }
    // Handling Saturation Q15 and Update present value y[n]
    f->y[0] = __IRRSATF32(output);
    // return
    return f->y[0];
}

//
// xfn_IrrFilter_int16(): Applies an IIR filter to the current input sample in format int16_t.
//
int16_t xfn_IrrFilter_int16(IIR_FILTER_int16_t *f, int16_t new_input, int16_t coeff_scale)
{
    uint8_t index1;
    int32_t output;
    output = 0;
    // Error order
    if (f->order < 1 || f->order > MAX_IIR_ORD_FILTER)
        return 0;
    // History data
    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }
    // Get new value
    f->x[0] = new_input;
    // Apply SUM(x[n]*b[n])
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]);
    }
    // Apply (-1)*SUM(y[n-1]*b[n])
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }
    // Normalize
    output /= (int32_t)coeff_scale;
    // Handling Saturation int16_t and Update present value y[n]
    f->y[0] = __IRRSATINT16(output);
    // return value int32_t
    return f->y[0];
}

//
// xfn_IrrFilter_int32(): Applies an IIR filter to the current input sample in format int32_t.
//
int32_t xfn_IrrFilter_int32(IIR_FILTER_int32_t *f, int32_t new_input, int32_t coeff_scale)
{
    uint8_t index1;
    int64_t output;
    output = 0;
    // Error order
    if (f->order < 1 || f->order > MAX_IIR_ORD_FILTER)
        return 0;
    // History data
    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }
    // Get new value
    f->x[0] = new_input;
    // Apply SUM(x[n]*b[n])
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]);
    }
    // Apply (-1)*SUM(y[n-1]*b[n])
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }
    // Normalize
    output /= (int64_t)coeff_scale;
    // Handling Saturation int32_t and Update present value y[n]
    f->y[0] = __IRRSATINT32(output);
    // return value int16_t
    return f->y[0];
}

//
// xfn_IrrFilter_q15(): Applies an IIR filter to the current input sample in format Q15.
//
q15_t xfn_IrrFilter_q15(IIR_FILTER_q15_t *f, q15_t new_input)
{
    uint8_t index1;
    q31_t output;
    output = 0;
    // Error order
    if (f->order < 1 || f->order > MAX_IIR_ORD_FILTER)
        return 0;
    // History data
    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }
    // Get new value
    f->x[0] = new_input;
    // Apply SUM(x[n]*b[n])
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]); // Q15 * Q15 = Q30
    }
    // Apply (-1)*SUM(y[n-1]*b[n])
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }
    // Q31 to Q15
    output >>= 15U;
    // Handling Saturation Q15 and Update present value y[n]
    f->y[0] = __IRRSATQ15(output);
    // return value Q15
    return f->y[0];
}

//
// xfn_IrrFilter_q31(): Applies an IIR filter to the current input sample in format Q31.
//
q31_t xfn_IrrFilter_q31(IIR_FILTER_q31_t *f, q31_t new_input)
{
    uint8_t index1;
    q63_t output;
    output = 0;
    // Error order
    if (f->order < 1 || f->order > MAX_IIR_ORD_FILTER)
        return 0;
    // History data
    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }
    // Get new value
    f->x[0] = new_input;
    // Apply SUM(x[n]*b[n])
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]); // Q31 * Q31 = Q62
    }
    // Apply (-1)*SUM(y[n-1]*b[n])
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }
    // Q62 to Q31
    output >>= 31U;
    // Handling Saturation Q31 and Update present value y[n]
    f->y[0] = __IRRSATQ31(output);
    // return value Q15
    return f->y[0];
}

/**
 * @brief Entry point for standalone IIR filter test and data generation.
 *
 * This function generates a sine wave combined with a secondary noise component,
 * applies an IIR filter to it, writes the raw and filtered data to a CSV file,
 * and then launches a Python script to plot the results.
 *
 * The CSV file is saved as `output.csv` with the following columns:
 * - `Time`: time in seconds
 * - `Sine wave with noise`: raw signal with added noise
 * - `Sine wave filtered`: result of IIR filter processing
 *
 * After generating the file, the function verifies if Python is installed
 * (via `python --version`) and runs the plotting script `plot.py` if available.
 *
 * @note Requires the IIR filter to be initialized and accessible via `iir_filter_500hz`.
 * @note Requires `plot.py` to exist in the same directory and have the necessary dependencies installed (e.g., `matplotlib`, `pandas`).
 *
 * @return `0` if successful, `-1` if file I/O fails, or `1` if Python is not found.
 */
//
// Optional test main(): IIR_TEST_MAIN should be previously defined
// To compile -> gcc IIR.c -DIIR_TEST_MAIN -o iir_test & iir_test
//
#ifdef IIR_TEST_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define AMPLITUDE 1.0        // Peak amplitude
#define FREQUENCY 2000.0     // Frequency in Hz
#define SAMPLING_RATE 200000 // Sampling rate in Hz
#define NOISE_FREQ 50000     // Noise Frequency
#define DURATION 1 / 100     // Duration in seconds
#define PI 3.1416            // PI value
// Definition of filter
IIR_FILTER_f32_t t_IrrFilter500Hz = {
    .freq_sampling = 20e3,
    .freq_cutoff = 500,
    .order = 2,
    .a = {1.0f, -1.1429805f, 0.4128016f},
    .b = {0.06745527f, 0.13491054f, 0.06745527f},
    .x = {0},
    .y = {0},
};
int16_t main(void)
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }
    fprintf(file, "Time,Sine wave with noise,Sine wave filtered\n");
    int16_t total_samples = (int16_t)(SAMPLING_RATE * DURATION);
    float t, sample, sample_filter;
    for (int16_t i = 0; i < total_samples; i++)
    {
        t = (float)i / SAMPLING_RATE;
        sample = AMPLITUDE * sin(2.0 * PI * FREQUENCY * t) + sin(2.0 * PI * NOISE_FREQ * (t + (PI / 9)));
        sample_filter = xfn_IrrFilter_f32(&t_IrrFilter500Hz, sample);
        fprintf(file, "%.6f,%.4f,%.4f\n", t, sample, sample_filter);
        fflush(file);
    }
    fclose(file);
    // Be sure to have installed Python dependencies
    int16_t ret = system("python --version");

    if (ret != 0)
    {
        printf("Python not found or not in PATH.\n");
        return 1;
    }
    else
    {
        system("python plot.py");
    }
    return 0;
}
#endif
// End of File
