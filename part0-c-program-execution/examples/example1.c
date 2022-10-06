/*
  NOTE: You can find it at the "examples" folders as "example1.c"
*/

#include <stdio.h>
#include <stdlib.h>

int global_var;
int init_global_var = 5;

void function(){
	int stack_var;
	int init_stack_var = 30;

	char buff_var[32];
	
	printf("function's stack_var is at address %p\n", &stack_var);
	printf("function's init_stack_var is at address %p\n", &init_stack_var);
	printf("function's buff_var is at address %p\n", buff_var);
}

int main(int argc, char **argv){
	int stack_var;
	int init_stack_var = 10;

	static int static_var;
	static int init_static_var = 20;
	
	char buff_var[32];

	int *heap_var_ptr;
	heap_var_ptr = (int *) malloc(32);

	printf("the first command line argument is at address %p\n\n", &argv[0]);

	printf("program's init_global_var is at address %p\n", &init_global_var);
	printf("main's init_static var is at address %p\n\n", &init_static_var);

	printf("program's global_var is at address %p\n", &global_var);
	printf("main's static_var_ptr is at address %p\n\n", &static_var);

	printf("main's heap_var is at address %p\n\n", heap_var_ptr);

	printf("main's stack_var is at address %p\n", &stack_var);
	printf("main's init_stack_var is at address %p\n", &init_stack_var);
	printf("main's buff_var is at address %p\n\n", &buff_var);

	function();
	
	return 0;
}
