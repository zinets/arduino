//{{AVR_IO_DESIGNER_HDR
// C:\Users\Zinetz Victor\Documents\Arduino\NYclock\NYclock.aid

//
// PORTB variable definitions
//
typedef struct
{
    unsigned char	_SEG_B           : 1;	//
    unsigned char	_SEG_A           : 1;	//
    unsigned char	_CAT1            : 1;	//
    unsigned char	_CAT2            : 1;	//
    unsigned char	_CAT3            : 1;	//
    unsigned char	_CAT4            : 1;	//
    unsigned char	_Unused6         : 1;	//
    unsigned char	_Unused7         : 1;	//
} PORTB_STRUCT;

// PORTB0 for 1: SEG_B as Output -
#define	SEG_B_PORT              ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	SEG_B_PIN               ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	SEG_B_DDR               ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	SEG_B                   SEG_B_PORT->_SEG_B                      // output
#define	SEG_B_TOGGLE            SEG_B_PIN->_SEG_B                       // output toggle
#define	SEG_B_DIR               SEG_B_DDR->_SEG_B                       // direction control
#define	SEG_B_MASK              0x01                                    // mask
#define	SEG_B_SHIFT             0x00                                    // shift count

// PORTB1 for 1: SEG_A as Output -
#define	SEG_A_PORT              ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	SEG_A_PIN               ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	SEG_A_DDR               ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	SEG_A                   SEG_A_PORT->_SEG_A                      // output
#define	SEG_A_TOGGLE            SEG_A_PIN->_SEG_A                       // output toggle
#define	SEG_A_DIR               SEG_A_DDR->_SEG_A                       // direction control
#define	SEG_A_MASK              0x02                                    // mask
#define	SEG_A_SHIFT             0x01                                    // shift count

// PORTB2 for 1: CAT1 as Output -
#define	CAT1_PORT               ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	CAT1_PIN                ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	CAT1_DDR                ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	CAT1                    CAT1_PORT->_CAT1                        // output
#define	CAT1_TOGGLE             CAT1_PIN->_CAT1                         // output toggle
#define	CAT1_DIR                CAT1_DDR->_CAT1                         // direction control
#define	CAT1_MASK               0x04                                    // mask
#define	CAT1_SHIFT              0x02                                    // shift count

// PORTB3 for 1: CAT2 as Output -
#define	CAT2_PORT               ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	CAT2_PIN                ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	CAT2_DDR                ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	CAT2                    CAT2_PORT->_CAT2                        // output
#define	CAT2_TOGGLE             CAT2_PIN->_CAT2                         // output toggle
#define	CAT2_DIR                CAT2_DDR->_CAT2                         // direction control
#define	CAT2_MASK               0x08                                    // mask
#define	CAT2_SHIFT              0x03                                    // shift count

// PORTB4 for 1: CAT3 as Output -
#define	CAT3_PORT               ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	CAT3_PIN                ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	CAT3_DDR                ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	CAT3                    CAT3_PORT->_CAT3                        // output
#define	CAT3_TOGGLE             CAT3_PIN->_CAT3                         // output toggle
#define	CAT3_DIR                CAT3_DDR->_CAT3                         // direction control
#define	CAT3_MASK               0x10                                    // mask
#define	CAT3_SHIFT              0x04                                    // shift count

// PORTB5 for 1: CAT4 as Output -
#define	CAT4_PORT               ((volatile PORTB_STRUCT*)&PORTB)        // PORT register
#define	CAT4_PIN                ((volatile PORTB_STRUCT*)&PINB)         // PIN register
#define	CAT4_DDR                ((volatile PORTB_STRUCT*)&DDRB)         // DDR register
#define	CAT4                    CAT4_PORT->_CAT4                        // output
#define	CAT4_TOGGLE             CAT4_PIN->_CAT4                         // output toggle
#define	CAT4_DIR                CAT4_DDR->_CAT4                         // direction control
#define	CAT4_MASK               0x20                                    // mask
#define	CAT4_SHIFT              0x05                                    // shift count

//
// PORTD variable definitions
//
typedef struct
{
    unsigned char	_Unused0         : 1;	//
    unsigned char	_Unused1         : 1;	//
    unsigned char	_SEG_DP          : 1;	//
    unsigned char	_SEG_G           : 1;	//
    unsigned char	_SEG_F           : 1;	//
    unsigned char	_SEG_E           : 1;	//
    unsigned char	_SEG_D           : 1;	//
    unsigned char	_SEG_C           : 1;	//
} PORTD_STRUCT;

// PORTD2 for 1: SEG_DP as Output -
#define	SEG_DP_PORT             ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_DP_PIN              ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_DP_DDR              ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_DP                  SEG_DP_PORT->_SEG_DP                    // output
#define	SEG_DP_TOGGLE           SEG_DP_PIN->_SEG_DP                     // output toggle
#define	SEG_DP_DIR              SEG_DP_DDR->_SEG_DP                     // direction control
#define	SEG_DP_MASK             0x04                                    // mask
#define	SEG_DP_SHIFT            0x02                                    // shift count

// PORTD3 for 1: SEG_G as Output -
#define	SEG_G_PORT              ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_G_PIN               ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_G_DDR               ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_G                   SEG_G_PORT->_SEG_G                      // output
#define	SEG_G_TOGGLE            SEG_G_PIN->_SEG_G                       // output toggle
#define	SEG_G_DIR               SEG_G_DDR->_SEG_G                       // direction control
#define	SEG_G_MASK              0x08                                    // mask
#define	SEG_G_SHIFT             0x03                                    // shift count

// PORTD4 for 1: SEG_F as Output -
#define	SEG_F_PORT              ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_F_PIN               ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_F_DDR               ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_F                   SEG_F_PORT->_SEG_F                      // output
#define	SEG_F_TOGGLE            SEG_F_PIN->_SEG_F                       // output toggle
#define	SEG_F_DIR               SEG_F_DDR->_SEG_F                       // direction control
#define	SEG_F_MASK              0x10                                    // mask
#define	SEG_F_SHIFT             0x04                                    // shift count

// PORTD5 for 1: SEG_E as Output -
#define	SEG_E_PORT              ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_E_PIN               ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_E_DDR               ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_E                   SEG_E_PORT->_SEG_E                      // output
#define	SEG_E_TOGGLE            SEG_E_PIN->_SEG_E                       // output toggle
#define	SEG_E_DIR               SEG_E_DDR->_SEG_E                       // direction control
#define	SEG_E_MASK              0x20                                    // mask
#define	SEG_E_SHIFT             0x05                                    // shift count

// PORTD6 for 1: SEG_D as Output -
#define	SEG_D_PORT              ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_D_PIN               ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_D_DDR               ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_D                   SEG_D_PORT->_SEG_D                      // output
#define	SEG_D_TOGGLE            SEG_D_PIN->_SEG_D                       // output toggle
#define	SEG_D_DIR               SEG_D_DDR->_SEG_D                       // direction control
#define	SEG_D_MASK              0x40                                    // mask
#define	SEG_D_SHIFT             0x06                                    // shift count

// PORTD7 for 1: SEG_C as Output -
#define	SEG_C_PORT              ((volatile PORTD_STRUCT*)&PORTD)        // PORT register
#define	SEG_C_PIN               ((volatile PORTD_STRUCT*)&PIND)         // PIN register
#define	SEG_C_DDR               ((volatile PORTD_STRUCT*)&DDRD)         // DDR register
#define	SEG_C                   SEG_C_PORT->_SEG_C                      // output
#define	SEG_C_TOGGLE            SEG_C_PIN->_SEG_C                       // output toggle
#define	SEG_C_DIR               SEG_C_DDR->_SEG_C                       // direction control
#define	SEG_C_MASK              0x80                                    // mask
#define	SEG_C_SHIFT             0x07                                    // shift count

// TIMER_COUNTER_2 -  -
#define	TIMER_COUNTER_2_Divisor	128
#define	TIMER_COUNTER_2_CountsPerSecond	125000
#define	TIMER_COUNTER_2_usPerCount	8               // (8)
#define	TIMER_COUNTER_2_nsPerCount	8000            // (8000)
#define	TIMER_COUNTER_2_msPeriod	2               // (2)
#define	TIMER_COUNTER_2_usPeriod	2000            // (2000)
#define	TIMER_COUNTER_2_nsPeriod	2000000         // (2000000)
//}}AVR_IO_DESIGNER_HDR

// digit 0
#define PORTB_0 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_0 (1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_E_SHIFT | 1 << SEG_F_SHIFT)

// digit 1
#define PORTB_1  (1 << SEG_B_SHIFT)
#define PORTD_1  (1 << SEG_C_SHIFT)

// digit 2
#define PORTB_2 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_2 (1 << SEG_G_SHIFT | 1 << SEG_E_SHIFT | 1 << SEG_D_SHIFT)

// digit 3
#define PORTB_3 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_3 (1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_G_SHIFT)

// digit 4
#define PORTB_4 (1 << SEG_B_SHIFT)
#define PORTD_4 (1 << SEG_C_SHIFT | 1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT)

// digit 5
#define PORTB_5 (1 << SEG_A_SHIFT)
#define PORTD_5 (1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT | 1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT)

// digit 6
#define PORTB_6 (1 << SEG_A_SHIFT)
#define PORTD_6 (1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT | 1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_E_SHIFT)

// digit 7
#define PORTB_7 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_7 (1 << SEG_C_SHIFT)

// digit 8
#define PORTB_8 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_8 (1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_E_SHIFT | 1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT)

// digit 9
#define PORTB_9 (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_9 (1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT)

// empty
#define PORTB_E  (1 << SEG_A_SHIFT | 1 << SEG_B_SHIFT)
#define PORTD_E  (1 << SEG_C_SHIFT | 1 << SEG_D_SHIFT | 1 << SEG_E_SHIFT | 1 << SEG_F_SHIFT | 1 << SEG_G_SHIFT)

// sign C
#define PORTB_C  (SEG_A_MASK)
#define PORTD_C  (SEG_F_MASK | SEG_E_MASK | SEG_D_MASK)

// sign 'Н'
#define PORTB_N (SEG_B_MASK)
#define PORTD_N (SEG_C_MASK | SEG_G_MASK | SEG_F_MASK | SEG_E_MASK)

// sign 'Г'
#define PORTB_Y (SEG_A_MASK)
#define PORTD_Y (SEG_F_MASK | SEG_E_MASK)

// sign '-'
#define PORTB_MIN 0
#define PORTD_MIN (SEG_G_MASK)
