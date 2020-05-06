#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//--------------------------------------------------------
int gen_ahb_decoder( unsigned int num, char* prefix, FILE* fo )
{
    int i, j;
    unsigned int start=0x00000000;
    unsigned int size =0x00010000;

    if ((num<2)||(prefix==NULL)) return 1;

fprintf(fo, "module %sahb_decoder_s%d\n", prefix, num);
fprintf(fo, "     #(parameter P_NUM        =%d\n", num);
    for (i=0; i<num; i++) {
fprintf(fo, "               , P_HSEL%d_START=32'h%08X, P_HSEL%d_SIZE=32'h%08X\n", i, start, i, size);
fprintf(fo, "               , P_HSEL%d_END  =P_HSEL%d_START+P_HSEL%d_SIZE\n", i, i, i);
        start += (num>16) ? 0x01000000: 0x10000000;
    }
fprintf(fo, "               )\n");
fprintf(fo, "(\n");
fprintf(fo, "       input   wire [31:0] HADDR\n");
fprintf(fo, "     , output  wire        HSELd // default slave\n");
    for (i=0; i<num; i++) {
fprintf(fo, "     , output  wire        HSEL%d\n", i);
    }
fprintf(fo, "     , input   wire        REMAP\n");
fprintf(fo, ");\n");
fprintf(fo, "   wire [%d:0] ihsel;\n", num-1);
fprintf(fo, "   wire       ihseld = ~|ihsel;\n");
fprintf(fo, "   assign HSELd = ihseld;\n");
fprintf(fo, "   assign HSEL0 = (REMAP) ? ihsel[1] : ihsel[0];\n");
fprintf(fo, "   assign HSEL1 = (REMAP) ? ihsel[0] : ihsel[1];\n");
    for (i=2; i<num; i++) {
fprintf(fo, "   assign HSEL%d = ihsel[%d];\n", i, i);
    }
    for (i=0; i<num; i++) {
fprintf(fo, "   assign ihsel[%d] = ((P_NUM>%d)&&(HADDR>=P_HSEL%d_START)&&(HADDR<P_HSEL%d_END)) ? 1'b1 : 1'b0;\n", i, i, i, i);
    }
fprintf(fo, "endmodule\n");

    return 0;
}

