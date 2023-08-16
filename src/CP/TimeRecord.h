#ifndef  _TIME_RECORD_H_
#define _TIME_RECORD_H_

#define TIME_RECORD_NUM 8
#define TIME_RECORD_MASK (TIME_RECORD_NUM-1)

struct TimeRecord {
    U32 m_time[TIME_RECORD_NUM];
    int m_start;  // ��һ����¼��time[]�е�λ��
    int m_num;    // ��¼������

	TimeRecord()
	{
		Reset();
	}

    void Reset() {
        m_start = m_num = 0;
    }

    // ����һ����¼��������ˣ�ɾ�����ϵ�һ��
    void AddRecord(U32 time) {
        int idx = (m_start + m_num) & TIME_RECORD_MASK;
        m_time[idx] = time;
        if(m_num==TIME_RECORD_NUM)
            m_start = (m_start+1) & TIME_RECORD_MASK;
        else
            m_num++;
    }

    // ɾ�����г�ʱ��¼  (��¼ʱ�䵽���ڳ���ms����)
    void DelTimeOutRecords(int ms) {
        while(m_num>0) {
            if(TimeGap(m_time[m_start])<ms)
                // ��һ��δ��ʱ���˳�
                break;
            m_start = (m_start+1) & TIME_RECORD_MASK;
            m_num--;
        }
    }

    // ����һ����¼�����ڵ�ʱ��<ms���룬����TRUE������FALSE
    BOOL LastRecordIsFresh(int ms) {
        if(m_num==0)
            return FALSE;
        int idx = (m_start + m_num - 1) & TIME_RECORD_MASK;
        return TimeGap(m_time[idx])<ms;
    }
	U32 LastRecordTimeGrap() {
		if (m_num == 0)
			return 0;
		int idx = (m_start + m_num - 1) & TIME_RECORD_MASK;
		return TimeGap(m_time[idx]);

	}
};

#endif //_TIME_RECORD_H_

