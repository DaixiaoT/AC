#ifndef __G0_OPT_H__
#define __G0_OPT_H__


#define G0_REG_UNVALID_ADDR	0xff

int g0_write_u8reg(U8 dev_addr, U8 reg_addr, U8 *data, int len);//�Ĵ�����ַΪ8λ��д
int g0_read_u8reg(U8 dev_addr, U8 reg_addr, U8 *data, int len);//�Ĵ�����ַΪ8λ�Ķ�





extern U32  g0_isp_tick;

#endif // !__G0_OPT_H__
