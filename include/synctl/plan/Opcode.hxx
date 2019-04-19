#ifndef _INCLUDE_SYNCTL_OPCODE_HXX_
#define _INCLUDE_SYNCTL_OPCODE_HXX_


typedef uint8_t opcode_t;


#define OP_TREE_NONE          ((opcode_t) 0)
#define OP_TREE_REFERENCE     ((opcode_t) 1)
#define OP_TREE_DIRECTORY_1   ((opcode_t) 2)
#define OP_TREE_REGULAR_1     ((opcode_t) 3)
#define OP_TREE_SYMLINK_1     ((opcode_t) 4)
#define OP_TREE_LINK_1        ((opcode_t) 5)
#define OP_TREE_LINKTABLE_1   ((opcode_t) 6)

#define OP_FLT_NONE           ((opcode_t) 0)
#define OP_FLT_FMATCH         ((opcode_t) 1)

#define OP_PAT_NONE           ((opcode_t) 0)
#define OP_PAT_GLOB           ((opcode_t) 1)

#define OP_ACT_EXIT           ((opcode_t) 0)
#define OP_ACT_PUSH           ((opcode_t) 1)
#define OP_ACT_PULL           ((opcode_t) 2)

#define OP_RET_OK             ((opcode_t) 0)
#define OP_RET_INVBRANCH      ((opcode_t) 1)
#define OP_RET_INVSNAPSHOT    ((opcode_t) 2)


#endif
