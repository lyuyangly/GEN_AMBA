#ifndef GEN_AMBA_APB_H
#define GEN_AMBA_APB_H

extern int gen_ahb2apb       ( unsigned int num, char* module, char* prefix, FILE* fo );
extern int gen_ahb2apb_bridge( char *prefix, FILE* fo );
extern int gen_axi2apb       ( unsigned int num, char* module, char* prefi, FILE* fox );
extern int gen_axi2apb_bridge( char *prefix, FILE* fo );
extern int gen_apb_amba      ( unsigned int numS, char *module, char *prefix, int bridge, FILE* fo );
extern int gen_apb_amba_core ( unsigned int num, char *module, char* prefix, int bridge, FILE* fo );
extern int gen_apb_decoder   ( unsigned int num, char* prefix, FILE* fo );
extern int gen_apb_mux       ( unsigned int num, char* prefix, FILE* fo );

#endif
