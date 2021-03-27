/*
 * RF_Handling.h
 *
 *  Created on: 28.11.2017
 *      Author:
 */

#ifndef INC_RF_HANDLING_H_
#define INC_RF_HANDLING_H_

#include "RF_Config.h"

typedef enum
{
	RF_IDLE,
	RF_IN_SYNC,
#if INCLUDE_BUCKET_SNIFFING == 1
	RF_BUCKET_REPEAT,
	RF_BUCKET_IN_SYNC,
	RF_DECODE_BUCKET,
#endif
	RF_FINISHED
} rf_state_t;

typedef enum
{
	STANDARD,
	ADVANCED
} rf_sniffing_mode_t;

extern bool buffer_out(SI_VARIABLE_SEGMENT_POINTER(bucket, uint16_t, SI_SEG_XDATA));
extern void HandleRFBucket(rf_sniffing_mode_t sniffing_mode, uint16_t duration, bool high_low);
extern void PCA0_DoSniffing(void);
extern void PCA0_StopSniffing(void);
extern void SendRFBuckets(
		SI_VARIABLE_SEGMENT_POINTER(buckets, uint16_t, SI_SEG_XDATA),
		SI_VARIABLE_SEGMENT_POINTER(rfdata, uint8_t, SI_SEG_XDATA), uint8_t data_len);
extern void SendBuckets(
		uint16_t *pulses,
		SI_VARIABLE_SEGMENT_POINTER(start, uint8_t, SI_SEG_CODE), uint8_t start_size,
		SI_VARIABLE_SEGMENT_POINTER(bit0, uint8_t, SI_SEG_CODE), uint8_t bit0_size,
		SI_VARIABLE_SEGMENT_POINTER(bit1, uint8_t, SI_SEG_CODE), uint8_t bit1_size,
		SI_VARIABLE_SEGMENT_POINTER(end, uint8_t, SI_SEG_CODE), uint8_t end_size,
		uint8_t bit_count,
		SI_VARIABLE_SEGMENT_POINTER(rfdata, uint8_t, SI_SEG_XDATA));
extern void SendBucketsByIndex(uint8_t index, SI_VARIABLE_SEGMENT_POINTER(rfdata, uint8_t, SI_SEG_XDATA));
extern void Bucket_Received(uint16_t duration, bool high_low);

// 112 byte == 896 bits, so a RF signal with maximum of 896 bits is possible
// for bucket transmission, this depends on the number of buckets.
// E.g. 4 buckets have a total overhead of 11, allowing 101 bits (high/low pairs)
#if INCLUDE_BUCKET_SNIFFING == 1
#define RF_DATA_BUFFERSIZE		112
#else
#define RF_DATA_BUFFERSIZE		32
#endif

#define RF_DATA_RECEIVED_MASK	0x80

extern SI_SEGMENT_VARIABLE(RF_DATA[RF_DATA_BUFFERSIZE], uint8_t, SI_SEG_XDATA);
// RF_DATA_STATUS
// Bit 7:	1 Data received, 0 nothing received
// Bit 6-0:	Protocol identifier
extern SI_SEGMENT_VARIABLE(RF_DATA_STATUS, uint8_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(rf_state, rf_state_t, SI_SEG_XDATA);

extern SI_SEGMENT_VARIABLE(SYNC_LOW, uint16_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(BIT_HIGH, uint16_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(BIT_LOW, uint16_t, SI_SEG_XDATA);

extern SI_SEGMENT_VARIABLE(actual_byte, uint8_t, SI_SEG_XDATA);

extern SI_SEGMENT_VARIABLE(buckets[7], uint16_t, SI_SEG_XDATA);

#if INCLUDE_BUCKET_SNIFFING == 1
extern SI_SEGMENT_VARIABLE(bucket_sync, uint16_t, SI_SEG_XDATA);
extern SI_SEGMENT_VARIABLE(bucket_count, uint8_t, SI_SEG_XDATA);
#endif

#endif /* INC_RF_HANDLING_H_ */
