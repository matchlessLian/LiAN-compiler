/***** ***** ***** ***** ***** ***** ***** ***** 
  
  前言
  
  到底什么是编译器呢？听上去就觉得是一个很高大上的东西
  我用暴力与搜索造就的编译器，只是一个实现而已，什么是实现?就是它确实是能够编译的，是能做到那样的事情的。
  它可以称之为是一个编译器但是算是一个合格的编译器，因为一个合格的编译器其编写远不止像这样简单，
  他追求东西会更多 包括运行过程中的加速 内存管理 编译性能上的加速 代码优化 当然这些都仅仅只是体现在将文件编译成可执行文件中的一些其他要求 
  当然还包括更多细枝末节的东西
  目前我倒不考虑编写一个极其优秀的编译器，因为能够理解编译器已经是幸运了。
  不了解的东西永远神圣而触不可及，总是无尽的猜忌，不过以暴力拟合出它最初的样子的时候，才发现一切就像米开朗基罗的《创造亚当》一样，
  一切的开始就像即将触及的极限一样。
  人总是好奇的，浏览器的工作原理是什么呢？人们为什么想要发明它？输入法的工作原理是什么呢？
  这些我全都想过，我非常好奇，我会想知道玻璃是如何造出来的，我会想知道树是怎样生长的，仅仅知道我却依旧不会满足，
  我希望一切在我的面前真正的流一遍，如果不能实际操作与亲眼见证，我会极其怀疑这些知识，我永远不会承认，我会一直认为这是虚妄的，
  也许你会说我是不是被三体中智子封锁人类科技给唬住了？这么神经？但是我就是这么如此，从小到大。迷迷糊糊地读入知识，在我看来要比迷迷糊糊地被骗些钱更可怕，
  但是我也不过只是一个遵从天性的行为罢了，知识的学习上一步三回头，知识的理解上追根溯源，在我看来，与我的努力无关，
  只是一种莫名的“好奇”天性。工学的兴奋点在于，你铸造一切，你创造一切，这种感觉真是让人兴奋（外国译本读多的后遗症，莫名其妙的语言），
  理学同样让人足够兴奋，你知晓一切，你探索一切。专精于一个领域并有所成就都是极其困难的事情了，更不用说于整个计算机了。
  计算机所牵连的知识虽然足够庞大，但是相比于探索人类文明的总和，后者的惊喜却是更多，尽管不能全然专精，但是探索足以让蜉蝣一世无憾，
  人们常举例那些上不了高中也赚了大钱的人，人们也常常举例那些如今专科学历仍有一番作为的人，可是总是忽略了，他们也在学习，
  即使是于生意场或者职场上左右逢迎的圆滑人物，你也不可否认他们也是一直在学习，虽然是上不得台面的为人处世之道，但是总之都是在学习的。
  
 ***** ***** ***** ***** ***** ***** ***** *****/ 
/***** ***** ***** ***** ***** ***** ***** ***** 
  
  对于整个文件编写工程，内部的变量命名我都遵循了一种特殊的规范....
    
  全局变量的定义前面会加上“g_”前缀，例如g_pstrNowFilename存储当前源文件文件名
  局部变量的定义，除了尽可能表达这个变量的意义，还会前标其类型，例如pstrMnenonic表示助记符（字符串类型），iInstrIndex表示指令代号（整型）
  枚举类型的定义前面会加上“enum”前缀
  宏尽量由，字符全大写和“_”组成
  自定义结构体（数据结构）尽量由，字符全大写组成
  
  在我的注释中，许多注释解释都有着严格规范
  码：由我规定的官方结构
  索引：由源文件产生的一系列东西
  入口：指令在源程序文件中具体的物理位置
  
  实际上我们认为真正的汇编语言是什么呢它不是一个简单的move a XBX
  后面需要检查一下标识符定义的大小 如果标识符他那个串太长了我们是需要报错的
  整型判断有更严格的规定
  函数感觉有不少漏洞
  也许需要更好的报错，比如标识符缺失指出缺失类型是？
  语法分析中感觉缺失了很多部分。。。。
  一些指令加入浮动参数数量,语法分析时自动检测参数浮动（参数>2时）
  很多存储字符串的地方有define后期得再规划
  
 ***** ***** ***** ***** ***** ***** ***** *****/

#ifndef _STDIO_ 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define _STDIO_
#endif
#include"StringProcessing.h"
#include"BasicStructAndMacro.h"


int AddFunc(char* pstrFuncName,int iFuncEntryName);
int AddHostAPI(LINKEDLIST* pList,char* pstrHostAPIName);
int AddInstr(char* pstrMnenonic,int iOpcode,int iOpCount);
int AddLabel(char* pstrIdentName,int iTargetIndex,int iWorkingFuncIndex);
int AddNode(LINKEDLIST* pList,void* pvData);
int AddString(LINKEDLIST* pList,char* pstrString);
int AddSymbol(char* pstrIdentName,int iSize,int iStackUnitIndex,int iWorkingFuncIndex);
void CharExpectedErrorAndExit(char c);
FUNCCONTROL CreateFuncContorlBlock();
void CreateInstrDictionary();
void dy();
void Exit();
void ExitOnError(char* pstrErrorMsg);
void FreeLinkedList(LINKEDLIST* pList);
char GetAndLookAheadAChar();
char* GetCurrLexeme();
FUNCNODE* GetFuncByName(char* pstrFuncName);
int GetInstrByMnemonic(INSTRDIRNODE* pInstr,char* pstrMnemonic);
LABELNODE* GetLabelByIdent(char* pstrIdentName,int iWorkingFuncIndex);
Token GetNextToken();
SYMBOLNODE* GetSymbolByIndex(char* pstrIdentName,int iWorkingFuncIndex);
int GetSymbolSizeByIdent(char* pstrIdentName,int iWorkingFuncIndex);
int GetSymbolStackUnitIndexByIdent(char* pstrIdentName,int iWorkingFuncIndex);
void InitLinkedList(LINKEDLIST* pList);
void Parsing();
void ResetFuncControlBlock(FUNCCONTROL* Block);
void ResetLexer();
void ResultingCodeErrorAndExit(char* pstErrorMsg);
void SetAllowOpTypeForInstr(int iInstrIndex,int iOpIndex,allow_op_type_bit_cmb iOpType);
void SetFuncInfo(char* pstrFuncName,int iParamCount,int iLocalDataSize);
int SkipNextLine();
void StripComments(char* pstrSourceCodeLine);
void TrimWhitespace(char* pstrString);
int ReturnFuncEntryPoint(FUNCNODE* pf);



//***** ***** ***** ***** ***** ***** ***** ***** 
//静态指令字典相关函数

//向指令字典内录入指令
int AddInstr(char* pstrMnenonic,int iOpcode,int iOpCount)
{
	//活用static是利用语言的一种特性，可能有些人不知道这种定义到底有什么用，
	//就为什么要专门加个static，你可以理解为它是作用于这个函数的全局变量，你调用完这个函数，
	//这个变量的值它并不会被清空，当你再次调用这个函数，这个变量所在的空间与它的值仍然存在
	static int iInstrIndex=0;
	
	if(iInstrIndex>=MAX_INSTR_DIR_COUNT)
		return -1;
	
	strcpy(g_InstrDirTable.InstrDirTable[iInstrIndex].pstrMnenonic,pstrMnenonic);
	strupr(g_InstrDirTable.InstrDirTable[iInstrIndex].pstrMnenonic);
	g_InstrDirTable.InstrDirTable[iInstrIndex].iOpCount=iOpCount;
	g_InstrDirTable.InstrDirTable[iInstrIndex].iOpcode=iOpcode;
	g_InstrDirTable.InstrDirTable[iInstrIndex].OpList=(allow_op_type_bit_cmb*)malloc(iOpCount*sizeof(allow_op_type_bit_cmb));
	
	int iReturnInstrIndex=iInstrIndex;
	
	++iInstrIndex;
	++g_InstrDirTable.iInstrCount;
	
	return iReturnInstrIndex;
}

//设置指令可运行的所跟操作数类型流
void SetAllowOpTypeForInstr(int iInstrIndex,int iOpIndex,allow_op_type_bit_cmb iOpType)
{
	//实际上啊真的没有必要做一些检查，比如检查传入的这个指令索引它是否是真实存在的，
	//或者是否有越界怎么怎么的，这些都没有必要，因为这些都是你的内部函数，你构建整个汇编器的工程函数，
	//他们并不是面向用户的，这些函数只是你自己调用，除非你想给自己找不愉快，你自己明明知道怎么调用是违规的还违规调用了
	//所以有时候面向你自己的函数，那些检查是很没有必要的
	g_InstrDirTable.InstrDirTable[iInstrIndex].OpList[iOpIndex]=iOpType;
}

//获取指令结构，辅助语法分析阶段对指令语法结构的检查
int GetInstrByMnemonic(INSTRDIRNODE* pInstr,char* pstrMnemonic)
{
	for(int iCurrInstrIndex=0;iCurrInstrIndex<g_InstrDirTable.iInstrCount;iCurrInstrIndex++)
	{
		if(strcmp(g_InstrDirTable.InstrDirTable[iCurrInstrIndex].pstrMnenonic,pstrMnemonic)==0)
		{
			*pInstr=g_InstrDirTable.InstrDirTable[iCurrInstrIndex];
			return true;
		}
	}
	
	return false;
}

//创建所有的指令结构，即初始化指令字典
void CreateInstrDictionary()
{
	int iInstrIndex;
	
	iInstrIndex=AddInstr((char*)"Mov",INSTR_MOV,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_REG_BIT|
										 OP_FLAG_MEM_REF_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT|
										 OP_FLAG_FLOAT_BIT|
										 OP_FLAG_REG_BIT|
										 OP_FLAG_STRING_BIT|
										 OP_FLAG_MEM_REF_BIT);
	
	iInstrIndex=AddInstr((char*)"Add",INSTR_ADD,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	for(int i=1;i<2;i++)
		SetAllowOpTypeForInstr(iInstrIndex,i,OP_FLAG_FLOAT_BIT|
											 OP_FLAG_INT_BIT|
											 OP_FLAG_STRING_BIT|
											 OP_FLAG_MEM_REF_BIT|
											 OP_FLAG_REG_BIT);
	
	iInstrIndex=AddInstr((char*)"Sub",INSTR_SUB,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	for(int i=1;i<2;i++)
		SetAllowOpTypeForInstr(iInstrIndex,i,OP_FLAG_FLOAT_BIT|
											 OP_FLAG_INT_BIT|
											 OP_FLAG_STRING_BIT|
											 OP_FLAG_MEM_REF_BIT|
											 OP_FLAG_REG_BIT);
	
	iInstrIndex=AddInstr((char*)"Mul",INSTR_MUL,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	for(int i=1;i<2;i++)
		SetAllowOpTypeForInstr(iInstrIndex,i,OP_FLAG_FLOAT_BIT|
											 OP_FLAG_INT_BIT|
											 OP_FLAG_STRING_BIT|
											 OP_FLAG_MEM_REF_BIT|
											 OP_FLAG_REG_BIT);
	
	iInstrIndex=AddInstr((char*)"Div",INSTR_DIV,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	for(int i=1;i<2;i++)
		SetAllowOpTypeForInstr(iInstrIndex,i,OP_FLAG_FLOAT_BIT|
											 OP_FLAG_INT_BIT|
											 OP_FLAG_STRING_BIT|
											 OP_FLAG_MEM_REF_BIT|
											 OP_FLAG_REG_BIT);
	
	iInstrIndex=AddInstr((char*)"Inc",INSTR_INC,1);
	//我想搞个int进去
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT|
										 OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	
	// Jmp          Label
	iInstrIndex=AddInstr((char*)"Jmp",INSTR_JMP,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_LINE_LABEL_BIT);
	
	// JE           Op0, Op1, Label
	iInstrIndex=AddInstr("JE",INSTR_JE,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT|
										 OP_FLAG_FLOAT_BIT |
										 OP_FLAG_STRING_BIT |
										 OP_FLAG_MEM_REF_BIT |
										 OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// JNE          Op0, Op1, Label
	iInstrIndex=AddInstr("JNE",INSTR_JNE,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// JG           Op0, Op1, Label
	iInstrIndex=AddInstr("JG",INSTR_JG,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// JL           Op0, Op1, Label
	iInstrIndex=AddInstr("JL",INSTR_JL,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// JGE          Op0, Op1, Label
	iInstrIndex=AddInstr("JGE",INSTR_JGE,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// JLE           Op0, Op1, Label
	iInstrIndex=AddInstr("JLE",INSTR_JLE,3);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,2,OP_FLAG_LINE_LABEL_BIT);
	
	// Push          Source
	iInstrIndex=AddInstr("Push",INSTR_PUSH,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_INT_BIT |
										OP_FLAG_FLOAT_BIT |
										OP_FLAG_STRING_BIT |
										OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	
	// Pop           Destination
	iInstrIndex=AddInstr("Pop",INSTR_POP,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT |
										OP_FLAG_REG_BIT);
	
	// Call          FunctionName
	iInstrIndex=AddInstr("Call",INSTR_CALL,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_FUNC_NAME_BIT);
	
	// Ret
	iInstrIndex=AddInstr("Ret",INSTR_RET,0);
	
	// CallHost      FunctionName
	iInstrIndex=AddInstr("CallHost",INSTR_CALLHOST,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_HOST_API_CALL_BIT);

	
	iInstrIndex=AddInstr((char*)"In",INSTR_IN,2);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	SetAllowOpTypeForInstr(iInstrIndex,1,CONTROL_FLAG_KEYBOARD_BIT);
	
	iInstrIndex=AddInstr((char*)"Out",INSTR_OUT,1);
	SetAllowOpTypeForInstr(iInstrIndex,0,OP_FLAG_FLOAT_BIT|
										 OP_FLAG_INT_BIT|
										 OP_FLAG_STRING_BIT|
										 OP_FLAG_MEM_REF_BIT|
										 OP_FLAG_REG_BIT);
	
	
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//链表相关函数

//链表初始化
void InitLinkedList(LINKEDLIST* pList)
{
	pList->pListHead=NULL;
	pList->pListTail=NULL;
	
	pList->iNodeCount=0;
}

//为链表插入节点
int AddNode(LINKEDLIST* pList,void* pvData)
{
	/*这里并没有再进行判断malloc是否分配成功，原因是： 
	  一是原编写者并没有进行判断；
	  二是如果进行判断，则进行返回处理会更为麻烦，所以我遵循原作者能简单就简单的方法，目前只是一个实验品设计而已*/
	LINKEDLISTNODE* pNewNode=(LINKEDLISTNODE*)malloc(sizeof(LINKEDLISTNODE));
	
	//赋值
	pNewNode->pvData=pvData;
	pNewNode->pNext=NULL;
	
	//确定位置，并同时更新头尾指针
	if(pList->iNodeCount==0)
		pList->pListHead=pList->pListTail=pNewNode;
	else
	{
		pList->pListTail->pNext=pNewNode;
		pList->pListTail=pNewNode;
	}
	
	++pList->iNodeCount;
	
	return (pList->iNodeCount)-1;
}

//对链表结构进行一个内存释放与回收
/*实际上有些地方我并不打算遵循原作者的一些习惯，因为作为一个实验品我还是希望它的可读性会更高，
  比如下面有这么些点（含@标记处）我会标注上些原作者的习惯与代码风格*/
void FreeLinkedList(LINKEDLIST* pList)
{
	//判空
	if(pList==NULL)return;
	
	//实际上我们释放的只是malloc开辟的内存，如果链表中没有元素，也就是iNodeCount等于零，那就意味着这个链表根本没有malloc开辟的空间
	//此时的pList只是一个LINKEDLIST类型的指针变量，与动态内存释放无关，所以我们不需要管任何事情
	/*@1:if(pList->iNodeCount)*/
	if(pList->iNodeCount!=0)
	{
		//准备两个指针，这两个指针用于遍历整个链表
		LINKEDLISTNODE* pCurrNode;
		LINKEDLISTNODE* pNextNode;
		
		pCurrNode=pList->pListHead;
		
		//遍历，一个一个释放干净
		while(1){
			pNextNode=pCurrNode->pNext;
			
			/*@2:if(pCurrNode->pvData)*/
			if(pCurrNode->pvData!=NULL)
				free(pCurrNode->pvData);
			
			/*@3:if(pCurrNode)*/
			if(pCurrNode!=NULL)
				free(pCurrNode);
			
			if(pNextNode!=NULL)
				pCurrNode=pNextNode;
			else
				break;
		}
	}
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//函数表相关函数

//使用基础链表结构 是AddNode()的包装，但是专门为函数表（链表）服务，
//用于将扫描到的新函数添加到函数表中
int AddFunc(char* pstrFuncName,int iFuncEntryName)
{
	//检查是否存在函数命名重复
	if(GetFuncByName(pstrFuncName)!=NULL)
		return -1;
	
	//赋值
	FUNCNODE* pNewFunc=(FUNCNODE*)malloc(sizeof(FUNCNODE));
	
	strcpy(pNewFunc->pstrName,pstrFuncName);
	pNewFunc->iEntryPoint=iFuncEntryName;
	
	//添加
	int iFuncIndex=AddNode(&g_FunctionTable,pNewFunc);
	
	pNewFunc->iIndex=iFuncIndex;
	
	return iFuncIndex;
}

//完善函数信息
void SetFuncInfo(char* pstrFuncName,int iParamCount,int iLocalDataSize)
{
	FUNCNODE* pCurrFuncNode=GetFuncByName(pstrFuncName);
	
	pCurrFuncNode->iParamCount=iParamCount;
	pCurrFuncNode->iLocalDataSize=iLocalDataSize;
}

//主要是辅助AddFunc（）函数，AddFunc（）函数在向函数表中添加新函数时，首先需要检查是否存在定义重复，
//当然该函数还能辅助语法分析阶段的SetFuncInfo（）函数，辅助函数信息的完善
FUNCNODE* GetFuncByName(char* pstrFuncName)
{
	//判头
	if(g_FunctionTable.iNodeCount==0)
		return NULL;
	
	//获得函数表的一个引用指针，辅助完成函数表的遍历
	LINKEDLISTNODE* pCurrNode=g_FunctionTable.pListHead;
	
	//遍历，是否存在
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_FunctionTable.iNodeCount;iCurrNodeIndex++)
	{
		//实际上我们定的链表可以放任何东西,因为它的数据域我们给的是一个void类型的指针，这意味着你想挂什么样的数据就挂什么样的数据 
		//所以说一个链表它具体是什么，主要取决于这个void类型指针下挂的是什么具体类型的东西
		//我们现在是在对一个函数表进行操作，而函数表它所用的结构是链表，
		//函数表之所以叫函数表是因为我们在链表数据域那个void类型指针下面挂的是FUNCNODE结构体，
		//所以这里要用FUNCNODE类型指针来接收链表结点（LINKEDLISTNODE）的数据域
		FUNCNODE* pCurrFunc=(FUNCNODE*)pCurrNode->pvData;
		if(strcmp(pCurrFunc->pstrName,pstrFuncName)==0)
			return pCurrFunc;
		
		pCurrNode=pCurrNode->pNext;
	}
	
	//没有找到
	return NULL;
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//字符串表相关函数 

//使用基础链表结构 是AddNode()的包装，但是专门为字符串表（链表）服务，
//用于将扫描到的新字符串添加到字符串表中，实现对字符串的统一管理
int AddString(LINKEDLIST* pList,char* pstrString)
{
	//获得字符串表的一个引用，用于完成对字符串表的遍历
	LINKEDLISTNODE* pNode=pList->pListHead;
	
	//遍历，目的是判断是否存在重复定义的字符串
	for(int iCurrNodeIndex=0;iCurrNodeIndex<pList->iNodeCount;iCurrNodeIndex++)
	{
		if(strcmp((char*)pNode->pvData,pstrString)==0)
			return iCurrNodeIndex;
		
		pNode=pNode->pNext;
	}
	
	//没有找到，则添加
	char* pstrStringNode=(char*)malloc(strlen(pstrString)+1);
	strcpy(pstrStringNode,pstrString);
	
	return AddNode(pList,pstrStringNode);
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//符号表相关结构 

//使用基础链表结构，是AddNode（）的包装，但是是专门为符号表（链表）服务的，
//用于将扫描到的新符号添加到符号表中
int AddSymbol(char* pstrIdentName,int iSize,int iStackUnitIndex,int iWorkingFuncIndex)
{
	//检查是否存在符号命名重复
	if(GetSymbolByIndex(pstrIdentName,iWorkingFuncIndex)!=NULL)
		return -1;
	
	//赋值
	SYMBOLNODE* pNewSymbol=(SYMBOLNODE*)malloc(sizeof(SYMBOLNODE));
	
	strcpy(pNewSymbol->pstrIdentName,pstrIdentName);
	pNewSymbol->iSize=iSize;
	pNewSymbol->iStackUnitIndex=iStackUnitIndex;
	pNewSymbol->iWorkingFuncIndex=iWorkingFuncIndex;
	
	//添加
	int iIndex=AddNode(&g_SymbolTable,pNewSymbol);
	
	pNewSymbol->iIndex=iIndex;
	
	return iIndex;
}

//主要目的是辅助AddSymbol（）函数，AddSymbol（）函数在向符号表（也就是变量表）中添加符号时，首先要做的就是检查是否有重复定义，
//那么这样一个检查的步骤就外包给了GetSymbolByIndex（）函数，当然你会发现它是返回出来一个结构，
//这其实和GetFuncByName（）函数是类似的
SYMBOLNODE* GetSymbolByIndex(char* pstrIdentName,int iWorkingFuncIndex)
{
	//判头
	if(g_SymbolTable.iNodeCount==0)
		return NULL;
	
	//获得符号表的一个引用，辅助完成符号表的遍历 
	LINKEDLISTNODE* pCurrNode=g_SymbolTable.pListHead;
	
	//遍历，是否存在
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_SymbolTable.iNodeCount;iCurrNodeIndex++)
	{
		SYMBOLNODE* pCurrSymbol=(SYMBOLNODE*)pCurrNode->pvData;
		
		if(strcmp(pstrIdentName,pCurrSymbol->pstrIdentName)==0)
			if(pCurrSymbol->iWorkingFuncIndex == iWorkingFuncIndex ||pCurrSymbol->iStackUnitIndex >= 0)
				return pCurrSymbol;
		
		pCurrNode=pCurrNode->pNext;
	}
	
	//没有找到
	return NULL;
}

//辅助函数，对GetSymbolByIndex（）的包装，辅助获取符号堆栈索引
int GetSymbolStackUnitIndexByIdent(char* pstrIdentName,int iWorkingFuncIndex)
{
	SYMBOLNODE* pSym=GetSymbolByIndex(pstrIdentName,iWorkingFuncIndex);
	
	return pSym->iStackUnitIndex;
}

//辅助函数，对GetSymbolByIndex（）的包装，辅助获取符号堆栈索引
int GetSymbolSizeByIdent(char* pstrIdentName,int iWorkingFuncIndex)
{
	SYMBOLNODE* pSym=GetSymbolByIndex(pstrIdentName,iWorkingFuncIndex);
	
	return pSym->iSize;
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//标签表相关函数 

//向标签表内添加标签
int AddLabel(char* pstrIdentName,int iTargetIndex,int iWorkingFuncIndex)
{
	if(GetLabelByIdent(pstrIdentName,iWorkingFuncIndex)!=NULL)
		return -1;
	
	LABELNODE* pNewLabel=(LABELNODE*)malloc(sizeof(LABELNODE));
	
	strcpy(pNewLabel->pstrIdentName,pstrIdentName);
	pNewLabel->iTargetIndex=iTargetIndex;
	pNewLabel->iWorkingFuncIndex=iWorkingFuncIndex;
	
	int iIndex=AddNode(&g_LabelTabel,pNewLabel);
	
	pNewLabel->iIndex=iIndex;
	
	return iIndex;
}

//根据标签标识符名称返回对应的标签节点  
LABELNODE* GetLabelByIdent(char* pstrIdentName,int iWorkingFuncIndex)
{
	if(g_LabelTabel.iNodeCount==0)
		return NULL;
	
	LINKEDLISTNODE* pCurrNode=g_LabelTabel.pListHead;
	
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_LabelTabel.iNodeCount;iCurrNodeIndex++)
	{
		LABELNODE* pCurrLabel=(LABELNODE*)pCurrNode->pvData;
		if(strcmp(pCurrLabel->pstrIdentName,pstrIdentName)==0)
			if(pCurrLabel->iWorkingFuncIndex==iWorkingFuncIndex)
				return pCurrLabel;
		
		pCurrNode=pCurrNode->pNext;
	}
	
	return NULL;
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//主应用程序表相关函数

//AddHostAPI()
//向主应用程序API函数表中添加,新的API申请调用函数,
//由于本实验品在主应用程序API调用上所花费的精力并不是非常的多，所以它本质上是一个字符串表结构，是AddString()的一个包装
int AddHostAPI(LINKEDLIST* pList,char* pstrHostAPIName)
{
	return AddString(pList,pstrHostAPIName);
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//文件预处理过程函数

//strip->去除 comment->注释，评论
void StripComments(char* pstrSourceCodeLine)
{
	int iInString=0;
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(pstrSourceCodeLine);iCurrCharIndex++)
	{
		if(pstrSourceCodeLine[iCurrCharIndex]=='"')
		{
			if(iInString==1)
				iInString=0;
			else
				iInString=1;
		}
		if(pstrSourceCodeLine[iCurrCharIndex]==';')
			if(iInString==0)
			{
				pstrSourceCodeLine[iCurrCharIndex]='\n';
				pstrSourceCodeLine[iCurrCharIndex+1]='\0';
				break;
			}
	}
}

//trim->去除 Whitespace->空格
void TrimWhitespace(char* pstrString)
{
	unsigned int iStringLength=strlen(pstrString);
	
	int iRealStringStartIndex=0;
	
	if(iStringLength>1)//等于1的时候为什么也不管，明明不是有字符？
	{
		for(unsigned int iCurrCharIndex=0;iCurrCharIndex<iStringLength;iCurrCharIndex++)
			if(TheCharIsWhitespace(pstrString[iCurrCharIndex])==false)
			{
				iRealStringStartIndex=iCurrCharIndex;
				break;
			}
		
		if(iRealStringStartIndex!=0)
		{
			for(unsigned int iCurrCharIndex=iRealStringStartIndex;iCurrCharIndex<iStringLength;iCurrCharIndex++)
				pstrString[iCurrCharIndex-iRealStringStartIndex]=pstrString[iCurrCharIndex];
			for(unsigned int iCurrCharIndex=iStringLength-iRealStringStartIndex;iCurrCharIndex<iStringLength;iCurrCharIndex++)
				pstrString[iCurrCharIndex]=' ';
			
		}
		
		//这里我啥时候加的，感觉没啥用了
		if(iRealStringStartIndex==0 && pstrString[0]==' ')
			pstrString[0]='\0';
		else
			for(unsigned int iCurrCharIndex=iStringLength-1;iCurrCharIndex>0;iCurrCharIndex--)//这个不能放进去因为，左对齐
				if(TheCharIsWhitespace(pstrString[iCurrCharIndex])==false)
				{
					pstrString[iCurrCharIndex+1]='\0';
					break;
				}
	}
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//词法分析阶段相关函数

//使用词法分析器返回当前Token的字符串
char* GetCurrLexeme()
{
	return g_Lexer.pstrCurrLexeme;
}

//向下读取token
//它会跳过空白符，但是不会跳过换行符
 Token GetNextToken()
{
	g_Lexer.iStartIndex=g_Lexer.iEndIndex;
	
	if(g_Lexer.iStartIndex>=strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine]))
		if(SkipNextLine()==false)
			return END_OF_TOKEN_STREAM;
	
	if(g_Lexer.iCurrLexerState==LEXER_STATE_STRING_ANALYSE_ED)
		g_Lexer.iCurrLexerState=LEXER_STATE_SIMPLE_ANALYSE;
	
	if(g_Lexer.iCurrLexerState!=LEXER_STATE_STRING_ANALYSE_ING)
	{
		while(1)
		{
			//通过未跳过空白符得出问题出错在此9.10
			if(TheCharIsWhitespace(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][g_Lexer.iStartIndex])==false)
				break;
			
			g_Lexer.iStartIndex++;
		}	
	}
	//！！！写上面了
	g_Lexer.iEndIndex=g_Lexer.iStartIndex;
	
	//换行不也带进来了吗，不会，因为换行属于一个单词类型
	while(1)
	{
		if(g_Lexer.iCurrLexerState==LEXER_STATE_STRING_ANALYSE_ING)
		{
			if(g_Lexer.iEndIndex>=strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine]))
			{
				g_Lexer.CurrToken=TOKEN_TYPE_INVALID;
				return g_Lexer.CurrToken;
			}
			
			//之所以直接跳过是因为我们没有那么高级的转义功能
			if(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][g_Lexer.iEndIndex]=='\\')
			{
				g_Lexer.iEndIndex+=2;
				continue;
			}
			
			//检查到了不立即改状态是因为
			if(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][g_Lexer.iEndIndex]=='"')
				break;
			//！！！少写个这
			g_Lexer.iEndIndex++;
		}
		else
		{
			if(g_Lexer.iEndIndex>=strlen(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine]))
				break;
			
			//如果检测到“不立刻改状态是因为考虑大局观，将一切滞后
			if(TheCharIsDelimiter(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][g_Lexer.iEndIndex])==true)
				break;
			
			g_Lexer.iEndIndex++;
		}	
	}
	
	if(g_Lexer.iEndIndex-g_Lexer.iStartIndex==0)
		g_Lexer.iEndIndex++;
	
	int iStoreLexIndex=0;
	int iEscapeDoubleQuotationMark=0;
	
	for(unsigned int iCurrLexIndex=g_Lexer.iStartIndex;iCurrLexIndex<g_Lexer.iEndIndex;iCurrLexIndex++)
	{
		//之所以直接判断是因为
		if(g_Lexer.iCurrLexerState==LEXER_STATE_STRING_ANALYSE_ING)
			if(g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][iCurrLexIndex]=='\\')
			{
				iEscapeDoubleQuotationMark=1;
				iCurrLexIndex++;
			}	
		
		g_Lexer.pstrCurrLexeme[iStoreLexIndex]=g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine][iCurrLexIndex];
		iStoreLexIndex++;
	}
	
	g_Lexer.pstrCurrLexeme[iStoreLexIndex]='\0';
	
	if(g_Lexer.iCurrLexerState!=LEXER_STATE_STRING_ANALYSE_ING)
		strupr(g_Lexer.pstrCurrLexeme);
	
	//预设
	g_Lexer.CurrToken=TOKEN_TYPE_INVALID;
	
	//假如说是字符串，而且字符串就只有一个”呢，我们有专门判断，实际上应当分流进两种情况
	if(g_Lexer.iCurrLexerState==LEXER_STATE_STRING_ANALYSE_ING)
		if((strlen(g_Lexer.pstrCurrLexeme)>1) ||
			(strlen(g_Lexer.pstrCurrLexeme)==1 && iEscapeDoubleQuotationMark==1) ||
			(g_Lexer.pstrCurrLexeme[0]!='"'))
		{
			g_Lexer.CurrToken=TOKEN_TYPE_STRING;
			return g_Lexer.CurrToken;
		}
	
	if(strlen(g_Lexer.pstrCurrLexeme)==1)
	{
		switch (g_Lexer.pstrCurrLexeme[0]) 
		{
		case '"':
			switch (g_Lexer.iCurrLexerState) 
			{
			case LEXER_STATE_STRING_ANALYSE_ING:
				g_Lexer.iCurrLexerState=LEXER_STATE_STRING_ANALYSE_ED;
				break;
			case LEXER_STATE_SIMPLE_ANALYSE:
				g_Lexer.iCurrLexerState=LEXER_STATE_STRING_ANALYSE_ING;
				break;
			}
			g_Lexer.CurrToken=TOKEN_TYPE_QUOTE;
			break;
			
		case ',':
			g_Lexer.CurrToken=TOKEN_TYPE_COMMA;
			break;
			
		case ':':
			g_Lexer.CurrToken=TOKEN_TYPE_COLON;
			break;
			
		case '[':
			g_Lexer.CurrToken=TOKEN_TYPE_OPEN_MIDDLE_BRACKET;
			break;
			
		case ']':
			g_Lexer.CurrToken=TOKEN_TYPE_CLOSE_MIDDLE_BRACKET;
			break;
			
		case '{':
			g_Lexer.CurrToken=TOKEN_TYPE_OPEN_BRACE;
			break;
			
		case '}':
			g_Lexer.CurrToken=TOKEN_TYPE_CLOSE_BRACE;
			break;
			
		case '\n':
			g_Lexer.CurrToken=TOKEN_TYPE_NEWLINE;
			break;
		}
	}
	
	if(TheStringIsInt(g_Lexer.pstrCurrLexeme))
		g_Lexer.CurrToken=TOKEN_TYPE_INT;
	
	if(TheStringIsFloat(g_Lexer.pstrCurrLexeme))
		g_Lexer.CurrToken=TOKEN_TYPE_FLOAT;
	
	if(TheStringIsIdent(g_Lexer.pstrCurrLexeme))
		g_Lexer.CurrToken=TOKEN_TYPE_IDENT;
	
	if(strcmp(g_Lexer.pstrCurrLexeme,"SETSTACKSIZE")==0)
		g_Lexer.CurrToken=TOKEN_TYPE_SETSTACKSIZE;
	
	if(strcmp(g_Lexer.pstrCurrLexeme,"VAR")==0)
		g_Lexer.CurrToken=TOKEN_TYPE_VAR;
	
	if(strcmp(g_Lexer.pstrCurrLexeme,"FUNC")==0)
		g_Lexer.CurrToken=TOKEN_TYPE_FUNC;
	
	if(strcmp(g_Lexer.pstrCurrLexeme,"PARAM")==0)
		g_Lexer.CurrToken=TOKEN_TYPE_PARAM;
	
	if(strcmp(g_Lexer.pstrCurrLexeme,"_RETVAL")==0)
		g_Lexer.CurrToken=TOKEN_TYPE_REG_RETVAL;
	
	INSTRDIRNODE Instr;
	if(GetInstrByMnemonic(&Instr,g_Lexer.pstrCurrLexeme))
		g_Lexer.CurrToken=TOKEN_TYPE_INSTR;
	
	return g_Lexer.CurrToken;
}

int SkipNextLine()
{
	g_Lexer.iCurrSourceCodeLine++;
	
	if(g_Lexer.iCurrSourceCodeLine>=g_iSourceCodeRows)
		return false;
	
	g_Lexer.iStartIndex=g_Lexer.iEndIndex=0;
	
	g_Lexer.iCurrLexerState=LEXER_STATE_SIMPLE_ANALYSE;
	
	return true;
}

void ResetLexer()
{
	g_Lexer.iCurrSourceCodeLine=0;
	
	g_Lexer.iStartIndex=g_Lexer.iEndIndex=0;
	
	g_Lexer.CurrToken=TOKEN_TYPE_INVALID;
	
	g_Lexer.iCurrLexerState=LEXER_STATE_SIMPLE_ANALYSE;
	
}

char GetAndLookAheadAChar()
{
	int itempCurrSourceCodeLine=g_Lexer.iCurrSourceCodeLine;
	unsigned int iStreamIndex=g_Lexer.iEndIndex;
	
	if(g_Lexer.iCurrLexerState!=LEXER_STATE_STRING_ANALYSE_ING)
	{
		while(1)
		{
			if(iStreamIndex>=strlen(g_ppstrSourceCode[itempCurrSourceCodeLine]))
			{
				itempCurrSourceCodeLine++;
				
				if(itempCurrSourceCodeLine>=g_iSourceCodeRows)
					return 0;
				
				iStreamIndex=0;
			}
			
			if(TheCharIsWhitespace(g_ppstrSourceCode[itempCurrSourceCodeLine][iStreamIndex])==false)
				break;
			
			iStreamIndex++;
		}
	}
	
	return g_ppstrSourceCode[itempCurrSourceCodeLine][iStreamIndex];
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//错误处理相关函数

//终止程序的一个函数
void Exit()
{
	system("pause");
	exit(0);
}

//主要负责在汇编过程中通知汇编出错
void ExitOnError(char* pstrErrorMsg)
{
	printf("存在终止解析的错误：%s\n",pstrErrorMsg);
	
	Exit();
}

//主要负责在汇编出错后报告汇编代码具体哪里出错 
void ResultingCodeErrorAndExit(char* pstErrorMsg)
{
	printf("错误：%s.\n\n",pstErrorMsg);
	printf("位于行%d.\n",g_Lexer.iCurrSourceCodeLine);
	
	char ptempstrSourceCodeLine[MAX_SOURCE_CODE_LINE_SIZE];
	strcpy(ptempstrSourceCodeLine,g_ppstrSourceCode[g_Lexer.iCurrSourceCodeLine]);
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(ptempstrSourceCodeLine);iCurrCharIndex++)
		if(ptempstrSourceCodeLine[iCurrCharIndex]=='\t')
			ptempstrSourceCodeLine[iCurrCharIndex]=' ';
	
	printf("%s\n",ptempstrSourceCodeLine);
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<g_Lexer.iStartIndex;iCurrCharIndex++)
		printf(" ");
	printf("^\n");
	
	printf("因为此错误，无法完成文件[%s]的汇编.\n",g_pstrSourceFilename);
	
	Exit();
}

//负责汇编代码出错后报告“缺少所期望的字符”出错信息函数
void CharExpectedErrorAndExit(char c)
{
	char* pstrErrorMsg=(char*)malloc(strlen("字符\' \'缺失"));
	sprintf(pstrErrorMsg,"字符\'%c\'缺失",c);
	
	ResultingCodeErrorAndExit(pstrErrorMsg);
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//

//应该改成size of我觉得是最好的说实话 你直接输数字肯定不是多好
void Load()
{
	FILE* pfExeFile;
	pfExeFile=fopen(g_pstrExecutableFilename,"wb");
	if(pfExeFile==NULL)
		ExitOnError((char*)"无法生成用于保存汇编的可执行文件！");
	
	fwrite(LIAN_FLAG_STRING,4,1,pfExeFile);
	char cVersionMajor=VERSION_MAJOR;
	char cVersionMinor=VERSION_MINOR;
	fwrite(&cVersionMajor,1,1,pfExeFile);
	fwrite(&cVersionMinor,1,1,pfExeFile);
	fwrite(&(g_SourceCodeHeader.iStackSize),4,1,pfExeFile);
	fwrite(&(g_SourceCodeHeader.iGlobalDataSize),4,1,pfExeFile);
	bool cIsHaveMain=false;
	if(g_SourceCodeHeader.iMainFuncPresent==1)
		cIsHaveMain=true;
	fwrite(&cIsHaveMain,1,1,pfExeFile);
	fwrite(&(g_SourceCodeHeader.iMainFuncIndex),4,1,pfExeFile);
	
	fwrite(&g_iInstrStreamRows,4,1,pfExeFile);
	//printf("g_iInstrStreamRows:%d\n",g_iInstrStreamRows);
	for(int iCurrInstrIndex=0;iCurrInstrIndex<g_iInstrStreamRows;iCurrInstrIndex++)
	{
		int sCurrInstrOpCode=g_pInstrStreamFile[iCurrInstrIndex].iOpcode;
		fwrite(&sCurrInstrOpCode,2,1,pfExeFile);
		//printf("sCurrInstrOpCode:%d\n",sCurrInstrOpCode);
		int bCurrOperandCount=g_pInstrStreamFile[iCurrInstrIndex].iOpCount;
		fwrite(&bCurrOperandCount,1,1,pfExeFile);
		//printf("bCurrOperandCount:%d\n",bCurrOperandCount);
		for(int iCurrOperandIndex=0;iCurrOperandIndex<bCurrOperandCount;iCurrOperandIndex++)
		{
			OP CurrOperand=g_pInstrStreamFile[iCurrInstrIndex].pOpList[iCurrOperandIndex];
			int bOperandType=CurrOperand.iType;
			fwrite(&bOperandType,1,1,pfExeFile);
			
			switch(CurrOperand.iType)
			{
			case OP_TYPE_INT:
				{
					fwrite(&(CurrOperand.iIntLiteralValue),sizeof(int),1,pfExeFile);
					break;
				}
			case OP_TYPE_FLOAT:
				{
					fwrite(&(CurrOperand.fFloatLiteralValue),sizeof(float),1,pfExeFile);
					break;
				}
			case OP_TYPE_STRING_INDEX:
				{
					fwrite(&(CurrOperand.iStringTableIndex),sizeof(STRING_TABLE_INDEX_SIZE),1,pfExeFile);
					break;
				}
			case OP_TYPE_INSTR_LOCATION:
				{
					fwrite(&(CurrOperand.iInstrNoodlesIndex),sizeof(INSTR_LOCATION_VALUE),1,pfExeFile);
					break;
				}
			case OP_TYPE_ABS_STACK_INDEX:
				{
					fwrite(&(CurrOperand.iStackUnitIndex),sizeof(STACK_INDEX_SIZE),1,pfExeFile);
					break;
				}
			case OP_TYPE_REL_STACK_INDEX:
				{
					fwrite(&(CurrOperand.iStackUnitIndex),sizeof(STACK_INDEX_SIZE),1,pfExeFile);
					fwrite(&(CurrOperand.iAddressingOffset),sizeof(OFFSET_INDEX_SIZE),1,pfExeFile);
					break;
				}
			case OP_TYPE_FUNC_INDEX:
				{
					fwrite(&(CurrOperand.iFuncIndex),sizeof(FUNCTION_INDEX_SIZE),1,pfExeFile);
					break;
				}
			case OP_TYPE_HOST_API_CALL_INDEX:
				{
					fwrite(&(CurrOperand.iHostAPICallIndex),sizeof(HOSTAPIFUNCTION_INDEX_SIZE),1,pfExeFile);
					break;
				}
			case OP_TYPE_REG:
				{
					fwrite(&(CurrOperand.iRegisterIndex),sizeof(REGISTER_INDEX_SIZE),1,pfExeFile);
					break;
				}
			}
		}
	}
	
	
	LINKEDLISTNODE* pNode;
	
	pNode=g_StringTable.pListHead;
	fwrite(&(g_StringTable.iNodeCount),4,1,pfExeFile);
	
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_StringTable.iNodeCount;iCurrNodeIndex++)
	{
		char* pstrCurrString=(char*)pNode->pvData;
		int iCurrStringLength=strlen(pstrCurrString);
		fwrite(&iCurrStringLength,sizeof(int),1,pfExeFile);

		//为什么不用iCurrStringLength，是因为这个是int，而fwrite是size_t类型
		fwrite(pstrCurrString,strlen(pstrCurrString),1,pfExeFile);
		pNode=pNode->pNext;
	}
	
	
	pNode=g_FunctionTable.pListHead;
	fwrite(&(g_FunctionTable.iNodeCount),sizeof(int),1,pfExeFile);
	
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_FunctionTable.iNodeCount;iCurrNodeIndex++)
	{
		FUNCNODE* pFunc=(FUNCNODE*)pNode->pvData;
		int iEntryPoint=pFunc->iEntryPoint;
		fwrite(&iEntryPoint,sizeof(int),1,pfExeFile);
		int iParamCount=pFunc->iParamCount;
		fwrite(&iParamCount,sizeof(int),1,pfExeFile);
		int iLocalDataSize=pFunc->iLocalDataSize;
		fwrite(&iLocalDataSize,sizeof(int),1,pfExeFile);
		
		pNode=pNode->pNext;
	}
	
	
	pNode=g_HostAPICallTable.pListHead;
	fwrite(&(g_HostAPICallTable.iNodeCount),sizeof(int),1,pfExeFile);
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_HostAPICallTable.iNodeCount;iCurrNodeIndex++)
	{
		char* pstrCurrHOSTAPI=(char*)pNode->pvData;
		byte bCurrHOSTAPILength=strlen(pstrCurrHOSTAPI);
		fwrite(&bCurrHOSTAPILength,sizeof(byte),1,pfExeFile);
		fwrite(pstrCurrHOSTAPI,bCurrHOSTAPILength,1,pfExeFile);
		
		pNode=pNode->pNext;
	}
	
	fclose(pfExeFile);
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//操纵函数控制块相关函数

//创建一个函数控制块 
FUNCCONTROL CreateFuncContorlBlock()
{
	FUNCCONTROL Block;
	Block.iIsInFunc=false;
	Block.iCurrFuncParamCount=0;
	Block.iCurrFuncLocalDataSize=0;
	
	return Block;
}

//重置当前函数控制块的所有状态 
void ResetFuncControlBlock(FUNCCONTROL* Block)
{
	Block->iIsInFunc=false;
	Block->iCurrFuncParamCount=0;
	Block->iCurrFuncLocalDataSize=0;
}
//***** ***** ***** ***** ***** ***** ***** ***** 


//***** ***** ***** ***** ***** ***** ***** ***** 
//语法分析阶段相关函数
void Parsing()
{
	g_SourceCodeHeader.iStackSize=0;
	g_SourceCodeHeader.iMainFuncPresent=No;
	
	g_iInstrStreamRows=0;
	g_iSetStackSizeIsFound=false;
	g_SourceCodeHeader.iGlobalDataSize=0;
	
	FUNCCONTROL FuncContorlBlock=CreateFuncContorlBlock();
	
	INSTRDIRNODE CurrInstr;
	
	ResetLexer();
	
	while(1)
	{
		if(GetNextToken()==END_OF_TOKEN_STREAM)
		{
			if(FuncContorlBlock.iIsInFunc==true)
				CharExpectedErrorAndExit('}');
			break;
		}
		
		//printf("token:%d lexeme:%s\n",g_Lexer.CurrToken,g_Lexer.pstrCurrLexeme);
		//printf("lextop:%d lextail:%d line:%d\n\n",g_Lexer.iStartIndex,g_Lexer.iEndIndex,g_Lexer.iCurrSourceCodeLine);
		
		
		switch (g_Lexer.CurrToken) 
		{
		case TOKEN_TYPE_SETSTACKSIZE:
			{
				if(FuncContorlBlock.iIsInFunc==true)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_SETSTACKSIZE_IN_GLOBAL);
				
				if(g_iSetStackSizeIsFound==true)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_REPEAT_DEFINE_SETSTACKSIZES);
				
				if(GetNextToken()!=TOKEN_TYPE_INT)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_STACK_SIZE);
				
				g_SourceCodeHeader.iStackSize=atoi(GetCurrLexeme());
				
				g_iSetStackSizeIsFound=true;
				
				break;
			}
		case TOKEN_TYPE_FUNC:
			{
				if(FuncContorlBlock.iIsInFunc==true)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_NESTED_FUNC);
				
				if(GetNextToken()!=TOKEN_TYPE_IDENT)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_EXPECTED);
				char* pstrNowFuncName=GetCurrLexeme();
				
				int iFuncEntryPoint=g_iInstrStreamRows;
				
				int iFuncIndex=AddFunc(pstrNowFuncName,iFuncEntryPoint);
				if(iFuncIndex==-1)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_FUNC_REDEFINITION);
				
				if(strcmp(pstrNowFuncName,__START__FUNC)==0)
				{
					g_SourceCodeHeader.iMainFuncPresent=Yes;
					g_SourceCodeHeader.iMainFuncIndex=iFuncIndex;
				}
				
				FuncContorlBlock.iIsInFunc=true;
				strcpy(FuncContorlBlock.pstrCurrFuncName,pstrNowFuncName);
				FuncContorlBlock.iCurrFuncIndex=iFuncIndex;
				FuncContorlBlock.iCurrFuncParamCount=0;
				FuncContorlBlock.iCurrFuncLocalDataSize=0;
				
				while(GetNextToken()==TOKEN_TYPE_NEWLINE);
				
				if(g_Lexer.CurrToken!=TOKEN_TYPE_OPEN_BRACE)
					CharExpectedErrorAndExit('{');
				
				g_iInstrStreamRows++;
				
				break;
			}
		case TOKEN_TYPE_CLOSE_BRACE:
			{
				//本来想会不会有那种情况func a{  func b{}这样就也会编译成功，但是实际上b这里会出错
				if(FuncContorlBlock.iIsInFunc==false)
					CharExpectedErrorAndExit('}');
				
				SetFuncInfo(FuncContorlBlock.pstrCurrFuncName,FuncContorlBlock.iCurrFuncParamCount,FuncContorlBlock.iCurrFuncLocalDataSize);
				
				FuncContorlBlock.iIsInFunc=false;
				
				break;
			}
		case TOKEN_TYPE_VAR:
			{
				if(GetNextToken()!=TOKEN_TYPE_IDENT)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_EXPECTED);
				char pstrVarName[MAX_IDENT_SIZE];
				strcpy(pstrVarName,GetCurrLexeme());
				
				int iVarSize=1;
				if(GetAndLookAheadAChar()=='[')
				{
					if(GetNextToken()!=TOKEN_TYPE_OPEN_MIDDLE_BRACKET)
						CharExpectedErrorAndExit('[');
					if(GetNextToken()!=TOKEN_TYPE_INT)
						ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY_INDEX);
					iVarSize=atoi(GetCurrLexeme());
					if(iVarSize<=0)
						ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY_SIZE);
					if(GetNextToken()!=TOKEN_TYPE_CLOSE_MIDDLE_BRACKET)
						CharExpectedErrorAndExit(']');
				}
				
				int iStackUnitIndex;
				if(FuncContorlBlock.iIsInFunc==true)
					iStackUnitIndex=-(FuncContorlBlock.iCurrFuncLocalDataSize+2);
				else
					iStackUnitIndex=g_SourceCodeHeader.iGlobalDataSize;
				
				if(AddSymbol(pstrVarName,iVarSize,iStackUnitIndex,FuncContorlBlock.iCurrFuncIndex)==-1)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_REDEFINITION);
				
				if(FuncContorlBlock.iIsInFunc==true)
					FuncContorlBlock.iCurrFuncLocalDataSize+=iVarSize;
				else
					g_SourceCodeHeader.iGlobalDataSize+=iVarSize;
				
				break;
			}
		case TOKEN_TYPE_PARAM:
			{
				if(GetNextToken()!=TOKEN_TYPE_IDENT)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_EXPECTED);
				
				char* pstrParamName=GetCurrLexeme();
				int iParamInStackUnitIndex=-(FuncContorlBlock.pCurrFunc->iLocalDataSize+2+(FuncContorlBlock.iCurrFuncParamCount+1));
				
				if(AddSymbol(pstrParamName,1,iParamInStackUnitIndex,FuncContorlBlock.iCurrFuncIndex)==-1)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_REDEFINITION);
				FuncContorlBlock.iCurrFuncParamCount++;
				
				break;
			}
			//不过这里处理的实际上是把传入的东西识别成行标签进行对比 
			//而把它翻译成标识符或者直接归类到标识符这么个大类里面总感觉是不妥的
			
			//你会和我一样疑惑，为什么会是这样，明明标识符范围包括的很广，各种各样，为什么只在这里说明
			//或者按照label进行检测呢？
			//因为语法，汇编的语法其实是非常的简单的 仔细思考一下就能发现，在第一轮的一个case句
			//就相当于是检测了一行代码，而不是就仅按照case这个条件语句后面跟着的那个token只分析那一个 
			//汇编的语法非常简单，说来说去除去汇编中其他几个指示关键字外 
			//我们将所有其他的指令关键字全都集中在下面那个case当中 
			//指示关键字它后面所跟的标识符在分析到指示关键字时就已经被自动捕获分析了那么我们就不需要再去管了 
			//接着除行标签外 剩下唯一需要考虑的是指令关键字后面所跟的标识符我们该怎么分析 
			//可是getnexttoken函数 会在捕获到字符串时就将该串与指令关键字进行比较
			//基本上所有的汇编语言指令关键字一定是放在指令流 的行首位位置的 
			//所以后面跟着的标识符也当然不需要再去管了 
			//再观察一下 指令token 它所对应的case语句 也就是说我们如果识别到指令token 就会直接跳到下一行了
			//这也就是为何这么设计的原因 
		case TOKEN_TYPE_IDENT:
			{
				if(GetAndLookAheadAChar()!=':')
					ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_INSTR);
				if(FuncContorlBlock.iIsInFunc==false)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_GLOBAL_LINE_LABEL);
				
				char* pstrNowLabelName=GetCurrLexeme();
				int iTargetIndx=g_iInstrStreamRows-1;
				
				if(AddLabel(pstrNowLabelName,iTargetIndx,FuncContorlBlock.iCurrFuncIndex)==-1)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_LINE_LABEL_REDEFINITION);
				
				break;
			}
			//我们单独开设了另一个检测的通道,从直译上面来看这是指令
			//但是又会让人很疑惑为什么检测指令的部分它的代码为什么这么短 
			//而且上面的那些东西VARFUC难道不也是指令吗 很巧的是这些东西确实不算是指令 
			//至少在我们这个规定中确实是不算的,从feeling上就很容易能看出来它叫做指示关键字
			//它其实这些东西呢是代表着我们这个汇编的一个框架,或者说是一个基本结构 而具体的指令实际上是用户他所的那些代码 
			//你可以跳转到指令关键字所对应的部分,就可以很明显的发觉两个东西的 差别 
			//而且也很明显的就能发现在第一遍的遍历中 其实也就是对这些个token指示关键字 简单语法分析一遍,
			//无非就是看看你这个声明定义的是不是对了 那么也很容易发现在第一遍的遍历中很多东西都可以浑水摸鱼地通过去 
			//因为他们都会被归类到INstr这一部分内，不过第二遍就会 开始关注用户所写的那些代码他的 语法是否正确
		case TOKEN_TYPE_INSTR:
			{
				if(FuncContorlBlock.iIsInFunc==false)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_GLOBAL_INSTR);
				
				g_iInstrStreamRows++;
				
				break;
			}
		default:
			if(g_Lexer.CurrToken !=TOKEN_TYPE_NEWLINE)
				ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_INPUT);
		}
		
		//一个是检查是不是读完了另一个方面呢就是换行了了读下一行的内容了
		if(!SkipNextLine())
			break;
	}
	
	g_pInstrStreamFile=(INSTRUCTION*)malloc(g_iInstrStreamRows*sizeof(INSTRUCTION));
	for(int iCurrInstrIndex=0;iCurrInstrIndex<g_iInstrStreamRows;iCurrInstrIndex++)
		g_pInstrStreamFile[iCurrInstrIndex].pOpList=NULL;
	
	g_iCurrInstrIndex=0;
	
	ResetLexer();
	
	//为什么语法分析阶段我们选择了二次遍历呢 
	//或者说思考思考如果不选择二次遍历会怎么样呢 
	//首先语法分析顾名思义就是验证语法的正确性 所要考虑的点很多 我们可以随便就能列举出来很多 
	//比如 是否使用了未定义的变量名还有包括是否是使用了未定义的其他任何标识符 
	//是否是按照了我们规定的指令语法结构进行书写呢 
	//他这个指令写的是否正确呢 
	
	//但实际上呢我个人认为它是没有必要二次遍历的 
	//因为大部分人首先在书写高级语言代码时都已经习惯了先定义后使用的这么一个规则 
	//而且汇编语言的这个语法结构又这么简单
	//你哪怕一边搜集信息 一边验证语法的正误其实都没有任何问题 
	//对于他这个两次循环呢第一次他实际上就是搜集信息 其他就没有任何的了 而第二遍才是验证这个语法的正确性 
	//我个人是认为是可以简化的
	while(1)
	{
		if(GetNextToken()==END_OF_TOKEN_STREAM)
			break;
		
		//printf("token:%d lexeme:%s\n",g_Lexer.CurrToken,g_Lexer.pstrCurrLexeme);
		//printf("lextop:%d lextail:%d line:%d\n\n",g_Lexer.iStartIndex,g_Lexer.iEndIndex,g_Lexer.iCurrSourceCodeLine);
		
		
		
		switch(g_Lexer.CurrToken)
		{
		case TOKEN_TYPE_FUNC:
			{
				GetNextToken();
				
				ResetFuncControlBlock(&FuncContorlBlock);
				FuncContorlBlock.pCurrFunc=GetFuncByName(GetCurrLexeme());
				FuncContorlBlock.iIsInFunc=true;
				FuncContorlBlock.iCurrFuncParamCount=0;
				FuncContorlBlock.iCurrFuncIndex=FuncContorlBlock.pCurrFunc->iIndex;
				
				while(GetNextToken()==TOKEN_TYPE_NEWLINE);
				
				break;
			}
		case TOKEN_TYPE_CLOSE_BRACE:
			{
				FuncContorlBlock.iIsInFunc=false;
				
				if(strcmp(FuncContorlBlock.pCurrFunc->pstrName,__START__FUNC)==0)
				{
					g_pInstrStreamFile[g_iCurrInstrIndex].iOpcode=INSTR_EXIT;
					g_pInstrStreamFile[g_iCurrInstrIndex].iOpCount=1;
					g_pInstrStreamFile[g_iCurrInstrIndex].pOpList=(OP*)malloc(1*sizeof(OP));
					g_pInstrStreamFile[g_iCurrInstrIndex].pOpList[0].iType=OP_TYPE_INT;
					g_pInstrStreamFile[g_iCurrInstrIndex].pOpList[0].iIntLiteralValue=0;
				}
				else
				{
					g_pInstrStreamFile[g_iCurrInstrIndex].iOpcode=INSTR_RET;
					g_pInstrStreamFile[g_iCurrInstrIndex].iOpCount=0;
					g_pInstrStreamFile[g_iCurrInstrIndex].pOpList=NULL;
				}
				
				++g_iCurrInstrIndex;
				break;
			}
		case TOKEN_TYPE_PARAM:
			{
				if(GetNextToken()!=TOKEN_TYPE_IDENT)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_EXPECTED);
				
				char* pstrParamIdent=GetCurrLexeme();
				
				int iStackIndex=-(FuncContorlBlock.pCurrFunc->iLocalDataSize+2+(FuncContorlBlock.iCurrFuncParamCount+1));
				if(AddSymbol(pstrParamIdent,1,iStackIndex,FuncContorlBlock.iCurrFuncIndex)==-1)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_IDENT_REDEFINITION);
				
				FuncContorlBlock.iCurrFuncParamCount++;
				
				break;
			}
		case TOKEN_TYPE_INSTR:
			{
				//不是，这玩意儿居然都不检查一下指令字是不是正确的吗？？？？？？
				//？？？？
				GetInstrByMnemonic(&CurrInstr,GetCurrLexeme());
				
				g_pInstrStreamFile[g_iCurrInstrIndex].iOpcode=CurrInstr.iOpcode;
				g_pInstrStreamFile[g_iCurrInstrIndex].iOpCount=CurrInstr.iOpCount;
				//printf("CurrInstr.iOpcode:%d\n",CurrInstr.iOpcode);
				//printf("CurrInstr.iOpCount:%d\n",CurrInstr.iOpCount);
				OP* pOpList=(OP*)malloc(CurrInstr.iOpCount*sizeof(OP));
				
				for(int iCurrOpIndex=0;iCurrOpIndex<CurrInstr.iOpCount;iCurrOpIndex++)
				{
					//命名的意思是curr op types bit cmb
					allow_op_type_bit_cmb CurrOpTsBCMB=CurrInstr.OpList[iCurrOpIndex];
					
					Token InitOpToken=GetNextToken();
					//printf("token:%d lexeme:%s\n",g_Lexer.CurrToken,g_Lexer.pstrCurrLexeme);
					//printf("lextop:%d lextail:%d line:%d\n\n",g_Lexer.iStartIndex,g_Lexer.iEndIndex,g_Lexer.iCurrSourceCodeLine);
					switch(InitOpToken)
					{
					case TOKEN_TYPE_INT:
						{
							if(CurrOpTsBCMB & OP_FLAG_INT_BIT)
							{
								pOpList[iCurrOpIndex].iType=OP_TYPE_INT;
								pOpList[iCurrOpIndex].iIntLiteralValue=atoi(GetCurrLexeme());
							}
							else
								ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_OP);
							
							break;
						}
					case TOKEN_TYPE_FLOAT:
						{
							if(CurrOpTsBCMB & OP_FLAG_FLOAT_BIT)
							{
								pOpList[iCurrOpIndex].iType=OP_TYPE_FLOAT;
								pOpList[iCurrOpIndex].fFloatLiteralValue=(float)atof(GetCurrLexeme());
							}
							else
								ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_OP);
							
							break;
						}
					case TOKEN_TYPE_QUOTE:
						{
							if(CurrOpTsBCMB & OP_FLAG_STRING_BIT)
							{
								GetNextToken();
								
								switch(g_Lexer.CurrToken)
								{
								case TOKEN_TYPE_QUOTE:
									{
										pOpList[iCurrOpIndex].iType=OP_TYPE_INT;
										pOpList[iCurrOpIndex].iIntLiteralValue=0;
										break;
									}
								case TOKEN_TYPE_STRING:
									{
										char* pstrString=GetCurrLexeme();
										
										int iCurrStringIndex=AddString(&g_StringTable,pstrString);
										if(GetNextToken()!=TOKEN_TYPE_QUOTE)
											CharExpectedErrorAndExit('\\');
										
										pOpList[iCurrOpIndex].iType=OP_TYPE_STRING_INDEX;
										pOpList[iCurrOpIndex].iStringTableIndex=iCurrStringIndex;
										
										break;
									}
								default:
									ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_STRING);
								}
							}
							else
								ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_OP);
							
							break;
						}
					case TOKEN_TYPE_REG_RETVAL:
						{
							if(CurrOpTsBCMB & OP_FLAG_REG_BIT)
							{
								pOpList[iCurrOpIndex].iType=OP_TYPE_REG;
								pOpList[iCurrOpIndex].iRegisterIndex=0;
							}
							else
								ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_OP);
							
							break;
						}
					case TOKEN_TYPE_IDENT:
						{
							if(CurrOpTsBCMB & OP_FLAG_MEM_REF_BIT)
							{
								char pstrIdent[MAX_IDENT_SIZE];
								strcpy(pstrIdent,GetCurrLexeme());
								
								if(GetSymbolByIndex(pstrIdent,FuncContorlBlock.iCurrFuncIndex)==NULL)
									ResultingCodeErrorAndExit((char*)ERROR_MSG_UNDEFINED_IDENT);
								
								int iIdentBaseIndex=GetSymbolStackUnitIndexByIdent(pstrIdent,FuncContorlBlock.iCurrFuncIndex);
								
								if(GetAndLookAheadAChar()!='[')
								{
									if(GetSymbolSizeByIdent(pstrIdent,FuncContorlBlock.iCurrFuncIndex)>1)
										ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY_NOT_INDEXED);
									
									pOpList[iCurrOpIndex].iType=OP_TYPE_ABS_STACK_INDEX;
									pOpList[iCurrOpIndex].iIntLiteralValue=iIdentBaseIndex;
								}
								else
								{
									if(GetSymbolSizeByIdent(pstrIdent,FuncContorlBlock.iCurrFuncIndex)==1)
										ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY);
									if(GetNextToken()!=TOKEN_TYPE_OPEN_MIDDLE_BRACKET)
										CharExpectedErrorAndExit('[');
									
									//在原文中一直使用的是offset以及index侧重于强调它的偏移量的这么一个效果 
									//但是我总认为它其实又能代表的是一个size之类的 也许我们后期可以再改改
									//index意味着索引那么我们就要详细思考一下有关于 语言中C语言中 数组的那个索引具体是什么意思 
									Token IdentSizeToken=GetNextToken();
									
									if(IdentSizeToken==TOKEN_TYPE_INT)
									{
										int iIdentSize=atoi(GetCurrLexeme());
										
										pOpList[iCurrOpIndex].iType=OP_TYPE_ABS_STACK_INDEX;
										pOpList[iCurrOpIndex].iStackUnitIndex=iIdentBaseIndex+iIdentSize;
									}
									else if(IdentSizeToken==TOKEN_TYPE_IDENT)
									{
										char* pstrIdentName=GetCurrLexeme();
										
										if(GetSymbolByIndex(pstrIdentName,FuncContorlBlock.iCurrFuncIndex))
											ResultingCodeErrorAndExit((char*)ERROR_MSG_UNDEFINED_IDENT);
										if(GetSymbolSizeByIdent(pstrIdentName,FuncContorlBlock.iCurrFuncIndex)>1)
											ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY_INDEX);
										
										int iOffsetIndex=GetSymbolStackUnitIndexByIdent(pstrIdentName,FuncContorlBlock.iCurrFuncIndex);
										
										pOpList[iCurrOpIndex].iType=OP_TYPE_REL_STACK_INDEX;
										pOpList[iCurrOpIndex].iStackUnitIndex=iIdentBaseIndex;
										//奇了怪了为什么定义这个结构的时候为什么不按照原文定义成offset 而是用addressing呢
										pOpList[iCurrOpIndex].iAddressingOffset=iOffsetIndex;
									}
									else 
									{
										ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_ARRAY_INDEX);
									}
									
									if(GetNextToken()!=TOKEN_TYPE_CLOSE_MIDDLE_BRACKET)
										CharExpectedErrorAndExit('[');
								}
							}
							
							if(CurrOpTsBCMB & OP_FLAG_LINE_LABEL_BIT)
							{
								char* pstrLabelIdent=GetCurrLexeme();
								
								LABELNODE* pLabelNode=GetLabelByIdent(pstrLabelIdent,FuncContorlBlock.iCurrFuncIndex);
								if(pLabelNode==NULL)
									ResultingCodeErrorAndExit((char*)ERROR_MSG_UNDEFINED_LINE_LABEL);
								
								pOpList[iCurrOpIndex].iType=OP_TYPE_INSTR_LOCATION;
								pOpList[iCurrOpIndex].iInstrNoodlesIndex=pLabelNode->iTargetIndex;
							}
							
							if(CurrOpTsBCMB & OP_FLAG_FUNC_NAME_BIT)
							{
								char* pstrFunctionIdent=GetCurrLexeme();
								
								FUNCNODE* pFuncNode=GetFuncByName(pstrFunctionIdent);
								if(pFuncNode==NULL)
									ResultingCodeErrorAndExit((char*)ERROR_MSG_UNDEFINED_FUNC);
								
								pOpList[iCurrOpIndex].iType=OP_TYPE_FUNC_INDEX;
								pOpList[iCurrOpIndex].iFuncIndex=pFuncNode->iIndex;
							}
							
							if(CurrOpTsBCMB & OP_FLAG_HOST_API_CALL_BIT)
							{
								char* pstrHostAPICallIdent=GetCurrLexeme();
								
								int iHostAPIIndex=AddString(&g_HostAPICallTable,pstrHostAPICallIdent);
								pOpList[iCurrOpIndex].iType=OP_TYPE_HOST_API_CALL_INDEX;
								pOpList[iCurrOpIndex].iHostAPICallIndex=iHostAPIIndex;
							}
							
							break;
						}
					default:
						{
							ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_OP);
							
							break;
						}
					}
					
					if(iCurrOpIndex<(CurrInstr.iOpCount-1))
						if(GetNextToken()!=TOKEN_TYPE_COMMA)
							CharExpectedErrorAndExit(',');
				}
				
				if(GetNextToken()!=TOKEN_TYPE_NEWLINE)
					ResultingCodeErrorAndExit((char*)ERROR_MSG_INVALID_INPUT);
				
				g_pInstrStreamFile[g_iCurrInstrIndex].pOpList=pOpList;
				
				g_iCurrInstrIndex++;
				
				break;
			}
		}
		
		if(SkipNextLine()==false)
			break;
	}
}

int ReturnFuncEntryPoint(FUNCNODE* pf)
{
	return pf->iEntryPoint;
}
//***** ***** ***** ***** ***** ***** ***** ***** 
