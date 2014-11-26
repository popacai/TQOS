import sys 
with open(sys.argv[1]) as f1:
    with open(sys.argv[2], "w+") as f2: 
        for line in f1.readlines():
            line = line.replace("|reader|", "")
            if "Machine halt" in line:
                exit()
            f2.write(line)

