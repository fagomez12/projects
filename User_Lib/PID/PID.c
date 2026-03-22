/**
 * @file PID.c
 * @brief Implementation of the PID module routines.
 *
 * This file contains the internal logic and algorithms behind
 * the public API declared in PID.h.
 *
 * @author Felipe A. Gómez Olaya
 * @date 2025
 * @addtogroup pid_module
 * @{
 */

//
// Include Files
//
#include "typedef_lib.h"
#include "PID.h"

//
// Functions
//

//
// proportional_pid(PID_CONTROL *): Applies the proportional gain to the input
//
float32 proportional_pid(PID_CONTROL *f)
{
    return (f->x[BUFFER_SIZE - 1]) * (f->kp);
}

//
// proportional_pid(PID_CONTROL *): Applies the integrative gain to the input
//
float32 integrative_pid(PID_CONTROL *f)
{
    float32 a;
    float32 windup_error;
    float32 output;

    a = (f->ki) * (f->execution_rate) / 2;
    output = (a * (f->x[BUFFER_SIZE - 1])) + (a * (f->x[BUFFER_SIZE - 2])) + (f->y[BUFFER_SIZE - 2]);
    windup_error = MAX(f->lower_saturation, MIN(output, f->upper_saturation)) - output;
    output += ((f->kwp) * windup_error);

    return (output);
}

//
// derivative_pid(PID_CONTROL *): Applies the derivative gain to the input
//
float32 derivative_pid(PID_CONTROL *f)
{
    float32 a;
    float32 b;
    float32 output;

    a = ((f->kd) * (f->freq_cutoff) * (2 / (f->execution_rate))) / ((2 / (f->execution_rate)) + (f->freq_cutoff));
    b = ((f->freq_cutoff) - (2 / (f->execution_rate))) / ((2 / (f->execution_rate)) + (f->freq_cutoff));
    output = (a * (f->x[BUFFER_SIZE - 1])) - (a * (f->x[BUFFER_SIZE - 2])) - (b * (f->y[BUFFER_SIZE - 2]));

    return (output);
}

//
// pid_controller(PID_CONTROL *, char[3])
//
float32 pid_controller(PID_CONTROL *f, float32 new_input)
{
    f->y[BUFFER_SIZE - 2] = f->y[BUFFER_SIZE - 1];
    f->x[BUFFER_SIZE - 2] = f->x[BUFFER_SIZE - 1];
    f->x[BUFFER_SIZE - 1] = new_input;
    switch (f->type_controller)
    {
    case 0: // Proportional
        f->y[BUFFER_SIZE - 1] = proportional_pid(f);
        break;

    case 1: // Proportional + Integrative
        f->y[BUFFER_SIZE - 1] = proportional_pid(f) + integrative_pid(f);
        break;

    case 2: // Proportional + Derivative
        f->y[BUFFER_SIZE - 1] = proportional_pid(f) + derivative_pid(f);
        break;

    case 3: // Proportional + Integral + Derivative
        f->y[BUFFER_SIZE - 1] = proportional_pid(f) + integrative_pid(f) + derivative_pid(f);
        break;

    default:
        f->y[BUFFER_SIZE - 1] = 0;
        break;
    }

    return MAX(f->lower_saturation, MIN(f->y[BUFFER_SIZE - 1], f->upper_saturation));
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
// To compile -> gcc PID.c -DPID_TEST_MAIN -o pid_test & pid_test
//
#ifdef PID_TEST_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define EXECUTION_RATE 0.001f // Execution Rate
#define TOTAL_STEPS 100000    // Max Time duration
#define TAU 0.5f
#define GAIN_PLANT 1.0f
#define GAIN_A (TAU / (EXECUTION_RATE + TAU))
#define GAIN_B (EXECUTION_RATE * GAIN_PLANT / (TAU + EXECUTION_RATE))

float32 step[TOTAL_STEPS] = {0};
float32 error = 0;
float32 time = 0;
struct PLANT
{
    float32 x[BUFFER_SIZE];
    float32 y[BUFFER_SIZE];
};
struct PLANT plant;

// Definition of PID
PID_CONTROL pid_example = {
    .type_controller = 3,
    .execution_rate = 0.001,
    .freq_cutoff = 10000,
    .kp = 10,
    .ki = 2,
    .kd = 1,
    .kwp = 0,
    .lower_saturation = -2.0,
    .upper_saturation = 2.0,
    .x = {0},
    .y = {0},
};

void step_init(void)
{
    Uint32 index1;
    for (index1 = (TOTAL_STEPS / 2); index1 < TOTAL_STEPS; index1++)
    {
        step[index1] = 1;
    }
}
int main(void)
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }
    fprintf(file, "Time,Y,Step\n");

    step_init();

    Uint32 index1;
    for (index1 = 0; index1 < TOTAL_STEPS; index1++)
    {
        time += EXECUTION_RATE;
        error = step[index1] - plant.y[BUFFER_SIZE - 1];
        plant.y[BUFFER_SIZE - 2] = plant.y[BUFFER_SIZE - 1];
        plant.x[BUFFER_SIZE - 2] = plant.x[BUFFER_SIZE - 1];
        plant.x[BUFFER_SIZE - 1] = pid_controller(&pid_example, error);
        plant.y[BUFFER_SIZE - 1] = (GAIN_A * plant.y[BUFFER_SIZE - 2]) + (GAIN_B * plant.x[BUFFER_SIZE - 1]);

        fprintf(file, "%.4f,%.4f,%.4f\n", time, plant.y[BUFFER_SIZE - 1], step[index1]);
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
