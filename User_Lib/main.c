

#include "main.h"

// gcc lib/lib.c main.c -o Test & Test
CHANNELS_ASSOCIATION_t t_ChnAss = {
    .bit.VA = 0,
    .bit.VB = 1,
    .bit.VC = 2,
    .bit.V0 = 3,
    .bit.IA = 4,
    .bit.IB = 5,
    .bit.IC = 6,
    .bit.I0 = 7,
};
SINGLE_CHN_PARTIAL_CALC_t t_SinglChnPartialCalc[NUM_SAMPLING_TOTAL][CHANNELS] = {0};
// SINGLE_CHN_PARTIAL_CALC_t t_SinglChnHoldVal[CHANNELS] = {0};
PAIR_CHN_PARTIAL_CALC_t t_PairChnPartialCalc[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES] = {0};
// PAIR_CHN_PARTIAL_CALC_t t_PairChnHoldVal[NUM_LINE_PHASES] = {0};
MEASUREMENTS_t t_Measurements = {0};
q15_t AdcAqcRaw[BUFFER_AQC][CHANNELS] = {0};
uint16_t gu16_IdxBufferAqc = 0;
uint16_t gu16_IdxSin[CHANNELS] = {0};
int main()
{
    FILE *file = fopen("output.csv", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    uint32_t n = 1;
    vfn_InitSin_LUT_q15();

    // n will increment at a rate of 1us
    while (n < 140000)
    {
        if (n % 125 == 0)
        {
            main_125us();
        }

        if (n % 250 == 0)
        {
            main_250us();
        }

        if (n % 1000 == 0)
        {

            fprintf(file, "%d,%d,%d,%d\n", n, t_Measurements.RMS.VA, t_Measurements.RMS.VB, t_Measurements.RMS.VC);
            fflush(file);

            main_1ms();
        }

        // if (++n > 1000)
        //     n = 1;
        n++;
    }
    fclose(file);
    return 0;
}

int main_125us()
{
}

int main_250us()
{
    q15_t voltages = 0;
    q15_t currents = 0;

    AdcAqcRaw[gu16_IdxBufferAqc][0] = xfn_GetCurrentSinVal(4000, 50, 0, &gu16_IdxSin[0]);   // Phase A Voltage 0°
    AdcAqcRaw[gu16_IdxBufferAqc][1] = xfn_GetCurrentSinVal(4000, 50, 120, &gu16_IdxSin[1]); // Phase B Voltage 120°
    AdcAqcRaw[gu16_IdxBufferAqc][2] = xfn_GetCurrentSinVal(4000, 50, 240, &gu16_IdxSin[2]); // Phase C Voltage 240°
    AdcAqcRaw[gu16_IdxBufferAqc][3] = xfn_GetCurrentSinVal(4000, 50, 0, &gu16_IdxSin[3]);   // Phase 0 Voltage 0°
    AdcAqcRaw[gu16_IdxBufferAqc][4] = xfn_GetCurrentSinVal(4000, 50, 30, &gu16_IdxSin[4]);  // Phase A Current 0°
    AdcAqcRaw[gu16_IdxBufferAqc][5] = xfn_GetCurrentSinVal(4000, 50, 150, &gu16_IdxSin[5]); // Phase B Current 120°
    AdcAqcRaw[gu16_IdxBufferAqc][6] = xfn_GetCurrentSinVal(4000, 50, 270, &gu16_IdxSin[6]); // Phase C Current 240°
    AdcAqcRaw[gu16_IdxBufferAqc][7] = xfn_GetCurrentSinVal(4000, 50, 0, &gu16_IdxSin[7]);   // Phase 0 Current 0°
    printf("%d\n", AdcAqcRaw[gu16_IdxBufferAqc][0]);
    // Circular Buffer handling
    if (++gu16_IdxBufferAqc >= 4)
        gu16_IdxBufferAqc = 0;
}

int main_1ms()
{
    static uint16_t su16_TskCounter = 0;
    uint16_t PosBuff = 0;
    uint16_t IdxBuff = 0;

    // Task Critical
    // IdxBuff = gu16_IdxBufferAqc - 4U;
    // PosBuff = gu16_IdxBufferAqc;
    // End Task Critical
    // Reset values in current position
    vfn_ResetSingleSample(su16_TskCounter);

    while (IdxBuff < 4)
    {
        // Same Partial Calculations for all channels
        for (uint16_t IdxChn = 0; IdxChn < CHANNELS; IdxChn++)
        {
            t_SinglChnPartialCalc[su16_TskCounter][IdxChn].q31_SUM_SQR += ((q31_t)AdcAqcRaw[IdxBuff][IdxChn] * AdcAqcRaw[IdxBuff][IdxChn]); // Q15*Q15 = Q30
        }

        // Partial Calculations for pairs of channels
        t_PairChnPartialCalc[su16_TskCounter][0].q31_DIFF = ((q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VA] - (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VB]);
        t_PairChnPartialCalc[su16_TskCounter][1].q31_DIFF = ((q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VB] - (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VC]);
        t_PairChnPartialCalc[su16_TskCounter][2].q31_DIFF = ((q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VA] - (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VC]);

        t_PairChnPartialCalc[su16_TskCounter][0].q63_SUM_SQR += (q63_t)t_PairChnPartialCalc[su16_TskCounter][0].q31_DIFF * t_PairChnPartialCalc[su16_TskCounter][0].q31_DIFF; // Q31*Q31 = Q62
        t_PairChnPartialCalc[su16_TskCounter][1].q63_SUM_SQR += (q63_t)t_PairChnPartialCalc[su16_TskCounter][1].q31_DIFF * t_PairChnPartialCalc[su16_TskCounter][1].q31_DIFF; // Q31*Q31 = Q62
        t_PairChnPartialCalc[su16_TskCounter][2].q63_SUM_SQR += (q63_t)t_PairChnPartialCalc[su16_TskCounter][2].q31_DIFF * t_PairChnPartialCalc[su16_TskCounter][2].q31_DIFF; // Q31*Q31 = Q62

        t_PairChnPartialCalc[su16_TskCounter][0].q31_SUM_PROD += (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VA] * AdcAqcRaw[IdxBuff][t_ChnAss.bit.IA]; // Q15*Q15 = Q30
        t_PairChnPartialCalc[su16_TskCounter][1].q31_SUM_PROD += (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VB] * AdcAqcRaw[IdxBuff][t_ChnAss.bit.IB]; // Q15*Q15 = Q30
        t_PairChnPartialCalc[su16_TskCounter][2].q31_SUM_PROD += (q31_t)AdcAqcRaw[IdxBuff][t_ChnAss.bit.VC] * AdcAqcRaw[IdxBuff][t_ChnAss.bit.IC]; // Q15*Q15 = Q30

        IdxBuff++;
    }

    // Measurements Calculations
    // RMS
    t_Measurements.RMS.VA = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.VA);
    t_Measurements.RMS.VB = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.VB);
    t_Measurements.RMS.VC = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.VC);
    t_Measurements.RMS.V0 = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.V0);
    t_Measurements.RMS.IA = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.IA);
    t_Measurements.RMS.IB = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.IB);
    t_Measurements.RMS.IC = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.IC);
    t_Measurements.RMS.I0 = xfn_RMSQ15(t_SinglChnPartialCalc, t_ChnAss.bit.I0);

    // Task Counter
    if (++su16_TskCounter >= NUM_SAMPLING_TOTAL)
        su16_TskCounter = 0;
}

q15_t xfn_RMSQ15(SINGLE_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][CHANNELS], uint16_t u16_ChnIdx)
{
    q31_t q31_Temp = 0;
    q15_t q15_output = 0;

    for (uint16_t u16_SmplIdx = 0; u16_SmplIdx < NUM_SAMPLING_TOTAL; u16_SmplIdx++)
    {
        q31_Temp += t_PartialCalcVector[u16_SmplIdx][u16_ChnIdx].q31_SUM_SQR / 80;
    }

    q15_output = q15_sqrt(q31_Temp);

    return q15_output;
}

void vfn_ResetSingleSample(uint16_t u16_SmplIdx)
{
    for (uint16_t u16_ChnIdx = 0; u16_ChnIdx < CHANNELS; u16_ChnIdx++)
    {
        t_SinglChnPartialCalc[u16_SmplIdx][u16_ChnIdx].q31_SUM_SQR = 0;
    }
    for (uint16_t u16_PairChnIdx = 0; u16_PairChnIdx < CHANNELS; u16_PairChnIdx++)
    {
        t_PairChnPartialCalc[u16_SmplIdx][u16_PairChnIdx].q31_DIFF = 0;
        t_PairChnPartialCalc[u16_SmplIdx][u16_PairChnIdx].q31_SUM_PROD = 0;
        t_PairChnPartialCalc[u16_SmplIdx][u16_PairChnIdx].q63_SUM_SQR = 0;
    }
}
/* End of File */
