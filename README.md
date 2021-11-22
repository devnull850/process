# Process Creation and Code Injection

This is a sample demo for running a program as a child process to a parent that injects the program in the child process with machine instructions. 

```
make test
./test
```

The above generates the initial ELF binary from the x86 assembly for printing 'Hello, World!' to the terminal. These are the machine instructions that will be injected in the demo.

```
make rip
./rip
```

This "rips" out the machine instructions from the ELF binary and stores it in a raw binary file, blob.

```
make nop
./nop
```

nop is used to nop the entire machine instructions from the test ELF binary. This is done to simulate the nop instructions being some other instructions in the test binary.

```
make process
./process
```

This command runs the demo program. The program takes the current last modified data timestamp for the test file. The binary data for the file, unmodified, is stored in a buffer. A child process is created. The parent process waits for the child to finish. The child process injects the machine instructions from the blob file into the test file. The child process then runs the test binary. When the child process ends, the parent breaks out of its busy wait. The parent then reverts the test binary back to its original data. The timestamp is also set back to its last modification time taken before.

