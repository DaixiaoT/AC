/* this file comes from lwIP */

#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))

static U16 ip_standard_chksum(void *dataptr, int len)
{
  U8 *pb = (U8 *)dataptr;
  U16 *ps, t = 0;
  U32 *pl;
  U32 sum = 0, tmp;
  /* starts at odd byte address? */
  int odd = ((U32)pb & 1);

  if (odd && len > 0) {
    ((U8 *)&t)[1] = *pb++;
    len--;
  }

  ps = (U16 *)pb;

  if (((U32)ps & 3) && len > 1) {
    sum += *ps++;
    len -= 2;
  }

  pl = (U32 *)ps;

  while (len > 7)  {
    tmp = sum + *pl++;          /* ping */
    if (tmp < sum) {
      tmp++;                    /* add back carry */
    }

    sum = tmp + *pl++;          /* pong */
    if (sum < tmp) {
      sum++;                    /* add back carry */
    }

    len -= 8;
  }

  /* make room in upper bits */
  sum = FOLD_U32T(sum);

  ps = (U16 *)pl;

  /* 16-bit aligned word remaining? */
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* dangling tail byte remaining? */
  if (len > 0) {                /* include odd byte */
    ((U8 *)&t)[0] = *(U8 *)ps;
  }

  sum += t;                     /* add end bytes */

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  if (odd) {
    sum = my_swap16(sum);
  }

  return (U16)sum;
}

U16 inet_chksum_pseudo(packet_t *pk, ip_addr_t *src, ip_addr_t *dest, U8 proto, U16 proto_len)
{
    U32 acc = 0;
    U8 swapped = 0;

    int total_len = pk->total_len;

    int data_len = my_min(total_len, pk->data_len);
    acc += ip_standard_chksum(packet_data(pk), data_len);
    acc = FOLD_U32T(acc);
    if (data_len & 1) {
      swapped = 1 - swapped;
      acc = my_swap16(acc);
    }
    total_len -= data_len;

    int i=0;
    while(total_len>0 && i<PACKET_EXT_BUF_NUM) {
        mi_assert(pk->ext_buf[i]);
        mi_assert(pk->ext_buf_len[i]>0);
        data_len = my_min(total_len, pk->ext_buf_len[i]);
        acc += ip_standard_chksum(pk->ext_buf[i], data_len);
        acc = FOLD_U32T(acc);
        if (data_len & 1) {
          swapped = 1 - swapped;
          acc = my_swap16(acc);
        }
        total_len -= data_len;
        i++;
    }

    if (swapped)
        acc = my_swap16(acc);

    acc += (src->addr & 0xffffUL);
    acc += ((src->addr >> 16) & 0xffffUL);
    acc += (dest->addr & 0xffffUL);
    acc += ((dest->addr >> 16) & 0xffffUL);
    acc += htons((U16)proto);
    acc += htons(proto_len);

    /* Fold 32-bit sum to 16 bits
    calling this twice is propably faster than if statements... */
    acc = FOLD_U32T(acc);
    acc = FOLD_U32T(acc);
    return (U16)~(acc & 0xffffUL);
}

