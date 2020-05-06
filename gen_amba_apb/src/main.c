#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#ifdef WIN32
#	include <windows.h>
#	include <io.h>
#endif
#include "gen_amba_apb.h"

int main(int argc, char *argv[]) {
  extern void sig_handle(int);
  extern int  arg_parser(int, char **);
  extern char license[];
  extern unsigned int numS;
  extern char module[];
  extern char prefix[];
  extern FILE* fo;
  extern int   ahb, axi;

  if ((signal(SIGINT, sig_handle)==SIG_ERR)
#ifndef WIN32
	  ||(signal(SIGQUIT, sig_handle)==SIG_ERR)
#endif
	  ) {
        fprintf(stderr, "Error: signal error\n");
        exit(1);
  }

  if (arg_parser(argc, argv)) return 1;

  if (ahb) {
      if (gen_ahb2apb( numS, module, prefix, fo )) return 1;
      if (gen_ahb2apb_bridge( prefix, fo )) return 1;
  } else if (axi) {
      if (gen_axi2apb( numS, module, prefix, fo )) return 1;
      if (gen_axi2apb_bridge( prefix, fo )) return 1;
  }
  if (gen_apb_amba( numS, module, prefix, axi|ahb, fo )) return 1;

  return(0);
}
//--------------------------------------------------------
void sig_handle(int sig) {
  extern void cleanup();
  switch (sig) {
  case SIGINT:
#ifndef WIN32
  case SIGQUIT:
#endif
       exit(0);
       break;
  }
}

