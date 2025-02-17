#include"BasicFunc.cpp"
#include"LogoPrint.cpp"
//我的浮动参数真是害惨我了！
int main(int argc,char* argv[])
{
	//打印logo以及基本信息
	LogoPrint();
	
	if(argc<2)
	{
		UsingManual();
		return 0;
	}
	
	strcpy(g_pstrSourceFilename,argv[1]);
	strupr(g_pstrSourceFilename);
	if(strstr(g_pstrSourceFilename,SOURCE_FILE_EXT)==NULL)
		strcat(g_pstrSourceFilename,SOURCE_FILE_EXT);
	
	if(argc==2)
	{
		if(strcmp(strupr(argv[1]),COMMAND_HELP)==0)
		{
			HelpPrint();
			return 0;
		}
		else if(strcmp(strupr(argv[1]),COMMAND_GRAMMAR)==0)
		{
			GrammarPrint();
			return 0;
		}
		else if(strcmp(strupr(argv[1]),COMMAND_ASK)==0)
		{
			AskPrint();
			return 0;
		}
		else
		{
			int iExeFilenameLength=strlen(g_pstrSourceFilename);
			int iExeFilenameOffset=iExeFilenameLength-strlen(strrchr(g_pstrSourceFilename,'.'));
			strncpy(g_pstrExecutableFilename,g_pstrSourceFilename,iExeFilenameOffset);
			g_pstrExecutableFilename[iExeFilenameOffset]='\0';
			strcat(g_pstrExecutableFilename,EXECUTABLE_FILE_EXT);
		}
	}
	else if(argc==3)
	{
		strcpy(g_pstrExecutableFilename,argv[2]);
		strupr(g_pstrExecutableFilename);
		
		if(strstr(g_pstrExecutableFilename,EXECUTABLE_FILE_EXT)==NULL)
			strcat(g_pstrExecutableFilename,EXECUTABLE_FILE_EXT);
	}
	else
	{
		printf("抱歉，汇编器不支持未定义的操作！\n\n");
		return 0;
	}

	
	//初始 
	CreateInstrDictionary();
	InitLinkedList(&g_SymbolTable);
	InitLinkedList(&g_LabelTabel);
	InitLinkedList(&g_FunctionTable);
	InitLinkedList(&g_StringTable);
	InitLinkedList(&g_HostAPICallTable);
	
	
	//装载源文 
	FILE* g_pfSourceFile=fopen(g_pstrSourceFilename,"rb");
	if(g_pfSourceFile==NULL)
		//          ļ     ˵
		ExitOnError((char*)"源文件打开失败，请检查是否是目录下正确的文件名！");
	
	while(!feof(g_pfSourceFile))
		if(fgetc(g_pfSourceFile)=='\n')
			g_iSourceCodeRows++;
	g_iSourceCodeRows++;
	
	fclose(g_pfSourceFile);
	
	g_pfSourceFile=fopen(g_pstrSourceFilename,"r");
	if(g_pfSourceFile==NULL)
		ExitOnError((char*)"源文件无法打开！");
	
	g_ppstrSourceCode=(char**)malloc(g_iSourceCodeRows*sizeof(char*));
	if(g_ppstrSourceCode==NULL)
		ExitOnError((char*)"源文件体积过大，无法为其分配内存！");	
	
	for(int iCurrLineCodeIndex=0;iCurrLineCodeIndex<g_iSourceCodeRows;iCurrLineCodeIndex++)
	{
		g_ppstrSourceCode[iCurrLineCodeIndex]=(char*)malloc(MAX_SOURCE_CODE_LINE_SIZE+1);
		if(g_ppstrSourceCode[iCurrLineCodeIndex]==NULL)
			ExitOnError((char*)"源文件分配过程中出现错误，试试重新汇编");
		
		fgets(g_ppstrSourceCode[iCurrLineCodeIndex],MAX_SOURCE_CODE_LINE_SIZE,g_pfSourceFile);
		
		StripComments(g_ppstrSourceCode[iCurrLineCodeIndex]);
		TrimWhitespace(g_ppstrSourceCode[iCurrLineCodeIndex]);
		
		int iNewCodeLineSize=strlen(g_ppstrSourceCode[iCurrLineCodeIndex]);
		int iNewCodeLineEndingIndex=iNewCodeLineSize-1;
		if(g_ppstrSourceCode[iCurrLineCodeIndex][iNewCodeLineEndingIndex]!='\n')
		{
			g_ppstrSourceCode[iCurrLineCodeIndex][iNewCodeLineEndingIndex+1]='\n';
			g_ppstrSourceCode[iCurrLineCodeIndex][iNewCodeLineEndingIndex+2]='\0';
		}
	}
	
	printf("%s 中的源代码装载完成!\n\n",g_pstrSourceFilename);
	
	fclose(g_pfSourceFile);
	
	
	//提示已经准备就绪，开始汇  
	printf("正在汇编 %s ...\n\n",g_pstrSourceFilename);
	
	
	// 始汇 
	Parsing();
	
	Load();
	
	int iVarCount=0;
	int iArrayCount=0;
	int iGlobalCount=0;
	
	LINKEDLISTNODE* pCurrNode=g_SymbolTable.pListHead;
	for(int iCurrNodeIndex=0;iCurrNodeIndex<g_SymbolTable.iNodeCount;iCurrNodeIndex++)
	{
		SYMBOLNODE* pCurrSymbolNode=(SYMBOLNODE*)pCurrNode->pvData;
		
		if(pCurrSymbolNode->iSize>1)
			iArrayCount++;
		else
			iVarCount++;
		
		if(pCurrSymbolNode->iStackUnitIndex>=0)
			iGlobalCount++;
		
		pCurrNode=pCurrNode->pNext;
	}
	
	printf("%s 汇编成功！\n\n",g_pstrSourceFilename);
	printf("这是您的代码使用资源情况!\n");
	
	if(g_SourceCodeHeader.iStackSize!=0)
		printf("使用堆栈大小:%d\n",g_SourceCodeHeader.iStackSize);
	else 
		printf("未占用堆栈\n");
	printf("局部变量数量:%d\n",iVarCount);
	printf("数组数量:%d\n",iArrayCount);
	printf("全局变量数量:%d\n",iGlobalCount);
	printf("常量区字符串常量数量:%d\n",g_StringTable.iNodeCount);
	printf("标签数量:%d\n",g_LabelTabel.iNodeCount);
	printf("引入API数量:%d\n",g_HostAPICallTable.iNodeCount);
	printf("函数数量:%d\n",g_FunctionTable.iNodeCount);
	if(g_SourceCodeHeader.iMainFuncPresent==Yes)
		printf("存在主函数，位于:%d 行\n",ReturnFuncEntryPoint(GetFuncByName(__START__FUNC)));
	else 
		printf("不存在主函数\n");
	
	
	//释放相应资源
	for(int iCurrCodeLineIndex=0;iCurrCodeLineIndex<g_iSourceCodeRows;iCurrCodeLineIndex++)
		free(g_ppstrSourceCode[iCurrCodeLineIndex]);
	free(g_ppstrSourceCode);
	
	if(g_pInstrStreamFile!=NULL)
	{
		for(int iCurrInstrIndex=0;iCurrInstrIndex<g_iInstrStreamRows;iCurrInstrIndex++)
			if(g_pInstrStreamFile[iCurrInstrIndex].pOpList!=NULL)
				free(g_pInstrStreamFile[iCurrInstrIndex].pOpList);
		free(g_pInstrStreamFile);
	}
	
	free(&g_SymbolTable);
	free(&g_LabelTabel);
	free(&g_FunctionTable);
	free(&g_StringTable);
	free(&g_HostAPICallTable);
	
	return 0;
}
