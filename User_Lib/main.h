

#ifndef _MAIN_INCLUDE_HEADER_
#define _MAIN_INCLUDE_HEADER_
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "lib/lib.h"

/**
 * @def CHANNELS
 * @brief Total number of ADC channels processed by the measurement engine.
 */
#define CHANNELS (8U)

/**
 * @def BUFFER_AQC
 * @brief Size of the circular ADC acquisition buffer.
 *
 * Each ADC conversion stores one sample per channel, resulting in
 * BUFFER_AQC × CHANNELS raw readings.
 */
#define BUFFER_AQC (8U)

/**
 * @def NUM_LINE_PHASES
 * @brief Number of line-to-line voltage phases (AB, BC, CA).
 */
#define NUM_LINE_PHASES (3U)

/**
 * @def NUM_AQC_PER_CYCLE
 * @brief Number of ADC acquisition blocks per electrical cycle.
 *
 * Used for timing, phase detection, and cycle-based averaging.
 */
#define NUM_AQC_PER_CYCLE (4U)

/**
 * @def NUM_SAMPLING_TOTAL
 * @brief Total number of processed samples used for RMS and power calculations.
 *
 * This defines the depth of the sliding/accumulating buffer that stores
 * intermediate partial computations such as sum of squares and V·I products.
 */
#define NUM_SAMPLING_TOTAL (20U)

/**
 * @var AdcAqcRaw
 * @brief Raw ADC acquisition data buffer.
 *
 * - First dimension: circular buffer position (BUFFER_AQC).
 * - Second dimension: channel index (CHANNELS).
 *
 * Each entry contains a q15_t ADC sample corresponding to a channel reading.
 */
q15_t AdcAqcRaw[BUFFER_AQC][CHANNELS];

/**
 * @var u16_IdxBufferAqc
 * @brief Current write index inside the circular acquisition buffer.
 *
 * Incremented on every ADC conversion and wrapped at BUFFER_AQC.
 */
uint16_t u16_IdxBufferAqc;

/**
 * @var gu16_IdxSin
 * @brief Phase index for each channel used in sine-based synchronization or filtering.
 *
 * Typically employed in PLL-like logic, phase tracking, or for sinusoidal-based
 * reference reconstruction per channel.
 */
uint16_t gu16_IdxSin[CHANNELS];

/**
 * @brief Channel association map used to link ADC inputs to logical signals.
 *
 * This union allows accessing either the raw 32-bit value or individual
 * bitfields that describe how physical channels correspond to voltages,
 * currents, and derived signals.
 */
typedef union
{
    /** Full 32-bit access */
    uint32_t all;

    /**
     * @brief Bitfield access for channel associations.
     */
    struct
    {
        uint32_t VA : 3; /**< Channel mapped to phase voltage A */
        uint32_t VB : 3; /**< Channel mapped to phase voltage B */
        uint32_t VC : 3; /**< Channel mapped to phase voltage C */
        uint32_t V0 : 3; /**< Channel mapped to zero-sequence voltage */

        uint32_t IA : 3; /**< Channel mapped to phase current A */
        uint32_t IB : 3; /**< Channel mapped to phase current B */
        uint32_t IC : 3; /**< Channel mapped to phase current C */
        uint32_t I0 : 3; /**< Channel mapped to zero-sequence current */

        uint32_t VA_VB : 2; /**< Channel for line-to-line voltage Va-Vb */
        uint32_t VB_VC : 2; /**< Channel for line-to-line voltage Vb-Vc */
        uint32_t VA_VC : 2; /**< Channel for line-to-line voltage Va-Vc */

        uint32_t rsvd : 1; /**< Reserved bit */
    } bit;

} CHANNELS_ASSOCIATION_t;

CHANNELS_ASSOCIATION_t t_ChnAss;

/**
 * @brief Accumulators for single-channel RMS calculations.
 */
typedef struct
{
    q31_t q31_SUM_SQR; /**< Sum of squares accumulator: sum(x²) */
} SINGLE_CHN_PARTIAL_CALC_t;

/**
 * @brief Accumulators for paired-channel calculations such as
 * line-to-line RMS and instantaneous power.
 */
typedef struct
{
    q31_t q31_DIFF;     /**< Instantaneous difference between two channels */
    q63_t q63_SUM_SQR;  /**< Sum of squares accumulator: sum((Va−Vb)²) */
    q31_t q31_SUM_PROD; /**< Sum of products: sum(V * I) → active power */
} PAIR_CHN_PARTIAL_CALC_t;

/**
 * @brief Complete structure holding all computed electrical measurements.
 */
typedef struct
{
    /**
     * @brief RMS voltage and current values.
     */
    struct
    {
        q15_t VA; /**< RMS voltage phase A */
        q15_t VB; /**< RMS voltage phase B */
        q15_t VC; /**< RMS voltage phase C */
        q15_t V0; /**< RMS zero-sequence voltage */
        q15_t IA; /**< RMS current phase A */
        q15_t IB; /**< RMS current phase B */
        q15_t IC; /**< RMS current phase C */
        q15_t I0; /**< RMS zero-sequence current */
    } RMS;

    /**
     * @brief Active power per phase and total.
     */
    struct
    {
        q15_t PH_A;  /**< Active power phase A */
        q15_t PH_B;  /**< Active power phase B */
        q15_t PH_C;  /**< Active power phase C */
        q31_t TOTAL; /**< Total active power */
    } P_ACTIVE;

    /**
     * @brief Reactive power per phase and total.
     */
    struct
    {
        q15_t PH_A;  /**< Reactive power phase A */
        q15_t PH_B;  /**< Reactive power phase B */
        q15_t PH_C;  /**< Reactive power phase C */
        q31_t TOTAL; /**< Total reactive power */
    } Q_REACTIVE;

    /**
     * @brief Apparent power per phase and total.
     */
    struct
    {
        q15_t PH_A;  /**< Apparent power phase A */
        q15_t PH_B;  /**< Apparent power phase B */
        q15_t PH_C;  /**< Apparent power phase C */
        q31_t TOTAL; /**< Total apparent power */
    } S_APPARENT;

    /**
     * @brief Power factor per phase and total.
     */
    struct
    {
        q15_t PH_A;  /**< Power factor phase A */
        q15_t PH_B;  /**< Power factor phase B */
        q15_t PH_C;  /**< Power factor phase C */
        q31_t TOTAL; /**< Total power factor */
    } POWER_FACTOR;

    /**
     * @brief Line-to-line RMS voltage values.
     */
    struct
    {
        q31_t PH_A_PH_B; /**< RMS of Va−Vb */
        q31_t PH_B_PH_C; /**< RMS of Vb−Vc */
        q31_t PH_A_PH_C; /**< RMS of Va−Vc */
    } VLINE_LINE_RMS;

    /**
     * @brief Phase difference between voltage and current.
     * Units typically in degrees or fixed-point scaled degrees.
     */
    struct
    {
        uint32_t PH_A; /**< Phase offset for A */
        uint32_t PH_B; /**< Phase offset for B */
        uint32_t PH_C; /**< Phase offset for C */
    } PHASE_DIFF_VI;

    /**
     * @brief Frequency measurement per phase.
     */
    struct
    {
        uint32_t PH_A; /**< Frequency of phase A */
        uint32_t PH_B; /**< Frequency of phase B */
        uint32_t PH_C; /**< Frequency of phase C */
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
void vfn_SingleMeasRMS(SINGLE_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][CHANNELS], CHANNELS_ASSOCIATION_t t_Chn, MEASUREMENTS_t *t_Meas);

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
void vfn_PairMeasRMS(PAIR_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES], CHANNELS_ASSOCIATION_t t_Chn, MEASUREMENTS_t *t_Meas);

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
void vfn_PowerActiveCalc(PAIR_CHN_PARTIAL_CALC_t t_PartialCalcVector[NUM_SAMPLING_TOTAL][NUM_LINE_PHASES], MEASUREMENTS_t *t_Meas);

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
void vfn_PowerApparentCalc(MEASUREMENTS_t *t_Meas);

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
void vfn_PowerReactiveCalc(MEASUREMENTS_t *t_Meas);

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
void vfn_PowerFactorCalc(MEASUREMENTS_t *t_Meas);

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
void vfn_ResetSingleSample(uint16_t u16_SmplIdx);

/**
 * @brief Computes the square root of a Q31 value and returns the result in Q15.
 *
 * This version converts the input to floating-point, computes the square root,
 * and returns the result converted back into Q15 format.
 *
 * @param x Input value in Q31 format (range: 0 to 1.0 in Q31).
 * @return q15_t Square root result in Q15 format. Returns 0 if input <= 0.
 */
static inline q15_t q15_sqrt(q31_t x)
{
    // Return zero for negative or zero inputs (sqrt undefined or trivial).
    if (x <= 0)
        return 0;

    // Convert Q31 integer into a float in the range [0.0, 1.0].
    float f = (float)x;

    // Compute the square root in floating-point.
    float f_sqrt = sqrtf(f);

    // Convert float result back into Q15 format.
    return (q15_t)(f_sqrt);
}

/**
 * @brief Computes sqrt(Q30) and returns result in Q15 using integer-only method.
 *
 * Implements the "binary restoring" integer square-root algorithm.
 * Input is in Q30 format (result of Q15 * Q15).
 * Output is in Q15 format (sqrt(Q30) = Q15).
 *
 * @param x_q30 Input value in Q30 format.
 * @return q15_t Square root in Q15 format. Returns 0 if input <= 0.
 */
static inline q15_t sqrt_q30_to_q15(q31_t x_q30)
{
    // Square-root of negative numbers is undefined → return zero.
    if (x_q30 <= 0)
        return 0;

    // Work with unsigned version of the Q30 input.
    uint32_t x = (uint32_t)x_q30;

    // Result accumulator (Q15 final).
    uint32_t res = 0;

    // Highest even bit suitable for Q30 square-root computation.
    uint32_t bit = 1UL << 30;

    // Reduce starting bit until it is <= input.
    while (bit > x)
        bit >>= 2;

    // Binary-restoring square-root algorithm.
    while (bit != 0)
    {
        // Check whether subtracting this term improves approximation.
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit; // Update result accumulator.
        }
        else
        {
            res >>= 1; // Shift result if no subtraction.
        }

        bit >>= 2; // Move to next lower even bit.
    }

    // Clamp to Q15 maximum (32767).
    if (res > 32767)
        res = 32767;

    return (q15_t)res;
}

/**
 * @brief Computes sqrt(Q62) and returns result in Q31 using integer-only method.
 *
 * For Q62 input (result of Q31 * Q31), the square root is a Q31 value.
 * Uses the binary-restoring integer square-root method.
 *
 * @param x_q62 Input value in Q62 format.
 * @return q31_t Square root in Q31 format. Returns 0 if input <= 0.
 */
static inline q31_t sqrt_q62_to_q31(q63_t x_q62)
{
    // Square-root undefined for negative or zero → return zero.
    if (x_q62 <= 0)
        return 0;

    // Convert input to unsigned 64-bit for manipulation.
    uint64_t x = (uint64_t)x_q62;

    // Result accumulator (Q31) initialized to zero.
    uint64_t res = 0;

    // Highest valid even bit for Q62 square-root.
    uint64_t bit = 1ULL << 62;

    // Adjust bit to the highest power-of-four <= x.
    while (bit > x)
        bit >>= 2;

    // Perform binary-restoring square-root iteration.
    while (bit != 0)
    {
        // Check if current approximation improves the result.
        if (x >= res + bit)
        {
            x -= res + bit;
            res = (res >> 1) + bit; // Update result.
        }
        else
        {
            res >>= 1; // Shift result.
        }

        bit >>= 2; // Move to the next even bit.
    }

    // Saturate to maximum Q31 positive value.
    if (res > 0x7FFFFFFF)
        res = 0x7FFFFFFF;

    return (q31_t)res;
}

#endif