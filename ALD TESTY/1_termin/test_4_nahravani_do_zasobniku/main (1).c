#include "check.h"
#include "TStack.c"
#include "TStack.h"

bool stack_init_file(struct TStack* aStack, FILE* aInputFile)
{
	if (aStack == NULL || aInputFile == NULL)
		return false;

	stack_init(aStack);
	int count;

	if (fscanf_s(aInputFile, "%d", &count) != 1)
		return false;

	for (size_t i = 0; i < count; i++)
	{
		int value;
		if (fscanf_s(aInputFile, "%d", &value) != 1)
		{
			stack_destroy(aStack);
			return false;
		}


		if (!stack_push(aStack, value))
		{
			stack_destroy(aStack);
			return false;
		}
	}

	return true;
}

int main(void)
{
	struct TStack stack;
	stack_init(&stack);
	FILE* inputfile = fopen("data.txt", "r");

	if (!inputfile)
	{
		printf("ERROR");
		return 1;
	}

	if (!stack_init_file(&stack, inputfile))
	{
		printf("ERROR");
		fclose(inputfile);
		stack_destroy(&stack);
		return 2;
	}

	printf("Stack contents: \n");

	struct TStackIterator iter = stack_iterator_begin(&stack);
	while (stack_iterator_is_valid(&iter))
	{
		printf("%d", stack_iterator_value(&iter));
		stack_iterator_to_next(&iter);
	}

	putchar("\n");

	int val;
	while (!stack_is_empty(&stack))
	{
		if (!stack_top(&stack, &val))
		{
			printf("Error: Failed to get top element\n");
			fclose(inputfile);
			stack_destroy(&stack);
			return 3;
		}

		printf("Popped value: %d\n", val);

		if (!stack_pop(&stack))
		{
			printf("Error: Failed to pop element\n");
			fclose(inputfile);
			stack_destroy(&stack);
			return 4;
		}

		if (!stack_is_empty(&stack))
		{
			int val2;
			if (!stack_top(&stack, &val2))
			{
				printf("Error: Failed to get top element\n");
				fclose(inputfile);
				stack_destroy(&stack);
				return 5;
			}

			printf("Next value: %d", val2);

			if (val > val2)
				putchar("\n");
		}
	}

	fclose(inputfile);
	stack_destroy(&stack);

	return 0;
}