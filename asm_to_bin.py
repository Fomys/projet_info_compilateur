with open("out.asm") as f:
    asm = map(lambda l: l.strip().upper().strip().split(" "), f.readlines())
    asm = list(map(lambda ins: (ins[0], f"{int(ins[1]):02x}", f"{int(ins[2]):02x}", f"{int(ins[3]):02x}"), asm))


ins = {
    "NOP":  lambda op1, op2, op3: "00_00_00_00",
    "AFC":  lambda op1, op2, op3: f"00_{op2}_{op1}_01",
    "LDR":  lambda op1, op2, op3: f"00_{op2}_{op1}_02", # refaire une partie du compilo + des trucs du cpu
    "STR":  lambda op1, op2, op3: f"{op1}_{op2}_00_03",
    "JMPI":  lambda op1, op2, op3: f"00_{op1}_00_04",
    "JMZI":  lambda op1, op2, op3: f"{op1}_{op2}_00_05",
    "CPY":  lambda op1, op2, op3: f"00_{op2}_{op1}_06",
    "LT":   lambda op1, op2, op3: f"{op3}_{op2}_{op1}_08",
    "ADD":  lambda op1, op2, op3: f"{op3}_{op2}_{op1}_09",
    "ADDI": lambda op1, op2, op3: f"{op2}_{op3}_{op1}_0A",
    "SUB":  lambda op1, op2, op3: f"{op3}_{op2}_{op1}_0B",
    "SUBI": lambda op1, op2, op3: f"{op2}_{op3}_{op1}_0C", # mal géré
    "JMP": lambda op1, op2, op3: f"00_{op1}_00_0C",
    "END":  lambda op1, op2, op3: "00_00_00_00",
    "LABEL": lambda op1, op2, op3: "00_00_00_00"
}
i = 0
for i, (opcode, *op) in enumerate(asm):
    print(f"rom_data({i}) <= x\"{ins[opcode](*op)}\";")

# fn fibo(n) {
#     if n < 2 {
#         return n;
#     } else {
#         return fibo(n - 1) + fibo(n - 2);
#     }
# }
#
# a = fibo(146);