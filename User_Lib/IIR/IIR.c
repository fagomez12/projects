/**
 * @file IIR.c
 * @brief Implementation of the IIR Filter module routines.
 *
 * This file contains the internal logic and algorithms behind
 * the public API declared in IIR.h.
 *
 * @author Felipe A. Gómez Olaya
 * @date 2025
 * @addtogroup iir_filter_module
 * @{
 */

//
// Include Files
//
#include "typedef_lib.h"
#include "IIR.h"

//
// Functions
//

//
// iir_filter(IIR_FILTER *, float32): Applies an IIR filter to the current input sample.
//
float32 iir_filter(IIR_FILTER *f, float32 new_input)
{
    Uint8 index1;
    float32 output;
    output = 0.0f;

    for (index1 = f->order; index1 > 0; index1--)
    {
        f->x[index1] = f->x[index1 - 1];
        f->y[index1] = f->y[index1 - 1];
    }

    f->x[0] = new_input;
    for (index1 = 0; index1 <= f->order; index1++)
    {
        output += (f->b[index1] * f->x[index1]);
    }
    for (index1 = 1; index1 <= f->order; index1++)
    {
        output -= (f->a[index1] * f->y[index1]);
    }

    f->y[0] = output;
    return output;
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
IIR_FILTER iir_filter_500hz = {
    .freq_sampling = 20e3,
    .freq_cutoff = 500,
    .order = 2,
    .a = {1.0f, -1.1429805f, 0.4128016f},
    .b = {0.06745527f, 0.13491054f, 0.06745527f},
    .x = {0},
    .y = {0},
};
int main(void)
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }
    fprintf(file, "Time,Sine wave with noise,Sine wave filtered\n");
    int16 total_samples = (int16)(SAMPLING_RATE * DURATION);
    float32 t, sample, sample_filter;
    for (int16 i = 0; i < total_samples; i++)
    {
        t = (float32)i / SAMPLING_RATE;
        sample = AMPLITUDE * sin(2.0 * PI * FREQUENCY * t) + sin(2.0 * PI * NOISE_FREQ * (t + (PI / 9)));
        sample_filter = iir_filter(&iir_filter_500hz, sample);
        fprintf(file, "%.6f,%.4f,%.4f\n", t, sample, sample_filter);
        fflush(file);
    }
    fclose(file);
    // Be sure to have installed Python dependencies
    int ret = system("python --version");

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
/** @} */ // end of IIR_Filter_Module
