# Buffer Overflow
The first module will be about the Buffer Overflow vulnerability, due to its low level of complexity. Lets take a look a the example:

```C
#include <stdio.h>

void secret_func() {
        printf("Shhhhh my secret is...\n");
}

int main() {
        char buf[20];
        printf("I'll tell mine if you tell me yours!\nMy secret is: ");
        gets(buf);

        return 0;
}
```

If we were to compile this example:

```
$ gcc ./example2.c -o example2
[...]
example2.c:(.text+0x50): warning: the `gets' function is dangerous and should not be used.
```

The compiler already warns us about the use of a unsafe function call. This warning is because the `gets` function reads input from the `stdin` until the escape character is typed (ENTER). Lets run the executable:

```
$ ./example

I'll tell mine if you tell me yours!
My secret is: <here you type your secret>
```

For instance, if we type `i like cupcakes` nothing happens. That's because the buffer, in which the string is stored, is 20-byte long so if we type any string that is up to 20 bytes of length, nothing new happens. Lets use `python` to print a 22-byte long string, which we expect to cause some abnormal behavior in our program:

```
$ python3 -c "print(22 * 'A')"
AAAAAAAAAAAAAAAAAAAAAA
```
Then, lets use this 22-byte long string as input in our program:

```
$ ./example2
I'll tell mine if you tell me yours!
My secret is: AAAAAAAAAAAAAAAAAAAAAA
```

As we can see, still nothing new happened. Why? Lets check out our binary in GDB:

```
$ gdb ./example2
```

As we want to understand what's happening, lets dissasemble our `main` function using the commanda `disas main` in gdb. The output in probably difficult to understand, that's because the assembly instrucitons are shown in AT&T syntax. In order to change to Intel syntax, we type `set disassembly-flavor intel`. Now the disassemble of main is clearer:

```
0x00000000000011c0 <+0>:     endbr64
0x00000000000011c4 <+4>:     push   rbp
0x00000000000011c5 <+5>:     mov    rbp,rsp
0x00000000000011c8 <+8>:     sub    rsp,0x20
[...]
0x0000000000001202 <+66>:    mov    rdx,QWORD PTR [rbp-0x8]
0x0000000000001206 <+70>:    xor    rdx,QWORD PTR fs:0x28
0x000000000000120f <+79>:    je     0x1216 <main+86>
0x0000000000001211 <+81>:    call   0x1090 <__stack_chk_fail@plt>
0x0000000000001216 <+86>:    leave
0x0000000000001217 <+87>:    ret
```
The first instruction (up to <+8>) create the `stack-frame` of our function `main`, which represents the place in memory where `main` can store its local variables. As we can see, initially `rbp` is pushed into the stack (which means that the address that rbp was pointing to is pushed into the stack), then `rbp` receives `rsp` (now rbp is pointing to the same address as rsp). Finally, we subtract `0x20` bytes from `rsp` (`rsp` now points to an address 32 bytes higher than the previous).

```
+------------------+
|                  | <- RSP (address X - 32bytes)
|                  |
|      MAIN'S      |
|    STACK-FRAME   |
|                  |
|                  | 
+------------------+
|   OLD RBP VALUE  | <- RBP (address X)
+------------------+
|   OLD RIP VALUE  |
+------------------+
```

Now that we know that the stack-frame is 32 bytes long, try writing 31 A's in the input. You probably got the following error:

```
I'll tell mine if you tell me yours!
My secret is: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
*** stack smashing detected ***: terminated
Aborted
```

So, if our stack-frame has 32 bytes of space, why does 31 bytes makes the program stop? Well now comes the seconda part of the disassemble of function `main` showed previously. The OS puts protections into binaries to make then exploit-safe and one of these protections is the `Stack Canary`: basically the OS puts some values on certain locations of our stack-frame and at the end of execution it compares to see if they are still there. When we overflowed the buffer we unintetionally overwrote the stack-canary value, so when the program reached the comparison instruction and the value wasn't the same, it signalled that the stack's integrity had been compromised. As we can see, the program compares the value stored in `[rbp-0x8]`, so if our stack has 32 bytes, but on the eighth is located the stack-canary, we have only 24 bytes to use. Now try typing 24 bytes on the input and you'll see that the program finishes correctly, whereas 25 bytes won't.

But why 32 bytes? That's due to the `stack allignment`: the value stored by `RSP` follows a pattern of increamenting based on a multiple of the size of a `word` in your OS (a performance improvement), which means that, as we are operating in a 64bit machine and a word is equal to 8 bytes, the stack alligment is 16 bytes. 20 is not a multiple of 16, so it has allocated 32 bytes.

In order to be able to exploit this program we must compile it with the GCC flag `-fno-stack-protector`, which will remove the stack-canary from our program. Compile and then try sending 32 bytes as input, you'll see that tha program finishes correctly.

```
$ gcc ./example2.c -o ./example2 -fno-stack-protector
```

Now try sending A LOT of bytes as input:

```
I'll tell mine if you tell me yours!
My secret is: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Segmentation fault
```

In contrast with the initial attempts to exploit the program where the OS realized that the "stack was smashed", now our program finishes with a `Segmentation Fault`. This indicates that we tried to access a region in memory which we didn't have permission to. Lets take a closer look using GDB: disassemble main and set a break point on the `ret` instruction. Then, try sending this payload `AAAABBBBCCCCDDDDEEEEFFFFGGGGHHHHIIIIJJJJKKKKLLLLMMMM` as input: the program crashes because we overflowed the stack up to the point where we overwrote both the `RBP` registers (check out its value using `info registers`) and the `RIP` register (remember the previous layout of the stack). As we can see, we tried to return to address `0x4c4c4c4c4b4b4b4b`, but that address is not in the text segment of memory, therefore we are not allowed to execute whatever instructions are on that address. In ASCII, character with hex code `4b` is `K` an `4c` is `L`, so  this means that whatever we put in the positions of K and L will be executed. 

So, why don't we try putting `secret_func`'s address? In order to do that, we need to know where that function will be, which means that we'll have to know the address during execution on GDB. Again, lets load our program on GDB and use the command `start` and the `disas secret_func`:

```
(gdb) disas secret_func
Dump of assembler code for function secret_func:
   0x0000555555555189 <+0>:     endbr64
   0x000055555555518d <+4>:     push   rbp
   0x000055555555518e <+5>:     mov    rbp,rsp
   [...]
   0x000055555555519e <+21>:    pop    rbp
   0x000055555555519f <+22>:    ret
```

Now we know that `secret_func` will be place on `0x0000555555555189` address. Due to little endianness we need to write that address reversed. So our payload would look like:















