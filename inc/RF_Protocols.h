/*
 * RF_Protocols.h
 *
 *  Created on: 28.11.2017
 *      Author:
 */

#ifndef INC_RF_PROTOCOLS_H_
#define INC_RF_PROTOCOLS_H_

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdint.h>
#include "RF_Config.h"

/*
 * bucket sniffing constants
 */
#define MIN_FOOTER_LENGTH	300
#define MIN_BUCKET_LENGTH	100

#define PT226x_SYNC_MIN		4500

/*
 * sync constants
 */
#define TOLERANCE_MAX		500
#define TOLERANCE_MIN		100

/*
 * number of repeating by default
 */
#define RF_TRANSMIT_REPEATS		8

typedef struct PROTOCOL_STATUS
{
	uint16_t status;
	uint8_t bit_count;
	uint8_t actual_bit_of_byte;
} PROTOCOL_STATUS;

typedef struct PROTOCOL_DATA_UINT8_T
{
	// pointer to array of uint8_t elements
	SI_VARIABLE_SEGMENT_POINTER(dat, uint8_t, SI_SEG_CODE);
	// size of the array
	uint8_t size;
} PROTOCOL_DATA_UINT8_T;

typedef struct PROTOCOL_DATA_UINT16_T
{
	// pointer to array of uint16_t elements
	SI_VARIABLE_SEGMENT_POINTER(dat, uint16_t, SI_SEG_CODE);
	// size of the array
	uint8_t size;
} PROTOCOL_DATA_UINT16_T;

typedef struct BUCKET_PROTOCOL_DATA
{
	// array and array size of buckets
	PROTOCOL_DATA_UINT16_T buckets;
	// array and array size of start buckets
	PROTOCOL_DATA_UINT8_T start;
	// array and array size of bit 0 buckets
	PROTOCOL_DATA_UINT8_T bit0;
	// array and array size of bit 1 buckets
	PROTOCOL_DATA_UINT8_T bit1;
	// array and array size of end buckets
	PROTOCOL_DATA_UINT8_T end;
	// bit count for this protocol
	uint8_t bit_count;
} BUCKET_PROTOCOL_DATA;

/*
 * PT2260, EV1527,... original RF bridge protocol
 * http://www.princeton.com.tw/Portals/0/Product/PT2260_4.pdf
 */
#if EFM8BB1_SUPPORT_PT226X_PROTOCOL == 1
#define PT226X
SI_SEGMENT_VARIABLE(PROTOCOL_BUCKETS(PT226X)[], static uint16_t, SI_SEG_CODE) = { 350, 1050, 10850 };
SI_SEGMENT_VARIABLE(PROTOCOL_START(PT226X)[], static uint8_t, SI_SEG_CODE) = { HIGH(0), LOW(2) };
SI_SEGMENT_VARIABLE(PROTOCOL_BIT0(PT226X)[], static uint8_t, SI_SEG_CODE) = { HIGH(0), LOW(1) };
SI_SEGMENT_VARIABLE(PROTOCOL_BIT1(PT226X)[], static uint8_t, SI_SEG_CODE) = { HIGH(1), LOW(0) };
#endif

SI_SEGMENT_VARIABLE(PROTOCOL_DATA[], static struct BUCKET_PROTOCOL_DATA, SI_SEG_CODE) =
{
#if EFM8BB1_SUPPORT_PT226X_PROTOCOL == 1
		/*
		 * PT2260, EV1527,... original RF bridge protocol
		 */
		{
			{ &PROTOCOL_BUCKETS(PT226X), ARRAY_LENGTH(PROTOCOL_BUCKETS(PT226X)) },
			{ &PROTOCOL_START(PT226X), ARRAY_LENGTH(PROTOCOL_START(PT226X)) },
			{ &PROTOCOL_BIT0(PT226X), ARRAY_LENGTH(PROTOCOL_BIT0(PT226X)) },
			{ &PROTOCOL_BIT1(PT226X), ARRAY_LENGTH(PROTOCOL_BIT1(PT226X)) },
			{ NULL, 0 },
			24
		},
#endif
};

#define PROTOCOLCOUNT (sizeof(PROTOCOL_DATA) / sizeof(PROTOCOL_DATA[0]))
#endif /* INC_RF_PROTOCOLS_H_ */
