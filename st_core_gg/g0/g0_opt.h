#ifndef __G0_OPT_H__
#define __G0_OPT_H__


#define G0_REG_UNVALID_ADDR	0xff

int g0_write_u8reg(U8 dev_addr, U8 reg_addr, U8 *data, int len);//寄存器地址为8位的写
int g0_read_u8reg(U8 dev_addr, U8 reg_addr, U8 *data, int len);//寄存器地址为8位的读





extern U32  g0_isp_tick;

#endif // !__G0_OPT_H__
