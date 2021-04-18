/*
 * main.c
 *
 *  Created on: Feb 4, 2021
 *      Author: zas
 */
#include "Std_types.h"
#include "Bit_math.h"

#include "RCC_Interface.h"

#include "SyStick_Interface.h"
#include "IR_Interface.h"

#include "LEDMAT_Interface.h"

#define  START    NO_SOUND
#define  UP       TL
#define  DOWN     RPT
#define  RIGHT    VOL_UP
#define  LEFT     EQ

extern u8 BUTTON   ;


u8 Init_Flag = 1;
u8 End_Flag  = 0;


void App_vidInit     (void);
void App_vidEnd      (void);
void App_vidMoveUp   (void);
void App_vidMoveDown (void);
void App_vidMoveRight(void);
void App_vidMoveLeft (void);

u8 Start_Arr[28] = { 0 , 0, 0,   38, 73, 73, 50, 0, 1, 127, 1, 0, 126, 9, 9, 126, 0, 127, 25, 41, 70, 0, 1, 127, 1, 0, 0, 0 };
u8 Score_Arr[28] = { 38, 73, 73, 50, 0, 62, 65, 65, 34, 0, 62, 65, 65, 62, 0, 127, 25, 41, 70, 0, 127, 73, 73, 73, 0 ,0 ,0};
u8 Nun_Arr[52]   = { 62, 65, 65, 62, 0, 2, 65, 127, 64, 0, 98, 81, 73, 70, 0, 34, 73, 73, 54, 0, 12, 10, 9, 127, 8, 0, 70, 73, 73, 49, 0, 62, 73, 73, 49, 0, 1, 1, 1, 127, 0, 54, 73, 73, 54, 0, 6, 73, 73, 62, 0, 0};
u8 ArrayOfPattern[8] = { 0 };

u8 TailCounter = 0;

u8 HeadCol = 0 ;
u8 HeadRow = 2 ;

u8 NodeCol ;
u8 NodeRow ;

u8 TailCol[30] = {0};
u8 TailRaw[30] = {0};



int main(void)
{
	RCC_vidInit();
	RCC_vidEnableClock(RCC_APB2 , AFIO_RCC);
	RCC_vidEnableClock(RCC_APB2 , GPIOA_RCC);
	RCC_vidEnableClock(RCC_APB2 , GPIOB_RCC);

	/*LEDMRX_vidInit();*/
	MSTK_vidInit();
	IR_vidInit();
	LEDMRX_vidInit();

	srand(NodeCol);
	srand(NodeRow);

	NodeCol = rand()%3;
	NodeRow = rand()%5;

	while(1)
	{
		if( BUTTON != 0xff )
		{

			if( BUTTON == START)
			{
				App_vidMoveDown();
			}
			else if( BUTTON == DOWN)
			{
				App_vidMoveUp();

			}
			else if( BUTTON == UP)
			{
				App_vidMoveDown();
			}
			else if( BUTTON == RIGHT)
			{
				App_vidMoveLeft();

			}
			else if( BUTTON == LEFT)
			{

				App_vidMoveRight();
			}

		}

		else
		{
			if(End_Flag == 1)
			{
				App_vidEnd();
				End_Flag = 0;

			}
			else
			{
				App_vidInit();
			}
		}

	}

	return 0;
}



void App_vidInit(void)
{
	for(u8 i = 0 ; i<28 ; i++)
	{
		for(u8 j=0 ; j<7 ; j++)
		{
			ArrayOfPattern[j] = ArrayOfPattern[j+1];
		}
		ArrayOfPattern[7] = Start_Arr[i];

		LEDMRX_vidDisplay( ArrayOfPattern , 8);
		MSTK_vidSetBusyWait(500);


	}
}



void App_vidMoveDown(void)
{
	for(u8 i=0; i<8 ; i++)
	{
		ArrayOfPattern[i] = 0;
	}

	for(u8 i=0; i<8 ; i++)
	{
		if( i == HeadCol )
		{
			ArrayOfPattern[i] |= ( 1 << HeadRow );
		}
		if( i == NodeCol )
		{
			ArrayOfPattern[i] |= ( 1 << NodeRow );
		}
		if( TailCounter != 0 )
		{
			for(u8 j=0 ; j< TailCounter; j++)
			{
				if( i == TailCol[j] )
				{
					ArrayOfPattern[i] |= ( 1 << TailRaw[j] );
				}
			}
		}
	}

	LEDMRX_vidDisplay(ArrayOfPattern , 8);

	if( (HeadCol == NodeCol) && (HeadRow == NodeRow))
	{
		NodeCol = rand()%5;
		NodeRow = rand()%5;

		TailCounter++;
	}

	if(TailCounter > 0)
	{
		for(u8 i=0; i<TailCounter ; i++)
		{
			if( (TailCol[i]==HeadCol) && (TailRaw[i]==HeadRow) )
			{
				End_Flag = 1;
				BUTTON = 0xff;

			}

		}
	}

	if(TailCounter > 0)
	{
		for(u8 i=(TailCounter-1) ;  i>0 ; i--)
		{
			TailCol[i] = TailCol[i-1];
			TailRaw[i] = TailRaw[i-1];
		}
		TailCol[0] = HeadCol;
		TailRaw[0] = HeadRow;
	}

	HeadRow++;
	if( HeadRow == 8 )
	{
		HeadRow = 0;
	}


}



void App_vidMoveUp(void)
{
	for(u8 i=0; i<8 ; i++)
	{
		ArrayOfPattern[i] = 0;
	}

	for(u8 i=0; i<8 ; i++)
	{
		if( i == HeadCol )
		{
			ArrayOfPattern[i] |= ( 1 << HeadRow );
		}
		if( i == NodeCol )
		{
			ArrayOfPattern[i] |= ( 1 << NodeRow );
		}
		if( TailCounter != 0 )
		{
			for(u8 j=0 ; j< TailCounter; j++)
			{
				if( i == TailCol[j] )
				{
					ArrayOfPattern[i] |= ( 1 << TailRaw[j] );
				}
			}
		}
	}

	LEDMRX_vidDisplay(ArrayOfPattern , 8);

	if( (HeadCol == NodeCol) && (HeadRow == NodeRow))
	{
		NodeCol = rand()%5;
	    NodeRow = rand()%5;

		TailCounter++;
	}

	if(TailCounter > 0)
	{
		for(u8 i=0; i<TailCounter ; i++)
		{
			if( (TailCol[i]==HeadCol) && (TailRaw[i]==HeadRow) )
			{
				End_Flag = 1;
				BUTTON = 0xff;

			}

		}
	}

	if(TailCounter > 0)
	{
		for(u8 i=(TailCounter-1) ;  i>0 ; i--)
		{
			TailCol[i] = TailCol[i-1];
			TailRaw[i] = TailRaw[i-1];
		}
		TailCol[0] = HeadCol;
		TailRaw[0] = HeadRow;
	}

	if( HeadRow == 0 )
	{
		HeadRow = 8;
	}

	HeadRow--;

}

void App_vidMoveRight(void)
{
	for(u8 i=0; i<8 ; i++)
	{
		ArrayOfPattern[i] = 0;
	}

	for(u8 i=0; i<8 ; i++)
	{
		if( i == HeadCol )
		{
			ArrayOfPattern[i] |= ( 1 << HeadRow );
		}
		if( i == NodeCol )
		{
			ArrayOfPattern[i] |= ( 1 << NodeRow );
		}
		if( TailCounter != 0 )
		{
			for(u8 j=0 ; j< TailCounter; j++)
			{
				if( i == TailCol[j] )
				{
					ArrayOfPattern[i] |= ( 1 << TailRaw[j] );
				}
			}
		}
	}

	LEDMRX_vidDisplay(ArrayOfPattern , 8);

	if( (HeadCol == NodeCol) && (HeadRow == NodeRow))
	{

		NodeCol = rand()%5;
		NodeRow = rand()%5;

		TailCounter++;
	}

	if(TailCounter > 0)
	{
		for(u8 i=0; i<TailCounter ; i++)
		{
			if( (TailCol[i]==HeadCol) && (TailRaw[i]==HeadRow) )
			{
				End_Flag = 1;
				BUTTON = 0xff;

			}

		}
	}

	if(TailCounter > 0)
	{
		for(u8 i=(TailCounter-1) ;  i>0 ; i--)
		{
			TailCol[i] = TailCol[i-1];
			TailRaw[i] = TailRaw[i-1];
		}
		TailCol[0] = HeadCol;
		TailRaw[0] = HeadRow;
	}

	HeadCol++;
	if( HeadCol == 8 )
	{
		HeadCol = 0;
	}

}
void App_vidMoveLeft(void)
{

	for(u8 i=0; i<8 ; i++)
	{
		ArrayOfPattern[i] = 0;
	}

	for(u8 i=0; i<8 ; i++)
	{
		if( i == HeadCol )
		{
			ArrayOfPattern[i] |= ( 1 << HeadRow );
		}
		if( i == NodeCol )
		{
			ArrayOfPattern[i] |= ( 1 << NodeRow );
		}
		if( TailCounter != 0 )
		{
			for(u8 j=0 ; j< TailCounter; j++)
			{
				if( i == TailCol[j] )
				{
					ArrayOfPattern[i] |= ( 1 << TailRaw[j] );
				}
			}
		}
	}

	LEDMRX_vidDisplay(ArrayOfPattern , 8);

	if( (HeadCol == NodeCol) && (HeadRow == NodeRow))
	{
		NodeCol = rand()%5;
		NodeRow = rand()%5;

		TailCounter++;
	}

	if(TailCounter > 0)
	{
		for(u8 i=0; i<TailCounter ; i++)
		{
			if( (TailCol[i]==HeadCol) && (TailRaw[i]==HeadRow) )
			{
				End_Flag = 1;
				BUTTON = 0xff;

			}

		}
	}
	if(TailCounter > 0)
	{
		for(u8 i=(TailCounter-1) ;  i>0 ; i--)
		{
			TailCol[i] = TailCol[i-1];
			TailRaw[i] = TailRaw[i-1];
		}
		TailCol[0] = HeadCol;
		TailRaw[0] = HeadRow;
	}

	if( HeadCol == 0 )
	{
		HeadCol = 8;
	}

	HeadCol--;

}

void App_vidEnd(void)
{
	u8 Num_1 = TailCounter/10;
	u8 Num_2 = TailCounter%10;

	for(u8 i=0; i<TailCounter; i++)
	{
		TailCol[i] = 0;
		TailRaw[i] = 0;
	}

	for(u8 i = 0 ; i<28 ; i++)
	{
		for(u8 j=0 ; j<7 ; j++)
		{
			ArrayOfPattern[j] = ArrayOfPattern[j+1];
		}
		ArrayOfPattern[7] = Score_Arr[i];

		LEDMRX_vidDisplay( ArrayOfPattern , 8);
		MSTK_vidSetBusyWait(500);
	}

	for(u8 i = 0 ; i<5 ; i++)
	{
		for(u8 j=0 ; j<7 ; j++)
		{
			ArrayOfPattern[j] = ArrayOfPattern[j+1];
		}
		ArrayOfPattern[7] = Nun_Arr[(Num_1*5) + i];

		LEDMRX_vidDisplay( ArrayOfPattern , 8);
		MSTK_vidSetBusyWait(500);
	}
	for(u8 i = 0 ; i<5 ; i++)
	{
		for(u8 j=0 ; j<7 ; j++)
		{
			ArrayOfPattern[j] = ArrayOfPattern[j+1];
		}
		ArrayOfPattern[7] = Nun_Arr[(Num_2*5) + i];

		LEDMRX_vidDisplay( ArrayOfPattern , 8);
		MSTK_vidSetBusyWait(500);
	}

	TailCounter = 0;
	HeadCol = 0 ;
	HeadRow = 2 ;

}

