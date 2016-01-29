#include "ch.h"
#include "hal.h"

///*
// * ADC conversion group.
// * Mode:        Linear buffer, 8 samples of 2 channels, SW triggered.
// * Channels:    IN7, IN8.
// */
//
//#define ADC_GRP1_NUM_CHANNELS   2
//#define ADC_GRP1_BUF_DEPTH      8
//
//adcsample_t samples1[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];
//
//static const ADCConversionGroup adcgrpcfg1 = {
//  FALSE,
//  ADC_GRP1_NUM_CHANNELS,
//  NULL,
//  NULL,
//  0,                        /* CFGR    */
//  ADC_TR(0, 4095),          /* TR1     */
//  0,                        /* CCR     */
//  {                         /* SMPR[2] */
//    0,
//    0
//  },
//  {                         /* SQR[4]  */
//    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN7) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN8),
//    0,
//    0,
//    0
//  }
//};



/*===========================================================================*/
/* ADC related.                                                              */
/*===========================================================================*/

#define ADC_NUM_CHANNELS 8
#define ADC_BUF_DEPTH 2

static adcsample_t adc_samples[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
static const ADCConversionGroup adc_group_config = {
  TRUE,
  ADC_NUM_CHANNELS,
  NULL,
  NULL,
  0,                                                            /* CFGR     */
  ADC_TR(0, 4095),                                              /* TR1      */
  ADC_CCR_DUAL(1),                                              /* CCR      */
  {                                                             /* SMPR[2]  */
	    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_601P5) | ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_601P5),
	    ADC_SMPR1_SMP_AN3(ADC_SMPR_SMP_601P5) | ADC_SMPR1_SMP_AN4(ADC_SMPR_SMP_601P5),
  },
  {                                                             /* SQR[4]   */
    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2) |
    ADC_SQR1_SQ3_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ4_N(ADC_CHANNEL_IN4),
    0,
    0
  },
  {                                                             /* SSMPR[2] */
	    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_601P5) | ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_601P5),
	    ADC_SMPR1_SMP_AN3(ADC_SMPR_SMP_601P5) | ADC_SMPR1_SMP_AN4(ADC_SMPR_SMP_601P5),
  },
  {                                                             /* SSQR[4]  */
	    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2) |
	    ADC_SQR1_SQ3_N(ADC_CHANNEL_IN3) | ADC_SQR1_SQ4_N(ADC_CHANNEL_IN4),
    0,
    0
  }
};

//#define ADC_NUM_CHANNELS 1
//#define ADC_BUF_DEPTH 2
//
//static adcsample_t adc_samples[ADC_NUM_CHANNELS * ADC_BUF_DEPTH];
//
//
//static const ADCConversionGroup adc_group_config = {
//  TRUE,
//  ADC_NUM_CHANNELS,
//  NULL,
//  NULL,
//  0,                                                            /* CFGR     */
//  ADC_TR(0, 4095),                                              /* TR1      */
//  0,                                              /* CCR      */
//  {                                                             /* SMPR[2]  */
//	    ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_601P5),
//  },
//  {                                                             /* SQR[4]   */
//    ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1),
//    0,
//    0
//  },
//};

/*
 * Application entry point.
 */
int main(void) {

	halInit();
	chSysInit();

	/*
	* Activates the ADC1 driver.
	*/
	adcStart(&ADCD1, NULL);

	/*
	* Starts an ADC continuous conversion.
	*/
	adcStartConversion(&ADCD1, &adc_group_config, adc_samples, ADC_BUF_DEPTH);

	for (;;) {
		palTogglePad(GPIOF, GPIOF_LED);
		chThdSleepMilliseconds(500);
	}

	return CH_SUCCESS;
}
