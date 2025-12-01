

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
    .bit.VA_VB = 0,
    .bit.VB_VC = 1,
    .bit.VA_VC = 2,
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
    fprintf(file,
            "n,VA_rms,VB_rms,VC_rms,Vab_rms,Vbc_rms,Vac_rms,"
            "Pact_A,Pact_B,Pact_C,Pact_Total,"
            "Sapp_A,Sapp_B,Sapp_C,Sapp_Total,"
            "Qreact_A,Qreact_B,Qreact_C,Qreact_Total,"
            "PF_A,PF_B,PF_C,PF_Total\n");

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

            fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                    n,
                    t_Measurements.RMS.VA,
                    t_Measurements.RMS.VB,
                    t_Measurements.RMS.VC,
                    t_Measurements.VLINE_LINE_RMS.PH_A_PH_B,
                    t_Measurements.VLINE_LINE_RMS.PH_B_PH_C,
                    t_Measurements.VLINE_LINE_RMS.PH_A_PH_C,
                    t_Measurements.P_ACTIVE.PH_A,
                    t_Measurements.P_ACTIVE.PH_B,
                    t_Measurements.P_ACTIVE.PH_C,
                    t_Measurements.P_ACTIVE.TOTAL,
                    t_Measurements.S_APPARENT.PH_A,
                    t_Measurements.S_APPARENT.PH_B,
                    t_Measurements.S_APPARENT.PH_C,
                    t_Measurements.S_APPARENT.TOTAL,
                    t_Measurements.Q_REACTIVE.PH_A,
                    t_Measurements.Q_REACTIVE.PH_B,
                    t_Measurements.Q_REACTIVE.PH_C,
                    t_Measurements.Q_REACTIVE.TOTAL,
                    t_Measurements.POWER_FACTOR.PH_A,
                    t_Measurements.POWER_FACTOR.PH_B,
                    t_Measurements.POWER_FACTOR.PH_C,
                    t_Measurements.POWER_FACTOR.TOTAL);
            fflush(file);

            main_1ms();
        }

        // if (++n > 1000)
        //     n = 1;
        ++n;
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
    gu16_IdxBufferAqc++;
}

int main_1ms()
{
    static uint16_t su16_TskCounter = 0;
    uint16_t PosBuff = 0;
    uint16_t IdxBuff = 0;

    // Task Critical
    IdxBuff = gu16_IdxBufferAqc - 4U;
    PosBuff = gu16_IdxBufferAqc;
    // End Task Critical
    // Reset values in current position
    vfn_ResetSingleSample(su16_TskCounter);

    while (IdxBuff < PosBuff)
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

    vfn_SingleMeasRMS(t_SinglChnPartialCalc, t_ChnAss, &t_Measurements);
    vfn_PairMeasRMS(t_PairChnPartialCalc, t_ChnAss, &t_Measurements);
    vfn_PowerActiveCalc(t_PairChnPartialCalc, &t_Measurements);
    vfn_PowerApparentCalc(&t_Measurements);
    vfn_PowerReactiveCalc(&t_Measurements);
    vfn_PowerFactorCalc(&t_Measurements);

    // Task Counter
    if (++su16_TskCounter >= NUM_SAMPLING_TOTAL)
        su16_TskCounter = 0;

    //  Circular Buffer handling
    if (gu16_IdxBufferAqc >= BUFFER_AQC)
        gu16_IdxBufferAqc = 0;
}

/**
 * @brief Computes RMS values for all configured channels.
 *
 * This function computes the RMS (Root Mean Square) value of each voltage and
 * current channel using the accumulated sum of squares stored in
 * SINGLE_CHN_PARTIAL_CALC_t. The function iterates through all samples,
 * averages the accumulated squared values, and then converts the result from
 * Q30 to Q15 using sqrt_q30_to_q15().
 *
 * The computed RMS values are written into the MEASUREMENTS_t structure in the
 * corresponding RMS fields (VA, VB, VC, IA, IB, etc.), based on the channel
 * association lookup table provided in @p t_Chn.
 *
 * @param[in] t_PartialCalcVector
 *      2D array containing partial RMS accumulators for each channel and each
 *      sample. Each entry contains q31_SUM_SQR which represents the squared
 *      sample accumulated in Q30 format.
 *
 * @param[in] t_Chn
 *      CHANNELS_ASSOCIATION_t structure containing the mapping between logical
 *      channels (VA, VB, IA, etc.) and their physical indices in the input
 *      vector.
 *
 * @param[out] t_Meas
 *      Pointer to a MEASUREMENTS_t structure where the final RMS results
 *      (in Q15 format) will be stored.
 *
 * @note The function assumes:
 *       - NUM_SAMPLING_TOTAL samples per channel.
 *       - Division by 80 is equivalent to averaging the squared samples over
 *         one full cycle.
 *       - sqrt_q30_to_q15() performs the square-root operation while converting
 *         Q30 → Q15.
 *
 * @warning The caller must ensure that:
 *          - @p t_Meas is a valid pointer.
 *          - The channel association fields (VA, VB, IA, etc.) in @p t_Chn
 *            contain valid channel indices.
 *
 * @return void
 */
void vfn_SingleMeasRMS(SINGLE_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][CHANNELS], CHANNELS_ASSOCIATION_t t_Chn, MEASUREMENTS_t *t_Meas)
{
    q31_t q31_Temp[CHANNELS] = {0};

    for (uint16_t u16_ChnIdx = 0; u16_ChnIdx < CHANNELS; u16_ChnIdx++)
    {
        for (uint16_t u16_SmplIdx = 0; u16_SmplIdx < NUM_SAMPLING_TOTAL; u16_SmplIdx++)
        {
            q31_Temp[u16_ChnIdx] += t_PartialCalcVector[u16_SmplIdx][u16_ChnIdx].q31_SUM_SQR / 80;
        }
    }

    t_Meas->RMS.VA = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.VA]);
    t_Meas->RMS.VB = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.VB]);
    t_Meas->RMS.VC = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.VC]);
    t_Meas->RMS.V0 = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.V0]);
    t_Meas->RMS.IA = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.IA]);
    t_Meas->RMS.IB = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.IB]);
    t_Meas->RMS.IC = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.IC]);
    t_Meas->RMS.I0 = sqrt_q30_to_q15(q31_Temp[t_Chn.bit.I0]);
}

/**
 * @brief Computes line-to-line RMS voltages from accumulated pair-channel partial results.
 *
 * This function processes the accumulated squared differences of channel pairs
 * (Va-Vb, Vb-Vc, Va-Vc) stored in @ref PAIR_CHN_PARTIAL_CALC_t and computes
 * their RMS values. The function iterates over all acquired samples,
 * accumulates the squared values for each line-to-line pair, performs scaling
 * (division by 80), and finally computes the square root using a fixed-point
 * Q62-to-Q31 sqrt function.
 *
 * The computed RMS values are written into the @ref MEASUREMENTS_t structure
 * under `VLINE_LINE_RMS`.
 *
 * @param[in]  t_PartialCalcVector
 *             2D array containing partial line-to-line squared values for all
 *             samples and line-pairs. Each element includes:
 *             - q63_SUM_SQR : Accumulated (Va - Vb)^2 in Q63 format.
 *
 * @param[in]  t_Chn
 *             Channel association structure. Defines which channel indices
 *             correspond to Va-Vb, Vb-Vc, and Va-Vc through bitfields:
 *             - t_Chn.bit.VA_VB
 *             - t_Chn.bit.VB_VC
 *             - t_Chn.bit.VA_VC
 *
 * @param[out] t_Meas
 *             Pointer to the measurements structure where the resulting
 *             line-to-line RMS values are stored:
 *             - t_Meas->VLINE_LINE_RMS.PH_A_PH_B
 *             - t_Meas->VLINE_LINE_RMS.PH_B_PH_C
 *             - t_Meas->VLINE_LINE_RMS.PH_A_PH_C
 *
 * @note The division by 80 assumes a fixed-size window of 80 samples
 *       contributing to the RMS calculation.
 *
 * @note The function uses `sqrt_q62_to_q31()` to convert the accumulated
 *       Q62 energy into a Q31 RMS value.
 *
 * @warning Ensure that the accumulated squared values do not overflow Q63,
 *          especially if input channels are not properly scaled.
 */
void vfn_PairMeasRMS(PAIR_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES], CHANNELS_ASSOCIATION_t t_Chn, MEASUREMENTS_t *t_Meas)
{
    q63_t q63_Temp[NUM_LINE_PHASES] = {0};

    for (uint16_t u16_ChnIdx = 0; u16_ChnIdx < NUM_LINE_PHASES; u16_ChnIdx++)
    {
        for (uint16_t u16_SmplIdx = 0; u16_SmplIdx < NUM_SAMPLING_TOTAL; u16_SmplIdx++)
        {
            q63_Temp[u16_ChnIdx] += t_PartialCalcVector[u16_SmplIdx][u16_ChnIdx].q63_SUM_SQR / 80;
        }
    }

    t_Meas->VLINE_LINE_RMS.PH_A_PH_B = sqrt_q62_to_q31(q63_Temp[t_Chn.bit.VA_VB]);
    t_Meas->VLINE_LINE_RMS.PH_B_PH_C = sqrt_q62_to_q31(q63_Temp[t_Chn.bit.VB_VC]);
    t_Meas->VLINE_LINE_RMS.PH_A_PH_C = sqrt_q62_to_q31(q63_Temp[t_Chn.bit.VA_VC]);
}

/**
 * @brief Computes the active power (P) for each phase and the total active power.
 *
 * This function processes a 2D array containing partial per-sample products
 * V(n) * I(n) accumulated in fixed-point Q31 format. For each phase,
 * the function sums all partial results across the sampling window and
 * computes the average active power. The final value is converted to Q15.
 *
 * Each partial value q31_SUM_PROD represents:
 *   - instantaneous V(n) * I(n) in Q31
 *   - already scaled so that averaging requires only division by the
 *     number of samples
 *
 * The function performs:
 *   1. Accumulation of sum(V*I) across all samples for each phase.
 *   2. Division by the fixed number of samples (80).
 *   3. Conversion of the averaged Q31 result into Q15 by shifting 15 bits.
 *   4. Computation of total active power as the sum of the three phases.
 *
 * @param[in]  t_PartialCalcVector
 *              2D array of size [NUM_SAMPLING_TOTAL][NUM_LINE_PHASES]
 *              containing V*I instantaneous products in Q31 format.
 *
 * @param[out] t_Meas
 *              Pointer to the measurement structure where results are stored.
 *              - P_ACTIVE.PH_A  : Active power of phase A (Q15)
 *              - P_ACTIVE.PH_B  : Active power of phase B (Q15)
 *              - P_ACTIVE.PH_C  : Active power of phase C (Q15)
 *              - P_ACTIVE.TOTAL : Total active power (Q31)
 *
 * @note The division by 80 assumes a fixed-size sampling window and
 *       a uniform sampling rate. Adjust if the window size changes.
 *
 * @warning No saturation protection is applied during accumulation.
 *          Ensure signal levels remain within Q31 safe limits.
 */
void vfn_PowerActiveCalc(PAIR_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES], MEASUREMENTS_t *t_Meas)
{
    q31_t q31_Temp[NUM_LINE_PHASES] = {0};

    for (uint16_t u16_ChnIdx = 0; u16_ChnIdx < NUM_LINE_PHASES; u16_ChnIdx++)
    {
        for (uint16_t u16_SmplIdx = 0; u16_SmplIdx < NUM_SAMPLING_TOTAL; u16_SmplIdx++)
        {
            q31_Temp[u16_ChnIdx] += t_PartialCalcVector[u16_SmplIdx][u16_ChnIdx].q31_SUM_PROD / 80;
        }
    }

    t_Meas->P_ACTIVE.PH_A = (q15_t)(q31_Temp[0] >> 15);
    t_Meas->P_ACTIVE.PH_B = (q15_t)(q31_Temp[1] >> 15);
    t_Meas->P_ACTIVE.PH_C = (q15_t)(q31_Temp[2] >> 15);
    t_Meas->P_ACTIVE.TOTAL = (q31_t)t_Meas->P_ACTIVE.PH_A + t_Meas->P_ACTIVE.PH_B + t_Meas->P_ACTIVE.PH_C;
}

/**
 * @brief Computes the apparent power (S) per phase and total.
 *
 * This function calculates the apparent power for each phase using the
 * RMS voltage and RMS current values previously computed.
 * The computation follows:
 *
 *      S_phase = (V_rms * I_rms) >> 15   // Q15 × Q15 → Q30 → Q15
 *
 * The total apparent power is the sum of the three phase values.
 *
 * @param[in,out] t_Meas
 *      Pointer to the measurement structure where RMS and apparent
 *      power values are stored. The function reads the RMS voltages
 *      and currents from this structure and updates the S_APPARENT fields.
 *
 * @note
 * - RMS values must already be computed before calling this function.
 * - All arithmetic uses fixed-point Q15 format.
 * - Casting to q31_t is required to prevent overflow during multiplication.
 */
void vfn_PowerApparentCalc(MEASUREMENTS_t *t_Meas)
{
    t_Meas->S_APPARENT.PH_A = (q15_t)(((q31_t)t_Meas->RMS.VA * t_Meas->RMS.IA) >> 15);
    t_Meas->S_APPARENT.PH_B = (q15_t)(((q31_t)t_Meas->RMS.VB * t_Meas->RMS.IB) >> 15);
    t_Meas->S_APPARENT.PH_C = (q15_t)(((q31_t)t_Meas->RMS.VC * t_Meas->RMS.IC) >> 15);
    t_Meas->S_APPARENT.TOTAL = (q31_t)t_Meas->S_APPARENT.PH_A + t_Meas->S_APPARENT.PH_B + t_Meas->S_APPARENT.PH_C;
}

/**
 * @brief Computes the reactive power (Q) per phase and total.
 *
 * This function calculates the reactive power using the standard relation:
 *
 *      Q = sqrt(S² − P²)
 *
 * where:
 *  - S is the apparent power (already computed in Q15),
 *  - P is the active power (also already in Q15),
 *  - Q is returned in Q15 format.
 *
 * The function performs the calculations for phases A, B, and C independently,
 * and then computes the total reactive power as the sum of the three phases.
 *
 * Internally, the squares S² and P² are computed in Q30 (because Q15 * Q15 → Q30),
 * and the square root is applied using `sqrt_q30_to_q15()`, which converts back to Q15.
 *
 * @param[in,out] t_Meas
 *      Pointer to the measurements structure.
 *      The function reads:
 *          - t_Meas->S_APPARENT.PH_x  (Q15)
 *          - t_Meas->P_ACTIVE.PH_x    (Q15)
 *      And writes:
 *          - t_Meas->Q_REACTIVE.PH_x  (Q15)
 *          - t_Meas->Q_REACTIVE.TOTAL (Q31 sum of 3 Q15 values)
 *
 * @note The function assumes that S ≥ P; no protection is applied for negative
 *       values inside the square root. It must be ensured upstream that
 *       measurement noise does not violate S² ≥ P².
 *
 * @warning If numerical noise causes (S² - P²) < 0, the sqrt function may
 *          saturate or return zero depending on its implementation.
 *
 */
void vfn_PowerReactiveCalc(MEASUREMENTS_t *t_Meas)
{
    q31_t q31_Temp_S_sqr[NUM_LINE_PHASES] = {0};
    q31_t q31_Temp_P_sqr[NUM_LINE_PHASES] = {0};

    q31_Temp_S_sqr[0] = (q31_t)t_Meas->S_APPARENT.PH_A * t_Meas->S_APPARENT.PH_A;
    q31_Temp_S_sqr[1] = (q31_t)t_Meas->S_APPARENT.PH_B * t_Meas->S_APPARENT.PH_B;
    q31_Temp_S_sqr[2] = (q31_t)t_Meas->S_APPARENT.PH_C * t_Meas->S_APPARENT.PH_C;

    q31_Temp_P_sqr[0] = (q31_t)t_Meas->P_ACTIVE.PH_A * t_Meas->P_ACTIVE.PH_A;
    q31_Temp_P_sqr[1] = (q31_t)t_Meas->P_ACTIVE.PH_B * t_Meas->P_ACTIVE.PH_B;
    q31_Temp_P_sqr[2] = (q31_t)t_Meas->P_ACTIVE.PH_C * t_Meas->P_ACTIVE.PH_C;

    t_Meas->Q_REACTIVE.PH_A = sqrt_q30_to_q15(q31_Temp_S_sqr[0] - q31_Temp_P_sqr[0]);
    t_Meas->Q_REACTIVE.PH_B = sqrt_q30_to_q15(q31_Temp_S_sqr[1] - q31_Temp_P_sqr[1]);
    t_Meas->Q_REACTIVE.PH_C = sqrt_q30_to_q15(q31_Temp_S_sqr[2] - q31_Temp_P_sqr[2]);
    t_Meas->Q_REACTIVE.TOTAL = (q31_t)t_Meas->Q_REACTIVE.PH_A + t_Meas->Q_REACTIVE.PH_B + t_Meas->Q_REACTIVE.PH_C;
}

/**
 * @brief Calculates the power factor (PF) per phase and total.
 *
 * This function computes the power factor using the formula:
 *      PF = P / S
 *
 * Where:
 *   - P is the active power (Q15 format)
 *   - S is the apparent power (Q15 format)
 *
 * Because fixed-point Q15 values cannot directly represent division,
 * the numerator is scaled by shifting 15 bits before performing the division:
 *
 *      PF_Q15 = (P_active << 15) / S_apparent
 *
 * The result is stored in Q15 format for each phase and the sum of all
 * phases is stored into the TOTAL field in Q31 format.
 *
 * @param[in,out] t_Meas   Pointer to the measurement structure containing:
 *                          - Active power per phase (P_ACTIVE.PH_x)
 *                          - Apparent power per phase (S_APPARENT.PH_x)
 *                          The function writes the computed PF values into:
 *                          - POWER_FACTOR.PH_x
 *                          - POWER_FACTOR.TOTAL
 *
 * @note The function does not check for division by zero.
 *       Ensure that S_APPARENT.PH_x is non-zero before calling.
 *
 * @return None.
 */
void vfn_PowerFactorCalc(MEASUREMENTS_t *t_Meas)
{
    t_Meas->POWER_FACTOR.PH_A = (q15_t)(((q31_t)t_Meas->P_ACTIVE.PH_A << 15) / t_Meas->S_APPARENT.PH_A);
    t_Meas->POWER_FACTOR.PH_B = (q15_t)(((q31_t)t_Meas->P_ACTIVE.PH_B << 15) / t_Meas->S_APPARENT.PH_B);
    t_Meas->POWER_FACTOR.PH_C = (q15_t)(((q31_t)t_Meas->P_ACTIVE.PH_C << 15) / t_Meas->S_APPARENT.PH_C);
    t_Meas->POWER_FACTOR.TOTAL = (q31_t)t_Meas->POWER_FACTOR.PH_A + t_Meas->POWER_FACTOR.PH_B + t_Meas->POWER_FACTOR.PH_C;
}

/**
 * @brief Resets all partial calculation accumulators for a given sample index.
 *
 * This function clears the accumulated values used for RMS, power, and
 * line-to-line computations for both single-channel and pair-channel
 * measurement structures. It effectively resets one complete sampling slot
 * (u16_SmplIdx) in the global accumulation buffers.
 *
 * @param[in] u16_SmplIdx
 *      Index of the sampling position to reset. This corresponds to one
 *      time-slice in the NUM_SAMPLING_TOTAL circular buffer.
 *
 * @note
 * - Single-channel structure fields reset:
 *      - q31_SUM_SQR
 * - Pair-channel structure fields reset:
 *      - q31_DIFF
 *      - q31_SUM_PROD
 *      - q63_SUM_SQR
 *
 * @warning No bounds checking is performed; u16_SmplIdx must be < NUM_SAMPLING_TOTAL.
 */
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
