/*! @file MobileSample.cpp
 *  @version 3.3
 *  @date May 2017
 *
 *  @brief
 *  MSDK Communication STM32 example.
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "MobileSample.h"
#include "zkrt.h"
#include <string.h>

using namespace DJI;
using namespace DJI::OSDK;

extern Vehicle  vehicle;
extern Vehicle* v;
////////////////////////////////////////////////////////////////////////////////////zkrt use
#include "sys.h"
#include "led.h"
#include "iap.h"
#include "mobileDataHandle.h"
//recv
void
parseFromMobileCallback_v2(DJI::OSDK::Vehicle*      vehicle,
                           DJI::OSDK::RecvContainer recvFrame,
                           DJI::OSDK::UserData      userData) {
	msg_handle_st *msgData = (msg_handle_st*) userData;
	uint8_t *data = msgData->data_recv_app;
	uint8_t *s_data = recvFrame.recvData.raw_ack_array;
	uint16_t datalen = recvFrame.recvInfo.len - OpenProtocol::PackageMin - 2; // minus cmd id and cmd set size //zkrt_test

	if ((datalen >= ZK_FIXED_LEN) && (datalen <= ZK_MAX_LEN)) {
		memcpy(data, s_data, datalen);
		msgData->datalen_recvapp = datalen;
	}
}
//send
extern "C" void sendToMobile(uint8_t *data, uint8_t len);
void sendToMobile(uint8_t *data, uint8_t len) {
	v->moc->sendDataToMSDK(data, len);
//	_FLIGHT_UART_TX_LED = 0;
//	usart1_tx_flag = TimingDelay;
}

