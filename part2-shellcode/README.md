# Shellcode
On the previous module we executed a function that wasn't gonna be called in our program: we redirected the flow of execution. That function didn't have mych use, it just printed some information, but what if we could execute our on code, that is, redirect the execution to some code that we injected! The name `shellcode` refers to the code that we execute to open a shell (in the past, shellcodes were commonly used to gain access to a shell in a machine).

Initially, lets look at how the processor performs the instrucions of our code. As we've seen in the past, when we compile a program, that gets transformed into assembly instructions, whcich are loaded into main memory and interpreted by the processor.

```
00000000000011a0 <main>:
    11a0:       f3 0f 1e fa             endbr64
    11a4:       55                      push   rbp
    11a5:       48 89 e5                mov    rbp,rsp
    11a8:       48 83 ec 20             sub    rsp,0x20
    11ac:       48 8d 3d 6d 0e 00 00    lea    rdi,[rip+0xe6d]        # 2020 <_IO_stdin_used+0x20>
    11b3:       b8 00 00 00 00          mov    eax,0x0
    11b8:       e8 c3 fe ff ff          call   1080 <printf@plt>
    11bd:       48 8d 45 e0             lea    rax,[rbp-0x20]
    11c1:       48 89 c7                mov    rdi,rax
    11c4:       b8 00 00 00 00          mov    eax,0x0
    11c9:       e8 c2 fe ff ff          call   1090 <gets@plt>
    11ce:       b8 00 00 00 00          mov    eax,0x0
    11d3:       c9                      leave
    11d4:       c3                      ret
    11d5:       66 2e 0f 1f 84 00 00    nop    WORD PTR cs:[rax+rax*1+0x0]
    11dc:       00 00 00
    11df:       90                      nop

```

The above snippet is the main section from the assembly dump made by `objdump` of the `example2` executable from the buffer overflow module. As we can see, the compiler transformed our code into assembly instructions: for instance, the creation of the stack-frame is made by:

```
    11a4:       55                      push   rbp
    11a5:       48 89 e5                mov    rbp,rsp
    11a8:       48 83 ec 20             sub    rsp,0x20
```

But how does the processor understands that it should "push rbp"? Well, at the side os the assembly instructions are the `opdcodes` of their respective instructions. The opcodes represente the instruction, but in a way the Intel x86_64 processor comprehends.

Imagine a simple program which requests the user input of it's name. The user will enter something like "John Doe", but the computer won't save the word itself, it will save, for example, the ASCII hexadecimal representation of each letter. This means that if we want to insert the "push rbp" instruction we can't do it by typing it, otherwise the letters are gonna get saved, not the instruction itself. Therefore, we need to send hte `opcodes` of each instruciton as input.

In order to contextualize, lets imagine we're trying to exploit the following program:

```C
#include <stdio.h>

int main() {
  
  return 0;
}
```


## Further reading
Pwn college
Phreck
LiveOverflow
