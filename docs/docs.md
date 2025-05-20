```bash
# build the project
g++ -o 00/07/hello-world/main 00/07/hello-world/main.cpp

# run the program
./00/07/hello-world/main

# disassemble
objdump -d ./00/07/hello-world/main > ./00/07/hello-world/main.asm

# get binary as a hex dump
xxd -p ./00/07/hello-world/main > ./00/07/hello-world/main.hex
xxd -g2 -c16 ./00/07/hello-world/main > ./00/07/hello-world/main.hex
```
