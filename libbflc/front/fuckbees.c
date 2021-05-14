#include "fuckbees.h"

error_t
scan_fuckbees(const char *src, size_t len, ir_t *ir)
{
    error_t err;
    error_init(&err, NULL, NULL);

    pos_t pos = {1, 1, 0};

    for (size_t i = 0; i < len; ++i)
    {
        ++pos.column;
        switch (src[i])
        {
            case 'f':
                pos.offset = i;
                ir_node(ir, INSTR_PTRINC, 1, pos);
                break;

            case 'u':
                pos.offset = i;
                ir_node(ir, INSTR_PTRDEC, 1, pos);
                break;

            case 'c':
                pos.offset = i;
                ir_node(ir, INSTR_CELINC, 1, pos);
                break;

            case 'k':
                pos.offset = i;
                ir_node(ir, INSTR_CELDEC, 1, pos);
                break;

            case 'b':
                pos.offset = i;
                ir_node(ir, INSTR_OUTPUT, -1, pos);
                break;

            case 'e':
                pos.offset = i;
                ir_node(ir, INSTR_INPUT, -1, pos);
                break;

            case 'E':
                pos.offset = i;
                ir_node(ir, INSTR_JMPBEG, -1, pos);
                break;

            case 's':
                pos.offset = i;
                ir_node(ir, INSTR_JMPEND, -1, pos);
                break;

            case '\n':
                pos.column = 1;
                ++pos.line;
                break;
        }
    }

    return err;
}
