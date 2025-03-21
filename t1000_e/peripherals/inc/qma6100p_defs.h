
#ifndef __QMA6100P_DEFS_H__
#define __QMA6100P_DEFS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QMA6100P_ADDRESS                0x12

#define SLAVE_ADDR  QMA6100P_ADDRESS

#define QMA6100P_DEVICE_ID				0x90

#define QMA6100P_REG_CHIP_ID		    0x00

#define QMA6100P_REG_XOUTL				0x01
#define QMA6100P_REG_XOUTH				0x02
#define QMA6100P_REG_YOUTL				0x03
#define QMA6100P_REG_YOUTH				0x04
#define QMA6100P_REG_ZOUTL				0x05
#define QMA6100P_REG_ZOUTH				0x06

#define QMA6100P_REG_STEP_CNT_L			0x07
#define QMA6100P_REG_STEP_CNT_M			0x08
#define QMA6100P_REG_STEP_CNT_H			0x0d

#define QMA6100P_REG_INT_STATUS_0		0x09
#define QMA6100P_REG_INT_STATUS_1		0x0a
#define QMA6100P_REG_INT_STATUS_2		0x0b
#define QMA6100P_REG_INT_STATUS_3		0x0c

#define QMA6100P_REG_FIFO_STATE			0x0e

#define QMA6100P_REG_RANGE				0x0f

#define QMA6100P_REG_BW_ODR				0x10

#define QMA6100P_REG_POWER_MANAGE		0x11

#define QMA6100P_REG_STEP_SAMPLE_CNT	0x12
#define QMA6100P_REG_STEP_PRECISION		0x13
#define QMA6100P_REG_STEP_TIME_LOW		0x14
#define QMA6100P_REG_STEP_TIME_UP		0x15

#define QMA6100P_REG_INT_EN_0			0x16
#define QMA6100P_REG_INT_EN_1			0x17
#define QMA6100P_REG_INT_EN_2			0x18

#define QMA6100P_REG_INT1_MAP_0			0x19
#define QMA6100P_REG_INT1_MAP_1			0x1a
#define QMA6100P_REG_INT2_MAP_0			0x1b
#define QMA6100P_REG_INT2_MAP_1			0x1c

#define QMA6100P_REG_INTPIN_CFG			0x20

#define QMA6100P_REG_INT_CFG			0x21

#define QMA6100P_REG_OS_CUST_X		    0x27
#define QMA6100P_REG_OS_CUST_Y			0x28
#define QMA6100P_REG_OS_CUST_Z			0x29

#define QMA6100P_REG_NVM				0x33
#define QMA6100P_REG_RESET				0x36


#define QMA6100P_REG_DRDY_BIT			0x10	// enable 1

#define QMA6100P_REG_AMD_X_BIT			0x01
#define QMA6100P_REG_AMD_Y_BIT			0x02
#define QMA6100P_REG_AMD_Z_BIT			0x04

typedef enum
{
	QMA6100P_MAP_INT1,
	QMA6100P_MAP_INT2,
	QMA6100P_MAP_INT_NONE
}qma6100p_int_map;

typedef enum
{
	QMA6100P_BW_100 = 0,
	QMA6100P_BW_200 = 1,
	QMA6100P_BW_400 = 2,
	QMA6100P_BW_800 = 3,
	QMA6100P_BW_1600 = 4,
	QMA6100P_BW_50 = 5,
	QMA6100P_BW_25 = 6,
	QMA6100P_BW_12_5 = 7,
	QMA6100P_BW_OTHER = 8
}qma6100p_bw;

typedef enum
{
	QMA6100P_RANGE_2G = 0x01,
	QMA6100P_RANGE_4G = 0x02,
	QMA6100P_RANGE_8G = 0x04,
	QMA6100P_RANGE_16G = 0x08,
	QMA6100P_RANGE_32G = 0x0f
}qma6100p_range;

typedef enum
{
	QMA6100P_LPF_OFF = (0x00<<5),
	QMA6100P_LPF_1 = (0x04<<5),
	QMA6100P_LPF_2 = (0x01<<5),
	QMA6100P_LPF_4 = (0x02<<5),
	QMA6100P_LPF_8 = (0x03<<5),
	QMA6100P_LPF_RESERVED = 0xff
}qma6100p_nlpf;

typedef enum
{
	QMA6100P_HPF_DIV_OFF = (0x00<<5),
	QMA6100P_HPF_DIV_10 = (0x01<<5),
	QMA6100P_HPF_DIV_25 = (0x02<<5),
	QMA6100P_HPF_DIV_50 = (0x03<<5),
	QMA6100P_HPF_DIV_100 = (0x04<<5),
	QMA6100P_HPF_DIV_200 = (0x05<<5),
	QMA6100P_HPF_DIV_400 = (0x06<<5),
	QMA6100P_HPF_DIV_800 = (0x07<<5),
	QMA6100P_HPF_RESERVED = 0xff
}qma6100p_nhpf;

typedef enum
{
	QMA6100P_MODE_STANDBY = 0,
	QMA6100P_MODE_ACTIVE = 1,
	QMA6100P_MODE_MAX
}qma6100p_mode;

typedef enum
{
	QMA6100P_MCLK_102_4K = 0x03,
	QMA6100P_MCLK_51_2K = 0x04,
	QMA6100P_MCLK_25_6K = 0x05,
	QMA6100P_MCLK_12_8K = 0x06,
	QMA6100P_MCLK_6_4K = 0x07,
	QMA6100P_MCLK_RESERVED = 0xff
}qma6100p_mclk;

typedef enum
{
	QMA6100P_STEP_LPF_0 = (0x00<<6),
	QMA6100P_STEP_LPF_2 = (0x01<<6),
	QMA6100P_STEP_LPF_4 = (0x02<<6),
	QMA6100P_STEP_LPF_8 = (0x03<<6),
	QMA6100P_STEP_LPF_RESERVED = 0xff
}qma6100p_step_lpf;

typedef enum
{
	QMA6100P_STEP_AXIS_ALL = 0x00,
	QMA6100P_STEP_AXIS_YZ = 0x01,
	QMA6100P_STEP_AXIS_XZ = 0x02,
	QMA6100P_STEP_AXIS_XY = 0x03,
	QMA6100P_STEP_AXIS_RESERVED = 0xff
}qma6100p_step_axis;

typedef enum
{
	QMA6100P_STEP_START_0 = 0x00,
	QMA6100P_STEP_START_4 = 0x20,
	QMA6100P_STEP_START_8 = 0x40,
	QMA6100P_STEP_START_12 = 0x60,
	QMA6100P_STEP_START_16 = 0x80,
	QMA6100P_STEP_START_24 = 0xa0,
	QMA6100P_STEP_START_32 = 0xc0,
	QMA6100P_STEP_START_40 = 0xe0,
	QMA6100P_STEP_START_RESERVED = 0xff
}qma6100p_step_start_cnt;

typedef enum
{
	QMA6100P_FIFO_MODE_NONE,
	QMA6100P_FIFO_MODE_FIFO,
	QMA6100P_FIFO_MODE_STREAM,
	QMA6100P_FIFO_MODE_BYPASS,
	QMA6100P_FIFO_MODE_MAX
}qma6100p_fifo_mode;

typedef enum
{
	QMA6100P_TAP_SINGLE = 0x80,
	QMA6100P_TAP_DOUBLE = 0x20,
	QMA6100P_TAP_TRIPLE = 0x10,
	QMA6100P_TAP_QUARTER = 0x01,
	QMA6100P_TAP_MAX = 0xff
}qma6100p_tap;

typedef enum
{
	QMA6100P_SENSITITY_2G = 244,
	QMA6100P_SENSITITY_4G = 488,
	QMA6100P_SENSITITY_8G = 977,
	QMA6100P_SENSITITY_16G = 1950,
	QMA6100P_SENSITITY_32G = 3910
}qma6100p_sensitity;

#ifdef __cplusplus
}
#endif

#endif
