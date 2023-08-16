#ifndef __VALVE_H__
#define __VALVE_H__

#define VALVE1_ADDR 0x20
#define VALVE2_ADDR 0x21

enum {
	// A-version registers
	MODE=0, /**< 0x00 Mode register */
	WDTOI, /**< 0x01 WatchDog Time-Out Interval register */
	WDCNTL, /**< 0x02 WatchDog Control register */
	IO_CFG, /**< 0x03 I/O Configuration register */
	INTMODE, /**< 0x04 Interrupt Mode register */
	MSK, /**< 0x05 Mask interrupt register */
	INTSTAT, /**< 0x06 Interrupt Status register */
	IP, /**< 0x07 Input Port register */
	INT_MTR_ACT, /**< 0x08 Interrupt motor action control register */
	EXTRASTEPS0, /**< 0x09 Extra steps count for INTP0 control register */
	EXTRASTEPS1, /**< 0x0A Extra steps count for INTP1 control register */
	OP_CFG_PHS, /**< 0x0B Output Port Configuration and Phase control register */
	OP_STAT_TO, /**< 0x0C Output state and time-out control register */
	RUCNTL, /**< 0x0D Ramp-up control register */
	RDCNTL, /**< 0x0E Ramp-down control register */
	PMA, /**< 0x0F Perform multiple of actions control register */
	LOOPDLY_CW, /**< 0x10 Loop delay timer for CW to CCW control register */
	LOOPDLY_CCW, /**< 0x11 Loop delay timer for CCW to CW control register */
	CWSCOUNTL, /**< 0x12 Number of clockwise steps register (low byte) */
	CWSCOUNTH, /**< 0x13 Number of clockwise steps register (high byte) */
	CCWSCOUNTL, /**< 0x14 Number of counter-clockwise steps register (low byte) */
	CCWSCOUNTH, /**< 0x15 Number of counter-clockwise steps register (high byte) */
	CWPWL, /**< 0x16 Clockwise step pulse width register (low byte) */
	CWPWH, /**< 0x17 Clockwise step pulse width register (high byte) */
	CCWPWL, /**< 0x18 Counter-clockwise step pulse width register (low byte) */
	CCWPWH, /**< 0x19 Counter-clockwise step pulse width register (high byte) */
	MCNTL, /**< 0x1A Motor control register */
	SUBADR1, /**< 0x1B I2C-bus subaddress 1 */
	SUBADR2, /**< 0x1C I2C-bus subaddress 2 */
	SUBADR3, /**< 0x1D I2C-bus subaddress 3 */
	ALLCALLADR, /**< 0x1E All Call I2C-bus address */
	STEPCOUNT0, /**< 0x1F Step counter registers: STEPCOUNT0 */
	STEPCOUNT1, /**< 0x20 Step counter registers: STEPCOUNT1 */
	STEPCOUNT2, /**< 0x21 Step counter registers: STEPCOUNT2 */
	STEPCOUNT3 /**< 0x22 Step counter registers: STEPCOUNT3 */
} PCA9629ARegisters;

void valve_init();
void valve_stop(U8 dev_addr);
void valve_open(U8 dev_addr,U16 num, U16 pps);
void valve_close(U8 dev_addr,U16 num, U16 pps);

U8 valve_get_mcntl(U8 addr);
U32 valve_get_step(U8 dev_addr);

#define VALVE_STEP_MAX 550


#endif//__VALVE_H__


