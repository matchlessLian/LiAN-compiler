#ifndef NULL
	#define NULL 0
#endif


//***** ***** ***** ***** ***** ***** ***** ***** 
//可执行文件相关宏信息存储

#define VERSION_MAJOR 1				//暂时没有用的版本号
#define VERSION_MINOR 0				//暂时没有用的版本号
#define LIAN_FLAG_STRING "LIAN"
#define COMMAND_HELP "HELP"			//命令行指令参数help
#define COMMAND_ASK "ASK"			//命令行指令参数ask
#define COMMAND_GRAMMAR "GRAMMAR"	//命令行指令参数grammar
#define SOURCE_FILE_EXT ".txt"		//源文件后缀
#define EXECUTABLE_FILE_EXT ".LiAN"	//可执行文件后缀
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//报错信息宏

//SetStackSize关键字错误信息 
#define ERROR_MSG_SETSTACKSIZE_IN_GLOBAL	\
"运行堆栈关键字（SetStackSize）只允许出现在全局范围，不允许在函数内出现"
#define ERROR_MSG_INVALID_STACK_SIZE	\
"所设定的运行时堆栈大小值不为整型，应使用正确的整型值进行设定"
#define ERROR_MSG_REPEAT_DEFINE_SETSTACKSIZES	\
"运行时堆栈关键字（SetStackSize）已经出现过一次，不可重复出现"

//数组语法错误信息 
#define ERROR_MSG_INVALID_ARRAY_INDEX	\
"数组定义时，数组的大小应该正确定义，请保证数组的大小是大于零的"
#define ERROR_MSG_INVALID_ARRAY_NOT_INDEXED	\
"对于数组的使用没有带上数组索引"
#define ERROR_MSG_INVALID_ARRAY_SIZE	\
"数组定义时，数组大小的设定值不为整型，应使用正确的整型值进行设定"

//标识符语法错误信息 
#define ERROR_MSG_IDENT_EXPECTED	\
"缺失标识符"
#define ERROR_MSG_IDENT_REDEFINITION	\
"该标识符名称已经出现过一次，不可定义重复的标识符"
#define ERROR_MSG_UNDEFINED_IDENT	\
"该标识符名称未进行定义，应使用定义过的标识符"

//函数语法错误信息 
#define ERROR_MSG_NESTED_FUNC	\
"不允许在函数内定义函数，我们允许递归使用，请在全局范围内正确定义函数"
#define ERROR_MSG_FUNC_REDEFINITION	\
"该函数名称已经出现过一次，不可定义重复的函数名称"
#define ERROR_MSG_UNDEFINED_FUNC	\
"该函数名称未进行定义，应使用定义过的函数"

//参数语法错误信息 
#define ERROR_MSG_GLOBAL_PARAM	\
"不允许在全局范围内出现Param关键字，你应当在函数内部正确使用它"
#define ERROR_MSG_MAIN_PARAM	\
"_Main（）函数不需要Param关键字进行参数传递"

//行标签语法错误信息 
#define ERROR_MSG_GLOBAL_LINE_LABEL	\
"不允许在全局范围内出现行标签，你应当在函数内部正确使用它"
#define ERROR_MSG_LINE_LABEL_REDEFINITION	\
"该行标签名称已经出现过一次，不可定义重复的行标签名称"
#define ERROR_MSG_UNDEFINED_LINE_LABEL	\
"该行标签名称未进行定义，应使用定义过的行标签"

//指令关键字错误信息 
#define ERROR_MSG_GLOBAL_INSTR	\
"不允许在全局范围内出现指令，你应当在函数内部正确使用它"
#define ERROR_MSG_INVALID_INSTR	\
"不存在这样的指令关键字，请使用真正存在的指令"
#define ERROR_MSG_INVALID_OP	\
"指令用法存在错误，请阅读使用手册正确使用指令"
#define ERROR_MSG_INVALID_INPUT	\
"非法输入"

//其他错误信息
#define ERROR_MSG_INVALID_STRING	\
"Invalid string"
#define ERROR_MSG_UNKNOWN_THINGS	\
"非法操作"
#define ERROR_MSG_INVALID_ARRAY	\
"Invalid array"

//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** *****
//为什么叫token？
//在我们给出所有的token宏中，不难发现每一个token宏就是一个类别 
//对于token的这个类型命名，我们也可以说是对所有类型的字符串的一个分类
//每一类字符串，就可以归类为是一个token宏中，所以token也是字符串的抽象态 
//可以做一个比喻世界上有很多很多的物体 我们按形状给所有的物体进行抽象 
//但是形状是一个大类我们还可以将形状向下细分成各种各样的小类 
//比如形状中可以分为正方形，长方形，圆形，梯形，平行四边形，不规则形等各种各样你可以去命名的形状类型
//通过这样一个比喻就可以明白字符串和token和token宏之间的关系了
typedef int Token;

#define TOKEN_TYPE_INT              0           // 单词属性：整数
#define TOKEN_TYPE_FLOAT            1           // 单词属性：小数（浮点数）
#define TOKEN_TYPE_STRING           2           // 单词属性：字符串
#define TOKEN_TYPE_QUOTE            3           // 单词属性：引号
#define TOKEN_TYPE_IDENT            4           // 单词属性：标识符（用户自定义变量名，函数名等）
#define TOKEN_TYPE_COLON            5           // 单词属性：冒号 
#define TOKEN_TYPE_OPEN_MIDDLE_BRACKET     6    // 单词属性：左中括号
#define TOKEN_TYPE_CLOSE_MIDDLE_BRACKET    7    // 单词属性：右中括号
#define TOKEN_TYPE_COMMA            8           // 单词属性：逗号
#define TOKEN_TYPE_OPEN_BRACE       9           // 单词属性：左大括号
#define TOKEN_TYPE_CLOSE_BRACE      10          // 单词属性：右大括号
#define TOKEN_TYPE_NEWLINE          11          // 单词属性：换行

#define TOKEN_TYPE_INSTR			12			// 单词属性：指令关键字

#define TOKEN_TYPE_SETSTACKSIZE     13          // 单词属性：指示关键字-SetStackSize
// 指令关键字和指示关键字都属于关键字，但是完全不一样，指示关键字起到补充说明的效果
// 而指令关键字起到具体的控制与操作效果
#define TOKEN_TYPE_VAR              14          // 单词属性：指示关键字-Var
#define TOKEN_TYPE_FUNC             15          // 单词属性：指示关键字-Func
#define TOKEN_TYPE_PARAM            16          // 单词属性：指示关键字-Param
#define TOKEN_TYPE_REG_RETVAL       17          // 单词属性：指示关键字-_RetVal寄存器

#define TOKEN_TYPE_INVALID          18          // 单词属性：指示关键字-SetStackSize
#define END_OF_TOKEN_STREAM         19          // 单词属性：指示关键字-SetStackSize

#define LEXER_STATE_SIMPLE_ANALYSE			0	//现在是正常扫描阶段,也就是说没有进入字符串 
#define LEXER_STATE_STRING_ANALYSE_ING		1	//这个是进入字符串的阶段,表示目前所扫描的所有东西都属于字符串的内容 
#define LEXER_STATE_STRING_ANALYSE_ED		2	//已经出来字符串了 提示词法分析器要把当前状态转换为正常扫描阶段，脱离字符串扫描阶段 
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** ***** 
//其他

#define __START__FUNC "_START"				//相对于main函数
#define byte char							//新类型：byte，一字节变量
#define STRING_TABLE_INDEX_SIZE int			//可执行文件录入数据块大小：字符串在字符串表中对应的索引值 这个值的size
#define INSTR_LOCATION_VALUE int			//可执行文件录入数据块大小：指令指令码 该码值的size 
#define STACK_INDEX_SIZE int				//可执行文件录入数据块大小：堆栈中的索引值 这个值的size
#define FUNCTION_INDEX_SIZE int				//可执行文件录入数据块大小：函数在函数表中对应的索引值 这个值的size
#define HOSTAPIFUNCTION_INDEX_SIZE int		//可执行文件录入数据块大小：API函数在API函数表中对应的索引值 这个值的size 
#define OFFSET_INDEX_SIZE int				//可执行文件录入数据块大小：堆栈中相对偏移量 这个值的size
#define REGISTER_INDEX_SIZE int				//可执行文件录入数据块大小：寄存器索引值 这个值的size 

//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//指令关键字码

//赋值关键字码 
#define INSTR_MOV               0

//算术运算关键字码 
#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

//逻辑运算关键字码
#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

//字符串操作关键字码
#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

//条件跳转关键字码
#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

//堆栈操作关键字码
#define INSTR_PUSH              26
#define INSTR_POP               27

//函数操作关键字码
#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

//程序操作关键字码
#define INSTR_PAUSE             31
#define INSTR_EXIT              32

//输入输出控制关键字码
#define INSTR_IN				33
#define INSTR_OUT				34
#define INSTR_TRAVEL			35

//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//指令操作数序列中合法指令操作控制结构中，每个位上所对应的类型
//在合法指令操作控制结构中，如果该位值标1，则该类型的出现是合法的

#define OP_FLAG_INT_BIT        		1           //第一位，对应整型位
#define OP_FLAG_FLOAT_BIT       	2           //第二位，对应小数类型位
#define OP_FLAG_STRING_BIT      	4           //第三位，对应字符串类型（尽管实际存储的是字符串表中的对应字符串索引）
#define OP_FLAG_MEM_REF_BIT     	8           //第四位，对应内存使用类型位（尽管对于内存的使用我们使用的是数组，其中包含直接索引和相对索引）
#define OP_FLAG_LINE_LABEL_BIT  	16          //第五位，对应行标签类型位
#define OP_FLAG_FUNC_NAME_BIT   	32          //第六位，对应函数类型位
#define OP_FLAG_HOST_API_CALL_BIT   64      	//第七位，对应主应用程序API类型位
#define OP_FLAG_REG_BIT         	128         //第八位，对应寄存器类型位

//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//???

#define CONTROL_FLAG_KEYBOARD_BIT	1
#define CONTROL_FLAG_SCREEN_BIT		2
#define CONTROL_01_BIT				4
#define CONTROL_02_BIT				8
#define CONTROL_03_BIT				16
#define CONTROL_04_BIT				32
#define CONTROL_05_BIT				64
#define CONTROL_06_BIT				128

//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//真正的语法解析中需要根据目前已经得到的操作数类型，和合法指令操作控制结构中的操作数掩码进行比较
//因此需要专门定义一栏在指令流里解析出来的操作数类型

#define OP_TYPE_INT                 0           //整型值
#define OP_TYPE_FLOAT               1           //小数值
#define OP_TYPE_STRING_INDEX        2           //字符串索引值
#define OP_TYPE_ABS_STACK_INDEX     3           //绝对数组索引值（常量索引）
#define OP_TYPE_REL_STACK_INDEX     4           //相对数组索引值（变量索引）
#define OP_TYPE_INSTR_LOCATION      5           //指令位置值
#define OP_TYPE_FUNC_INDEX          6           //函数 函数索引值
#define OP_TYPE_HOST_API_CALL_INDEX 7           //主应用程序API api索引值
#define OP_TYPE_REG                 8           //寄存器码值
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//指令结构
//首先我们需要一个用于存储指令结构的struct，
//一条汇编指令的结构很简单，用不着很复杂的结构，即指令字加操作数即可
//由于包含关系所以我们先声明操作数的结构体，这个结构体实际上是嵌套联合结构，
//使用联合结构可以使指令字后面跟着的操作数序列具有极强的自由性 

#define MAX_SOURCE_CODE_LINE_SIZE 4096

typedef struct _OP
{
	int iType;
	union
	{
		int iIntLiteralValue;
		float fFloatLiteralValue;
		int iStringTableIndex;
		int iStackUnitIndex;
		int iInstrNoodlesIndex;
		int iFuncIndex;
		int iHostAPICallIndex;
		int iRegisterIndex;
	};
	int iAddressingOffset;
}OP;

typedef struct _INSTRUCTION
{ 
	int iOpcode;
	int iOpCount;
	OP* pOpList;
}INSTRUCTION;

#define MAX_FILENAME_SIZE 256			//原来的是2048

char** g_ppstrSourceCode=NULL;			//源代码
int g_iSourceCodeRows=0;				//源代码行数

char g_pstrSourceFilename[MAX_FILENAME_SIZE];
char g_pstrExecutableFilename[MAX_FILENAME_SIZE];
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//指令流管理结构

enum enumJUDGE
{
	No=0,
	Yes=1
};

//实际上用户在编写一份汇编文件时，
//并不是简单的就是如同早期学习汇编语言时往黑框里面一直输入汇编指令那样而已
//一份汇编文件的格式它的风格其实更加类似于或者偏向于成熟的高级语言，
//这么做的原因很明显：
//方便高级语言从汇编层向下过渡，高级语言所带来的很多特性以及属性，向下过渡时必须有相应的承接，
//当然这也带来了其他的优势，包括但不限于用户体验（阅读，使用。。。）
typedef struct _INSTR_STREAM_HEADER
{
	int iStackSize;						//运行时堆栈容量
	int iGlobalDataSize;				//全局变量所占空间大小
	enumJUDGE iMainFuncPresent;			//是否定义了Main函数
	int iMainFuncIndex;					//Main函数函数索引值
}INSTR_STREAM_HEADER;

//汇编器需要一个结构，将用户所输入的所有汇编代码进行一个存储
//这样才能真正的将画面上所显示的，用户所想的内容转成实实在在的数据以供后面的处理
//下面两个变量一个是指针 一个是数字，指针毫无疑问指向所有的指令，数字代表着指令的条数
INSTRUCTION* g_pInstrStreamFile=NULL;	
int g_iInstrStreamRows=0;
int g_iCurrInstrIndex=0;

INSTR_STREAM_HEADER g_SourceCodeHeader;	//源代码头

int g_iSetStackSizeIsFound;
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//静态指令字典，叫这个名字是因为语法分析阶段依靠这个数组存储的所有指令结构来判断指令语法的正确与否

#define MAX_INSTR_MNENONIC_SIZE 16

#define FLOAT_INSTR_OP_COUNT 16

typedef int allow_op_type_bit_cmb;					//原作者的写的其实是OpTypes,但是我认为这样的命名其实更好
//该指令所规定后面能跟什么样的操作数类型组合列表

typedef struct _INSTRDIRNODE
{
	char pstrMnenonic[MAX_INSTR_MNENONIC_SIZE];		//指令关键字（比作你的名字）
	int iOpcode;									//指令码（比作你的身份证号）
	int iOpCount;									//指令后面可以跟的操作数个数
	allow_op_type_bit_cmb* OpList;						//指令后面所跟的操作数列表（实际上存储的是操作数类型列表）
}INSTRDIRNODE;

#define MAX_INSTR_DIR_COUNT 256

//有点小耻辱，应该是dic，不是dir的
typedef struct _INSTRDIR							//原作者中并没有这个结构，只是我觉得原作者那个方法太过于暴力了，我觉得浪费，就改了一下
{
	INSTRDIRNODE InstrDirTable[MAX_INSTR_DIR_COUNT];//指令字典
	int iInstrCount;								//有效存储指令条数
}INSTRDIR;

INSTRDIR g_InstrDirTable;							//静态指令字典结构
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** *****
//基础链表结构，是整个汇编器存储信息的核心数据结构......
//虽然说是暴力，但是还是需要有一个动态结构的，因为这是必须的，很多事情仅仅依靠静态的数组完全不够的
//使用一个单链表就OK，简单不烧脑

//链表节点
typedef struct _LINKEDLISTNODE
{
	void* pvData;
	_LINKEDLISTNODE* pNext;
}LINKEDLISTNODE;

//链表
typedef struct _LINKEDLIST
{
	LINKEDLISTNODE* pListHead;
	LINKEDLISTNODE* pListTail;
	int iNodeCount;
}LINKEDLIST;
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//汇编器基础存储结构：函数表结构

#define MAX_IDENT_SIZE 256			//最大允许定义的名称长度

//函数表链表数据域类型（该结构并不代表链表，它只代表数据域）
typedef struct _FUNCNODE
{
	int iIndex;						//函数索引值 
	char pstrName[MAX_IDENT_SIZE];	//函数名字 
	int iEntryPoint;				//函数入口点 
	int iParamCount;				//函数接口所接收参数个数 
	int iLocalDataSize;				//函数局部变量所占总空间大小
}FUNCNODE;

LINKEDLIST g_FunctionTable;			//函数表定义
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//汇编器基础存储结构：字符串表结构
//实际上字符串表并没有什么特殊的,它只是存储一系列字符串,所以链表有关的数据域不需要特殊定义

LINKEDLIST g_StringTable;	//字符串表
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//汇编器基础存储结构：符号表结构
//实际上呀它就是变量表,你也可以叫它标识符表

//符号表链表数据域类型（该结构并不代表链表，它只代表数据域）
typedef struct _SYMBOLNODE
{
	int iIndex;								//符号在表中的索引 
	char pstrIdentName[MAX_IDENT_SIZE];		//符号名称 
	int iSize;								//符号大小（该符号占了多少空间，变量是1，数组是N）
	int iStackUnitIndex;					//符号位于堆栈的具体索引位置
	int iWorkingFuncIndex;					//作用域（符号所在的函数）
}SYMBOLNODE;

LINKEDLIST g_SymbolTable;					//符号表
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//汇编器基础存储结构：标签表结构
//标签,就像是C语言goto语句的那种标签，
//当然标签也是汇编语言的一个核心，它是未来构成高级语言的核心，无论是构成分支结构还是循环结构

//
typedef struct _LABELNODE
{
	int iIndex;								//标签存储在表中的索引
	char pstrIdentName[MAX_IDENT_SIZE];		//标签的名称 
	int iTargetIndex;						//它所指向的索引位置 
	int iWorkingFuncIndex;					//作用域，即标签所在的函数 
}LABELNODE;

LINKEDLIST g_LabelTabel;					//标签表
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//汇编器基础存储结构：主应用程序API函数表结构

LINKEDLIST g_HostAPICallTable;		//主应用程序API函数表
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//词法分析器结构
//它就像一个盒子记录了当前词法分析器的所有状态，包括当前token等一系列信息
//前面有说Token是什么意思，这里又用到了一个lexeme，实际上它的概念是很简单的 
//我们前面有说token是字符串的抽象态，那么lexeme就是字符串本身，就是它实体这个值 

#define MAX_LEXEME_SIZE 256
typedef struct _LEXER
{
	int iCurrSourceCodeLine;					// 当前源代码行索引
	unsigned int iStartIndex;					// 分离字符流单词的第一个指针
	unsigned int iEndIndex;						// 分类字符流单词的第二个指针
	Token CurrToken;							// 当前正处理词法单元
	// 上面是它的学名，即单词属性，我们分析每个单词的性质由此分类成不同的token_type
	char pstrCurrLexeme[MAX_LEXEME_SIZE];		// 当前正处理词素
	// 上面是它的学名，相信从这个定义不难猜出，即分离出的整个单词，所以词素就是字符序列，词法单元的实例
	int iCurrLexerState;						// 当前词法分析器状态
}LEXER;

LEXER g_Lexer;

extern char** g_ppstrSourceCode;
//***** ***** ***** ***** ***** ***** ***** *****


//***** ***** ***** ***** ***** ***** ***** *****
//函数控制块单元

typedef struct _FUNCCONTROL
{
	int iIsInFunc;					//原来的时候我的这个数据结构是加了一个iCloseFuncState防止有人不加闭大括号而使编译通过了，但是好像没有必要
	int iCurrFuncIndex;
	int iCurrFuncParamCount;
	int iCurrFuncLocalDataSize;
	char pstrCurrFuncName[MAX_IDENT_SIZE];
	FUNCNODE* pCurrFunc;
}FUNCCONTROL;

//***** ***** ***** ***** ***** ***** ***** *****



