#include <stdio.h>

#include "shell.h"

#define OP_RTYPE 0x0
#define OP_ADDI 0x8
#define OP_ADDIU 0x9
#define OP_ANDI 0xc
#define OP_SLTI 0x0A
#define OP_SLTIU 0x0B
#define OP_ORI 0xd
#define OP_XORI 0xe
#define OP_BEQ 0x4
#define OP_BNE 0x5
#define OP_BLEZ 0x6
#define OP_BGTZ 0x7
#define OP_J 0x2
#define OP_JAL 0x3
#define OP_LUI 0xf
#define OP_LB 0x20
#define OP_LBU 0x24
#define OP_LH 0x21
#define OP_LHU 0x25
#define OP_LW 0x23
#define OP_SB 0x28
#define OP_SH 0x29
#define OP_SW 0x2b


#define FUNCT_SLL 0x00
#define FUNCT_SRL 0x02
#define FUNCT_SRA 0x03
#define FUNCT_SLLV 0x04
#define FUNCT_SRLV 0x06
#define FUNCT_SRAV 0x07
#define FUNCT_JR 0x08
#define FUNCT_JALR 0x09
#define FUNCT_SYSCALL 0x0c
#define FUNCT_MFHI 0x10
#define FUNCT_MTHI 0x11
#define FUNCT_MFLO 0x12
#define FUNCT_MTLO 0x13
#define FUNCT_MULT 0x18
#define FUNCT_MULTU 0x19
#define FUNCT_DIV 0x1a
#define FUNCT_DIVU 0x1b
#define FUNCT_ADD 0x20
#define FUNCT_ADDU 0x21
#define FUNCT_SUB 0x22
#define FUNCT_SUBU 0x23
#define FUNCT_AND 0x24
#define FUNCT_OR 0x25
#define FUNCT_XOR 0x26
#define FUNCT_NOR 0x27
#define FUNCT_SLT 0x2a
#define FUNCT_SLTU 0x2b

#define OP_BSPECIAL 0x01
#define RT_BLTZ 0x00
#define RT_BLTZAL 0x10
#define RT_BGEZ 0x01
#define RT_BGEZAL 0x11

uint32_t sign_ext(uint32_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t sign_ext_byte(uint8_t imm) {
    int32_t signed_imm = *((int8_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t sign_ext_half(uint16_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t zero_ext(uint32_t imm) { return imm; }

uint32_t zero_ext_byte(uint8_t imm) { return imm; }

uint32_t zero_ext_half(uint16_t imm) { return imm; }

void process_instruction() {
    uint32_t inst = mem_read_32(CURRENT_STATE.PC);

    printf("Instruction: 0x%08x\n", inst);

    // 根据机器码获得对应值
    uint32_t op = inst >> 26;
    uint32_t rs = (inst >> 21) & 0x1f;
    uint32_t rt = (inst >> 16) & 0x1f;
    uint32_t imm = inst & 0xffff;
    uint32_t rd = (inst >> 11) & 0x1f;
    uint32_t shamt = (inst >> 6) & 0x1f;
    uint32_t funct = inst & 0x3f;
	
    switch (op) {
	case OP_RTYPE: {
	    switch (funct) {
		case FUNCT_SLL: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SRL: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SRA: {
		    int32_t val = *((int32_t*)&CURRENT_STATE.REGS[rt]);
		    val = val >> shamt;
		    NEXT_STATE.REGS[rd] = val;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SLLV: {
		    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SRLV: {
		    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SRAV: {
		    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
		    int32_t val = *((int32_t*)&CURRENT_STATE.REGS[rt]);
		    val = val >> shamt;
		    NEXT_STATE.REGS[rd] = val;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_JR: {
		    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
		    break;
		}
		case FUNCT_JALR: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
		    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
		    break;
		}
		case FUNCT_SYSCALL: {
		    if (CURRENT_STATE.REGS[2] == 0x0a) {
		        RUN_BIT = FALSE;
		    } else {
		        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    }
		    break;
		}
		case FUNCT_MFHI: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_MTHI: {
		    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_MFLO: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_MTLO: {
		    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_MULT: {
                    int64_t lhs = *((int32_t*)&CURRENT_STATE.REGS[rs]);
                    int64_t rhs = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    int64_t product = lhs * rhs;
                    uint64_t uint_product = *((uint32_t*)&product);
                    NEXT_STATE.HI =
                        (uint32_t)((uint_product >> 32) & 0xffffffff);
                    NEXT_STATE.LO = (uint32_t)(uint_product & 0xffffffff);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_MULTU: {
                    uint64_t lhs = CURRENT_STATE.REGS[rs];
                    uint64_t rhs = CURRENT_STATE.REGS[rt];
                    uint64_t product = lhs * rhs;
                    NEXT_STATE.HI = (uint32_t)((product >> 32) & 0xffffffff);
                    NEXT_STATE.LO = (uint32_t)(product & 0xffffffff);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_DIV: {
		    int32_t lhs = *((int32_t*)&CURRENT_STATE.REGS[rs]);
                    int32_t rhs = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.LO = lhs / rhs;
                    NEXT_STATE.HI = lhs % rhs;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_DIVU: {
	            uint32_t lhs = CURRENT_STATE.REGS[rs];
                    uint32_t rhs = CURRENT_STATE.REGS[rt];
                    NEXT_STATE.LO = lhs / rhs;
                    NEXT_STATE.HI = lhs % rhs;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;	    if (CURRENT_STATE.REGS[rt] != 0) {
		        NEXT_STATE.HI = CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt];
		        NEXT_STATE.LO = CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt];
		    }
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_ADD: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_ADDU: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SUB: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SUBU: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_AND: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_OR: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_XOR: {
		    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_NOR: {
		    NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SLT: {
		    if ((int32_t)CURRENT_STATE.REGS[rs] < (int32_t)CURRENT_STATE.REGS[rt]) {
		        NEXT_STATE.REGS[rd] = 1;
		    } else {
		        NEXT_STATE.REGS[rd] = 0;
		    }
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
		case FUNCT_SLTU: {
		    if (CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt]) {
		        NEXT_STATE.REGS[rd] = 1;
		    } else {
		        NEXT_STATE.REGS[rd] = 0;
		    }
		    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
		    break;
		}
                default: {
                    printf("Unknown instruction: 0x%08x\n\n", inst);
                    break;
                }
	    }
	}

        case OP_ADDI: {
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + sign_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_ADDIU: {
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + sign_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
	case OP_SLTI:{
	    if (CURRENT_STATE.REGS[rs] < sign_ext(imm)){
		NEXT_STATE.REGS[rt] = 1;
	    }
            else{
		NEXT_STATE.REGS[rt] = 0;
            }
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
	    break;
	}
	case OP_SLTIU:{
	    if (CURRENT_STATE.REGS[rs] < sign_ext(imm)){
		NEXT_STATE.REGS[rt] = 1;
	    }
	    else{
		NEXT_STATE.REGS[rt] = 0;
	    }
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;	    
	    break;     
        }   
        case OP_ANDI: {
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }        
        case OP_ORI: {
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_XORI: {
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_BEQ: {
            if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + sign_ext(imm) * 4 + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }

        case OP_BSPECIAL: {
            uint32_t offset = sign_ext(imm) << 2;

            switch (rt) {
                case RT_BLTZ: {
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case RT_BLTZAL: {
                    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case RT_BGEZ: {
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case RT_BGEZAL: {
                    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                default: {
                    printf("Unknown instruction: 0x%08x\n", inst);
                    break;
                }
            }
            break;
        }       
        
        
        case OP_BNE: {
            if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + sign_ext(imm) * 4 + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }
        case OP_BLEZ: {
            if ((int32_t)CURRENT_STATE.REGS[rs] <= 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC + sign_ext(imm) * 4 + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }
        case OP_BGTZ: {
            if ((int32_t)CURRENT_STATE.REGS[rs] > 0) {
                NEXT_STATE.PC = CURRENT_STATE.PC + sign_ext(imm) * 4 + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }
        case OP_J: {
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000) | ((inst & 0x3ffffff) * 4);
            break;
        }
        case OP_JAL: {
            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000) | ((inst & 0x3ffffff) * 4);
            break;
        }
        case OP_LUI: {
            NEXT_STATE.REGS[rt] = imm << 16;
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_LB: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint8_t byte = mem_read_32(addr) & 0xff;
            NEXT_STATE.REGS[rt] = sign_ext_byte(byte);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_LBU: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint8_t byte = mem_read_32(addr) & 0xff;
            NEXT_STATE.REGS[rt] = zero_ext_byte(byte);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_LH: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint16_t half = mem_read_32(addr) & 0xffff;
            NEXT_STATE.REGS[rt] = sign_ext_half(half);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_LHU: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint16_t half = mem_read_32(addr) & 0xffff;
            NEXT_STATE.REGS[rt] = zero_ext_half(half);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_LW: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            NEXT_STATE.REGS[rt] = mem_read_32(addr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_SB: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint32_t val = (mem_read_32(addr) & 0xffffff00) |
                           (CURRENT_STATE.REGS[rt] & 0xff);
            mem_write_32(addr, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_SH: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            uint32_t val = (mem_read_32(addr) & 0xffff0000) |
                           (CURRENT_STATE.REGS[rt] & 0xffff);
            mem_write_32(addr, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case OP_SW: {
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];
            mem_write_32(addr, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        default: {
            printf("Unknown instruction: 0x%08x\n", inst);
            break;
        }
    }
}

