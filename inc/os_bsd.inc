; Copyright (C) 2001 Konstantin Boldyshev <konst@linuxassembly.org>
;
; $Id: os_bsd.inc,v 1.1 2001/11/14 16:01:42 konst Exp $
;
; file          : os_bsd.inc
; created       : 12-Nov-2000
; modified      : 14-Nov-2001
; version       : 0.16
; assembler     : nasm 0.98
; description   : common BSD defines
; author        : Konstantin Boldyshev <konst@linuxassembly.org>
; comment       : included internally from os_xxxbsd.inc

%ifndef __SYSTEM_INC
%error "this file must be included internally from system.inc !"
%endif

%ifndef __BSD_INC
%define __BSD_INC

%macro	DO_SYSCALL 0
%if __OPTIMIZE__=__O_SIZE__
	push	ebp
	push	edi
	push	esi
	push	edx
	push	ecx
	push	ebx
	push	eax
%endif
	int	0x80
	jnc	%%_ok
	neg	eax
%%_ok:
%if __OPTIMIZE__=__O_SIZE__
	add	esp,byte 7*4
%endif
%endmacro

%macro	SYSCALL_GATE 0
__syscall_gate:

	DO_SYSCALL
	ret
%endmacro

;
;sys/socket.h
;

%assign	SO_DEBUG	0x0001		;turn on debugging info recording
%assign	SO_ACCEPTCONN	0x0002		;socket has had listen()
%assign	SO_REUSEADDR	0x0004		;allow local address reuse
%assign	SO_KEEPALIVE	0x0008		;keep connections alive
%assign	SO_DONTROUTE	0x0010		;just use interface addresses
%assign	SO_BROADCAST	0x0020		;permit sending of broadcast msgs
%assign	SO_USELOOPBACK	0x0040		;bypass hardware when possible
%assign	SO_LINGER	0x0080		;linger on close if data present
%assign	SO_OOBINLINE	0x0100		;leave received OOB data in line
%assign	SO_REUSEPORT	0x0200		;allow local address & port reuse
%assign	SO_TIMESTAMP	0x0400		;timestamp received dgram traffic

%assign	SOL_SOCKET	0xffff		;options for socket level

;
;sys/mount.h
;

%assign	MNT_WAIT	1
%assign MNT_NOWAIT	2

%assign	MFSNAMELEN	16	;length of fs type name, including null
%assign	MNAMELEN	90	;length of buffer for returned name


struc statfs
.f_spare2	LONG	1	;placeholder
.f_bsize	LONG	1	;fundamental file system block size
.f_iosize	LONG	1	;optimal trasfer block size
.f_blocks	LONG	1	;total data blocks in file system
.f_bfree	LONG	1	;free blocks in fs
.f_bavail	LONG	1	;free blocks avail to non-superuser
.f_files	LONG	1	;total file nodes in file system
.f_ffree	LONG	1	;free file nodes in fs
.f_fsid		LONG	1	;file system id
.f_owner	LONG	1	;user that mounted the filesystem
.f_type		INT	1	;type of filesystem
.f_flags	INT	1	;copy of mount flags
.f_spare	LONG	2	;spare for later
.f_fstypename	CHAR	MFSNAMELEN	;fs type name
.f_mntonname	CHAR	MNAMELEN	;mount point
.f_mntfromname	CHAR	MNAMELEN	;mounted filesystem
endstruc

;
;sys/stat.h
;

struc stat
.st_dev		U32	1
.st_ino		U32	1
.st_mode	U16	1
.st_nlink	U16	1
.st_uid		U32	1
.st_gid		U32	1
.st_rdev	U32	1
.st_atime	LONG	1
.st_atimensec	LONG	1
.st_mtime	LONG	1
.st_mtimensec	LONG	1
.st_ctime	LONG	1
.st_ctimensec	LONG	1
.st_size	U32	2
.st_blocks	U32	2
.st_blksize	U32	1
.st_flags	U32	1
.st_gen		U32	1
.st_lspare	U32	1
.st_qspare	U32	4
endstruc

;
;sys/dirent.h
;

struc dirent
.d_fileno	U32	1
.d_reclen	U16	1
.d_type		U8	1
.d_namlen	U8	1
.d_name		CHAR	NAME_MAX + 1
endstruc

%assign	DT_UNKNOWN	0
%assign	DT_FIFO		1
%assign	DT_CHR		2
%assign	DT_DIR		4
%assign	DT_BLK		6
%assign	DT_REG		8
%assign	DT_LNK		10
%assign	DT_SOCK		12
%assign	DT_WHT		14

;
;sys/termios.h
;

%assign NCCS 20
struc termios
.c_iflag	UINT	1	; input flags
.c_oflag	UINT	1	; output flags
.c_cflag	UINT	1	; control flags
.c_lflag	UINT	1	; local flags
.c_cc		UCHAR	NCCS	; control chars
.c_ispeed	UINT	1	; input speed
.c_ospeed	UINT	1	; output speed
endstruc

%assign TIOSZ	NCCS+4*6

%assign	ECHOE		0x00000002
%assign	ECHOK		0x00000004
%assign ECHO		0x00000008
%assign	ECHONL		0x00000010
%assign	ISIG		0x00000080
%assign	ICANON		0x00000100
%assign	IEXTEN		0x00000400
%assign EXTPROC         0x00000800
%assign TOSTOP		0x00400000
%assign	NOFLSH		0x80000000

;
;sys/iocomm.h
;

%define	IOCPARM_MASK	0x1fff		;parameter length, at most 13 bits
%define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
%define	IOCBASECMD(x)	((x) & ~(IOCPARM_MASK << 16))
%define	IOCGROUP(x)	(((x) >> 8) & 0xff)

;%define	IOCPARM_MAX	PAGE_SIZE	; max size of ioctl, mult. of PAGE_SIZE */
%define	IOC_VOID	0x20000000	; no parameters
%define	IOC_OUT		0x40000000	; copy out parameters
%define	IOC_IN		0x80000000	; copy in parameters
%define	IOC_INOUT	(IOC_IN|IOC_OUT)
%define	IOC_DIRMASK	0xe0000000	;mask for IN/OUT/VOID

%define	_IOC(inout,group,num,len) ((inout | ((len & IOCPARM_MASK) << 16) | ((group) << 8) | (num)))
%define	_IO(g,n)	_IOC(IOC_VOID,	(g), (n), 0)
%define	_IOR(g,n,sz)	_IOC(IOC_OUT,	(g), (n), sz)
%define	_IOW(g,n,sz)	_IOC(IOC_IN,	(g), (n), sz)
%define	_IOWR(g,n,sz)	_IOC(IOC_INOUT,	(g), (n), sz)

;
;sys/cdio.h
;

%define	CDIOCEJECT	_IO('c',24)

;
;sys/ttycom.h
;

%define	TIOCFLUSH	_IOW('t', 16, 4)	; flush buffers
%define	TIOCGETA	_IOR('t', 19, TIOSZ)	; get termios struct
%define	TIOCSETA	_IOW('t', 20, TIOSZ)	; set termios struct
%define	TIOCSETAW	_IOW('t', 21, TIOSZ)	; drain output, set
%define	TIOCSETAF	_IOW('t', 22, TIOSZ)	; drn out, fls in, set
%define	TIOCGETD	_IOR('t', 26, 4)	; get line discipline
%define	TIOCSETD	_IOW('t', 27, 4)	; set line discipline
%define	TIOCSBRK	_IO('t', 123)		; set break bit
%define	TIOCCBRK	_IO('t', 122)		; clear break bit
%define	TIOCSDTR	_IO('t', 121)		; set data terminal ready
%define	TIOCCDTR	_IO('t', 120)		; clear data terminal ready
%define	TIOCGPGRP	_IOR('t', 119, 4)	; get pgrp of tty
%define	TIOCSPGRP	_IOW('t', 118, 4)	; set pgrp of tty
%define	TIOCOUTQ	_IOR('t', 115, 4)	; output queue size
%define	TIOCSTI		_IOW('t', 114, 1)	; simulate terminal input
%define	TIOCNOTTY	_IO('t', 113)		; void tty association
%define	TIOCPKT		_IOW('t', 112, 4)	; pty: set/clear packet mode
%define	TIOCPKT_DATA		0x00		; data packet
%define	TIOCPKT_FLUSHREAD	0x01		; flush packet
%define	TIOCPKT_FLUSHWRITE	0x02		; flush packet
%define	TIOCPKT_STOP		0x04		; stop output
%define	TIOCPKT_START		0x08		; start output
%define	TIOCPKT_NOSTOP		0x10		; no more ^S, ^Q
%define	TIOCPKT_DOSTOP		0x20		; now do ^S ^Q
%define	TIOCPKT_IOCTL		0x40		; state change of pty driver
%define	TIOCSTOP	_IO('t', 111)		; stop output, like ^S
%define	TIOCSTART	_IO('t', 110)		; start output, like ^Q
%define	TIOCMSET	_IOW('t', 109, 4)	; set all modem bits
%define	TIOCMBIS	_IOW('t', 108, 4)	; bis modem bits
%define	TIOCMBIC	_IOW('t', 107, 4)	; bic modem bits
%define	TIOCMGET	_IOR('t', 106, 4)	; get all modem bits
%define	TIOCREMOTE	_IOW('t', 105, 4)	; remote input editing
%define	TIOCGWINSZ	_IOR('t', 104, 8)	; get window size
%define	TIOCSWINSZ	_IOW('t', 103, 8)	; set window size
%define	TIOCUCNTL	_IOW('t', 102, 4)	; pty: set/clr usr cntl mode
%define	TIOCSTAT	_IO('t', 101)		; simulate ^T status message
%define	UIOCCMD(n)	_IO('u', n)		; usr cntl op "n"
%define	TIOCCONS	_IOW('t', 98, 4)	; become virtual console
%define	TIOCSCTTY	_IO('t', 97)		; become controlling tty
%define	TIOCEXT		_IOW('t', 96, 4)	; pty: external processing
%define	TIOCSIG		_IO('t', 95)		; pty: generate signal
%define	TIOCDRAIN	_IO('t', 94)		; wait till output drained
%define	TIOCMSDTRWAIT	_IOW('t', 91, 4)	; modem: set wait on close
%define	TIOCMGDTRWAIT	_IOR('t', 90, 4)	; modem: get wait on close
%define	TIOCTIMESTAMP	_IOR('t', 89, 8)	; enable/get timestamp of last input event
%define	TIOCDCDTIMESTAMP _IOR('t', 88, 8)	; enable/get timestamp of last DCd rise
%define	TIOCSDRAINWAIT	_IOW('t', 87, 4)	; set ttywait timeout
%define	TIOCGDRAINWAIT	_IOR('t', 86, 4)	; get ttywait timeout

;

%define	TCGETS	TIOCGETA
%define	TCSETS	TIOCSETA
%define	TCSETSW	TIOCSETAW

;
;BSD specific syscalls
;

%macro sys_getfsstat 0-3
	__syscall getfsstat, 3, %0, %1, %2, %3
%endmacro

%macro sys_getdirentries 0-5
	__syscall getdirentries, 5, %0, %1, %2, %3, %4, %5
%endmacro

%macro	sys_sbrk	0-1
	__syscall sbrk, 1, %0, %1
%endmacro

%endif	;__BSD_INC