#-----------------------------------------------------------------------------
SHELL		= /bin/sh
MAKEFILE	= Makefile

#-------------------------------------------------------------
CC			= gcc
DEFS		=
INCDIRS		= -Isrc
LIBS		=
ifeq ($(PLATFORM),cygwin)
DEFS		+= -DWIN32
LIBS		+=
else ifeq ($(PLATFORM),mingw)
DEFS		+= -DWIN32
LIBS		+=
endif
OPT		= -g
CCFLAGS		= $(DEFS) $(OPT)

#-------------------------------------------------------------
TARGET		= gen_amba_apb

SRC	=\
	main.c\
	arg_parser.c\
	gen_ahb2apb.c\
	gen_ahb2apb_bridge.c\
	gen_axi2apb.c\
	gen_axi2apb_bridge.c\
	gen_apb_amba.c\
	gen_apb_decoder.c\
	gen_apb_mux.c
		
OBJS	= $(SRC:.c=.o)

#-------------------------------------------------------------
H_DIR		= src
C_DIR		= src

vpath %.h	$(H_DIR)
vpath %.c	$(C_DIR)

#-------------------------------------------------------------
ifndef OBJECTDIR
  OBJECTDIR = obj
endif
ifeq (${wildcard $(OBJECTDIR)},)
  DUMMY := ${shell mkdir $(OBJECTDIR)}
endif

#-------------------------------------------------------------
$(OBJECTDIR)/%.o: %.c
	$(CC) -c $(CCFLAGS) $(INCDIRS) -o $@ $<

#-------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(addprefix $(OBJECTDIR)/, $(OBJS))
	$(CC) -static -o $(TARGET) $^ $(LIBS)

#-------------------------------------------------------------
run: $(TARGET)
	./$(TARGET) --ahb --slv=4 --out=ahb_to_apb_s4.v
	./$(TARGET) --axi --slv=4 --out=axi_to_apb_s4.v

#-------------------------------------------------------------
clean:
	-rm -fr $(OBJECTDIR)
	-rm -f *.v

