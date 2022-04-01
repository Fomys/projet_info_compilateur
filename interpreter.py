pc = 0
mem = [0 for i in range(1024)]



def afc(ins):
    global pc, mem
    mem[ins[1]] = ins[2]
    pc += 1

def cop(ins):
    global pc, mem
    mem[ins[1]] = mem[ins[2]]
    pc += 1

def add(ins):
    global pc, mem
    mem[ins[1]] = mem[ins[2]] + mem[ins[3]]
    pc += 1

def sou(ins):
    global pc, mem
    mem[ins[1]] = mem[ins[2]] - mem[ins[3]]
    pc += 1

def mul(ins):
    global pc, mem
    mem[ins[1]] = mem[ins[2]] * mem[ins[3]]
    pc += 1

def div(ins):
    global pc, mem
    mem[ins[1]] = mem[ins[2]] / mem[ins[3]]
    pc += 1
    
def equ(ins):
    global pc, mem
    if mem[ins[2]] == mem[ins[3]] :
        mem[ins[1]] = 1
    else :
        mem[ins[1]] = 0
    pc += 1
    
def sup(ins):
    global pc, mem
    print(mem[ins[2]], mem[ins[3]])
    if mem[ins[2]] > mem[ins[3]] :
        mem[ins[1]] = 1
    else :
        mem[ins[1]] = 0
    pc += 1

def inf(ins):
    global pc, mem
    if mem[ins[2]] < mem[ins[3]] :
        mem[ins[1]] = 1
    else :
        mem[ins[1]] = 0
    pc += 1
        
def jmp(ins):
    global pc, mem
    pc = ins[1]

def jmf(ins):
    global pc, mem
    if mem[ins[1]] == 0:
        pc = ins[2]
    else:
        pc += 1

def pri(ins):
    global pc, mem
    print(f"mem[{ins[1]}] = {mem[ins[1]]}")
    pc += 1

instructions = {
    "AFC": afc,
    "COP": cop,
    "ADD": add,
    "SOU": sou,
    "MUL": mul,
    "DIV": div,
    "EQU": equ,
    "SUP": sup,
    "INF": inf,
    "JMP": jmp,
    "JMF": jmf,
    "PRI": pri
}

asm = []
with open("out.asm") as f:
    asm = f.readlines()

while pc < len(asm):
    line = asm[pc]
    ins = line.upper().strip().split(" ")
    ins = (ins[0], int(ins[1]), int(ins[2]), int(ins[3]))
    instructions[ins[0]](ins)
