regs = [0 for _ in range(16)]
mem = [0 for _ in range(1024)]
i = 0

def afc(ins):
    regs[ins[1]] = ins[2]


def add(ins):
    regs[ins[1]] = regs[ins[2]] + regs[ins[3]]


def sub(ins):
    regs[ins[1]] = regs[ins[2]] - regs[ins[3]]


def jmpi(ins):
    regs[-1] = ins[1] - 1


def jmp(ins):
    regs[-1] = regs[ins[1]] - 1


def jmzi(ins):
    if regs[ins[1]] == 0:
        regs[-1] = ins[2] - 1


def lt(ins):
    regs[ins[1]] = 1 if regs[ins[2]] < regs[ins[3]] else 0


def nop(ins): pass

def end(ins):
    global i
    print(i)
    print(regs)
    print(mem)
    exit()


def cpy(ins):
    regs[ins[1]] = regs[ins[2]]


def addi(ins):
    regs[ins[1]] = regs[ins[2]] + ins[3]


def subi(ins):
    regs[ins[1]] = regs[ins[2]] - ins[3]


def stor(ins):
    mem[regs[ins[2]]] = regs[ins[1]]


def ldr(ins):
    regs[ins[1]] = mem[regs[ins[2]]]


instructions = {
    "AFC": afc,
    "ADD": add,
    "JMPI": jmpi,
    "JMP": jmp,
    "LT": lt,
    "JMZI": jmzi,
    "SUB": sub,
    "NOP": nop,
    "END": end,
    "CPY": cpy,
    "ADDI": addi,
    "SUBI": subi,
    "STR": stor,
    "LDR": ldr
}
asm = []

with open("out.asm") as f:
    asm = map(lambda l: l.strip().upper().strip().split(" "), f.readlines())
    asm = list(map(lambda ins: (ins[0], int(ins[1]), int(ins[2]), int(ins[3])), asm))

while regs[-1] < len(asm):
    ins = asm[regs[-1]]
    instructions[ins[0]](ins)
    print(ins, regs, mem, sep="\t")
    regs[-1] += 1
    i += 1
