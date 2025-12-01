

#ifndef _MAIN_INCLUDE_HEADER_
#define _MAIN_INCLUDE_HEADER_
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "lib/lib.h"

#define CHANNELS (8U)
#define BUFFER_AQC (8U)
#define NUM_LINE_PHASES (3U)
#define NUM_AQC_PER_CYCLE (4U)
#define NUM_SAMPLING_TOTAL (20U)

q15_t AdcAqcRaw[BUFFER_AQC][CHANNELS];
uint16_t u16_IdxBufferAqc;
uint16_t gu16_IdxSin[CHANNELS];

typedef union
{
    uint32_t all;
    struct
    {
        uint32_t VA : 3;
        uint32_t VB : 3;
        uint32_t VC : 3;
        uint32_t V0 : 3;
        uint32_t IA : 3;
        uint32_t IB : 3;
        uint32_t IC : 3;
        uint32_t I0 : 3;
        uint32_t rsvd : 7;
    } bit;

} CHANNELS_ASSOCIATION_t;
CHANNELS_ASSOCIATION_t t_ChnAss;

typedef struct
{
    q31_t q31_SUM_SQR;
} SINGLE_CHN_PARTIAL_CALC_t;

typedef struct
{
    q31_t q31_DIFF;
    q63_t q63_SUM_SQR;
    q31_t q31_SUM_PROD;
} PAIR_CHN_PARTIAL_CALC_t;

typedef struct
{
    struct
    {
        q15_t VA;
        q15_t VB;
        q15_t VC;
        q15_t V0;
        q15_t IA;
        q15_t IB;
        q15_t IC;
        q15_t I0;
    } RMS;

    struct
    {
        q15_t PH_A;
        q15_t PH_B;
        q15_t PH_C;
        q31_t TOTAL;
    } P_ACTIVE;

    struct
    {
        q15_t PH_A;
        q15_t PH_B;
        q15_t PH_C;
        q31_t TOTAL;
    } Q_REACTIVE;

    struct
    {
        q15_t PH_A;
        q15_t PH_B;
        q15_t PH_C;
        q31_t TOTAL;
    } S_APPARENT;

    struct
    {
        q15_t PH_A;
        q15_t PH_B;
        q15_t PH_C;
        q31_t TOTAL;
    } POWER_FACTOR;

    struct
    {
        q31_t PH_A_PH_B;
        q31_t PH_B_PH_C;
        q31_t PH_A_PH_C;
    } VLINE_LINE;

    struct
    {
        uint32_t PH_A;
        uint32_t PH_B;
        uint32_t PH_C;
    } PHASE_DIFF_VI;

    struct
    {
        uint32_t PH_A;
        uint32_t PH_B;
        uint32_t PH_C;
    } FREQ;

} MEASUREMENTS_t;

SINGLE_CHN_PARTIAL_CALC_t t_SinglChnPartialCalc[NUM_SAMPLING_TOTAL][CHANNELS];
// SINGLE_CHN_PARTIAL_CALC_t t_SinglChnHoldVal[CHANNELS];
PAIR_CHN_PARTIAL_CALC_t t_PairChnPartialCalc[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES];
// PAIR_CHN_PARTIAL_CALC_t t_PairChnHoldVal[NUM_LINE_PHASES];
MEASUREMENTS_t t_Measurements;

int main_125us();

int main_250us();

int main_1ms();

q15_t xfn_RMSQ15(SINGLE_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][CHANNELS], uint16_t u16_ChnIdx);

void vfn_ResetSingleSample(uint16_t u16_SmplIdx);

static inline q15_t q15_sqrt(q31_t x)
{
    if (x <= 0)
        return 0;

    // Convertir Q31 → flotante [0..1]
    float f = (float)x / 2147483647.0f;

    // Raíz cuadrada
    float f_sqrt = sqrtf(f);

    // Convertir flotante [0..1] → Q15
    return (q15_t)(f_sqrt * 32767.0f);
}

#endif