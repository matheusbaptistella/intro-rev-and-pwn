# Buffer Overflow
The first module will be about the Buffer Overflow vulnerability, due to its low level of complexity. Lest take a look a the example:

```C
#include <stdio.h>

void secret_func() {
        printf("Shhhhh my secret is...\n");
}

int main() {
        char buf[20];
        printf("I'll tell mine if you tell me yours!\n");
        gets(buf);

        return 0;
}
```
