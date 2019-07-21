// Copyright 2019 Gauthier Voron
//
// This file is part of Synctl
//
// Synctl is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Fundation, either version 3 of the License, or (at your option) any later
// version.
//
// Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// Synctl. If not, see <https://www.gnu.org/licenses/>.
//


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

#define OP_PUSH_1_LINKTRACK   ((opcode_t) 128)

#define OP_LIST_1_TRUNK       ((opcode_t) 0)
#define OP_LIST_1_BRANCH      ((opcode_t) 1)

#define OP_FLT_NONE           ((opcode_t) 0)
#define OP_FLT_FMATCH         ((opcode_t) 1)

#define OP_PAT_NONE           ((opcode_t) 0)
#define OP_PAT_GLOB           ((opcode_t) 1)

#define OP_ACT_EXIT           ((opcode_t) 0)
#define OP_ACT_LIST           ((opcode_t) 1)
#define OP_ACT_PUSH           ((opcode_t) 2)
#define OP_ACT_PULL           ((opcode_t) 3)

#define OP_RET_OK             ((opcode_t) 0)
#define OP_RET_INVTRUNK       ((opcode_t) 1)
#define OP_RET_INVSNAPSHOT    ((opcode_t) 2)


#endif
