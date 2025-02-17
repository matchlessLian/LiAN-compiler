#include"Q_BasicFunc.cpp"
int main(int argc,char* argv[])
{
	LogoPrint();
	
	if(argc<2)
	{
		UsingManual();
		return 0;
	}
	else if(argc>2)
	{
		printf("抱歉，汇编器不支持未定义的操作！\n\n");
		return 0;
	}
	
	char* pstrExecutableFilename=(char*)malloc((strlen(argv[1])+5)*sizeof(char));
	strcpy(pstrExecutableFilename,argv[1]);
	strupr(pstrExecutableFilename);
	
	if(strstr(pstrExecutableFilename,EXECUTABLE_FILE_EXT)==NULL)
		strcat(pstrExecutableFilename,EXECUTABLE_FILE_EXT);
	
	//
	g_FileControl.iIsMainFuncPresent=false;
	g_FileControl.iIsPaused=false;
	
	g_FileControl.InstrStream.pInstrs=NULL;
	g_FileControl.Stack.pElem=NULL;
	g_FileControl.pFuncTable=NULL;
	g_FileControl.HostAPICallTable.ppstrHOSTAPI=NULL;
	
	FILE* pf;
	pf=fopen(pstrExecutableFilename,"rb");
	if(pf==NULL)
	{
		ERROR_LOAD_FILE_IO;
		return 0;
	}
	
	char* pstrFileFlag=(char*)malloc(5*sizeof(char));
	fread(pstrFileFlag,4,1,pf);
	//printf("pstrFileFlag:%s\n",pstrFileFlag);
	pstrFileFlag[strlen(LIAN_FLAG_STRING)]='\0';
	if(strcmp(pstrFileFlag,LIAN_FLAG_STRING)!=0)
	{
		ERROR_LOAD_INVALID_LIANFILE;
		return 0;
	}
	
	free(pstrFileFlag);
	
	int iMajorVersion=0,iMinorVersion=0;
	fread(&iMajorVersion,1,1,pf);
	fread(&iMinorVersion,1,1,pf);
	//printf("iMajorVersion:%d\n",iMajorVersion);
	//printf("iMinorVersion:%d\n",iMinorVersion);
	
	if(iMajorVersion!=1 || iMinorVersion!=0)
	{
		ERROR_LOAD_UNSUPPORTED;
		return 0;
	}
	
	fread(&(g_FileControl.Stack.iStackSize),4,1,pf);
	//printf("g_FileControl.Stack.iStackSize:%d\n",g_FileControl.Stack.iStackSize);
	if(g_FileControl.Stack.iStackSize==0)
		g_FileControl.Stack.iStackSize=DEFAULT_RUNTIME_STACK_SIZE;
	
	g_FileControl.Stack.pElem=(VARDATA*)malloc(g_FileControl.Stack.iStackSize*sizeof(VARDATA));
	fread(&(g_FileControl.iGlobalDataSize),4,1,pf);
	fread(&(g_FileControl.iIsMainFuncPresent),1,1,pf);
	fread(&(g_FileControl.iMainFuncIndex),4,1,pf);
	fread(&(g_FileControl.InstrStream.iInstrRows),4,1,pf);
	//printf("g_FileControl.iGlobalDataSize:%d\n",g_FileControl.iGlobalDataSize);
	//printf("g_FileControl.iIsMainFuncPresent:%d\n",g_FileControl.iIsMainFuncPresent);
	//printf("g_FileControl.iMainFuncIndex:%d\n",g_FileControl.iMainFuncIndex);
	//printf("g_FileControl.InstrStream.iInstrRows:%d\n",g_FileControl.InstrStream.iInstrRows);
	g_FileControl.InstrStream.pInstrs=(INSTR*)malloc(g_FileControl.InstrStream.iInstrRows*sizeof(INSTR));
	if(feof(pf))printf("yes0!\n");
	for(int iCurrInstrIndex=0;iCurrInstrIndex<g_FileControl.InstrStream.iInstrRows;iCurrInstrIndex++)
	{
		g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpcode=0;
		fread (&(g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpcode),2,1,pf);
		//printf("g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpcode:%d\n",g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpcode);
		g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount=0;
		fread (&(g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount),1,1,pf);
		//printf("g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount:%d\n",g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount);
		int iOpCount=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount;

		VARDATA* pOpList=(VARDATA*)malloc(iOpCount*sizeof(VARDATA));
		
		for (int iCurrOpIndex=0;iCurrOpIndex<iOpCount;++iCurrOpIndex)
		{			
			pOpList[iCurrOpIndex].iType=0;
			fread (&(pOpList[iCurrOpIndex].iType),1,1,pf);
			
			switch (pOpList[iCurrOpIndex].iType)
			{				
			case OP_TYPE_INT:
				{
					fread(&(pOpList[iCurrOpIndex].iIntLiteralValue),sizeof(int),1,pf);
					break;
				}				
			case OP_TYPE_FLOAT:
				{
					fread(&(pOpList[iCurrOpIndex].fFloatLiteralValue),sizeof(float),1,pf);
					break;
				}
			case OP_TYPE_STRING:
				{
					fread(&(pOpList[iCurrOpIndex].iIntLiteralValue),sizeof(int),1,pf);
					pOpList[iCurrOpIndex].iType=OP_TYPE_STRING;
					break;
				}
			case OP_TYPE_INSTR_LOCATION:
				{
					fread(&(pOpList[iCurrOpIndex].iInstrIndex),sizeof(int),1,pf);
					break;
				}
			case OP_TYPE_ABS_STACK_INDEX:
				{
					fread(&(pOpList[iCurrOpIndex].iStackUnitIndex),sizeof(int),1,pf);
					break;
				}
			case OP_TYPE_REL_STACK_INDEX:
				{
					fread(&(pOpList[iCurrOpIndex].iStackUnitIndex),sizeof(int),1,pf);
					fread(&(pOpList[iCurrOpIndex].iAddressingOffset),sizeof(int),1,pf);
					break;
				}
			case OP_TYPE_FUNC_INDEX:
				{
					fread(&(pOpList[iCurrOpIndex].iFuncIndex),sizeof(int),1,pf);
					break;
				}
			case OP_TYPE_HOST_API_CALL_INDEX:
				{
					fread(&(pOpList[iCurrOpIndex].iHostAPICallIndex),sizeof(int),1,pf);
					break;
				}
			case OP_TYPE_REG:
				{
					fread(&(pOpList[iCurrOpIndex].iRegisterIndex),sizeof(int),1,pf);
					break;
				}
			}
		}

		g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].pOpList=pOpList;
	}
	
	int iStringTableSize=0;
	fread(&iStringTableSize,sizeof(int),1,pf);
	if(iStringTableSize!=0)
	{
		char** ppstrStringTable=(char**)malloc(iStringTableSize*sizeof(char*));
		for(int iCurrStringIndex;iCurrStringIndex<iStringTableSize;iCurrStringIndex++)
		{
			int iStringLength=0;
			fread(&iStringLength,sizeof(int),1,pf);

			char* pstrCurrString=(char*)malloc(iStringLength+1);	
			fread(pstrCurrString,(size_t)iStringLength,1,pf);
			pstrCurrString[iStringLength]='\0';

			ppstrStringTable[iCurrStringIndex]=pstrCurrString;
		}
		for (int iCurrInstrIndex=0;iCurrInstrIndex<g_FileControl.InstrStream.iInstrRows;iCurrInstrIndex++)
		{
			int iOpCount=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount;
			VARDATA* pOpList=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].pOpList;
			
			for (int iCurrOpIndex=0;iCurrOpIndex<iOpCount;iCurrOpIndex++)
			{
				if (pOpList[iCurrOpIndex].iType==OP_TYPE_STRING)
				{	
					int iStringIndex=pOpList[iCurrOpIndex].iIntLiteralValue;
					char* pstrStringCopy=(char*)malloc((strlen(ppstrStringTable[iStringIndex])+1)*sizeof(char));
					strcpy ( pstrStringCopy, ppstrStringTable [ iStringIndex ] );
		
					pOpList[iCurrOpIndex].pstrStringLiteralValue = pstrStringCopy;
				}
			}
		}
		for(int iCurrStringIndex=0;iCurrStringIndex<iStringTableSize;iCurrStringIndex++)
			free(ppstrStringTable[iCurrStringIndex]);
		
		free(ppstrStringTable);
	}

	int iFuncTableSize=0;
	fread(&iFuncTableSize,4,1,pf);
		
	g_FileControl.pFuncTable=(FUNCTION*)malloc(iFuncTableSize*sizeof(FUNCTION));
	
	for(int iCurrFuncIndex=0;iCurrFuncIndex<iFuncTableSize;iCurrFuncIndex++)
	{
		int iEntryPoint;
		fread(&iEntryPoint,4,1,pf);
		
		int iParamCount=0;
		fread(&iParamCount,4,1,pf);
		
		int iLocalDataSize;
		fread(&iLocalDataSize,4,1,pf);
	
		int iStackFrameSize=iParamCount+1+iLocalDataSize;
		
		g_FileControl.pFuncTable[iCurrFuncIndex].iEntryPoint=iEntryPoint;
		g_FileControl.pFuncTable[iCurrFuncIndex].iParamCount=iParamCount;
		g_FileControl.pFuncTable[iCurrFuncIndex].iLocalDataSize=iLocalDataSize;
		g_FileControl.pFuncTable[iCurrFuncIndex].iStackFrameSize=iStackFrameSize;
	}
	
	fread(&(g_FileControl.HostAPICallTable.iAPICount),4,1,pf);
	
	g_FileControl.HostAPICallTable.ppstrHOSTAPI=(char**)malloc(g_FileControl.HostAPICallTable.iAPICount*sizeof(char*));
	
	for(int iCurrAPIIndex=0;iCurrAPIIndex<g_FileControl.HostAPICallTable.iAPICount;iCurrAPIIndex++)
	{
		int iCurrAPILength=0;
		fread(&iCurrAPILength,1,1,pf);

		char* pstrCurrAPI=(char*)malloc(iCurrAPILength+1);
		fread(pstrCurrAPI,iCurrAPILength,1,pf);
		pstrCurrAPI[iCurrAPILength]='\0';
		
		g_FileControl.HostAPICallTable.ppstrHOSTAPI[iCurrAPIIndex]=pstrCurrAPI;
	}
	
	fclose(pf);
	
	printf("%s 装载成功!\n\n",pstrExecutableFilename);
	printf("您的.LiAN文件目前版本号为:%d.%d\n",iMajorVersion, iMinorVersion);
	printf("使用堆栈容量为:%d\n",g_FileControl.Stack.iStackSize);
	printf("全局变量数量为:%d\n",g_FileControl.iGlobalDataSize);
	printf("已定义函数数量为:%d\n",iFuncTableSize);
	
	if(g_FileControl.iIsMainFuncPresent)
		printf("存在Start主函数(于内存中存储编号为:%d)",g_FileControl.iMainFuncIndex);
	else
		printf("不存在Start主函数");
	printf("\n");
	
	printf("调用API数量为:%d\n",g_FileControl.HostAPICallTable.iAPICount);
	printf("指令条目:%d\n",g_FileControl.InstrStream.iInstrRows);
	printf("常量区字符串数目:%d\n",iStringTableSize);
	
	printf("\n");
	
	printf("文件已经装载运行完成！\n");
	printf("请按任意键开始运行...\n");
	printf("\n");
	
	getch();
	while(kbhit())
		getch();
	
	ResetFile();
	
	RunFile();
	
	for(int iCurrInstrIndex=0;iCurrInstrIndex<g_FileControl.InstrStream.iInstrRows;iCurrInstrIndex++)
	{
		int iOpCount=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].iOpCount;
		VARDATA* pOpList=g_FileControl.InstrStream.pInstrs[iCurrInstrIndex].pOpList;
		
		for(int iCurrOpIndex=0;iCurrOpIndex<iOpCount;iCurrOpIndex++)
			if(pOpList[iCurrOpIndex].pstrStringLiteralValue)
				free(pOpList[iCurrOpIndex].pstrStringLiteralValue);
	}

	if(g_FileControl.InstrStream.pInstrs)
		free(g_FileControl.InstrStream.pInstrs);
	
	for(int iCurrElmtnIndex=0;iCurrElmtnIndex<g_FileControl.Stack.iStackSize;iCurrElmtnIndex++)
		if(g_FileControl.Stack.pElem[iCurrElmtnIndex].iType==OP_TYPE_STRING)
			free(g_FileControl.Stack.pElem[iCurrElmtnIndex].pstrStringLiteralValue);
	
	if(g_FileControl.Stack.pElem)
		free(g_FileControl.Stack.pElem);
	
	if(g_FileControl.pFuncTable)
		free(g_FileControl.pFuncTable);
	
	for(int iCurrCallIndex=0;iCurrCallIndex<g_FileControl.HostAPICallTable.iAPICount;iCurrCallIndex++)
		if(g_FileControl.HostAPICallTable.ppstrHOSTAPI[iCurrCallIndex])
			free(g_FileControl.HostAPICallTable.ppstrHOSTAPI[iCurrCallIndex]);
	
	if(g_FileControl.HostAPICallTable.ppstrHOSTAPI)
		free(g_FileControl.HostAPICallTable.ppstrHOSTAPI);
} 
