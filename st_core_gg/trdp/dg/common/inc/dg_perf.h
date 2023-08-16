/* ==========================================================================
 *
 *  File      : DG_PERF.H
 *
 *  Purpose   : Performance Measurement for RPC Calls
 *
 *  Licence   : Duagon Software Licence (see file 'licence.txt')
 *
 * --------------------------------------------------------------------------
 *
 *  (C) COPYRIGHT, Duagon AG, CH-8953 Dietikon, Switzerland
 *  All Rights Reserved.
 *
 * ==========================================================================
 */

#ifndef DG_PERF_H
#define DG_PERF_H
#include "os_def.h"

#define DG_PERF_MAX_MEASUREMENTS 256

#ifndef DG_MULTI_CARD

extern
void
dg_perf_init
(
        void
);

extern
void
dg_perf_print_statistics
(
        void
);

extern
DG_BOOL
dg_perf_print_statistic_line
(
        DG_CHAR8 *Line,
        DG_CHAR8 length,
        DG_S32 *proto_index,
        DG_S32 *meas_index
);
#endif

extern
void
dg_perf_init_nbr
(
        DG_U8 card_number
);

extern
void
dg_perf_print_statistics_nbr
(
        DG_U8 card_number
);

extern
DG_BOOL
dg_perf_print_statistic_line_nbr
(
        DG_CHAR8 *Line,
        DG_CHAR8 length,
        DG_S32 *proto_index,
        DG_S32 *meas_index,
        DG_U8 card_number
);

typedef DG_BOOL (*PERF_PRINT_FN)(
        DG_CHAR8 *Line,
        DG_CHAR8 length,
        DG_S32 *proto_index,
        DG_S32 *meas_index
);


typedef void (*PerfFunc)(DG_U8 channel, DG_U8 card_index);
typedef DG_U8 (*PerfFuncU8)(DG_U8 channel, DG_U8 card_index);

/* workaround for MVB MDFULL performance measurements */
#define MVB_PROTOCOL_ID         0x10
#define MVB_PROTOCOL_CHANNEL    0
#define CMD_MVB_PD_GET          0x01
#define CMD_MVB_PD_PUT          0x02


typedef struct dg_perf_measurement
{
    DG_U8   protocol;
    DG_U8   command;
    DG_U64  curr_start;
    DG_U64  avg_time;
    DG_U64  max_time;
    DG_U64  min_time;
    DG_U32  nb_calls;
#ifdef MEASURE_DETAILED
    DG_U64  get_mutex_avg;
    DG_U64  get_mutex_min;
    DG_U64  get_mutex_max;
    DG_U64  send_start;
    DG_U64  send_done;
    DG_U64  send_avg;
    DG_U64  send_min;
    DG_U64  send_max;
    DG_U64  recv_h_done;
    DG_U64  recv_h_avg;
    DG_U64  recv_h_min;
    DG_U64  recv_h_max;
    DG_U64  recv_p_done;
    DG_U64  recv_p_avg;
    DG_U64  recv_p_min;
    DG_U64  recv_p_max;
#ifdef MEASURE_BYTE_ACCESS
    DG_U64  write_byte_avg_min;
    DG_U64  write_byte_avg_max;
    DG_U32  write_byte_cnt_max;
    DG_U64  write_byte_min;
    DG_U64  write_byte_max;
    DG_U64  read_byte_avg_min;
    DG_U64  read_byte_avg_max;
    DG_U32  read_byte_cnt_max;
    DG_U64  read_byte_min;
    DG_U64  read_byte_max;
    DG_U64  read_status_avg_min;
    DG_U64  read_status_avg_max;
    DG_U32  read_status_cnt_max;
    DG_U64  read_status_min;
    DG_U64  read_status_max;
#endif
    DG_U32 cnt_bucket;
    DG_U32 bucket_latency[20];
    DG_U64  bucket_min;
    DG_U64  bucket_max;
    DG_U64  bucket_delta;
#endif
} DG_PERF_MEASUREMENT_DEF;

#ifdef MEASURE_BYTE_ACCESS
typedef struct {
    DG_U64  access_byte_start[DG_NR_OF_CHANNELS];
    DG_U64  write_byte_sum[DG_NR_OF_CHANNELS];
    DG_U64  write_byte_min[DG_NR_OF_CHANNELS];
    DG_U64  write_byte_max[DG_NR_OF_CHANNELS];
    DG_U64  write_byte_cnt[DG_NR_OF_CHANNELS];
    DG_U64  read_byte_sum[DG_NR_OF_CHANNELS];
    DG_U64  read_byte_min[DG_NR_OF_CHANNELS];
    DG_U64  read_byte_max[DG_NR_OF_CHANNELS];
    DG_U64  read_byte_cnt[DG_NR_OF_CHANNELS];
    DG_U64  read_status_sum[DG_NR_OF_CHANNELS];
    DG_U64  read_status_min[DG_NR_OF_CHANNELS];
    DG_U64  read_status_max[DG_NR_OF_CHANNELS];
    DG_U64  read_status_cnt[DG_NR_OF_CHANNELS];
} BYTE_ACCESS_MEASUREMENTS;
#endif


#endif /* DG_PERF_H */
