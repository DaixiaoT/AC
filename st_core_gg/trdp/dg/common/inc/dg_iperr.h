/* ==========================================================================
 *
 *  File      : DG_IPERR.H
 *
 *  Purpose   : Error Codes for  IP_XXX calls
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

#ifndef DG_IPERR_H
#define DG_IPERR_H


/*----------------------------------------------------------------------------
 * following error Codes are may be set after unsuccessful call to an ip_xx func
 * All error codes are set in errno
 */

#define IP_ENOERR           0     /* No error */
#define IP_EPERM            1     /* Not permitted */
#define IP_ENOENT           2     /* No such entity */
#define IP_ESRCH            3     /* No such process */
#define IP_EINTR            4     /* Operation interrupted */
#define IP_EIO              5     /* I/O error */
#define IP_EBADF            9     /* Bad file handle */
#define IP_EAGAIN           11    /* Try again later */
#define IP_EWOULDBLOCK      IP_EAGAIN
#define IP_ENOMEM           12    /* Out of memory */
#define IP_EBUSY            16    /* Resource busy */
#define IP_EXDEV            18    /* Cross-device link */
#define IP_ENODEV           19    /* No such device */
#define IP_ENOTDIR          20    /* Not a directory */
#define IP_EISDIR           21    /* Is a directory */
#define IP_EINVAL           22    /* Invalid argument */
#define IP_ENFILE           23    /* Too many open files in system */
#define IP_EMFILE           24    /* Too many open files */
#define IP_EFBIG            27    /* File too large */
#define IP_ENOSPC           28    /* No space left on device */
#define IP_ESPIPE           29    /* Illegal seek */
#define IP_EROFS            30    /* Read-only file system */
#define IP_EDOM             33    /* Argument to math function outside valid */
                               /* domain */
#define IP_ERANGE           34    /* Math result cannot be represented */
#define IP_EDEADLK          35    /* Resource deadlock would occur */
#define IP_EDEADLOCK        IP_EDEADLK
#define IP_ENOSYS           38    /* Function not implemented */
#define IP_ENAMETOOLONG     60    /* File name too long */
#define IP_ENOTEMPTY        66    /* Directory not empty */
#define IP_ENOTSUP          95    /* Not supported error */
#define IP_EEOF             200   /* End of file reached */
#define IP_ENOSUPP          201   /* Operation not supported */
#define IP_EDEVNOSUPP       202   /* Device does not support this operation */

/* Additional errors used by networking */
#define IP_ENXIO            300   /* Device not configured */
#define IP_EACCES           301   /* Permission denied */
#define IP_EEXIST           302   /* File exists */
#define IP_ENOTTY           303   /* Inappropriate ioctl for device */
#define IP_EPIPE            304   /* Broken pipe */

/* non-blocking and interrupt i/o */
#define IP_EINPROGRESS      310   /* Operation now in progress */
#define IP_EALREADY         311   /* Operation already in progress */

/* ipc/network software -- argument errors */
#define IP_ENOTSOCK         320   /* Socket operation on non-socket */
#define IP_EDESTADDRREQ     321   /* Destination address required */
#define IP_EMSGSIZE         322   /* Message too long */
#define IP_EPROTOTYPE       323   /* Protocol wrong type for socket */
#define IP_ENOPROTOOPT      324   /* Protocol not available */
#define IP_EPROTONOSUPPORT  325   /* Protocol not supported */
#define IP_ESOCKTNOSUPPORT  326   /* Socket type not supported */
#define IP_EOPNOTSUPP       327   /* Operation not supported */
#define IP_EPFNOSUPPORT     328   /* Protocol family not supported */
#define IP_EAFNOSUPPORT     329   /* Address family not supported by */
                               /* protocol family */
#define IP_EADDRINUSE       330   /* Address already in use */
#define IP_EADDRNOTAVAIL    331   /* Can't assign requested address */
#define IP_EDATASIZE        332   /* Data size > 64k */

/* ipc/network software -- operational errors */
#define IP_ENETDOWN         350   /* Network is down */
#define IP_ENETUNREACH      351   /* Network is unreachable */
#define IP_ENETRESET        352   /* Network dropped connection on reset */
#define IP_ECONNABORTED     353   /* Software caused connection abort */
#define IP_ECONNRESET       354   /* Connection reset by peer */
#define IP_ENOBUFS          355   /* No buffer space available */
#define IP_EISCONN          356   /* Socket is already connected */
#define IP_ENOTCONN         357   /* Socket is not connected */
#define IP_ESHUTDOWN        358   /* Can't send after socket shutdown */
#define IP_ETOOMANYREFS     359   /* Too many references: can't splice */
#define IP_ETIMEDOUT        360   /* Operation timed out */
#define IP_ECONNREFUSED     361   /* Connection refused */

#define IP_EHOSTDOWN        364   /* Host is down */
#define IP_EHOSTUNREACH     365   /* No route to host */


#endif /* DG_IPERR_H */
