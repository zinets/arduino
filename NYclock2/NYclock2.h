//               Pgfedcba
#define DIG_1  0b00000110
#define DIG_2  0b01011011
#define DIG_3  0b01001111
#define DIG_4  0b01100110
#define DIG_5  0b01101101
#define DIG_6  0b01111101
#define DIG_7  0b00000111
#define DIG_8  0b01111111
#define DIG_9  0b01101111
#define DIG_0  0b00111111
#define DIG_DP 0b10000000
#define SIGN_N 0b01110110
#define SIGN_Y 0b00110001
#define SIGN_C 0b00111001
#define SIGN_MINUS 0b01000000
//               Pgfedcba

//{{AVR_IO_DESIGNER_HDR
// C:\projects\arduino\NYclock2\NYclock2.aid

//
// PORTB variable definitions
//
typedef struct
{
	unsigned char	_Unused0         : 1;	// 
	unsigned char	_Unused1         : 1;	// 
	unsigned char	_LATCH           : 1;	// 
	unsigned char	_Unused3         : 1;	// 
	unsigned char	_Unused4         : 1;	// 
	unsigned char	_Unused5         : 1;	// 
	unsigned char	_Unused6         : 1;	// 
	unsigned char	_Unused7         : 1;	// 
} PORTB_STRUCT;

// PORTB2 for 1: LATCH as Output - 
#define	LATCH_PORT              ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	LATCH_PIN               ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	LATCH_DDR               ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	LATCH                   LATCH_PORT->_LATCH                      // output
#define	LATCH_TOGGLE            LATCH_PIN->_LATCH                       // output toggle
#define	LATCH_DIR               LATCH_DDR->_LATCH                       // direction control
#define	LATCH_MASK              0x04                                    // mask
#define	LATCH_SHIFT             0x02                                    // shift count

//
// PORTC variable definitions
//
typedef struct
{
	unsigned char	_CAT1            : 1;	// Reserved for pin PC0
	unsigned char	_CAT2            : 1;	// 
	unsigned char	_CAT3            : 1;	// 
	unsigned char	_CAT4            : 1;	// 
	unsigned char	_Unused4         : 1;	// 
	unsigned char	_Unused5         : 1;	// 
	unsigned char	_Unused6         : 1;	// 
} PORTC_STRUCT;

// PORTC0 for 1: CAT1 as Output - Reserved for pin PC0
#define	CAT1_PORT               ((volatile PORTC_STRUCT*)&PORTC)        // PORT register
#define	CAT1_PIN                ((volatile PORTC_STRUCT*)&PINC)         // PIN register
#define	CAT1_DDR                ((volatile PORTC_STRUCT*)&DDRC)         // DDR register
#define	CAT1                    CAT1_PORT->_CAT1                        // output
#define	CAT1_TOGGLE             CAT1_PIN->_CAT1                         // output toggle
#define	CAT1_DIR                CAT1_DDR->_CAT1                         // direction control
#define	CAT1_MASK               0x01                                    // mask
#define	CAT1_SHIFT              0x00                                    // shift count

// PORTC1 for 1: CAT2 as Output - 
#define	CAT2_PORT               ((volatile PORTC_STRUCT*)&PORTC)        // PORT register
#define	CAT2_PIN                ((volatile PORTC_STRUCT*)&PINC)         // PIN register
#define	CAT2_DDR                ((volatile PORTC_STRUCT*)&DDRC)         // DDR register
#define	CAT2                    CAT2_PORT->_CAT2                        // output
#define	CAT2_TOGGLE             CAT2_PIN->_CAT2                         // output toggle
#define	CAT2_DIR                CAT2_DDR->_CAT2                         // direction control
#define	CAT2_MASK               0x02                                    // mask
#define	CAT2_SHIFT              0x01                                    // shift count

// PORTC2 for 1: CAT3 as Output - 
#define	CAT3_PORT               ((volatile PORTC_STRUCT*)&PORTC)        // PORT register
#define	CAT3_PIN                ((volatile PORTC_STRUCT*)&PINC)         // PIN register
#define	CAT3_DDR                ((volatile PORTC_STRUCT*)&DDRC)         // DDR register
#define	CAT3                    CAT3_PORT->_CAT3                        // output
#define	CAT3_TOGGLE             CAT3_PIN->_CAT3                         // output toggle
#define	CAT3_DIR                CAT3_DDR->_CAT3                         // direction control
#define	CAT3_MASK               0x04                                    // mask
#define	CAT3_SHIFT              0x02                                    // shift count

// PORTC3 for 1: CAT4 as Output - 
#define	CAT4_PORT               ((volatile PORTC_STRUCT*)&PORTC)        // PORT register
#define	CAT4_PIN                ((volatile PORTC_STRUCT*)&PINC)         // PIN register
#define	CAT4_DDR                ((volatile PORTC_STRUCT*)&DDRC)         // DDR register
#define	CAT4                    CAT4_PORT->_CAT4                        // output
#define	CAT4_TOGGLE             CAT4_PIN->_CAT4                         // output toggle
#define	CAT4_DIR                CAT4_DDR->_CAT4                         // direction control
#define	CAT4_MASK               0x08                                    // mask
#define	CAT4_SHIFT              0x03                                    // shift count

// TIMER_COUNTER_2 -  - 
#define	TIMER_COUNTER_2_Divisor	128
#define	TIMER_COUNTER_2_CountsPerSecond	125000
#define	TIMER_COUNTER_2_usPerCount	8               // (8)
#define	TIMER_COUNTER_2_nsPerCount	8000            // (8000)
#define	TIMER_COUNTER_2_msPeriod	2               // (2)
#define	TIMER_COUNTER_2_usPeriod	2000            // (2000)
#define	TIMER_COUNTER_2_nsPeriod	2000000         // (2000000)
//}}AVR_IO_DESIGNER_HDR
