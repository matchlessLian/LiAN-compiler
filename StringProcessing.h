//前言：实际上我们总是仰仗着目前丰腴的土地 肆意挥霍来构造我们的语言

//这是对于字符流进行判断的一些函数，这些函数就像是一种筛选器，对于字符串的基本处理与分类，
//是让源代码到机器可以理解的10编码的第一步，也就是词法分析，在词法分析阶段我们并不考虑更加复杂的事情，
//我们只关心用户所写的源代码流是否至少做足了表面功夫，比如没有使用不属于这个语言的指令，没有随意使用特殊的符号来定义变量名...
//一个语言处理器对于源代码解读并不是一次性完成的，它也分为不同的阶段，而每个阶段只关注相当小且简单的任务，
//当你在用一门语言进行编程的时候，是经常会用空格的，空格的作用就是分隔开来两个英文单词，
//实际上你会发现，你的源代码文件里其实是由一个个英文单词组成的，我们规定的一门语言中可能对单词或者字符规定了了很多种属性，
//词法分析器将这些连续的源代码流，分解成一个个真正的单词，并与已经规定的合法属性一一比较，
//该单词的属性属于我们规定的属性，那就OK；不属于，那就要报错，
//所以词法分析器无非就是分解与比较。
//一些强大的语言，比如高级语言C语言，在词法分析这个阶段可能就更为复杂，其合法属性不仅更多，
//而且要容忍用户各种各样的编程习惯，同时还要弱化语言的规则感以提升用户体验，于是其在词法分析阶段可能会运用更加高级的方法。
//在这里词法分析仅仅依靠暴力搜索与暴力遍历来完成。

#ifndef _STDIO_

	#include<stdio.h>
	#include<string.h>
	#include<stdlib.h>

#define _STDIO_
#endif

//该字符是不是数字字符 
bool TheCharIsCharNumeric(char c)
{
	if(c>='0' && c<='9')
		return true;
	else
		return false;
}

//该字符是不是空白符 
bool TheCharIsWhitespace(char c)
{
	if(c==' ' || c=='\t')
		return true;
	else
		return false;
}

//该字符是不是属于组成标识符的合法字符 
bool TheCharIsBelongIdent(char c)
{
	if((c>='0' && c<='9') ||
	   (c>='a' && c<='z') ||
	   (c>='A' && c<='Z') ||
	   c=='_')
	    return true;
	else
		return false;
}

//该字符是不是分隔符 
bool TheCharIsDelimiter(char c)
{
	if( c==':' || 
		c==',' ||
		c=='"' ||
		c=='[' ||
		c==']' ||
		c=='{' ||
		c=='}' ||
		TheCharIsWhitespace(c) || c=='\n')
		return true;
	else
		return false;
}

//对传入的一个字符串进行扫描判断它是不是数字
//我们规定如果一个字符串它是合法数字那么他应当只由,数字字符,减号字符，加号字符构成
bool TheStringIsInt(char* s)
{
	if(s==NULL)return false;
	
	if(strlen(s)==0)return false;
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(TheCharIsCharNumeric(s[iCurrCharIndex])==false && 
		   s[iCurrCharIndex]!='-' && 
		   s[iCurrCharIndex]!='+')
			return false;
	
	for(unsigned int iCurrCharIndex=1;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(s[iCurrCharIndex]=='-' || s[iCurrCharIndex]=='+')
			return false;
	
	return true;
}

//对传入的一个字符串进行扫描判断它是不是浮点数
//我们规定如果一个字符串它是合法浮点数那么它应当只由，数字字符，小数点字符，减号字符，加号字符构成
bool TheStringIsFloat(char* s)
{
	if(s==NULL)return false;
	
	if(strlen(s)==0)return false;
	
	//确保字符串内只由合法字符构成 
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(TheCharIsCharNumeric(s[iCurrCharIndex])==false && 
			s[iCurrCharIndex]!='-' && 
			s[iCurrCharIndex]!='+' &&
			s[iCurrCharIndex]!='.')
			return false;
	
	bool FloatPointIsFound=false;
	bool FloatOperatorIsExist=false;
	
	//判头，判断有没有正负号 
	FloatOperatorIsExist=(s[0]=='+' || s[0]=='-') ? true : false;
	
	//不允许在其他地方出现正负号
	for(unsigned int iCurrCharIndex=1;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(s[iCurrCharIndex]=='-' || s[iCurrCharIndex]=='+')
			return false;
	
	//依旧判头，不允许字符串第一个字符就是小数点，
	//如果开头存在正负号，则不允许正负号后面紧跟小数点
	if(s[0+FloatOperatorIsExist]=='.')return false;
	
	//只允许字符串存在一个小数点
	for(unsigned int iCurrCharIndex=1+FloatOperatorIsExist;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(s[iCurrCharIndex]=='.')
		{
			if(FloatPointIsFound)return false;
			else FloatPointIsFound=true;
		}
	
	//
	if(FloatPointIsFound)return true;
	else return false;
}

//对传入的一个字符串进行扫描判断它是不是空白符
//我们规定一个字符串如果是合法空白符，那么他应当只由空白字符构成，或者它是一个有效空串 
bool TheStringIsWhitespace(char* s)
{
	if(s==NULL)return false;
	
	if(strlen(s)==0)return true;
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(TheCharIsWhitespace(s[iCurrCharIndex])!=false)
			return false;
	
	return true;
}

//对传入的一个字符串进行扫描判断他是不是标识符
//我们规定一个字符串如果是合法标识符，那么它应当只由合法标识符字符组成
bool TheStringIsIdent(char* s)
{
	if(s==NULL)return false;
	
	if(strlen(s)==0)return false;
	
	if(s[0]>='0' && s[0]<='9')return false;
	
	for(unsigned int iCurrCharIndex=0;iCurrCharIndex<strlen(s);iCurrCharIndex++)
		if(TheCharIsBelongIdent(s[iCurrCharIndex])==false)
			return false;
	
	return true;
}
