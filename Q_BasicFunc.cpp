#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <conio.h>
#include <math.h>

#include"Q_BasicStructAndMacro.cpp"
#include"Q_LogoPrint.cpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//未赋值就直接使用真可怕

int GetOperandType(int iOpIndex);
VARDATA GetStackValue(int iIndex);
int CoerceValueToInt(VARDATA v);
float CoerceValueToFloat(VARDATA v);
char* CoerceValueToString(VARDATA v);
int ResolveOperandStackIndex(int iOpIndex);
VARDATA ResolveOperandValue(int iOpIndex);
int ResolveOpAsInt(int iOpIndex);
VARDATA* ResolveOpPointer(int iOpIndex);
float ResolveOpAsFloat(int iOpIndex);
char* ResolveOpAsString(int iOpIndex);
void Push(VARDATA v);
VARDATA Pop();
void CopyValue(VARDATA* pDest,VARDATA Source);
FUNCTION GetFunction(int iIndex);
char* GetAPI(int iIndex);
void ResetFile();
void RunFile();
int ResolveOpAsInstrIndex(int iOpIndex);
char* ResolveOpAsAPI(int iOpIndex);
int ResolveOpAsFuncIndex(int iOpIndex);


#define ResolveStackIndex( iIndex )	\
( iIndex < 0 ? iIndex += g_FileControl.Stack.iFrameIndex : iIndex )  

int GetOperandType(int iOpIndex)
{
	int iCurrInstr=g_FileControl.InstrStream.iCurrInstrIndex;
	
	return g_FileControl.InstrStream.pInstrs[iCurrInstr].pOpList[iOpIndex].iType;
}
VARDATA GetStackValue(int iIndex)
{
	return g_FileControl.Stack.pElem[ResolveStackIndex(iIndex)];
}
void SetStackValue(int iIndex,VARDATA v)
{
	g_FileControl.Stack.pElem[ResolveStackIndex(iIndex)]=v;
}
int CoerceValueToInt(VARDATA v)
{
	switch(v.iType)
	{
	case OP_TYPE_INT:
		return v.iIntLiteralValue;
	case OP_TYPE_FLOAT:
		return (int)v.fFloatLiteralValue;
	case OP_TYPE_STRING:
		return atoi(v.pstrStringLiteralValue);
	default:
		return 0;
	}
}
float CoerceValueToFloat(VARDATA v)
{
	switch(v.iType)
	{
	case OP_TYPE_FLOAT:
		return v.fFloatLiteralValue;
	case OP_TYPE_INT:
		return (float)v.iIntLiteralValue;
	case OP_TYPE_STRING:
		return (float)atof(v.pstrStringLiteralValue);
	default:
		return 0;
	}
}
char* CoerceValueToString(VARDATA v)
{
	char* pstrCoercion;
	if(v.iType!=OP_TYPE_STRING)
		pstrCoercion=(char*)malloc((MAX_COERCION_STRING_SIZE+1)*sizeof(char));
	
	switch(v.iType)
	{
	case OP_TYPE_INT:
		{
			itoa(v.iIntLiteralValue,pstrCoercion,10);
			return pstrCoercion;
		}
	case OP_TYPE_FLOAT:
		{
			sprintf(pstrCoercion,"%f",v.fFloatLiteralValue);
			return pstrCoercion;
		}
	case OP_TYPE_STRING:
		return v.pstrStringLiteralValue;
	default:
		return NULL;
	}
}

//resolve是解析的意思 
int ResolveOperandStackIndex(int iOpIndex)
{
	int iCurrInstr=g_FileControl.InstrStream.iCurrInstrIndex;
	
	VARDATA OpValue=g_FileControl.InstrStream.pInstrs[iCurrInstr].pOpList[iOpIndex];
	
	switch(OpValue.iType)
	{
	case OP_TYPE_ABS_STACK_INDEX:
		return OpValue.iStackUnitIndex;
	case OP_TYPE_REL_STACK_INDEX:
		{
			int iBaseIndex=OpValue.iStackUnitIndex;
			int iOffsetIndex=OpValue.iAddressingOffset;
			
			VARDATA StackValue=GetStackValue(iOffsetIndex);
			
			return iBaseIndex+StackValue.iIntLiteralValue;
		}
	default:
		return 0;
	}
}
VARDATA ResolveOperandValue(int iOpIndex)
{
	int iCurrInstr=g_FileControl.InstrStream.iCurrInstrIndex;
	
	VARDATA Operand=g_FileControl.InstrStream.pInstrs[iCurrInstr].pOpList[iOpIndex];
	
	switch(Operand.iType)
	{
	case OP_TYPE_ABS_STACK_INDEX:
	case OP_TYPE_REL_STACK_INDEX:
		{
			int iAbsIndex=ResolveOperandStackIndex(iOpIndex);
			return GetStackValue(iAbsIndex);
		}
	case OP_TYPE_REG:
		return g_FileControl._Reg;
	default:
		return Operand;
	}
}
int ResolveOpAsInt(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	int iInt=CoerceValueToInt(OpValue);
	return iInt;
}
VARDATA* ResolveOpPointer(int iOpIndex)
{
	int iIndirMethod=GetOperandType(iOpIndex);
	
	switch(iIndirMethod)
	{
	case OP_TYPE_ABS_STACK_INDEX:
	case OP_TYPE_REL_STACK_INDEX:
		{
			int iStackIndex=ResolveOperandStackIndex(iOpIndex);
			return &g_FileControl.Stack.pElem[ResolveStackIndex(iStackIndex)];
		}
	case OP_TYPE_REG:
		return &(g_FileControl._Reg);
	default:
		return NULL;
	}
}
char* ResolveOpAsString(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	char* pstr=CoerceValueToString(OpValue);
	return pstr;
}
float ResolveOpAsFloat(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	float fFloat=CoerceValueToFloat(OpValue);
	return fFloat;
}
int ResolveOpAsInstrIndex(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	return OpValue.iInstrIndex;
}
char* ResolveOpAsAPI(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	int iAPIIndex=OpValue.iHostAPICallIndex;
	return GetAPI(iAPIIndex);
}
int ResolveOpAsFuncIndex(int iOpIndex)
{
	VARDATA OpValue=ResolveOperandValue(iOpIndex);
	
	return OpValue.iFuncIndex;
}
void Push(VARDATA v)
{
	int iTopIndex=g_FileControl.Stack.iTopElemIndex;
	g_FileControl.Stack.pElem[iTopIndex]=v;
	
	g_FileControl.Stack.iTopElemIndex++;
}
VARDATA Pop()
{
	g_FileControl.Stack.iTopElemIndex--;
	
	int iTopIndex=g_FileControl.Stack.iTopElemIndex;
	VARDATA v=g_FileControl.Stack.pElem[iTopIndex];
	
	return v;
}
void PushFrame(int iSize)
{
	g_FileControl.Stack.iTopElemIndex+=iSize;
	
	g_FileControl.Stack.iFrameIndex=g_FileControl.Stack.iTopElemIndex;
}
void PopFrame(int iSize)
{
	g_FileControl.Stack.iFrameIndex-=iSize;
}
void CopyValue(VARDATA* pDest,VARDATA Source)
{
	if(pDest->iType==OP_TYPE_STRING)
		free(pDest->pstrStringLiteralValue);
	
	*pDest=Source;
	
	if(Source.iType==OP_TYPE_STRING)
	{
		pDest->pstrStringLiteralValue=(char*)malloc((strlen(Source.pstrStringLiteralValue)+1)*sizeof(char));
		strcpy(pDest->pstrStringLiteralValue,Source.pstrStringLiteralValue);
	}
}
FUNCTION GetFunction(int iIndex)
{
	return g_FileControl.pFuncTable[iIndex];
}
char* GetAPI(int iIndex)
{
	return g_FileControl.HostAPICallTable.ppstrHOSTAPI[iIndex];
}
void ResetFile()
{
	int iMainFuncIndex=g_FileControl.iMainFuncIndex;
	
	if(g_FileControl.pFuncTable)
		if(g_FileControl.iIsMainFuncPresent)
			g_FileControl.InstrStream.iCurrInstrIndex=g_FileControl.pFuncTable[iMainFuncIndex].iEntryPoint;
	
	g_FileControl.Stack.iTopElemIndex=0;
	g_FileControl.Stack.iFrameIndex=0;
	
	for(int iCurrElemIndex=0;iCurrElemIndex<g_FileControl.Stack.iStackSize;iCurrElemIndex++)
		g_FileControl.Stack.pElem[iCurrElemIndex].iType=OP_TYPE_NULL;
	
	g_FileControl.iIsPaused=false;
	
	PushFrame(g_FileControl.iGlobalDataSize);
	
	PushFrame(g_FileControl.pFuncTable[iMainFuncIndex].iStackFrameSize+1);
}
int GetCurrTime()
{
	return GetTickCount();
}
void  PrintOpValue(int iOpIndex)
{
	VARDATA Op=ResolveOperandValue(iOpIndex);
	switch ( Op.iType )
	{
	case OP_TYPE_NULL:
		printf ( "Null" );
		break;
		
	case OP_TYPE_INT:
		printf ( "%d", Op.iIntLiteralValue );
		break;
		
	case OP_TYPE_FLOAT:
		printf ( "%f", Op.fFloatLiteralValue );
		break;
		
	case OP_TYPE_STRING:
		printf ( "\"%s\"", Op.pstrStringLiteralValue );
		break;
		
	case OP_TYPE_INSTR_LOCATION:
		printf ( "%d", Op.iInstrIndex );
		break;
		
	case OP_TYPE_HOST_API_CALL_INDEX:
		{
			char * pstrHostAPICall = ResolveOpAsAPI ( iOpIndex );
			printf ( "%s", pstrHostAPICall );
			break;
		}
	}
}
void PrintOpIndir(int iOpIndex)
{
	int iIndirMethod = GetOperandType ( iOpIndex );
	
	// Print it out
	
	switch ( iIndirMethod )
	{
		// It's _RetVal
		
	case OP_TYPE_REG:
		printf ( "_RetVal" );
		break;
		
		// It's on the stack
		
	case OP_TYPE_ABS_STACK_INDEX:
	case OP_TYPE_REL_STACK_INDEX:
		{
			int iStackIndex = ResolveOperandStackIndex ( iOpIndex );
			printf ( "[ %d ]", iStackIndex );
			break;
		}
	}
}
void RunFile()
{
	int iExitExecLoop=false;
	
	while(!kbhit())
	{		
		if(g_FileControl.iIsPaused)
		{
			if(GetCurrTime() >= g_FileControl.iPauseEndTime)
				g_FileControl.iIsPaused=false;
			else
				continue;
		}
		
		int iCurrInstrIndex=g_FileControl.InstrStream.iCurrInstrIndex;
		int iOpcode=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpcode;
		
		printf ( "\t" );
		if ( iOpcode < 10 )
			printf ( " %d", iOpcode );
		else
			printf ( "%d", iOpcode );
		printf ( " %s ", ppstrMnemonics [ iOpcode ] );
		
		switch(iOpcode)
		{	
		case INSTR_MOV:
			
		case INSTR_ADD:
		case INSTR_SUB:
		case INSTR_MUL:
		case INSTR_DIV:
			{				
				VARDATA Dest=ResolveOperandValue(0);
				VARDATA Source=ResolveOperandValue(1);
				
				switch(iOpcode)
				{	
				case INSTR_MOV:
					{
						if(ResolveOpPointer(0)==ResolveOpPointer(1))
							break;
						
						CopyValue(&Dest,Source);
						
						break;
					}					
				case INSTR_ADD:
					{
						if(Dest.iType==OP_TYPE_INT)
							Dest.iIntLiteralValue+=ResolveOpAsInt(1);
						else
							Dest.fFloatLiteralValue+=ResolveOpAsFloat(1);
						
						break;
					}
				case INSTR_SUB:
					{
						if(Dest.iType==OP_TYPE_INT)
							Dest.iIntLiteralValue-=ResolveOpAsInt(1);
						else
							Dest.fFloatLiteralValue-=ResolveOpAsFloat(1);
						
						break;
					}
				case INSTR_MUL:
					{
						if(Dest.iType==OP_TYPE_INT)
							Dest.iIntLiteralValue *= ResolveOpAsInt(1);
						else
							Dest.fFloatLiteralValue *= ResolveOpAsFloat(1);
						
						break;
					}
				case INSTR_DIV:
					{
						if(Dest.iType==OP_TYPE_INT)
							Dest.iIntLiteralValue /= ResolveOpAsInt(1);
						else
							Dest.fFloatLiteralValue /= ResolveOpAsFloat(1);
						
						break;
					}
				}
				
				*ResolveOpPointer(0)=Dest;
				
				PrintOpIndir(0);
				printf(", ");
				PrintOpValue(0);
				printf(", ");
				PrintOpValue(1);
				break;
			}			
		case INSTR_JMP:
			{
				int iTargetIndex=ResolveOpAsInstrIndex(0);
				
				g_FileControl.InstrStream.iCurrInstrIndex=iTargetIndex;
				
				PrintOpValue(0);
				break;
			}
			
		case INSTR_JE:
		case INSTR_JNE:
		case INSTR_JG:
		case INSTR_JL:
		case INSTR_JGE:
		case INSTR_JLE:
			{
				VARDATA Op0=ResolveOperandValue(0);
				VARDATA Op1=ResolveOperandValue(1);
				
				int iTargetIndex=ResolveOpAsInstrIndex(2);
				
				int iJump=false;
				
				switch(iOpcode)
				{
				case INSTR_JE:
					{
						switch(Op0.iType)
						{
						case OP_TYPE_INT:
							if(Op0.iIntLiteralValue == Op1.iIntLiteralValue)
								iJump=true;
							break;		
						case OP_TYPE_FLOAT:
							if(Op0.fFloatLiteralValue == Op1.fFloatLiteralValue)
								iJump=true;
							break;						
						case OP_TYPE_STRING:
							if(strcmp(Op0.pstrStringLiteralValue, Op1.pstrStringLiteralValue) == 0)
								iJump=true;
							break;
						}
						break;
					}
				case INSTR_JNE:
					{
						switch(Op0.iType)
						{
						case OP_TYPE_INT:
							if(Op0.iIntLiteralValue != Op1.iIntLiteralValue)
								iJump=true;
							break;							
						case OP_TYPE_FLOAT:
							if(Op0.fFloatLiteralValue != Op1.fFloatLiteralValue)
								iJump=true;
							break;						
						case OP_TYPE_STRING:
							if(strcmp(Op0.pstrStringLiteralValue,Op1.pstrStringLiteralValue) != 0)
								iJump=true;
							break;
						}
						break;
					}
				case INSTR_JG:
					{
						if(Op0.iType == OP_TYPE_INT)
						{
							if(Op0.iIntLiteralValue>Op1.iIntLiteralValue)
								iJump=true;
						}
						else
						{
							if(Op0.fFloatLiteralValue>Op1.fFloatLiteralValue)
								iJump=true;
						}	
						break;
					}
				case INSTR_JL:
					{
						if(Op0.iType == OP_TYPE_INT)
						{
							if(Op0.iIntLiteralValue<Op1.iIntLiteralValue)
								iJump=true;
						}
						else
						{
							if(Op0.fFloatLiteralValue<Op1.fFloatLiteralValue)
								iJump=true;
						}
						break;
					}
				case INSTR_JGE:
					{
						if(Op0.iType == OP_TYPE_INT)
						{
							if(Op0.iIntLiteralValue >= Op1.iIntLiteralValue)
								iJump=true;
						}
						else
						{
							if(Op0.fFloatLiteralValue >= Op1.fFloatLiteralValue)
								iJump=true;
						}
						break;
					}
				case INSTR_JLE:
					{
						if(Op0.iType == OP_TYPE_INT)
						{
							if(Op0.iIntLiteralValue <= Op1.iIntLiteralValue)
								iJump=true;
						}
						else
						{
							if(Op0.fFloatLiteralValue <= Op1.fFloatLiteralValue)
								iJump=true;
						}
						break;
					}
				}
				
				PrintOpValue(0);
				printf ( ", " );
				PrintOpValue ( 1 );
				printf ( ", " );
				PrintOpValue ( 2 );
				printf ( " " );
				
				if(iJump)
				{
					g_FileControl.InstrStream.iCurrInstrIndex=iTargetIndex;
					printf ( "(True)" );
				}
				else
					printf ( "(False)" );
				
				break;
			}
		case INSTR_PUSH:
			{
				VARDATA Source=ResolveOperandValue(0);
				
				Push (Source);
				
				PrintOpValue(0);
				break;
			}
		case INSTR_POP:
			{
				*ResolveOpPointer(0)=Pop();
				
				PrintOpIndir(0);
				break;
			}
		case INSTR_CALL:
			{
				int iFuncIndex=ResolveOpAsFuncIndex(0);
				FUNCTION Dest=GetFunction(iFuncIndex);
				
				VARDATA ReturnAddr;
				ReturnAddr.iInstrIndex=g_FileControl.InstrStream.iCurrInstrIndex+1;
				Push(ReturnAddr);
				
				PushFrame(Dest.iLocalDataSize+1);
				
				VARDATA FuncIndex;
				FuncIndex.iFuncIndex=iFuncIndex;
				SetStackValue(g_FileControl.Stack.iTopElemIndex-1,FuncIndex);
				
				g_FileControl.InstrStream.iCurrInstrIndex=Dest.iEntryPoint;
				
				printf("$$[%d]$$", g_FileControl.InstrStream.iCurrInstrIndex);
				
				printf ( "%d ( Entry Point: %d, Frame Size: %d )", iFuncIndex, Dest.iEntryPoint, Dest.iStackFrameSize );
				break;
			}			
		case INSTR_RET:
			{
				VARDATA FuncIndex=Pop();
				FUNCTION CurrFunc=GetFunction(FuncIndex.iFuncIndex);
				int iFrameIndex=FuncIndex.iAddressingOffset;
				
				VARDATA ReturnAddr=GetStackValue(g_FileControl.Stack.iTopElemIndex-(CurrFunc.iLocalDataSize+1));
				
				PopFrame(CurrFunc.iStackFrameSize);
				
				g_FileControl.Stack.iFrameIndex=iFrameIndex;
				
				g_FileControl.InstrStream.iCurrInstrIndex=ReturnAddr.iInstrIndex;
				
				printf( "%d", ReturnAddr.iInstrIndex);
				
				break;
			}	
		case INSTR_CALLHOST:
			{
				PrintOpValue(0);
				break;
			}
		case INSTR_PAUSE:
			{
				if(g_FileControl.iIsPaused)
					break;
				
				int iPauseDuration=ResolveOpAsInt(0);
				
				g_FileControl.iPauseEndTime=GetCurrTime()+iPauseDuration;
				
				g_FileControl.iIsPaused=true;
				
				PrintOpValue(0);
				break;
			}	
		case INSTR_EXIT:
			{
				VARDATA ExitCode=ResolveOperandValue(0);
				
				int iExitCode=ExitCode.iIntLiteralValue;
				
				iExitExecLoop=true;
				
				PrintOpValue(0);
				break;
			}
		}
		
		printf ( "\n" );
		
		if(iCurrInstrIndex == g_FileControl.InstrStream.iCurrInstrIndex)
			g_FileControl.InstrStream.iCurrInstrIndex++;
		
		if(iExitExecLoop)
			break;
	}
}

