#include "mdrv_pwm.h"
#include "mdrv_types.h"

#include <stdio.h>

static U32 NONPM_MapBase = 0;
static BOOL _gPWM_VDBen  = 0;

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_SetIOMapBase
/// @brief \b Function   \b Description: Set IO Map base
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_SetIOMapBase(U32 u32Base)
{
    NONPM_MapBase = u32Base;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: HAL_PWM_Init
/// @brief \b Function   \b Description: Initial PWM
/// @param <IN>          \b None :
/// @param <OUT>       \b None :
/// @param <RET>        \b BOOL :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
BOOL HAL_PWM_Init(void)
{
    BOOL ret = FALSE;

//currently no idea what this does, probably set io mux? have to RE
#if defined(PRJ_X10) || defined(PRJ_X10P)
    WRITE_WORD_MASK((NONPM_MapBase + REG_TOP_BASE) + (0x64<<2),0xFFFF, BIT12); //Pad_PWM3
#endif

    ret = TRUE;
    return ret;
}


//-------------------------------------------------------------------------------------------------
/// Description: Initial PWM driver
/// @param void
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Init(void)
{
    HAL_PWM_SetIOMapBase(PWM_ADDR_BASE);

    /* Set All pad output and Is PWM. But it still needs to be enable */
    if( !HAL_PWM_Init() )
    {
        printf("PWM Hal Initial Fail\n");
        return E_PWM_FAIL;
    }
    return E_PWM_OK;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PWM_Oen
/// @brief \b Function  \b Description: Switch PWM PAD as Output or Input
/// @param <IN>         \b U16 : index
/// @param <IN>         \b BOOL : letch, 1 for Input; 0 for Output
/// @param <OUT>      \b None :
/// @param <RET>       \b BOOL :
/// @param <GLOBAL>  \b None :
////////////////////////////////////////////////////////////////////////////////
BOOL HAL_PWM_Oen(PWM_ChNum index, BOOL letch)
{

    BOOL bMode = letch? 0: 1;

    HAL_SUBBANK1;

    switch(index)
    {
        case E_PWM_CH0:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(0:0,letch), BMASK(0:0));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(2:2,bMode), BMASK(2:2));
            break;
        case E_PWM_CH1:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(1:1,letch), BMASK(1:1));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(6:6,bMode), BMASK(6:6));
            break;
        case E_PWM_CH2:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(2:2,letch), BMASK(2:2));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(7:7,bMode), BMASK(7:7));
            break;
        case E_PWM_CH3:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(3:3,letch), BMASK(3:3));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(12:12,bMode), BMASK(12:12));
            break;
        case E_PWM_CH4:
            HAL_TOP_WriteRegBit(REG_PWM_OEN,BITS(4:4,letch), BMASK(4:4));
            HAL_TOP_WriteRegBit(REG_PWM_MODE,BITS(13:13,bMode), BMASK(13:13));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
			printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Description: IOutput enable_bar of PWM pads
/// @param u8IndexPWM    \b which pwm is setting
/// @param bOenPWM    \b True/False for enable/disable
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Oen(PWM_ChNum u8IndexPWM, BOOL bOenPWM)
{
    PWM_Result ret = E_PWM_OK;
    // printf("MDrv_PWM_Oen Start\n");

    if(!(HAL_PWM_Oen(u8IndexPWM, bOenPWM)))
    {
        ret = E_PWM_FAIL;
    }
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Period
/// @brief \b Function   \b Description : Set the period of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b U32 : the 18-bit Period value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Period(PWM_ChNum index, U32 u32PeriodPWM)
{
    U16  Period_L, Period_H;
    Period_L = (U16)u32PeriodPWM;
    Period_H = (U16)(u32PeriodPWM >> 16);

    HAL_SUBBANK1;

    /* the Period capability is restricted to ONLY 18-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM0_PERIOD_EXT,BITS(1:0,Period_H),BMASK(1:0));
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM1_PERIOD_EXT,BITS(3:2,Period_H),BMASK(3:2));
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM2_PERIOD_EXT,BITS(5:4,Period_H),BMASK(5:4));
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM3_PERIOD_EXT,BITS(7:6,Period_H),BMASK(7:6));
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_PERIOD, Period_L);
            HAL_PWM_WriteRegBit(REG_PWM4_PERIOD_EXT,BITS(9:8,Period_H),BMASK(9:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;

}

//-------------------------------------------------------------------------------------------------
/// Description: Set the period of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16PeriodPWM    \b the 18-bit period value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_Period(PWM_ChNum u8IndexPWM, U32 u32PeriodPWM)
{
    PWM_Result ret = E_PWM_FAIL;

    do{
        HAL_PWM_Period(u8IndexPWM, u32PeriodPWM);
        ret = E_PWM_OK;
    }while(0);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_DutyCycle
/// @brief \b Function   \b Description : Set the Duty of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b U32 : the 18-bit Duty value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_DutyCycle(PWM_ChNum index, U32 u32DutyPWM)
{
    U16  Duty_L, Duty_H;

    Duty_L = (U16)u32DutyPWM;
    Duty_H = (U8)(u32DutyPWM >> 16);

    HAL_SUBBANK1;
    /* the Duty capability is restricted to ONLY 10-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_Write2Byte(REG_PWM0_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM0_DUTY_EXT,BITS(1:0,Duty_H),BMASK(1:0));
            break;
        case E_PWM_CH1:
            HAL_PWM_Write2Byte(REG_PWM1_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM1_DUTY_EXT,BITS(3:2,Duty_H),BMASK(3:2));
            break;
        case E_PWM_CH2:
            HAL_PWM_Write2Byte(REG_PWM2_DUTY, Duty_L);
	    HAL_PWM_WriteRegBit(REG_PWM2_DUTY_EXT,BITS(5:4,Duty_H),BMASK(5:4));
            break;
        case E_PWM_CH3:
            HAL_PWM_Write2Byte(REG_PWM3_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM3_DUTY_EXT,BITS(7:6,Duty_H),BMASK(7:6));
            break;
        case E_PWM_CH4:
            HAL_PWM_Write2Byte(REG_PWM4_DUTY, Duty_L);
            HAL_PWM_WriteRegBit(REG_PWM4_DUTY_EXT,BITS(9:8,Duty_H),BMASK(9:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;

}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Duty of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DutyPWM    \b the 18-bit Duty value
/// @return E_PWM_OK : succeed
/// @return E_PWM_FAIL : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
PWM_Result MDrv_PWM_DutyCycle(PWM_ChNum u8IndexPWM, U32 u32DutyPWM)
{
    PWM_Result ret = E_PWM_FAIL;

    do{
        HAL_PWM_DutyCycle(u8IndexPWM, u32DutyPWM);
        ret = E_PWM_OK;
    }while(0);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Div
/// @brief \b Function   \b Description : Set the Div of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b U16 : the 16-bit Div value
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Div(PWM_ChNum index, U16 u16DivPWM)
{
	U8 u8DivPWM[2] = {0};

	u8DivPWM[0] = (U8)(u16DivPWM&0xFF);
	u8DivPWM[1] = (U8)(u16DivPWM >> 8);

    HAL_SUBBANK1;
    /* the Div capability is restricted to ONLY 16-bit */
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteByte(REG_PWM0_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM0_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteByte(REG_PWM1_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteRegBit(REG_PWM1_DIV_EXT,u8DivPWM[1],BMASK(15:8));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteByte(REG_PWM2_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM2_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteByte(REG_PWM3_DIV,     u8DivPWM[0]);
            HAL_PWM_WriteRegBit(REG_PWM3_DIV_EXT,u8DivPWM[1],BMASK(15:8));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteByte(REG_PWM4_DIV, 	u8DivPWM[0]);
            HAL_PWM_WriteByte(REG_PWM4_DIV_EXT, u8DivPWM[1]);
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_Polarity
/// @brief \b Function   \b Description : Set the Polarity of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_Polarity(PWM_ChNum index, BOOL bPolPWM)
{
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_PORARITY,BITS(8:8,bPolPWM),BMASK(8:8));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Div of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param u16DivPWM    \b the 16-bit div value
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Div(PWM_ChNum u8IndexPWM, U16 u16DivPWM)
{
    HAL_PWM_Div(u8IndexPWM, u16DivPWM);
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Polarity of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bPolPWM    \b True/False for Inverse/Non-inverse
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Polarity(PWM_ChNum u8IndexPWM, BOOL bPolPWM)
{
    HAL_PWM_Polarity(u8IndexPWM, bPolPWM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_VDBen
/// @brief \b Function   \b Description : Set the Vsync Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_VDBen(PWM_ChNum index, BOOL bVdbenPWM)
{
    _gPWM_VDBen = bVdbenPWM;
    HAL_SUBBANK1;
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_VDBEN,BITS(9:9,bVdbenPWM),BMASK(9:9));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
        default:
            break;
    }
    HAL_SUBBANK0;
}

//-------------------------------------------------------------------------------------------------
/// Description: Set the Vsync Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bVdbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Vdben(PWM_ChNum u8IndexPWM, BOOL bVdbenPWM)
{
    HAL_PWM_VDBen(u8IndexPWM, bVdbenPWM);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name : HAL_PWM_DBen
/// @brief \b Function   \b Description : Set the Double buffer of the specific pwm
/// @param <IN>          \b PWM_ChNum : Enum of the PWM CH
/// @param <IN>          \b BOOL : 1 for enable; 0 for disable
/// @param <OUT>       \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_DBen(PWM_ChNum index, BOOL bdbenPWM)
{
    switch(index)
    {
        case E_PWM_CH0:
            HAL_PWM_WriteRegBit(REG_PWM0_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH1:
            HAL_PWM_WriteRegBit(REG_PWM1_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH2:
            HAL_PWM_WriteRegBit(REG_PWM2_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH3:
            HAL_PWM_WriteRegBit(REG_PWM3_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH4:
            HAL_PWM_WriteRegBit(REG_PWM4_DBEN,BITS(11:11,bdbenPWM),BMASK(11:11));
            break;
        case E_PWM_CH5:
        case E_PWM_CH6:
        case E_PWM_CH7:
        case E_PWM_CH8:
        case E_PWM_CH9:
            printf("[Utopia] The PWM%d is not support\n", (int)index);
            break;
    }
}

//-------------------------------------------------------------------------------------------------
/// Description:  Set the Double buffer of the specific pwm
/// @param u8IndexPWM    \b which pwm is setting
/// @param bDbenPWM    \b True/False for Enable/Disable
//-------------------------------------------------------------------------------------------------
void MDrv_PWM_Dben(PWM_ChNum u8IndexPWM, BOOL bDbenPWM)
{
    // printf("%s(%u, %u)\r\n", __FUNCTION__, u8IndexPWM, bDbenPWM);
    HAL_PWM_DBen(u8IndexPWM, bDbenPWM);
}