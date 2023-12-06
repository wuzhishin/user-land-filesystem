#ifndef _TYPES_H_
#define _TYPES_H_

/******************************************************************************
* SECTION: Type def
*******************************************************************************/
typedef int          boolean;
typedef uint16_t     flag16;

typedef enum nfs_file_type {
    NFS_REG_FILE,
    NFS_DIR,
    NFS_SYM_LINK
} NFS_FILE_TYPE;
/******************************************************************************
* SECTION: Macro
*******************************************************************************/
#define TRUE                    1
#define FALSE                   0
#define UINT32_BITS             32
#define UINT8_BITS              8

#define NFS_MAGIC_NUM           0x52415453  
#define NFS_SUPER_OFS           0
#define NFS_ROOT_INO            0



#define NFS_ERROR_NONE          0
#define NFS_ERROR_ACCESS        EACCES
#define NFS_ERROR_SEEK          ESPIPE     
#define NFS_ERROR_ISDIR         EISDIR
#define NFS_ERROR_NOSPACE       ENOSPC
#define NFS_ERROR_EXISTS        EEXIST
#define NFS_ERROR_NOTFOUND      ENOENT
#define NFS_ERROR_UNSUPPORTED   ENXIO
#define NFS_ERROR_IO            EIO     /* Error Input/Output */
#define NFS_ERROR_INVAL         EINVAL  /* Invalid Args */

#define NFS_MAX_FILE_NAME       128
#define NFS_INODE_PER_FILE      1
#define NFS_DATA_PER_FILE       4
#define NFS_DEFAULT_PERM        0777

#define NFS_IOC_MAGIC           'S'
#define NFS_IOC_SEEK            _IO(NFS_IOC_MAGIC, 0)

#define NFS_FLAG_BUF_DIRTY      0x1
#define NFS_FLAG_BUF_OCCUPY     0x2
/******************************************************************************
* SECTION: Macro Function
*******************************************************************************/
#define NFS_IO_SZ()                     (nfs_super.sz_io)
#define NFS_DISK_SZ()                   (nfs_super.sz_disk)
#define NFS_DRIVER()                    (nfs_super.driver_fd)

#define NFS_ROUND_DOWN(value, round)    (value % round == 0 ? value : (value / round) * round)
#define NFS_ROUND_UP(value, round)      (value % round == 0 ? value : (value / round + 1) * round)

#define NFS_BLKS_SZ(blks)               (blks * 2 * NFS_IO_SZ())
#define NFS_ASSIGN_FNAME(psfs_dentry, _fname)\
                                        memcpy(psfs_dentry->name, _fname, strlen(_fname))
#define NFS_INO_OFS(ino)                (nfs_super.inode_offset + ino * NFS_BLKS_SZ(1))
#define NFS_DATA_OFS(ino)               (nfs_super.data_offset + ino * NFS_BLKS_SZ(1))

#define NFS_IS_DIR(pinode)              (pinode->dentry->ftype == NFS_DIR)
#define NFS_IS_REG(pinode)              (pinode->dentry->ftype == NFS_REG_FILE)
#define NFS_IS_SYM_LINK(pinode)         (pinode->dentry->ftype == NFS_SYM_LINK)

struct nfs_dentry;
struct nfs_inode;
struct nfs_super;

struct custom_options {
	const char*        device;
    boolean            show_help;
};

struct nfs_super {
    uint32_t magic;
    int      driver_fd;
    /* TODO: Define yourself */

    int                sz_io;
    int                sz_disk;
    int                sz_usage;
    
    int                max_ino;
    int                max_data;
    uint8_t*           map_inode;
    uint8_t*           map_data;
    int                map_inode_blks;
    int                map_inode_offset;
    int                map_data_blks;
    int                map_data_offset;
    
    int                inode_offset;
    int                data_offset;

    boolean            is_mounted;

    struct nfs_dentry* root_dentry;
};

struct nfs_inode {
    uint32_t ino;
    /* TODO: Define yourself */

    int                size;                          /* 文件已占用空间 */
    char               target_path[NFS_MAX_FILE_NAME];/* store traget path when it is a symlink */
    int                dir_cnt;
    struct nfs_dentry* dentry;                        /* 指向该inode的dentry */
    struct nfs_dentry* dentrys;                       /* 所有目录项 */
    uint8_t*           data;    
    int data_block[NFS_DATA_PER_FILE];
};

struct nfs_dentry {
    char     name[NFS_MAX_FILE_NAME];
    /* TODO: Define yourself */

     struct nfs_dentry* parent;                        /* 父亲Inode的dentry */
    struct nfs_dentry* brother;                       /* 兄弟 */
    int                ino;
    struct nfs_inode*  inode;                         /* 指向inode */
    NFS_FILE_TYPE      ftype;
};

static inline struct nfs_dentry* new_dentry(char * fname, NFS_FILE_TYPE ftype) {
    struct nfs_dentry * dentry = (struct nfs_dentry *)malloc(sizeof(struct nfs_dentry));
    memset(dentry, 0, sizeof(struct nfs_dentry));
    NFS_ASSIGN_FNAME(dentry, fname);
    dentry->ftype   = ftype;
    dentry->ino     = -1;
    dentry->inode   = NULL;
    dentry->parent  = NULL;
    dentry->brother = NULL;                                  
}


/******************************************************************************
* SECTION: FS Specific Structure - Disk structure
*******************************************************************************/
struct nfs_super_d
{
    uint32_t           magic_num;
    int                sz_usage;
    
    int                max_ino;
    int                map_inode_blks;
    int                map_inode_offset;
    int                map_data_blks;
    int                map_data_offset;
    int                inode_offset;
    int                data_offset;
};

struct nfs_inode_d
{
    int                ino;                           /* 在inode位图中的下标 */
    int                size;                          /* 文件已占用空间 */
    char               target_path[NFS_MAX_FILE_NAME];/* store traget path when it is a symlink */
    int                dir_cnt;
    int data_block[NFS_DATA_PER_FILE];
    NFS_FILE_TYPE      ftype;   
};  

struct nfs_dentry_d
{
    char               fname[NFS_MAX_FILE_NAME];
    NFS_FILE_TYPE      ftype;
    int                ino;                           /* 指向的ino号 */
};  

#endif /* _TYPES_H_ */