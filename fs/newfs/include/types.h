#ifndef _TYPES_H_
#define _TYPES_H_

#define MAX_NAME_LEN    128     

#define FILE_SYSTEM_SIZE 16383
#define FILE_SYSTEM_BLOCK_SIZE 512

#define SUPERBLOCK_INDEX 0


struct custom_options {
	const char*        device;
};

struct nfs_super {
    uint32_t magic;
    int      fd;
    /* TODO: Define yourself */
    int sz_io;
    int sz_disk;
    int is_mounted;
};

struct nfs_inode {
    uint32_t ino;
    /* TODO: Define yourself */
};

struct nfs_dentry {
    char     name[MAX_NAME_LEN];
    uint32_t ino;
    /* TODO: Define yourself */
};

#endif /* _TYPES_H_ */