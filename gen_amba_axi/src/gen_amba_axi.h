#ifndef GEN_AMBA_AXI_H
#define GEN_AMBA_AXI_H

extern int gen_axi_amba( unsigned int numM // num of masters
                       , unsigned int numS // num of slaves
                       , char *module
                       , char *prefix
                       , FILE *fo);
extern int gen_axi_amba_core( unsigned int numM // num of masters
                            , unsigned int numS // num of slaves
                            , char *module
                            , char *prefix
                            , FILE *fo);
extern int gen_axi_mtos( unsigned int num // num of masters
                       , char *prefix
                       , FILE *fo);
extern int gen_axi_stom( unsigned int num // num of slaves
                       , char *prefix
                       , FILE *fo);
extern int gen_axi_arbiter_mtos( unsigned int num // num of masters
                               , char *prefix
                               , FILE *fo);
extern int gen_axi_arbiter_stom( unsigned int num // num of slaves
                               , char *prefix
                               , FILE *fo);
extern int gen_axi_default_slave( char* prefix
                                , FILE* fo );

#endif
