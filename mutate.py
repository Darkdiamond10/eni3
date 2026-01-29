import random
import sys
import os

def generate_junk():
    junk_templates = [
        "volatile int var_{id} = {val}; var_{id} = (var_{id} * {val2}) ^ {val3};",
        "for(int i_{id}=0; i_{id}<{limit}; i_{id}++) {{ __asm__(\"nop\"); }}",
        "if({val1} > {val2}) {{ __asm__(\"nop\"); }} else {{ __asm__(\"nop\"); }}",
        "char *str_{id} = \"{string}\"; if(str_{id}[0] == 'Z') return -1;"
    ]
    
    t = random.choice(junk_templates)
    return t.format(
        id=random.randint(0, 10000),
        val=random.randint(1, 100),
        val2=random.randint(1, 100),
        val3=random.randint(1, 1000),
        limit=random.randint(2, 10),
        val1=random.randint(1, 100),
        string=''.join(random.choices("abcdefghijklmnopqrstuvwxyz", k=5))
    )

def mutate_file(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()
    
    new_lines = []
    for line in lines:
        new_lines.append(line)
        # Solo insertar junk en funciones, buscando por indentación simple o llaves
        if '{' in line or (';' in line and random.random() < 0.2):
            if not line.strip().startswith("#") and not line.strip().startswith("//"):
                new_lines.append(f"    {generate_junk()} // Junk\n")
                
    with open(filepath + ".mutated.c", 'w') as f:
        f.writelines(new_lines)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: mutate.py <file.c>")
        sys.exit(1)
    mutate_file(sys.argv[1])
