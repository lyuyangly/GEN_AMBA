#ifndef GEN_AMBA_AHB_H
#define GEN_AMBA_AHB_H

extern int gen_ahb_amba( unsigned int numM // num of masters
                       , unsigned int numS // num of slaves
                       , char *module
                       , char *prefix
                       , FILE *fo);
extern int gen_ahb_amba_core( unsigned int numM // num of masters
                            , unsigned int numS // num of slaves
                            , char *module
                            , char *prefix
                            , FILE *fo);
extern int gen_ahb_arbiter  ( unsigned int numM
                            , unsigned int numS
                            , char* prefix
                            , FILE *fo);
extern int gen_ahb_m2s      ( unsigned int numM
                            , char* prefix
                            , FILE *fo);
extern int gen_ahb_lite     ( unsigned int lite 
                            , unsigned int numS
                            , char* module
                            , char* prefix
                            , FILE *fo);
extern int gen_ahb_lite_core( unsigned int lite
                            , unsigned int num
                            , char* module
                            , char *prefix
                            , FILE *fo);
extern int gen_ahb_decoder  ( unsigned int num
                            , char* prefix
                            , FILE *fo);
extern int gen_ahb_s2m      ( unsigned int num
                            , char* prefix
                            , FILE *fo);
extern int gen_ahb_default_slave( char* prefix
                                , FILE *fo);

#endif
