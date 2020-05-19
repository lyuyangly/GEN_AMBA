#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gen_axi_utils.h"
#include "gen_amba_axi.h"

//--------------------------------------------------------
int gen_axi_amba( unsigned int numM // num of masters
                , unsigned int numS // num of slaves
                , char *module
                , char *prefix
                , FILE *fo)
{
    int ret=0;

    if ((numM<2)||(numS<2)||(module==NULL)||(prefix==NULL)) return 1;

    ret += gen_axi_amba_core(numM, numS, module, prefix, fo );
    ret += gen_axi_arbiter_mtos( numM, prefix, fo );
    ret += gen_axi_arbiter_stom( numS, prefix, fo );
    ret += gen_axi_mtos( numM, prefix, fo );
    ret += gen_axi_stom( numS, prefix, fo );
    ret += gen_axi_default_slave( prefix, fo );

    return ret;
}

//--------------------------------------------------------
int gen_axi_amba_core( unsigned int numM // num of masters
                     , unsigned int numS // num of slaves
                     , char *module
                     , char *prefix
                     , FILE *fo)
{
    int i, j;
    unsigned int start=0x00000000;

    if ((numM<2)||(numS<2)||(module==NULL)||(prefix==NULL)) return 1;

fprintf(fo, "module %s\n", module);
fprintf(fo, "      #(parameter WIDTH_CID   = 4 // Channel ID width in bits\n");
fprintf(fo, "                , WIDTH_ID    = 4 // ID width in bits\n");
fprintf(fo, "                , WIDTH_AD    =32 // address width\n");
fprintf(fo, "                , WIDTH_DA    =32 // data width\n");
fprintf(fo, "                , WIDTH_DS    =(WIDTH_DA/8)  // data strobe width\n");
fprintf(fo, "                , WIDTH_SID   =(WIDTH_CID+WIDTH_ID)// ID for slave\n");
fprintf(fo, "                `ifdef AMBA_AXI_AWUSER\n");
fprintf(fo, "                , WIDTH_AWUSER= 1 // Write-address user path\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                `ifdef AMBA_AXI_WUSER\n");
fprintf(fo, "                , WIDTH_WUSER = 1 // Write-data user path\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                `ifdef AMBA_AXI_BUSER\n");
fprintf(fo, "                , WIDTH_BUSER = 1 // Write-response user path\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                `ifdef AMBA_AXI_ARUSER\n");
fprintf(fo, "                , WIDTH_ARUSER= 1 // read-address user path\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                `ifdef AMBA_AXI_RUSER\n");
fprintf(fo, "                , WIDTH_RUSER = 1 // read-data user path\n");
fprintf(fo, "                `endif\n");
for (i=0; i<numS; i++) {
fprintf(fo, "                , SLAVE_EN%d   = 1 , ADDR_BASE%d  =32'h%08X , ADDR_LENGTH%d=12 // effective addre bits\n", i, i, start, i);
start += 0x2000;
}
fprintf(fo, "                , NUM_MASTER  = %d  // should not be changed\n", numM);
fprintf(fo, "                , NUM_SLAVE   = %d  // should not be changed\n", numS);
fprintf(fo, "       )\n");
fprintf(fo, "(\n");
fprintf(fo, "       input   wire                      ARESETn\n");
fprintf(fo, "     , input   wire                      ACLK\n");
for (i=0; i<numM; i++) {
char mp[4]; sprintf(mp, "M%d_", i);
fprintf(fo, "     //--------------------------------------------------------------\n");
fprintf(fo, "     , input   wire  [WIDTH_CID-1:0]     M%d_MID   // if not sure use 'h%x\n", i, i);
gen_axi_mport_aw(mp, "wire", fo);
gen_axi_mport_w (mp, "wire", fo);
gen_axi_mport_b (mp, "wire", fo);
gen_axi_mport_ar(mp, "wire", fo);
gen_axi_mport_r (mp, "wire", fo);
}
for (i=0; i<numS; i++) {
char sp[4]; sprintf(sp, "S%d_", i);
fprintf(fo, "     //--------------------------------------------------------------\n");
gen_axi_sport_aw(sp, "wire", fo);
gen_axi_sport_w (sp, "wire", fo);
gen_axi_sport_b (sp, "wire", fo);
gen_axi_sport_ar(sp, "wire", fo);
gen_axi_sport_r (sp, "wire", fo);
}
fprintf(fo, ");\n");
fprintf(fo, "     //-----------------------------------------------------------\n");
gen_axi_signal( "SD_", fo );
fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // driven by axi_mtos_s?\n");

#define MMX(A)\
sprintf(str, "M%d_%s_S0", i, (A)); fprintf(fo, "     wire %-15s", str);\
for (j=1; j<numS; j++) { sprintf(str, "M%d_%s_S%d", i, (A), j); fprintf(fo, ",%-15s", str); }\
sprintf(str, "M%d_%s_SD", i, (A)); fprintf(fo, ",%-15s;\n", str);\

for (i=0; i<numM; i++) {
char str[32];
MMX("AWREADY")
MMX("WREADY")
MMX("ARREADY")
}
fprintf(fo, "     //-----------------------------------------------------------\n");

#define MMY(A)\
sprintf(str, "M%d_%s", i, (A)); fprintf(fo, "     assign %-11s", str);\
sprintf(str, "M%d_%s_S0", i, (A)); fprintf(fo, " = %-15s", str);\
for (j=1; j<numS; j++) {\
sprintf(str, "M%d_%s_S%d", i, (A), j); fprintf(fo, "|%-15s", str); }\
sprintf(str, "M%d_%s_SD", i, (A)); fprintf(fo, "|%-15s;\n", str);

for (i=0; i<numM; i++) {
char str[32];
MMY("AWREADY")
MMY("WREADY")
MMY("ARREADY")
}
fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // driven by axi_stom_m?\n");

#define MMZ(A)\
fprintf(fo, "     wire S%d_%s_M0", i, (A));\
for (j=1; j<numM; j++) fprintf(fo, ",S%d_%s_M%d", i, (A), j); fprintf(fo, ";\n");

for (i=0; i<numS; i++) {
MMZ("BREADY")
MMZ("RREADY")
}

fprintf(fo, "     wire SD_BREADY_M0");
for (i=1; i<numM; i++) fprintf(fo, ",SD_BREADY_M%d", i); fprintf(fo, ";\n");
fprintf(fo, "     wire SD_RREADY_M0");
for (i=1; i<numM; i++) fprintf(fo, ",SD_RREADY_M%d", i); fprintf(fo, ";\n");

fprintf(fo, "     //-----------------------------------------------------------\n");

#define XXZ(A)\
fprintf(fo, "     assign S%d_%s = S%d_%s_M0", i, (A), i, (A));\
for (j=1; j<numM; j++) fprintf(fo, "|S%d_%s_M%d", i, (A), j); fprintf(fo, ";\n");

for (i=0; i<numS; i++) {
XXZ("BREADY")
XXZ("RREADY")
}
fprintf(fo, "     assign SD_BREADY = SD_BREADY_M0");
for (i=1; i<numM; i++) fprintf(fo, "|SD_BREADY_M%d", i); fprintf(fo, ";\n");
fprintf(fo, "     assign SD_RREADY = SD_RREADY_M0");
for (i=1; i<numM; i++) fprintf(fo, "|SD_RREADY_M%d", i); fprintf(fo, ";\n");
fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // driven by axi_mtos_m?\n");
fprintf(fo, "     wire [NUM_MASTER-1:0] AWSELECT_OUT[0:NUM_SLAVE-1];\n");
fprintf(fo, "     wire [NUM_MASTER-1:0] ARSELECT_OUT[0:NUM_SLAVE-1];\n");
fprintf(fo, "     wire [NUM_MASTER-1:0] AWSELECT; // goes to default slave\n");
fprintf(fo, "     wire [NUM_MASTER-1:0] ARSELECT; // goes to default slave\n");
fprintf(fo, "     //-----------------------------------------------------------\n");

for (i=0; i<numM; i++) {
fprintf(fo, "     assign AWSELECT[%d] = AWSELECT_OUT[0][%d]", i, i);
for (j=1; j<numS; j++) fprintf(fo, "|AWSELECT_OUT[%d][%d]", j, i); fprintf(fo, ";\n");
}
for (i=0; i<numM; i++) {
fprintf(fo, "     assign ARSELECT[%d] = ARSELECT_OUT[0][%d]", i, i);
for (j=1; j<numS; j++) fprintf(fo, "|ARSELECT_OUT[%d][%d]", j, i); fprintf(fo, ";\n");
}

for (i=0; i<numS; i++) {
fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // masters to slave for slave %d\n", i);
fprintf(fo, "     %saxi_mtos_m%d #(.SLAVE_ID    (%d           )\n", prefix, numM, i);
fprintf(fo, "                  ,.SLAVE_EN    (SLAVE_EN%d   )\n", i);
fprintf(fo, "                  ,.ADDR_BASE   (ADDR_BASE%d  )\n", i);
fprintf(fo, "                  ,.ADDR_LENGTH (ADDR_LENGTH%d)\n", i);
fprintf(fo, "                  ,.WIDTH_CID   (WIDTH_CID   )\n");
fprintf(fo, "                  ,.WIDTH_ID    (WIDTH_ID    )\n");
fprintf(fo, "                  ,.WIDTH_AD    (WIDTH_AD    )\n");
fprintf(fo, "                  ,.WIDTH_DA    (WIDTH_DA    )\n");
fprintf(fo, "                  ,.WIDTH_DS    (WIDTH_DS    )\n");
fprintf(fo, "                  ,.WIDTH_SID   (WIDTH_SID   )\n");
fprintf(fo, "                  `ifdef AMBA_AXI_AWUSER\n");
fprintf(fo, "                  ,.WIDTH_AWUSER(WIDTH_AWUSER)\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  `ifdef AMBA_AXI_WUSER\n");
fprintf(fo, "                  ,.WIDTH_WUSER (WIDTH_WUSER )\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  `ifdef AMBA_AXI_ARUSER\n");
fprintf(fo, "                  ,.WIDTH_ARUSER(WIDTH_ARUSER)\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  ,.SLAVE_DEFAULT(1'b0)\n");
fprintf(fo, "                 )\n");
fprintf(fo, "     u_axi_mtos_s%d (\n", i);
fprintf(fo, "                                .ARESETn              (ARESETn      )\n");
fprintf(fo, "                              , .ACLK                 (ACLK         )\n");
char sp[5]; sprintf(sp, "_S%d", i);
for (j=0; j<numM; j++) {
char mp[5]; sprintf(mp, "M%d_", j);
gen_axi_m2s_mcon_aw( mp, mp, sp, fo );
gen_axi_m2s_mcon_w ( mp, mp, sp, fo );
gen_axi_m2s_mcon_ar( mp, mp, sp, fo );
}
sprintf(sp, "S%d_", i);
gen_axi_m2s_scon_aw("S_", sp, fo);
gen_axi_m2s_scon_w ("S_", sp, fo);
gen_axi_m2s_scon_ar("S_", sp, fo);
fprintf(fo, "         , .AWSELECT_OUT         (AWSELECT_OUT[%d])\n", i);
fprintf(fo, "         , .ARSELECT_OUT         (ARSELECT_OUT[%d])\n", i);
fprintf(fo, "         , .AWSELECT_IN          (AWSELECT_OUT[%d])// not used since non-default-slave\n", i);
fprintf(fo, "         , .ARSELECT_IN          (ARSELECT_OUT[%d])// not used since non-default-slave\n", i);
fprintf(fo, "     );\n");
}

fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // masters to slave for default slave\n");
fprintf(fo, "     %saxi_mtos_m%d #(.SLAVE_ID    (NUM_SLAVE   )\n", prefix, numM);
fprintf(fo, "                  ,.SLAVE_EN    (1'b1        ) // always enabled\n");
fprintf(fo, "                  ,.ADDR_BASE   (ADDR_BASE1  )\n");
fprintf(fo, "                  ,.ADDR_LENGTH (ADDR_LENGTH1)\n");
fprintf(fo, "                  ,.WIDTH_CID   (WIDTH_CID   )\n");
fprintf(fo, "                  ,.WIDTH_ID    (WIDTH_ID    )\n");
fprintf(fo, "                  ,.WIDTH_AD    (WIDTH_AD    )\n");
fprintf(fo, "                  ,.WIDTH_DA    (WIDTH_DA    )\n");
fprintf(fo, "                  ,.WIDTH_DS    (WIDTH_DS    )\n");
fprintf(fo, "                  ,.WIDTH_SID   (WIDTH_SID   )\n");
fprintf(fo, "                  `ifdef AMBA_AXI_AWUSER\n");
fprintf(fo, "                  ,.WIDTH_AWUSER(WIDTH_AWUSER)\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  `ifdef AMBA_AXI_WUSER\n");
fprintf(fo, "                  ,.WIDTH_WUSER (WIDTH_WUSER )\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  `ifdef AMBA_AXI_ARUSER\n");
fprintf(fo, "                  ,.WIDTH_ARUSER(WIDTH_ARUSER)\n");
fprintf(fo, "                  `endif\n");
fprintf(fo, "                  ,.SLAVE_DEFAULT(1'b1)\n");
fprintf(fo, "                 )\n");
fprintf(fo, "     u_axi_mtos_sd (\n");
fprintf(fo, "           .ARESETn              (ARESETn      )\n");
fprintf(fo, "         , .ACLK                 (ACLK         )\n");
for (j=0; j<numM; j++) {
char mp[5]; sprintf(mp, "M%d_", j);
gen_axi_m2s_mcon_aw( mp, mp, "_SD", fo );
gen_axi_m2s_mcon_w ( mp, mp, "_SD", fo );
gen_axi_m2s_mcon_ar( mp, mp, "_SD", fo );
}
gen_axi_m2s_scon_aw("S_", "SD_", fo);
gen_axi_m2s_scon_w ("S_", "SD_", fo);
gen_axi_m2s_scon_ar("S_", "SD_", fo);
fprintf(fo, "         , .AWSELECT_OUT         (             )// not used since default-slave\n");
fprintf(fo, "         , .ARSELECT_OUT         (             )// not used since default-slave\n");
fprintf(fo, "         , .AWSELECT_IN          (AWSELECT     )\n");
fprintf(fo, "         , .ARSELECT_IN          (ARSELECT     )\n");
fprintf(fo, "     );\n");

for (i=0; i<numM; i++) {
fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     // slaves to master for master %d\n", i);
fprintf(fo, "     %saxi_stom_s%d #(.MASTER_ID(%d)\n", prefix, numS, i);
fprintf(fo, "                  ,.WIDTH_CID   (WIDTH_CID   )\n");
fprintf(fo, "                  ,.WIDTH_ID    (WIDTH_ID    )\n");
fprintf(fo, "                  ,.WIDTH_AD    (WIDTH_AD    )\n");
fprintf(fo, "                  ,.WIDTH_DA    (WIDTH_DA    )\n");
fprintf(fo, "                  ,.WIDTH_DS    (WIDTH_DS    )\n");
fprintf(fo, "                  ,.WIDTH_SID   (WIDTH_SID   )\n");
fprintf(fo, "                `ifdef AMBA_AXI_BUSER\n");
fprintf(fo, "                  ,.WIDTH_BUSER (WIDTH_BUSER)\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                `ifdef AMBA_AXI_RUSER\n");
fprintf(fo, "                  ,.WIDTH_RUSER (WIDTH_RUSER )\n");
fprintf(fo, "                `endif\n");
fprintf(fo, "                 )\n");
fprintf(fo, "     u_axi_stom_m%d (\n", i);
fprintf(fo, "           .ARESETn              (ARESETn     )\n");
fprintf(fo, "         , .ACLK                 (ACLK        )\n");
char mp[5]; sprintf(mp, "M%d_", i);
gen_axi_s2m_mcon_b( "M_", mp, fo);
gen_axi_s2m_mcon_r( "M_", mp, fo);
char pf[5]; sprintf(pf, "_M%d", i);
for (j=0; j<numS; j++) {
char sp[5]; sprintf(sp, "S%d_", j);
gen_axi_s2m_scon_b( sp, sp, pf, fo );
gen_axi_s2m_scon_r( sp, sp, pf, fo );
}
gen_axi_s2m_scon_b( "SD_", "SD_", pf, fo );
gen_axi_s2m_scon_r( "SD_", "SD_", pf, fo );
fprintf(fo, "     );\n");
}

fprintf(fo, "     //-----------------------------------------------------------\n");
fprintf(fo, "     %saxi_default_slave #(.WIDTH_CID(WIDTH_CID)// Channel ID width in bits\n", prefix);
fprintf(fo, "                        ,.WIDTH_ID (WIDTH_ID )// ID width in bits\n");
fprintf(fo, "                        ,.WIDTH_AD (WIDTH_AD )// address width\n");
fprintf(fo, "                        ,.WIDTH_DA (WIDTH_DA )// data width\n");
fprintf(fo, "                        )\n");
fprintf(fo, "     u_axi_default_slave (\n");
fprintf(fo, "            .ARESETn  (ARESETn )\n");
fprintf(fo, "          , .ACLK     (ACLK    )\n");
fprintf(fo, "          , .AWID     (SD_AWID    )\n");
fprintf(fo, "          , .AWADDR   (SD_AWADDR  )\n");
fprintf(fo, "     `ifdef AMBA_AXI4\n");
fprintf(fo, "          , .AWLEN    (SD_AWLEN   )\n");
fprintf(fo, "          , .AWLOCK   (SD_AWLOCK  )\n");
fprintf(fo, "     `else\n");
fprintf(fo, "          , .AWLEN    (SD_AWLEN   )\n");
fprintf(fo, "          , .AWLOCK   (SD_AWLOCK  )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .AWSIZE   (SD_AWSIZE  )\n");
fprintf(fo, "          , .AWBURST  (SD_AWBURST )\n");
fprintf(fo, "     `ifdef AMBA_AXI_CACHE\n");
fprintf(fo, "          , .AWCACHE  (SD_AWCACHE )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "     `ifdef AMBA_AXI_PROT\n");
fprintf(fo, "          , .AWPROT   (SD_AWPROT  )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .AWVALID  (SD_AWVALID )\n");
fprintf(fo, "          , .AWREADY  (SD_AWREADY )\n");
fprintf(fo, "     `ifdef AMBA_AXI4\n");
fprintf(fo, "          , .AWQOS    (SD_AWQOS   )\n");
fprintf(fo, "          , .AWREGION (SD_AWREGION)\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .WID      (SD_WID     )\n");
fprintf(fo, "          , .WDATA    (SD_WDATA   )\n");
fprintf(fo, "          , .WSTRB    (SD_WSTRB   )\n");
fprintf(fo, "          , .WLAST    (SD_WLAST   )\n");
fprintf(fo, "          , .WVALID   (SD_WVALID  )\n");
fprintf(fo, "          , .WREADY   (SD_WREADY  )\n");
fprintf(fo, "          , .BID      (SD_BID     )\n");
fprintf(fo, "          , .BRESP    (SD_BRESP   )\n");
fprintf(fo, "          , .BVALID   (SD_BVALID  )\n");
fprintf(fo, "          , .BREADY   (SD_BREADY  )\n");
fprintf(fo, "          , .ARID     (SD_ARID    )\n");
fprintf(fo, "          , .ARADDR   (SD_ARADDR  )\n");
fprintf(fo, "     `ifdef AMBA_AXI4\n");
fprintf(fo, "          , .ARLEN    (SD_ARLEN   )\n");
fprintf(fo, "          , .ARLOCK   (SD_ARLOCK  )\n");
fprintf(fo, "     `else\n");
fprintf(fo, "          , .ARLEN    (SD_ARLEN   )\n");
fprintf(fo, "          , .ARLOCK   (SD_ARLOCK  )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .ARSIZE   (SD_ARSIZE  )\n");
fprintf(fo, "          , .ARBURST  (SD_ARBURST )\n");
fprintf(fo, "     `ifdef AMBA_AXI_CACHE\n");
fprintf(fo, "          , .ARCACHE  (SD_ARCACHE )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "     `ifdef AMBA_AXI_PROT\n");
fprintf(fo, "          , .ARPROT   (SD_ARPROT  )\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .ARVALID  (SD_ARVALID )\n");
fprintf(fo, "          , .ARREADY  (SD_ARREADY )\n");
fprintf(fo, "     `ifdef AMBA_AXI4\n");
fprintf(fo, "          , .ARQOS    (SD_ARQOS   )\n");
fprintf(fo, "          , .ARREGION (SD_ARREGION)\n");
fprintf(fo, "     `endif\n");
fprintf(fo, "          , .RID      (SD_RID     )\n");
fprintf(fo, "          , .RDATA    (SD_RDATA   )\n");
fprintf(fo, "          , .RRESP    (SD_RRESP   )\n");
fprintf(fo, "          , .RLAST    (SD_RLAST   )\n");
fprintf(fo, "          , .RVALID   (SD_RVALID  )\n");
fprintf(fo, "          , .RREADY   (SD_RREADY  )\n");
fprintf(fo, "     );\n");
fprintf(fo, "endmodule\n");

    return 0;
}

