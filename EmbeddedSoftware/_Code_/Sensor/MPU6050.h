#include "BoardConfig.h"

#define MPU6050_ADDRESS_AD0_LOW     0xD0 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0xD2// address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////REGISTER MAPPING																									
//																																																	
typedef enum
{	
	MPU6050_SELF_TEST_X					= 0x0D,
	MPU6050_SELF_TEST_Y					= 0x0E,
	MPU6050_SELF_TEST_Z					=	0x0F,					
	MPU6050_SELF_TEST_A					= 0x10,
	
  MPU6050_SMPLRT_DIV					=	0x19,	
  MPU6050_CONFIG							=	0x1A,
  MPU6050_GYRO_CONFIG					=	0x1B,
  MPU6050_ACCEL_CONFIG				=	0x1C,
  MPU6050_FF_THR							=	0x1D,
  MPU6050_FF_DUR							=	0x1E,
  MPU6050_MOT_THR							=	0x1F,
  MPU6050_MOT_DUR							=	0x20,
  MPU6050_ZRMOT_THR						=	0x21,
  MPU6050_ZRMOT_DUR						=	0x22,
  MPU6050_FIFO_EN							=	0x23,
  MPU6050_I2C_MST_CTRL				=	0x24,
  MPU6050_I2C_SLV0_ADDR				=	0x25,
  MPU6050_I2C_SLV0_REG				=	0x26,
  MPU6050_I2C_SLV0_CTRL				=	0x27,
  MPU6050_I2C_SLV1_ADDR				=	0x28,
  MPU6050_I2C_SLV1_REG				=	0x29,
  MPU6050_I2C_SLV1_CTRL				=	0x2A,
  MPU6050_I2C_SLV2_ADDR				=	0x2B,
  MPU6050_I2C_SLV2_REG				=	0x2C,
  MPU6050_I2C_SLV2_CTRL				=	0x2D,
  MPU6050_I2C_SLV3_ADDR				=	0x2E,
  MPU6050_I2C_SLV3_REG				=	0x2F,
  MPU6050_I2C_SLV3_CTRL				=	0x30,
  MPU6050_I2C_SLV4_ADDR				=	0x31,
  MPU6050_I2C_SLV4_REG				=	0x32,
  MPU6050_I2C_SLV4_DO					=	0x33,
  MPU6050_I2C_SLV4_CTRL				=	0x34,
  MPU6050_I2C_SLV4_DI					=	0x35,
  MPU6050_I2C_MST_STATUS			=	0x36,
  MPU6050_INT_PIN_CFG					=	0x37,
  MPU6050_INT_ENABLE					=	0x38,
  MPU6050_DMP_INT_STATUS			=	0x39,
  MPU6050_INT_STATUS					=	0x3A,
  MPU6050_ACCEL_XOUT_H				=	0x3B,
  MPU6050_ACCEL_XOUT_L				=	0x3C,
  MPU6050_ACCEL_YOUT_H				=	0x3D,
  MPU6050_ACCEL_YOUT_L				=	0x3E,
  MPU6050_ACCEL_ZOUT_H				=	0x3F,
  MPU6050_ACCEL_ZOUT_L				=	0x40,
  MPU6050_TEMP_OUT_H					=	0x41,
  MPU6050_TEMP_OUT_L					=	0x42,
  MPU6050_GYRO_XOUT_H					=	0x43,
  MPU6050_GYRO_XOUT_L					=	0x44,
  MPU6050_GYRO_YOUT_H					=	0x45,
  MPU6050_GYRO_YOUT_L					=	0x46,
  MPU6050_GYRO_ZOUT_H					=	0x47,
  MPU6050_GYRO_ZOUT_L					=	0x48,
  MPU6050_EXT_SENS_DATA_00		=	0x49,
  MPU6050_EXT_SENS_DATA_01		=	0x4A,
  MPU6050_EXT_SENS_DATA_02		=	0x4B,
  MPU6050_EXT_SENS_DATA_03		=	0x4C,
  MPU6050_EXT_SENS_DATA_04		=	0x4D,
  MPU6050_EXT_SENS_DATA_05		=	0x4E,
  MPU6050_EXT_SENS_DATA_06		=	0x4F,
  MPU6050_EXT_SENS_DATA_07		=	0x50,
  MPU6050_EXT_SENS_DATA_08		=	0x51,
  MPU6050_EXT_SENS_DATA_09		=	0x52,
  MPU6050_EXT_SENS_DATA_10		=	0x53,
  MPU6050_EXT_SENS_DATA_11		=	0x54,
  MPU6050_EXT_SENS_DATA_12		=	0x55,
  MPU6050_EXT_SENS_DATA_13		=	0x56,
  MPU6050_EXT_SENS_DATA_14 		=	0x57,
  MPU6050_EXT_SENS_DATA_15 		=	0x58,
  MPU6050_EXT_SENS_DATA_16 		=	0x59,
  MPU6050_EXT_SENS_DATA_17		=	0x5A,
  MPU6050_EXT_SENS_DATA_18		=	0x5B,
  MPU6050_EXT_SENS_DATA_19		=	0x5C,
  MPU6050_EXT_SENS_DATA_20		=	0x5D,
  MPU6050_EXT_SENS_DATA_21		=	0x5E,
  MPU6050_EXT_SENS_DATA_22		=	0x5F,
  MPU6050_EXT_SENS_DATA_23		=	0x60,
  MPU6050_MOT_DETECT_STATUS  	=	0x61,
  MPU6050_I2C_SLV0_DO      		=	0x63,
  MPU6050_I2C_SLV1_DO   			=	0x64,
  MPU6050_I2C_SLV2_DO   			=	0x65,
  MPU6050_I2C_SLV3_DO     		= 0x66,
  MPU6050_I2C_MST_DELAY_CTRL	= 0x67,
  MPU6050_SIGNAL_PATH_RESET		= 0x68,
  MPU6050_MOT_DETECT_CTRL			= 0x69,
  MPU6050_USER_CTRL						= 0x6A,
  MPU6050_PWR_MGMT_1     			= 0x6B,
  MPU6050_PWR_MGMT_2     			= 0x6C,
  MPU6050_BANK_SEL       			= 0x6D,
  MPU6050_MEM_START_ADDR  		= 0x6E,
  MPU6050_MEM_R_W         		= 0x6F,
  MPU6050_DMP_CFG_1        		=	0x70,
  MPU6050_DMP_CFG_2        		=	0x71,
  MPU6050_FIFO_COUNTH					=	0x72,
  MPU6050_FIFO_COUNTL					=	0x73,
  MPU6050_FIFO_R_W						=	0x74,
	MPU6050_WHO_AM_I						=	0x75,
	MPU6050_Debug								=	0x75,
}MPU6050_Register;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////CONFIG	
//
#define MPU6050_DLPF_CFG_Position			0	
#define MPU6050_DLPF_CFG_Clear				0XF8 // 3 bits
typedef enum									//	ACC (1kHz	)						//		GYRO														//
{															//	ACC BW	// ACC DELAY	//	GYR BW	//	GYR DELAY // 	GYRO Fs	//
	DLPF_CFG_0  			=0x00,		//	260			//	0					//		256		//	0.98			//		8			//
	DLPF_CFG_1  			=0x01,		//	184			//	2.0				//		188		//	1.9				//		1			//
	DLPF_CFG_2  			=0x02,		//	94			//	3.O				//		98 		//	2.8				//		1			//
	DLPF_CFG_3  			=0x03,		//	44			//	4.9				//		42		//	4.8 			//		1			//
	DLPF_CFG_4  			=0x04,		//	21			//	8.5				//		20		//	8.3				//		1			//
	DLPF_CFG_5  			=0x05,		//	10			//	13.8			//		10		//	13.4			//		1			//
	DLPF_CFG_6  			=0x06,		//	5				//	19.0			//		5			//	18.6			//		1			//
	DLPF_CFG_7  			=0x07,		//				REVERSE					//				REVERSE					//		8			//
	DLPF_CFG_DontCare  = 0x1FF
}MPU6050_DLPF_CFG;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////GYRO_CONFIG	
//
#define MPU6050_GYRO_FS_Position			3	
#define MPU6050_GYRO_FS_Clear					0XE7	// 2 bits	
typedef enum//MPU6050_GYRO_FS					//Full Scale Range//LSB Sensitivity//
{																			//	deg/s					// LSB/deg/s		 //
	MPU6050_GYRO_FS_250        = 0x00,	//	250						//  131					 //
	MPU6050_GYRO_FS_500        = 0x01,	//	500						//	65.5				 //
	MPU6050_GYRO_FS_1000       = 0x02,	//	1000					//  32.8				 //
	MPU6050_GYRO_FS_2000       = 0x03,	//	2000					//	16.4				 //
	MPU6050_GYRO_FS_DontCare 	 = 0x1FF	//
} MPU6050_GYRO_FS;				///degree/second

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////ACCEL_CONFIG
//
#define MPU6050_ACCEL_FS_Position			3	
#define MPU6050_ACCEL_FS_Clear				0XE7	// 2 bits
typedef enum//MPU6050_ACCEL_FS				//Full Scale Range//LSB Sensitivity//
{																			//	g							// LSB/g				 //
	MPU6050_ACCEL_FS_2         = 0x00,	//	2							//  16384				 //
	MPU6050_ACCEL_FS_4         = 0x01,	//	4							//  8192				 //
	MPU6050_ACCEL_FS_8         = 0x02,	//	8							//  4096				 //
	MPU6050_ACCEL_FS_16        = 0x03,	//	16						//  2048				 //
	MPU6050_ACCEL_FS_DontCare  = 0x1FF	
} MPU6050_ACCEL_FS;									//g


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////PWR_MGMT_1
//
#define MPU6050_SLEEP_Position				6	
#define MPU6050_SLEEP_Clear						0xBF // 1 bit	
typedef enum//MPU6050_CLOCK
{
	MPU6050_SLEEP_DISABLE		 = 0x00,
	MPU6050_SLEEP_ENABLE		 = 0x01,
	MPU6050_SLEEP_DontCare   = 0x1FF
} MPU6050_SLEEP;

#define MPU6050_TEMPERATURE_Position 	3	
#define MPU6050_TEMPERATURE_Clear		 	0x7F	 // 1 bit
typedef enum//MPU6050_TEMPERATURE
{
	MPU6050_TEMPERATURE_ENABLE		 = 0x00,
	MPU6050_TEMPERATURE_DISABLE		 = 0x01,
	MPU6050_TEMPERATURE_DontCare   = 0x1FF
} MPU6050_TEMPERATURE;

#define MPU6050_CLOCK_Position				0	
#define MPU6050_CLOCK_Clear						0xF8	// 3 bit
typedef enum//MPU6050_CLOCK
{
	MPU6050_CLOCK_INTERNAL         = 0x00,
	MPU6050_CLOCK_PLL_XGYRO        = 0x01,
	MPU6050_CLOCK_PLL_YGYRO        = 0x02,
	MPU6050_CLOCK_PLL_ZGYRO        = 0x03,
	MPU6050_CLOCK_PLL_EXT32K       = 0x04,
	MPU6050_CLOCK_PLL_EXT19M       = 0x05,
	MPU6050_CLOCK_KEEP_RESET       = 0x07,
	MPU6050_CLOCK_DontCare  			 = 0x1FF
} MPU6050_CLOCK;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////UNDONE
//
/*
#define MPU6050_TC_PWR_MODE_BIT     7
#define MPU6050_TC_OFFSET_BIT       6
#define MPU6050_TC_OFFSET_LENGTH    6
#define MPU6050_TC_OTP_BNK_VLD_BIT  0

#define MPU6050_VDDIO_LEVEL_VLOGIC  0
#define MPU6050_VDDIO_LEVEL_VDD     1

#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#define MPU6050_EXT_SYNC_DISABLED       0x0
#define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7


#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2



#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

#define MPU6050_MULT_MST_EN_BIT     7
#define MPU6050_WAIT_FOR_ES_BIT     6
#define MPU6050_SLV_3_FIFO_EN_BIT   5
#define MPU6050_I2C_MST_P_NSR_BIT   4
#define MPU6050_I2C_MST_CLK_BIT     3
#define MPU6050_I2C_MST_CLK_LENGTH  4

#define MPU6050_I2C_SLV_RW_BIT      7
#define MPU6050_I2C_SLV_ADDR_BIT    6
#define MPU6050_I2C_SLV_ADDR_LENGTH 7
#define MPU6050_I2C_SLV_EN_BIT      7
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
#define MPU6050_I2C_SLV_REG_DIS_BIT 5
#define MPU6050_I2C_SLV_GRP_BIT     4
#define MPU6050_I2C_SLV_LEN_BIT     3
#define MPU6050_I2C_SLV_LEN_LENGTH  4

#define MPU6050_I2C_SLV4_RW_BIT         7
#define MPU6050_I2C_SLV4_ADDR_BIT       6
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
#define MPU6050_I2C_SLV4_EN_BIT         7
#define MPU6050_I2C_SLV4_INT_EN_BIT     6
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU6050_MST_PASS_THROUGH_BIT    7
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
#define MPU6050_MST_I2C_LOST_ARB_BIT    5
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

#define MPU6050_INTMODE_ACTIVEHIGH  0x00
#define MPU6050_INTMODE_ACTIVELOW   0x01

#define MPU6050_INTDRV_PUSHPULL     0x00
#define MPU6050_INTDRV_OPENDRAIN    0x01

#define MPU6050_INTLATCH_50USPULSE  0x00
#define MPU6050_INTLATCH_WAITCLEAR  0x01

#define MPU6050_INTCLEAR_STATUSREAD 0x00
#define MPU6050_INTCLEAR_ANYREAD    0x01

#define MPU6050_INTERRUPT_FF_BIT            7
#define MPU6050_INTERRUPT_MOT_BIT           6
#define MPU6050_INTERRUPT_ZMOT_BIT          5
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU6050_INTERRUPT_DMP_INT_BIT       1
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0


#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

#define MPU6050_MOTION_MOT_XNEG_BIT     7
#define MPU6050_MOTION_MOT_XPOS_BIT     6
#define MPU6050_MOTION_MOT_YNEG_BIT     5
#define MPU6050_MOTION_MOT_YPOS_BIT     4
#define MPU6050_MOTION_MOT_ZNEG_BIT     3
#define MPU6050_MOTION_MOT_ZPOS_BIT     2
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define MPU6050_DETECT_FF_COUNT_BIT             3
#define MPU6050_DETECT_FF_COUNT_LENGTH          2
#define MPU6050_DETECT_MOT_COUNT_BIT            1
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

#define MPU6050_DETECT_DECREMENT_RESET  0x0
#define MPU6050_DETECT_DECREMENT_1      0x1
#define MPU6050_DETECT_DECREMENT_2      0x2
#define MPU6050_DETECT_DECREMENT_4      0x3

#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0


#define MPU6050_WAKE_FREQ_1P25      0x0
#define MPU6050_WAKE_FREQ_2P5       0x1
#define MPU6050_WAKE_FREQ_5         0x2
#define MPU6050_WAKE_FREQ_10        0x3

#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU6050_BANKSEL_MEM_SEL_BIT         4
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#define MPU6050_DMP_MEMORY_BANKS        8
#define MPU6050_DMP_MEMORY_BANK_SIZE    256
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16
*/
#define ACC_SCALE_FACTOR    32767 //+-2g
#define ACC_SENSITIVITY     2
//#define GYRO_SENSITIVITY    18.45f//+-2000
#define GYRO_SENSITIVITY    16.4f//+-2000
//#define GYRO_SENSITIVITY    24.6f//+-2000
//#define GYRO_SENSITIVITY    32.8f//+-2000
//#define GYRO_SENSITIVITY    131.0f//+-250
typedef enum 
{
    MPU6050_STATUS_OK 								= 0,
    MPU6050_STATUS_BUSY		 						= 1,
    MPU6050_STATUS_REQ_FAIL						= 2,
    MPU6050_STATUS_FAIL		 						= 3,
}MPU6050_Status;

typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}Gyro;

typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}Acce;

MPU6050_Status MPU6050_GetDeviceID(uint8_t *id);
MPU6050_Status MPU6050_ConnectionCheck(void);

MPU6050_Status MPU6050_Set_SMPLRT_DIV(uint8_t  div);
MPU6050_Status MPU6050_Set_Configuration(MPU6050_DLPF_CFG  DLPF_CFG);
MPU6050_Status MPU6050_Set_GYRO_Config (MPU6050_GYRO_FS  GYRO_FS);
MPU6050_Status MPU6050_Set_ACCEL_Config (MPU6050_ACCEL_FS  ACCEL_FS);
MPU6050_Status MPU6050_Set_PWR_MGMT_1 ( MPU6050_SLEEP SLEEP, MPU6050_TEMPERATURE  TEMPERATURE, MPU6050_CLOCK CLOCK );

MPU6050_Status MPU6050_Get_rawDATA(int16_t *Gx,int16_t *Gy,int16_t *Gz, int16_t *Ax,int16_t *Ay, int16_t *Az, float *Temp) ;
MPU6050_Status MPU6050_Get_rawMotion(int16_t *Gx,int16_t *Gy,int16_t *Gz, int16_t *Ax,int16_t *Ay,int16_t *Az);

