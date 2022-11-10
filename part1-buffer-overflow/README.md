# Buffer Overflow
The first module will be about the Buffer Overflow vulnerability, due to its low level of complexity. Lest take a look a the example:

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

For instance, if we type `i like cupcakes` nothing happens. That's because the buffer, in which the string is sotred is 20-byte long, so if we type any string that is up to 20 bytes of length, nothing new happens. Lets use `python` to print a 22-byte long string, which we expect to cause some abnormal behavior in our program:

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


