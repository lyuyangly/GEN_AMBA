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
TARGET		= gen_amba_ahb

SRC	=\
	main.c\
	arg_parser.c\
	gen_ahb_amba.c\
	gen_ahb_arbiter.c\
	gen_ahb_m2s.c\
	gen_ahb_lite.c\
	gen_ahb_decoder.c\
	gen_ahb_s2m.c\
	gen_ahb_default_slave.c
		
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
	./$(TARGET) --mst=4 --slv=4 --out=amba_ahb_m4s4.v

#-------------------------------------------------------------
clean:
	-rm -fr $(OBJECTDIR)
	-rm -f compile.log
	-rm -f *.v

