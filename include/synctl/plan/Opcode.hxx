#ifndef _INCLUDE_SYNCTL_OPCODE_HXX_
#define _INCLUDE_SYNCTL_OPCODE_HXX_


typedef uint8_t opcode_t;


#define OP_TREE_NONE          ((opcode_t) 0)
#define OP_TREE_DIRECTORY_1   ((opcode_t) 1)
#define OP_TREE_REGULAR_1     ((opcode_t) 2)
#define OP_TREE_SYMLINK_1     ((opcode_t) 3)

#define OP_ACT_EXIT           ((opcode_t) 0)
#define OP_ACT_PUSH           ((opcode_t) 1)
#define OP_ACT_PULL           ((opcode_t) 2)

#define OP_RET_OK             ((opcode_t) 0)


#endif
