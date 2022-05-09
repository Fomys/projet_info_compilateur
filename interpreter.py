regs = [0 for _ in range(16)]
mem = [0 for _ in range(1024)]


def afc(ins):
    regs[ins[1]] = ins[2]


def add(ins):
    regs[ins[1]] = regs[ins[2]] + regs[ins[3]]


def sub(ins):
    regs[ins[1]] = regs[ins[2]] - regs[ins[3]]


def jmp(ins):
    regs[-2] = ins[1] - 1


def jmz(ins):
    if regs[ins[1]] == 0:
        regs[-2] = ins[2] - 1


def lt(ins):
    regs[ins[1]] = 1 if regs[ins[2]] < regs[ins[3]] else 0


def nop(ins): pass


def sp_increase(ins):
    regs[-1] += ins[1]


def sp_decrease(ins):
    regs[-1] -= ins[1]


def ret(ins):
    regs[-2] = mem[regs[-1]] + 1


def end(ins):
    print(regs)
    print(mem)
    exit()


def cpy(ins):
    regs[ins[1]] = regs[ins[2]]


def addi(ins):
    regs[ins[1]] += ins[2]


def subi(ins):
    regs[ins[1]] -= ins[2]


def stor(ins):
    mem[regs[ins[2]]] = regs[ins[1]]


def ldr(ins):
    regs[ins[1]] = mem[regs[ins[2]]]


instructions = {
    "AFC": afc,
    "ADD": add,
    "JMP": jmp,
    "LT": lt,
    "JMZ": jmz,
    "SUB": sub,
    "NOP": nop,
    "RET": ret,
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

while regs[-2] < len(asm):
    ins = asm[regs[-2]]
    instructions[ins[0]](ins)
    print(ins, regs, mem, sep="\t")
    regs[-2] += 1
print(regs)
print(mem)
