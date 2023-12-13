#ifndef __AC_FILTER_H__
#define __AC_FILTER_H__
#include "ac_include.h"
/*
数字滤波器
*/
enum FILTAI_MODE {
	FILTAI_AVG = 0,
	FILTAI_MID = 1,
};
#define AD_FILT_BUF_MAX 16
class FILT_AIRT
{
public:
	FILT_AIRT() {
		init(1, FILTAI_MID);
	}
	void init(U8 filt_num, FILTAI_MODE mode) {
		if ((filt_num == 0) || (filt_num > AD_FILT_BUF_MAX)) {
			LOG_PRINT("\nFILTER Num Error!\n");
			filt_num = AD_FILT_BUF_MAX;
		}
		m_filt_num = filt_num;
		m_mode = mode;
		m_ok = 0;


	}
	S16 get_filter_val(S16 data) {
		if (m_filt_num == 1) {
			return data;
		}
		if (m_ok) {
			for (int i = 0; i < m_filt_num; i++) {
				m_buf[i] = data;
			}
			m_ok = 1;
			m_idx = 1;
			return data;
		}
		m_buf[m_idx] = data;
		m_idx++;
		if (m_idx >= m_filt_num) {
			m_idx = 0;
		}
		if (m_mode == FILTAI_AVG) {//计算缓冲区中所有数据的和，并返回平均值作为输出
			U32 sum = m_buf[0];
			for (int i = 1; i < m_filt_num; i++) {
				sum += m_buf[i];
			}
			return sum / m_filt_num;
		}
		else {//先排序，再输出中值
			S16 buf[AD_FILT_BUF_MAX];
			int i, j;

			for (i = 0; i < m_filt_num; i++)
				buf[i] = m_buf[i];

			for (i = 0; i < m_filt_num - 1; i++) {
				for (j = i + 1; j < m_filt_num; j++) {
					if (buf[j] > buf[i]) {
						int t = buf[i];
						buf[i] = buf[j];
						buf[j] = t;
					}
				}
			}

			return (buf[m_filt_num / 2 - 1] + buf[m_filt_num / 2]) / 2;
		}

	}

private:
	S16 m_buf[AD_FILT_BUF_MAX];
	FILTAI_MODE m_mode;
	U8	m_filt_num;
	int m_ok;
	int m_idx;
};





#endif // !__AC_FILTER_H__
