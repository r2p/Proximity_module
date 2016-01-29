#include "ch.h"
#include "hal.h"
#include "stm32_rcc.h"
#include "stm32_tim.h"

#include <r2p/Middleware.hpp>
#include <r2p/node/led.hpp>
#include <r2p/msg/proximity.hpp>

#ifndef R2P_MODULE_NAME
#define R2P_MODULE_NAME "Proximity"
#endif

static WORKING_AREA(wa_info, 1024);
static r2p::RTCANTransport rtcantra(RTCAND1);

RTCANConfig rtcan_config = { 1000000, 100, 60 };

r2p::Middleware r2p::Middleware::instance(R2P_MODULE_NAME, "BOOT_"R2P_MODULE_NAME);

uint32_t start[8] = {0};
uint32_t stop[8] = {0};
uint32_t diff[8] = {0};

void start_measure(int id) {
	  start[id - 1] = halGetCounterValue();
}

void stop_measure(int id) {
	  stop[id - 1] = halGetCounterValue();
	  diff[id - 1] = RTT2US(stop[id - 1] - start[id - 1]);
}


static void ext_cb(EXTDriver *extp, expchannel_t channel) {

	(void)extp;

	switch (channel) {
	case D1_PIN:
	if (palReadPad(D1_GPIO, D1_PIN)) {
			start_measure(1);
		} else {
			stop_measure(1);
		}
		break;

	case D2_PIN:
	if (palReadPad(D2_GPIO, D2_PIN)) {
			start_measure(2);
		} else {
			stop_measure(2);
		}
		break;

	case D3_PIN:
	if (palReadPad(D3_GPIO, D3_PIN)) {
			start_measure(3);
		} else {
			stop_measure(3);
		}
		break;

	case D4_PIN:
	if (palReadPad(D4_GPIO, D4_PIN)) {
			start_measure(4);
		} else {
			stop_measure(4);
		}
		break;

	case D5_PIN:
	if (palReadPad(D5_GPIO, D5_PIN)) {
			start_measure(5);
		} else {
			stop_measure(5);
		}
		break;

	case D6_PIN:
	if (palReadPad(D6_GPIO, D6_PIN)) {
			start_measure(6);
		} else {
			stop_measure(6);
		}
		break;

	case D7_PIN:
	if (palReadPad(D7_GPIO, D7_PIN)) {
			start_measure(7);
		} else {
			stop_measure(7);
		}
		break;

	case D8_PIN:
	if (palReadPad(D8_GPIO, D8_PIN)) {
			start_measure(8);
		} else {
			stop_measure(8);
		}
		break;

	default:
		break;
	}
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOB | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOA | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOA | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOA | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_BOTH_EDGES | EXT_MODE_GPIOA | EXT_CH_MODE_AUTOSTART, ext_cb},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

msg_t proxymity_node(void * arg) {
	r2p::Node node("proxy");
	r2p::Publisher<r2p::ProximityMsg> proxy_pub;
	r2p::ProximityMsg * msgp;

	(void) arg;
	chRegSetThreadName("proxy");

	node.advertise(proxy_pub, "proximity");

	// Ax
	palSetPadMode(A1_GPIO, A1_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A2_GPIO, A2_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A3_GPIO, A3_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A4_GPIO, A4_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A5_GPIO, A5_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A6_GPIO, A6_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A7_GPIO, A7_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPadMode(A8_GPIO, A8_PIN, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);

	// ER1EN
	palSetPadMode(GPIOA, 12, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPad(GPIOA, 12);

	// ER2EN
	palSetPadMode(GPIOC, 15, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);
	palSetPad(GPIOA, 12);

	r2p::Thread::sleep(r2p::Time::ms(500));

	extStart(&EXTD1, &extcfg);

	for (;;) {
		palSetPort(A_GPIO, A_MASK);
		halPolledDelay(US2RTT(10));
		palClearPort(A_GPIO, A_MASK);

		if (proxy_pub.alloc(msgp)) {
			msgp->value[0] = diff[0] / 5.8;
			msgp->value[1] = diff[1] / 5.8;
			msgp->value[2] = diff[2] / 5.8;
			msgp->value[3] = diff[3] / 5.8;
			msgp->value[4] = diff[4] / 5.8;
			msgp->value[5] = diff[5] / 5.8;
			msgp->value[6] = diff[6] / 5.8;
			msgp->value[7] = diff[7] / 5.8;

			proxy_pub.publish(msgp);
		}
		r2p::Thread::sleep(r2p::Time::ms(100));
	}

	return CH_SUCCESS;
}

/*
 * Application entry point.
 */
extern "C" {
int main(void) {

	halInit();
	chSysInit();

	r2p::Middleware::instance.initialize(wa_info, sizeof(wa_info), r2p::Thread::LOWEST);
	rtcantra.initialize(rtcan_config);
	r2p::Middleware::instance.start();

	r2p::ledsub_conf ledsub_conf = { "led" };
	r2p::Thread::create_heap(NULL, THD_WA_SIZE(512), NORMALPRIO, r2p::ledsub_node, &ledsub_conf);

	r2p::Thread::create_heap(NULL, THD_WA_SIZE(512), NORMALPRIO, proxymity_node, NULL);

	for (;;) {
		r2p::Thread::sleep(r2p::Time::ms(500));
	}

	return CH_SUCCESS;
}
}
