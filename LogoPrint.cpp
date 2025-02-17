#include<stdio.h>
#include<string.h>
void LogoPrint()
{
	printf("-------------------------------------------------------\n");
	printf("              |     +    --    |-   |                  \n");
	printf("              |         /  \\   | \\  |                  \n");
	printf("              |     |  +----+  | ++ |                  \n");
	printf("              |     |  |    |  |  \\ |                  \n");
	printf("              |---  |  |    |  |   -|                  \n");
	printf("-------------------------------------------------------\n");
	printf("                        ****                           \n");
	printf("                  这里是LiAN汇编器                     \n");
	printf("                        ****                           \n");
	printf("-------------------------------------------------------\n");
	printf("\n");
}

void UsingManual()
{
	printf("加上\"ask\"介绍LiAN汇编器作为LiAN编译器的承接作用\n");
	printf("加上\"help\",介绍如何使用LiAN汇编器将您的txt文件进行汇编\n");
	printf("加上\"grammar\"，介绍有关LiAN汇编器所支持的LiAN汇编语言的基本语法\n");
}

void GrammarPrint()
{
	printf("mov x1,x2 复制指令\n");
	printf("add x1,x2 加法运算\n");
	printf("func 与 ident之间存在bug，不能超过一个空格，不然会寄！\n");
	printf("允许一点点冗余的操作捏！\n");
	printf("剩下的懒得输了hhhh\n");
}

void AskPrint()
{
	printf("....\n");
}

void HelpPrint()
{
	printf("您按照下面步骤就可以将您的TXT文件汇编成汇编文件！\n");
	printf("您要输入的命令需要按照以下格式：LiAN Source.txt [Executable.LiAN]\n");
	printf("Source.txt 是指您的源文件，其中源文件仅输入文件名也可被识别\n");
	printf("Executable.LiAN 是指汇编后的文件，我们在这里加上了中括号表示可选项，您若加上这一项则表示指定汇编后文件的名称，如果不加则默认按照源文件名称命名\n");
}
