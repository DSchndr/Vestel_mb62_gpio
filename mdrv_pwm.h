//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_TOP_BASE (0x3C00)
#define REG_PWM_BASE (0x6400)
//Base adress, set pwm bank by writing 0x0 to 1, register val off = register val << 2 in REG_PWM_BASE
#define PWM_ADDR_BASE 0x1f200000 //nonpm

#define HAL_PWM_ReadByte(addr)             READ_BYTE((PWM_ADDR_BASE + REG_PWM_BASE) + ((addr)<<2))
#define HAL_PWM_Read2Byte(addr)              READ_WORD((PWM_ADDR_BASE + REG_PWM_BASE) + ((addr)<<2))

#define HAL_PWM_WriteByte(addr, val)          WRITE_BYTE((PWM_ADDR_BASE + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_Write2Byte(addr, val)        WRITE_WORD((PWM_ADDR_BASE + REG_PWM_BASE) + ((addr)<<2), (val))
#define HAL_PWM_WriteRegBit(addr, val, mask) WRITE_WORD_MASK((PWM_ADDR_BASE + REG_PWM_BASE) + ((addr)<<2), (val), (mask))

#define HAL_SUBBANK0    HAL_PWM_WriteByte(0,0)
#define HAL_SUBBANK1    HAL_PWM_WriteByte(0,1) /* PWM sub-bank */


//#define BIT(_bit_)                  (1 << (_bit_))
#define ALIGN_4(_x_)                (((_x_) + 3) & ~3)
#define ALIGN_8(_x_)                (((_x_) + 7) & ~7)
#define ALIGN_16(_x_)               (((_x_) + 15) & ~15)           // No data type specified, optimized by complier
#define ALIGN_32(_x_)               (((_x_) + 31) & ~31)           // No data type specified, optimized by complier

#define MASK(x)     (((1<<(x##_BITS))-1) << x##_SHIFT)
// #define BIT(_bit_)                  (1 << (_bit_))
#define BIT_(x)                     BIT(x) //[OBSOLETED] //TODO: remove it later
#define BITS(_bits_, _val_)         ((BIT(((1)?_bits_)+1)-BIT(((0)?_bits_))) & (_val_<<((0)?_bits_)))
#define BMASK(_bits_)               (BIT(((1)?_bits_)+1)-BIT(((0)?_bits_)))

#define REG_ALL_PAD_IN              (0x50) //bit 15;set all pads (except SPI) as input
#define REG_PWM_OEN                 (0x03) //bit 0~4

#define REG_PWM_MODE			(0x64)
#define PAD_PWM0_OUT                (BIT2)
#define PAD_PWM1_OUT                (BIT6)
#define PAD_PWM2_OUT                (BIT7)
#define PAD_PWM3_OUT                (BIT12)
#define PAD_PWM4_OUT                (BIT13)

#define REG_PWM0_DBEN               (0x04)  //bit11
#define REG_PWM1_DBEN               (0x07)  //bit11
#define REG_PWM2_DBEN               (0x0A)  //bit11
#define REG_PWM3_DBEN               (0x0D)  //bit11
#define REG_PWM4_DBEN               (0x10)  //bit11

#define REG_PWM0_VDBEN              (0x04)  //bit9
#define REG_PWM1_VDBEN              (0x07)  //bit9
#define REG_PWM2_VDBEN              (0x0A)  //bit9
#define REG_PWM3_VDBEN              (0x0D)  //bit9
#define REG_PWM4_VDBEN              (0x10)  //bit9

#define REG_PWM0_PERIOD             (0x02)  //bit0~15
#define REG_PWM1_PERIOD             (0x05)  //bit0~15
#define REG_PWM2_PERIOD             (0x08)  //bit0~15
#define REG_PWM3_PERIOD             (0x0B)  //bit0~15
#define REG_PWM4_PERIOD             (0x0E)  //bit0~15

#define REG_PWM0_PERIOD_EXT         (0x20)  //bit0~1
#define REG_PWM1_PERIOD_EXT         (0x20)  //bit2~3
#define REG_PWM2_PERIOD_EXT         (0x20)  //bit4~5
#define REG_PWM3_PERIOD_EXT         (0x20)  //bit6~7
#define REG_PWM4_PERIOD_EXT         (0x20)  //bit8~9

#define REG_PWM0_DUTY               (0x03)  //bit0~15
#define REG_PWM1_DUTY               (0x06)  //bit0~15
#define REG_PWM2_DUTY               (0x09)  //bit0~15
#define REG_PWM3_DUTY               (0x0C)  //bit0~15
#define REG_PWM4_DUTY               (0x0F)  //bit0~15

#define REG_PWM0_DUTY_EXT           (0x21)  //bit0~1
#define REG_PWM1_DUTY_EXT           (0x21)  //bit2~3
#define REG_PWM2_DUTY_EXT           (0x21)  //bit4~5
#define REG_PWM3_DUTY_EXT           (0x21)  //bit6~7
#define REG_PWM4_DUTY_EXT           (0x21)  //bit8~9

#define REG_PWM0_DIV                (0x04)  //bit0~7
#define REG_PWM1_DIV                (0x07)  //bit0~7
#define REG_PWM2_DIV                (0x0A)  //bit0~7
#define REG_PWM3_DIV                (0x0D)  //bit0~7
#define REG_PWM4_DIV                (0x10)  //bit0~7

#define REG_PWM0_DIV_EXT            (0x22)  //bit0~7
#define REG_PWM1_DIV_EXT            (0x22)  //bit8~15
#define REG_PWM2_DIV_EXT            (0x23)  //bit0~7
#define REG_PWM3_DIV_EXT            (0x23)  //bit8~15
#define REG_PWM4_DIV_EXT            (0x24)  //bit0~7

#define REG_PWM0_PORARITY           (0x04)  //bit8
#define REG_PWM1_PORARITY           (0x07)  //bit8
#define REG_PWM2_PORARITY           (0x0A)  //bit8
#define REG_PWM3_PORARITY           (0x0D)  //bit8
#define REG_PWM4_PORARITY           (0x10)  //bit8

typedef enum _PWM_Result
{
    E_PWM_OK,
    E_PWM_FAIL
} PWM_Result;

typedef enum _PWM_ChNum
{
    E_PWM_CH0,
    E_PWM_CH1,
    E_PWM_CH2,
    E_PWM_CH3,
    E_PWM_CH4,
    E_PWM_CH5,
    E_PWM_CH6,
    E_PWM_CH7,
    E_PWM_CH8,
    E_PWM_CH9
}PWM_ChNum;