/*
	yuer
*/

#include<string.h>
#include<stdlib.h>

int charnlen=1000;

int charntoint(char *ch)
{
	char c[10]="";
	int i,j=0,len,minus=1,n=0,b;
	len=strlen(ch);
	if(len>=10) len=9;
	for(i=0;i<=len-1;i++)
	{
		c[i] = ch[i];
	}
	c[i] = '\0';
	for(i=0;i<=len-1;i++)
	{
		if(c[i]=='-') break;
	}
	if(i==len) i=-1;
	else minus=-1;
	i++;
	for(;i<=len-1;i++)//去掉负号前面的字符，包括负号
	{
		if(c[i]>='0'&&c[i]<='9')//只取数字
		{
			c[j] = c[i];
			j++;
		}
	}
	c[j] = '\0';
	len=j;
	for(i=0;i<=len-1;i++)
	{
		b=1;
		for(j=i;j<=len-2;j++)
		{
			b=b*10;
		}
		n=n+b*(c[i]-48);
	}
	n=n*minus;
	return n;
}

char* inttocharn(int n,char *ch)
{
	int i=0,m,a,nc=n;
	if(n<0) nc=0-nc;
	while(nc>=10)
	{
		m = nc/10;
		a = nc-m*10;
		ch[i] = a+48;
		i++;
		nc = m;
	}
	ch[i] = nc+48;
	i++;
	if(n<0)
	{
		ch[i] = '-';
		i++;
	}
	ch[i]='\0';
	char c[15];
	strcpy(c,ch);
	int l=strlen(c);
	for(i=0;i<=l-1;i++)
	{
		ch[i] = c[l-1-i];
	}
	return ch;
}

char* subcharn(char *ch,int index,int l)
{
	int len=strlen(ch);
	if(index<0||index>=len||l<=0)
	{
		ch[0]='\0';
		return ch;
	}
	if(index+l>len) l=len-index;
	int i,j=0;
	for(i=index;i<=index+l-1;i++)
	{
		ch[j] = ch[i];
		j++;
	}
	ch[j] = '\0';
	return ch;
}

int findcharn(char *ch,char *a)
{
	int i,len,lena;
	len=strlen(ch);
	if(len==0) return -1;
	lena=strlen(a);
	if(lena==0) return -1;
	char *b=(char*)malloc(charnlen);
	for(i=0;i<=len-1;i++)
	{
		strcpy(b,ch);
		subcharn(b,i,lena);
		if(strcmp(a,b)==0) break;
	}
	free(b);
	if(i==len) i=-1;
	return i;
}

char* repcharn(char *ch,char *a,char *b)
{
	int i=0,j=0,len,lena,lenb,t;
	len=strlen(ch);
	if(len==0) return ch;
	lena=strlen(a);
	if(lena==0) return ch;
	lenb=strlen(b);
	char *c=(char*)malloc(charnlen);
	strcpy(c,ch);
	t=findcharn(c,a);
	while(t!=-1)
	{
		for(i=0;i<=t-1;i++)
		{
			ch[j] = c[i];
			j++;
		}
		len=strlen(c);
		for(i=0;i<=len-1-t-lena;i++)
		{
			c[i] = c[i+t+lena];
		}
		c[i]='\0';
		for(i=0;i<=lenb-1;i++)
		{
			ch[j] = b[i];
			j++;
		}
		t=findcharn(c,a);
	}
	len=strlen(c);
	for(i=0;i<=len-1;i++)
	{
		ch[j] = c[i];
		j++;
	}
	ch[j]='\0';
	free(c);
	return ch;
}

char *mygets(char *s)
{
	fgets(s, charnlen, stdin);
	int l = strlen(s);
	if (s[l - 1] == '\n') s[l - 1] = '\0';
	else while (getchar() != '\n');//清除输入缓冲区
	return s;
}