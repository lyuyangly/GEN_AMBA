# GEN_AMBA
'gen_amba' is a set of programs that generate AMBA bus Verilog-HDL, which include AMBA AXI, AMBA AHB, and AMBA APB.
* *gen_amba_axi*: AMBA AXI bus generator for multi-master and multi-slave
* *gen_amba_ahb*: AMBA AHB bus generator for multi-master and multi-slave
* *gen_amba_apb*: AMBA APB bus-bridge generator for AMBA AXI or AHB

## License
This is licensed with the 2-clause BSD license to make the program and library useful in open and closed source products independent of their licensing scheme.

# 1. Quick start for AMBA AXI
### 1.1 Generating AMBA AXI

   1) go to 'gen_amba_axi' directory\
      $ cd gen_amba_axi
   2) run Makefile\
      $ make cleanup\
      $ make\
      . It should generate 'gen_amba_axi'.
   3) have a look the help message\
      $ ./gen_amba_axi -h
   4) generate AMBA AXI\
      $ ./gen_amba_axi --mst=2 --slv=3 --out=amba_axi_m2s3.v\
      . It generates an AXI bus ("amba_axi_m2s3.v") with 2 master-ports and 3 slave-ports.
   
### 1.2 Verification AMBA AXI

   1) go to 'gen_amba_axi/verification/sim/modelsim' directory\
      $ cd gen_amba_axi/verification/sim/modelsim\
      . Use 'iverilog' for Icarus Verilog.
   2) run Makefile\
      $ make cleanup\
      $ make MST=2 SLV=3\
      . It should generate necessary bus and top model and then invoke ModelSim simulator.\
      . It uses top-level generator.
   3) have a look the result VCD wave\
      $ gtkwave wave.vcd

# 2. Quick start for AMBA AHB
### 2.1 Generating AMBA AHB

   1) go to 'gen_amba_ahb' directory\
      $ cd gen_amba_ahb
   2) run Makefile\
      $ make cleanup\
      $ make\
      . It should generate 'gen_amba_ahb'.
   3) have a look the help message\
      $ ./gen_amba_ahb -h
   4) generate AMBA AHB\
      $ ./gen_amba_ahb --mst=2 --slv=3 --out=amba_ahb_m2s3.v\
      . It generates an AHB bus ("amba_ahb_m2s3.v") with 2 master-ports and 3 slave-ports.
   
### 2.2 Verification AMBA AHB

   1) go to 'gen_amba_ahb/verification/sim/modelsim' directory\
      $ cd gen_amba_ahb/verification/sim/modelsim\
      . Use 'iverilog' for Icarus Verilog.
   2) run Makefile\
      $ make cleanup\
      $ make MST=2 SLV=3\
      . It should generate necessary bus and top model and then invoke ModelSim simulator.\
      . It uses top-level generator.
   3) have a look the result VCD wave\
      $ gtkwave wave.vcd

