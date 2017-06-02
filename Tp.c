/***************************************************************************
**                       $TITLE: BP.C$
** BP.C
** ---------
**    Brings data in from the codec, runs it through a digital bandpass
**    filter, and then outputs the data through the codec line out.  The
**    filter coefficients are in the file: fir.h
**
****************************************************************************/
/*--------------------------------------------------------------------------
    INCLUDES
--------------------------------------------------------------------------*/
#include <def21060.h>
#include <21065l.h>
#include <signal.h>
#include <macros.h>
#include <math.h>
#include <filters.h>
#include <ezkit/1819regs.h>

/*--------------------------------------------------------------------------
   CONSTANT & MACRO DEFINITIONS
--------------------------------------------------------------------------*/
/* Codec tag word to send out left and right samples */
#define DOUT_TAG         0x9800

/* Codec tag word to send out address and data slots */
#define REGOUT_TAG       0xe000

/* This is the codec register setting for line input on both channels */
#define SOURCE_IS_LINE   0x0404
#define SOURCE_IS_MIC    0

/* Codec addreses */
#define SOURCE_ADDR      0x1a00
#define RECGAIN_ADDR     0x1c00

#define NUM_TAPS 256

#define CODEC_ISR_VECT 0X9001

/*--------------------------------------------------------------------------
   EXTERNAL DECLARATIONS
--------------------------------------------------------------------------*/
/* These compile to the same addresses as in the kernel if the .ldf is set
   up correctly  */
extern volatile int user_tx_buf[6];
extern volatile int user_tx_ready;
extern volatile int user_rx_buf[6];
extern volatile int user_rx_ready;

/*--------------------------------------------------------------------------
   GLOBAL DECLARATIONS
--------------------------------------------------------------------------*/
/* This variable lets the debugger know that this is the bandpass demo */
int BANDPASS;

/* This array holds the filter coefficients */
float pm coeffs[5][NUM_TAPS] =
{
#include "fir.h"
};

float dm state[NUM_TAPS+1];

/* The host can set these 2 variables */
static int source;   /* 0 = codec, 1 = noise */
static int filter;   /* 0 - 4 are different bandpass ranges */

/*--------------------------------------------------------------------------
   FUNCTION PROTOTYPES
--------------------------------------------------------------------------*/
void init_codec( void );
void main (void);
float tp();

/****************************************************************************
**
** Procedure:  main()
**
** Arguments:  None
**
** Returns:    None
**
** Desc:       main code
**
****************************************************************************/
void main ( void )
{
    int i;
    float filter_input;

    // Initialize state array for FIR filter.
    for( i=0 ; i<NUM_TAPS+1 ; i++ )
        state[i] = 0.0;

    // Initialize some codec registers.
    init_codec();

    // Loop forever.
   
   user_tx_ready = 0;
   for(;;)
   {
      user_rx_ready = 1;
      idle();
      while (user_rx_ready);
      while (user_tx_ready);
 
      // Get sample from Codec or random noise.    
      if( source == 0 )
      {
         filter_input = user_rx_buf[RIGHT_CHNL];
      }
      else
      {
         filter_input = rand() & 0x00003fff;
      }

      // Filter sample and output.
      if( filter )
      {
         user_tx_buf[RIGHT_CHNL] = fir( filter_input, &coeffs[filter][0], &state[0], (int)NUM_TAPS );
         user_tx_buf[LEFT_CHANL] = user_tx_buf[RIGHT_CHNL];
      }
      else
      {
         user_tx_buf[RIGHT_CHNL] = filter_input;
         user_tx_buf[LEFT_CHANL] = filter_input;
      }
      user_tx_buf[TAG] = DOUT_TAG;
      user_tx_ready = 1;

   };
}

/****************************************************************************
**
** Procedure:  init_codec()
**
** Arguments:  None
**
** Returns:    None
**
** Desc:       Unmasks the sport1 interrupt and uses the kernel isr
**             to initialize the codec input source and record gain.
**
****************************************************************************/
void init_codec( void )
{
   asm("#include <def21065l.h>");
   interrupt(SIG_SPT1I,(void (*)(int))CODEC_ISR_VECT);
   asm("BIT SET IMASK SPT1I;");   /* unmasks sport interrupt */
    
   /* Set source to LINE */
   user_tx_buf[TAG] = REGOUT_TAG;
   user_tx_buf[ADDR] = SOURCE_ADDR;
   user_tx_buf[DATA] = SOURCE_IS_LINE;
   user_tx_ready = 1;                 /* Tell the isr that txbuf is ready */
   idle();
   idle();

   /* Set record gain */
   user_tx_buf[TAG] = REGOUT_TAG;
   user_tx_buf[ADDR] = RECGAIN_ADDR;
   user_tx_buf[DATA] = 0;
   user_tx_ready = 1;                 /* Tell the isr that txbuf is ready */
   idle();
   idle();
  
   return;
}


float coeff_tp[][];
float state_tf[][];
int ans_tf[];


float tp(float x[int], float coeff0[int][int], float state0[int][int], int ans[])
{
	float w0;
	float y;
	int len = 100;
	int sum = 0;
	for(i=1; i<len; i++)
	{
	
		w[k-1] = x[k] + state[0][]*a1 +w2 *a2;
		w2 = w1;
		w1 = w0;
		y = w0+ w1 *b1 +w2 *b2;
	
	
	}

	
	return y
}

