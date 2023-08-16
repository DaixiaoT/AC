#ifndef  _TIME_RECORD_H_
#define _TIME_RECORD_H_

#define TIME_RECORD_NUM 8
#define TIME_RECORD_MASK (TIME_RECORD_NUM-1)

struct TimeRecord {
    U32 m_time[TIME_RECORD_NUM];
    int m_start;  // 第一条记录在time[]中的位置
    int m_num;    // 记录总条数

	TimeRecord()
	{
		Reset();
	}

    void Reset() {
        m_start = m_num = 0;
    }

    // 增加一条记录，如果满了，删除最老的一条
    void AddRecord(U32 time) {
        int idx = (m_start + m_num) & TIME_RECORD_MASK;
        m_time[idx] = time;
        if(m_num==TIME_RECORD_NUM)
            m_start = (m_start+1) & TIME_RECORD_MASK;
        else
            m_num++;
    }

    // 删除所有超时记录  (记录时间到现在超过ms毫秒)
    void DelTimeOutRecords(int ms) {
        while(m_num>0) {
            if(TimeGap(m_time[m_start])<ms)
                // 第一条未超时，退出
                break;
            m_start = (m_start+1) & TIME_RECORD_MASK;
            m_num--;
        }
    }

    // 最新一条记录到现在的时间<ms毫秒，返回TRUE，否则FALSE
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

