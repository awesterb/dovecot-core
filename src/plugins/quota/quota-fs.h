#ifndef QUOTA_FS_H
#define QUOTA_FS_H

#if defined (HAVE_STRUCT_DQBLK_CURBLOCKS) || \
	defined (HAVE_STRUCT_DQBLK_CURSPACE)
#  define HAVE_FS_QUOTA
#endif


#ifdef HAVE_QUOTA_OPEN
/* absolute path to avoid confusion with ./quota.h */
#  include "/usr/include/quota.h" /* NetBSD with libquota */
#endif

#ifdef HAVE_LIBZFS
#  define HAVE_ZFS

#include <stdint.h> /* uint64_t */

typedef void libzfs_handle_t;

extern libzfs_handle_t *libzfs_init (void);
extern void libzfs_fini (libzfs_handle_t *);

typedef enum {
	ZFS_TYPE_FILESYSTEM	= (1 << 0),
	/* ... */
} zfs_type_t;

typedef void zfs_handle_t;
extern zfs_handle_t *zfs_path_to_zhandle(libzfs_handle_t *, const char *,
		zfs_type_t);
extern void zfs_close(zfs_handle_t *);

typedef enum {
	ZPROP_INVAL = -1,
	/* ... */
	ZFS_PROP_USED = 97-95,
	ZFS_PROP_QUOTA = 103-95,
	ZFS_PROP_REFQUOTA = 135-95,
	/* ... */
} zfs_prop_t;

extern uint64_t zfs_prop_get_int(zfs_handle_t *, zfs_prop_t);

#endif

#ifdef HAVE_SYS_QUOTA_H
#  include <sys/quota.h> /* Linux, HP-UX */
#elif defined(HAVE_SYS_FS_UFS_QUOTA_H)
#  include <sys/fs/ufs_quota.h> /* Solaris */
#elif defined(HAVE_UFS_UFS_QUOTA_H)
#  include <ufs/ufs/quota.h> /* BSDs */
#elif defined(HAVE_JFS_QUOTA_H)
#  include <jfs/quota.h> /* AIX */
#  ifdef HAVE_SYS_FS_QUOTA_COMMON_H
#    include <sys/fs/quota_common.h> /* quotactl() */
#  endif
#else
#  undef HAVE_FS_QUOTA
#endif

#ifdef HAVE_QUOTACTL
#  ifdef HAVE_SYS_QUOTA_H
#    ifndef _HPUX_SOURCE
#      define FS_QUOTA_LINUX
#    else
#      define FS_QUOTA_HPUX
#    endif
#  else
#    define FS_QUOTA_BSDAIX
#  endif
#elif defined (HAVE_Q_QUOTACTL)
#  define FS_QUOTA_SOLARIS
#else
#  undef HAVE_FS_QUOTA
#endif

#ifdef HAVE_QUOTA_OPEN /* NetBSD with libquota */
#  define FS_QUOTA_NETBSD
#  define HAVE_FS_QUOTA
#  undef FS_QUOTA_LINUX /* obtained because we also have <sys/quota.h> */
#endif

#endif
