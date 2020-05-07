#-----------------------------------------------------------------------------
SHELL		= /bin/sh
MAKEFILE	= Makefile

#-------------------------------------------------------------
CC              = gcc
DEFS            =
INCDIRS         = -Isrc
LIBS            =
ifeq ($(PLATFORM),cygwin)
DEFS            += -DWIN32
LIBS            +=
else ifeq ($(PLATFORM),mingw)
DEFS            += -DWIN32
LIBS            +=
endif
OPT             = -g
CCFLAGS         = $(DEFS) $(OPT)

#-------------------------------------------------------------
TARGET		= gen_amba_axi

SRC	=\
	main.c\
	arg_parser.c\
	gen_axi_amba.c\
	gen_axi_utils.c\
	gen_axi_mtos.c\
	gen_axi_stom.c\
	gen_axi_arbiter_mtos.c\
	gen_axi_arbiter_stom.c\
	gen_axi_default_slave.c
		
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

run: $(TARGET)
	./$(TARGET) --mst=4 --slv=4 --out=amba_axi_m4s4.v

#-------------------------------------------------------------
install: $(TARGET)
	@if [ ! -d $(INSTD) ]; then mkdir -p $(INSTD); fi
	cp $(TARGET) $(INSTD)/$(TARGET)
	chmod +x $(INSTD)/$(TARGET)

#-------------------------------------------------------------
clean:
	-rm -fr $(OBJECTDIR)
	-rm -f *.v

