#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#ifdef WIN32
#	include <windows.h>
#	include <io.h>
#endif

int verbose=0;
unsigned int numS=2;
char module[128]="\0\0";
char prefix[128]="\0\0";
char file[256]="\0\0";
FILE *fo;
int   ahb=0, axi=0;

//-----------------------------------------------------
static struct option longopts[] = {
       {"ahb", no_argument      , &ahb, 1 }
     , {"axi", no_argument      , &axi, 1 }
     , {"slv", required_argument,  0  ,'s'} // return 0
     , {"mod", required_argument,  0  ,'d'}
     , {"pre", required_argument,  0  ,'p'}
     , {"out", required_argument,  0  ,'o'}
     , {"ver", required_argument,  0  ,'v'}
     , {"lic", no_argument      ,  0  ,'l'}
     , { 0   , 0                ,  0  , 0 }
};

//--------------------------------------------------------
// 1. get simulator options from command line
// 2. returns 0 on successful completion
int arg_parser(int argc, char **argv) {
  int opt;
  int longidx=0;
  extern void help(int, char **);
  extern void print_license(void);

  //-----------------------------------------------------
  while ((opt=getopt_long(argc, argv, "s:d:p:o:v:lh?", longopts, &longidx))!=-1) {
     switch (opt) {
     case 's': numS = atoi(optarg); break;
     case 'd': strcpy(module, optarg); break;
     case 'p': strcpy(prefix,optarg); break;
     case 'o': strcpy(file,optarg); break;
     case 'v': verbose = atoi(optarg); break;
     case 'l': print_license(); exit(0); break;
     case 'h':
     case '?': help(argc, argv); exit(0); break;
     case  0 : //if (longopts[longidx].flag) printf("%d:%s\n", opt, longopts[longidx].name);
               //if (longopts[longidx].flag!=0) break;
               //strcpy(file,optarg);
               break;
     default:
        fprintf(stderr, "undefined option: %d\n", opt);
        fprintf(stderr, "undefined option: %c\n", optopt);
        help(argc, argv);
        exit(1);
     }
  }
  if (prefix[0]!='\0') {
      if ((prefix[0]<'A')||(prefix[0]>'z')||
          ((prefix[0]>'Z')&&(prefix[0]<'a'))) {
          fprintf(stderr, "prefix should start with alphabet, not %c\n", prefix[0]);
          return 1;
      }
  }
  if (file[0]!='\0') {
      #ifdef WIN32
      fo = fopen(file, "wb");
      #else
      fo = fopen(file, "w");
      #endif
      if (fo==NULL) {
          fprintf(stderr, "file open error\n");
          return 1;
      }
  } else {
    fo = stdout;
  }
  if (module[0]=='\0') {
      if (axi|ahb) sprintf(module, "%s_to_apb_s%d", (axi) ? "axi" : "ahb", numS);
      else         sprintf(module, "amba_apb_s%d", numS);
  }
  return 0;
}
#undef XXTX

//--------------------------------------------------------
void help(int argc, char **argv)
{
  fprintf(stderr, "[Usage] %s [options]\n", argv[0]);
  fprintf(stderr, "\t   --axi|ahb    make \"axi_to_apb\" or \"ahb_to_apb\" (apb if not given)\n");
  fprintf(stderr, "\t-s,--slv=num    num of APB ports  (default: %u)\n", numS);
  fprintf(stderr, "\t-d,--mod=str    module name (default: \"axi_to_apb_sX\" or \"ahb_to_apb_sX\")\n");
  fprintf(stderr, "\t-p,--pre=str    prefix of sub-module name (none if not given)\n");
  fprintf(stderr, "\t-o,--out=file   output file name (stdout if not given)\n");
  fprintf(stderr, "\t-v,--ver=num    verbose level  (default: %d)\n", verbose);
  fprintf(stderr, "\t-l,--lic        print license message\n");
  fprintf(stderr, "\t-h              print help message\n");
}

//--------------------------------------------------------
char license[] = "\
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n\n\
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n\n\
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n\n\
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";
//--------------------------------------------------------
void print_license(void)
{
     printf("%s", license);
}

