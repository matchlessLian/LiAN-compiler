

//***** ***** ***** ***** ***** ***** ***** ***** 
//报错的相关宏函数

#define ERROR_LOAD_FILE_IO \
	printf("装载报错！文件无法进行读写操作，请检查文件！")
#define ERROR_LOAD_INVALID_LIANFILE \
	printf("装载报错！您的文件不符合.LiAN可执行文件的标准结构，请检查文件！")
#define ERROR_LOAD_UNSUPPORTED \
	printf("装载报错！您的.LiAN可执行文件的标准目前不受支持！")
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//一些其他宏

#define DEFAULT_RUNTIME_STACK_SIZE 1024
#define LIAN_FLAG_STRING "LIAN"
#define EXECUTABLE_FILE_EXT ".LiAN"	//可执行文件后缀
#define MAX_COERCION_STRING_SIZE 65
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//指令关键字码，是与.LiAN同步的宏

#define INSTR_MOV                   0

#define INSTR_ADD                   1
#define INSTR_SUB                   2
#define INSTR_MUL                   3
#define INSTR_DIV                   4
#define INSTR_MOD                   5
#define INSTR_EXP                   6
#define INSTR_NEG                   7
#define INSTR_INC                   8
#define INSTR_DEC                   9

#define INSTR_AND                   10
#define INSTR_OR                    11
#define INSTR_XOR                   12
#define INSTR_NOT                   13
#define INSTR_SHL                   14
#define INSTR_SHR                   15

#define INSTR_CONCAT                16
#define INSTR_GETCHAR               17
#define INSTR_SETCHAR               18

#define INSTR_JMP                   19
#define INSTR_JE                    20
#define INSTR_JNE                   21
#define INSTR_JG                    22
#define INSTR_JL                    23
#define INSTR_JGE                   24
#define INSTR_JLE                   25

#define INSTR_PUSH                  26
#define INSTR_POP                   27

#define INSTR_CALL                  28
#define INSTR_RET                   29
#define INSTR_CALLHOST              30

#define INSTR_PAUSE                 31
#define INSTR_EXIT                  32
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//操作数类型，是与.LiAN同步的宏

#define OP_TYPE_NULL                -1          //NULL
#define OP_TYPE_INT                 0           //整型值
#define OP_TYPE_FLOAT               1           //小数值
#define OP_TYPE_STRING        		2           //字符串值
#define OP_TYPE_ABS_STACK_INDEX     3           //绝对数组索引值（常量索引）
#define OP_TYPE_REL_STACK_INDEX     4           //相对数组索引值（变量索引）
#define OP_TYPE_INSTR_LOCATION      5           //指令位置值
#define OP_TYPE_FUNC_INDEX          6           //函数 函数索引值
#define OP_TYPE_HOST_API_CALL_INDEX 7           //主应用程序API api索引值
#define OP_TYPE_REG                 8           //寄存器码值
//***** ***** ***** ***** ***** ***** ***** *****


//q大量复用L的内容其实 

//***** ***** ***** ***** ***** ***** ***** *****
//操作数存储结构
//指令结构中存在着大量的操作数
//我们需要有一个东西可以存储它的所有基本信息 
typedef struct _VARDATA
{
	int iType;
	union
	{
		int iIntLiteralValue;
		float fFloatLiteralValue;
		char* pstrStringLiteralValue;
		int iStackUnitIndex;
		int iInstrIndex;
		int iFuncIndex;
		int iHostAPICallIndex;
		int iRegisterIndex;
	};
	int iAddressingOffset;
}VARDATA;
//***** ***** ***** ***** ***** ***** ***** *****

//***** ***** ***** ***** ***** ***** ***** *****
//指令行
typedef struct _INSTR
{ 
	int iOpcode;
	int iOpCount;
	VARDATA* pOpList;
}INSTR;
//指令流
typedef struct _INSTRSTREAM
{
	INSTR* pInstrs;
	int iInstrRows;
	int iCurrInstrIndex;
}INSTRSTREAM;
//***** ***** ***** ***** ***** ***** ***** *****

//***** ***** ***** ***** ***** ***** ***** *****
//这是堆栈框架
//专门用来编译函数 
typedef struct _RUNTIMESTACK
{
	VARDATA* pElem;
	int iStackSize;
	int iTopElemIndex;
	int iFrameIndex;
}RUNTIMESTACK;
//函数
typedef struct _FUNCTION
{
	int iLocalDataSize;						//函数索引值 
	int iEntryPoint;				//函数入口点 
	int iParamCount;				//函数接口所接收参数个数 
	int iStackFrameSize;				//函数局部变量所占总空间大小
}FUNCTION;
//hostapi（直接交互c语言）
typedef struct _HOSTAPI
{
	char** ppstrHOSTAPI;
	int iAPICount;
}HOSTAPI;
//***** ***** ***** ***** ***** ***** ***** *****

//***** ***** ***** ***** ***** ***** ***** *****
//文件控制结构专门用来控制合法的可执行文件状态
typedef struct _EXEFILECONTROL
{
	int iGlobalDataSize;						//
	int iIsMainFuncPresent;                     //
	int iMainFuncIndex;							//
	
	int iIsPaused;								//
	int iPauseEndTime;			                //
	
	VARDATA _Reg;								//
	
	INSTRSTREAM InstrStream;                    //
	RUNTIMESTACK Stack;                         //
	FUNCTION* pFuncTable;                          //
	HOSTAPI HostAPICallTable;			//
}EXEFILECONTROL;

EXEFILECONTROL g_FileControl;
//***** ***** ***** ***** ***** ***** ***** *****





char ppstrMnemonics [][ 12 ] =
{
	"Mov",
	"Add", "Sub", "Mul", "Div", "Mod", "Exp", "Neg", "Inc", "Dec",
	"And", "Or", "XOr", "Not", "ShL", "ShR",
	"Concat", "GetChar", "SetChar",
	"Jmp", "JE", "JNE", "JG", "JL", "JGE", "JLE",
	"Push", "Pop",
	"Call", "Ret", "CallHost",
	"Pause", "Exit"
};
