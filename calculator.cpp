/*
	yuer
*/

#include<stdio.h>
#include<stdlib.h>
#include"mycharn.h"

#define numalen 100
#define numblen 20
#define charlen 1000
#define version "0.3.2"

#define exp "2.71828182845904523536028747135266249775724709369995"
#define pai "3.14159265358979323846264338327950288419716939937510"
#define ln10 "2.30258509299404568401799145468436420760110148862877"

struct number
{
	int mis;  //负号
	int a[numalen+1];
	int la;
	int b[numblen+2]; 
	int lb;
};

struct number remzero(struct number num);
struct number plus(struct number n1,struct number n2);
struct number minus(struct number n1,struct number n2);
struct number ln(struct number num);
struct number pownum(struct number n1,struct number n2);
void show(struct number num);

struct number initial()
{
	struct number num;
	int i;
	num.mis=0;
	for(i=0;i<=numalen-1;i++)
	{
		num.a[i]=0;
	}
	num.la=0;
	for(i=0;i<=numblen;i++)
	{
		num.b[i]=0;
	}
	num.lb=0;
	return num;
}

struct number ans;
int error=0;

struct number charntonum(char *ch)
{
	int i=0,n,len;
	struct number num=initial();
	char c[charlen];
	strcpy(c,ch);
	if(c[0]=='-')
	{
		num.mis=-1;
		repcharn(c,"-","");
	}
	len=strlen(c);
	n=findcharn(c,".");
	if(n==-1)
	{
		if(len>numalen) len=numalen;
		num.la=len;
		for(;i<=len-1;i++)
		{
			num.a[numalen-len+i]=c[i]-48;
		}
	}
	else
	{
		num.la=n;
		if(n>numalen)
		{
			i=n-numalen;
			num.la=numalen;
		}
		if(len-n-1>numblen) len=n+numblen+1;
		for(;i<=n-1;i++)
		{
			num.a[numalen-n+i]=c[i]-48;
		}
		i++;
		for(;i<=len-1;i++)
		{
			num.b[i-n-1]=c[i]-48;
		}
		num.lb=len-n-1;
	}
	return num;
}

void numtocharn(struct number num,char *ch)
{
	int i,j=0;
	if(num.mis==-1)
	{
		ch[j]='-';
		j++;
	}
	for(i=numalen-num.la;i<=numalen-1;i++)
	{
		ch[j]=num.a[i]+48;
		j++;
	}
	if(num.lb!=0)
	{
		ch[j]='.';
		j++;
		for(i=0;i<=num.lb-1;i++)
		{
			ch[j]=num.b[i]+48;
			j++;
		}
	}
	ch[j]='\0';
}

int numcmp(struct number n1,struct number n2)
{  //返回绝对值较大的
	if(n1.la>n2.la) return 1;
	if(n1.la<n2.la) return 2;
	int i,l;
	for(i=numalen-n1.la;i<=numalen-1;i++)
	{
		if(n1.a[i]>n2.a[i]) return 1;
		if(n1.a[i]<n2.a[i]) return 2;
	}
	if(n1.lb>n2.lb) l=n1.lb;
	else l=n2.lb;
	for(i=0;i<=l-1;i++)
	{
		if(n1.b[i]>n2.b[i]) return 1;
		if(n1.b[i]<n2.b[i]) return 2;
	}
	return 0;
}

struct number dectopos(struct number num)
{//去掉小数点
	if(num.lb==0) return num;
	struct number n=initial();
	n.mis=num.mis;
	n.la=num.la+num.lb;
	if(n.la>numalen)
	{
		error=1;
		return n;
	}
	int i,j;
	j=numalen-1;
	for(i=num.lb-1;i>=0;i--)
	{
		n.a[j]=num.b[i];
		j--;
	}
	for(i=numalen-1;i>=numalen-num.la;i--)
	{
		n.a[j]=num.a[i];
		j--;
	}
	n.a[j]=num.a[i];
	n=remzero(n);
	return n;
}

struct number postodec(struct number num,int lb)
{//还原小数点
	if(lb==0) return num;
	struct number n=initial();
	int i,j;
	j=numalen-num.la;
	n.la=num.la-lb;
	if(n.la<=0)
	{
		n.la=1;
		j=numalen-lb-1;
	}
	n.lb=lb;
	if(n.lb>numblen+1) n.lb=numblen+1;
	n.mis=num.mis;
	for(i=numalen-n.la;i<=numalen-1;i++)
	{
		n.a[i]=num.a[j];
		j++;
	}
	for(i=0;i<=n.lb-1;i++)
	{
		n.b[i]=num.a[j];
		j++;
	}
	return n;
}

struct number carry(struct number num)
{
	int i;
	if(num.lb==numblen+1)
	{
		char s[numblen+3]="0.";
		if(num.b[numblen]>=5)
		{
			for(i=1;i<=numblen-1;i++) strcat(s,"0");
			strcat(s,"1");
			num=plus(num,charntonum(s));
		}
		num.b[numblen]=0;
		num.lb--;
	}
	return num;
}

int countzero(struct number num)
{//数小数点后的0的个数
	int i,l=0;
	for(i=0;i<=num.lb-1;i++)
	{
		if(num.b[i]==0) l++;
		if(num.b[i]>0) break;
	}
	return l;
}

struct number getb(struct number num)
{//不管符号，只取小数
	struct number n=initial();
	int i;
	n.la=1;
	n.lb=num.lb;
	for(i=0;i<=n.lb;i++)
	{
		n.b[i]=num.b[i];
	}
	return n;
}

struct number remzero(struct number num)
{
	int i,t=0;
	for(i=0;i<=numalen-1;i++)
	{
		if(num.a[i]>0) break;
	}
	if(i==numalen) i=numalen-1;
	num.la=numalen-i;
	for(i=numblen-1;i>=0;i--)
	{
		if(num.b[i]>0) break;
	}
	num.lb=i+1;
	return num;
}

struct number plus(struct number n1,struct number n2)
{
	struct number n=initial();
	if(n1.mis==-1)
	{
		n1.mis=0;
		n=minus(n2,n1);
		return n;
	}
	int i,l,a=0;
	if(n1.lb>n2.lb) l = n1.lb;
	else l = n2.lb;
	n.lb = l;
	for(i=l-1;i>=0;i--)
	{
		n.b[i] = n1.b[i] + n2.b[i] + a;
		if(n.b[i]>=10)
		{
			a = n.b[i]/10;
			n.b[i] = n.b[i]%10;
		}
		else
		{
			a = 0;
		}
	}
	
	if(n1.la>n2.la) l = n1.la;
	else l = n2.la;
	for(i=numalen-1;i>=numalen-l;i--)
	{
		n.a[i] = n1.a[i] + n2.a[i] + a;
		if(n.a[i]>=10)
		{
			a = n.a[i]/10;
			n.a[i] = n.a[i]%10;
		}
		else
		{
			a = 0;
		}
	}
	n.la = l;
	if(a!=0)
	{
		if(n.la==numalen)
		{
			error=1;
			printf("位数溢出\n");
			return n;
		}
		n.a[numalen-n.la-1] = a;
		n.la++;
	}
	n=remzero(n);
	return n;
}

struct number minus(struct number n1,struct number n2)
{
	struct number n=initial();
	if(n1.mis==-1)
	{
		n1.mis=0;
		n=plus(n1,n2);
		if(error==1) return n;
		n.mis=-1;
		return n;
	}
	int i,l,a=0,t;
	struct number temp;
	t=numcmp(n1,n2);
	if(t==0) return charntonum("0");
	if(t==2)
	{
		temp=n2;
		n2=n1;
		n1=temp;
	}
	
	if(n1.lb>n2.lb) l = n1.lb;
	else l = n2.lb;
	n.lb = l;
	for(i=l-1;i>=0;i--)
	{
		n.b[i] = n1.b[i] - n2.b[i] - a;
		if(n.b[i]<0)
		{
			a = 1;
			n.b[i] = n.b[i]+10;
		}
		else
		{
			a = 0;
		}
	}
	
	l = n1.la;
	for(i=numalen-1;i>=numalen-l;i--)
	{
		n.a[i] = n1.a[i] - n2.a[i] - a;
		if(n.a[i]<0)
		{
			a = 1;
			n.a[i] = n.a[i]+10;
		}
		else
		{
			a = 0;
		}
	}
	n.la = l;
	if(t==2) n.mis=-1;
	n=remzero(n);
	return n;
}

struct number multiply(struct number n1,struct number n2)
{
	struct number n=initial();
	int i,j,k,t,a=0;
	t=n1.lb+n2.lb;
	n1=dectopos(n1);
	n2=dectopos(n2);
	if(error==1)
	{
		printf("位数溢出\n");
		return n;
	}
	if(numcmp(n1,charntonum("0"))==0||numcmp(n2,charntonum("0"))==0)
	{
		n.la=1;
		return n;
	}
	n.la = n1.la+n2.la-1;
	for(i=numalen-1;i>=numalen-n2.la;i--)
	{
		k=i;
		for(j=numalen-1;j>=numalen-n1.la;j--)
		{
			n.a[k]=n.a[k]+n2.a[i]*n1.a[j]+a;
			if(n.a[k]>=10)
			{
				a=n.a[k]/10;
				n.a[k]=n.a[k]%10;
			}
			else
			{
				a=0;
			}
			k--;
			if(k==-1&&j>numalen-n1.la) break;
			if(k==-1&&j==numalen-n1.la&&a!=0) break;
		}
		if(j>numalen-n1.la) break;
		if(j==numalen-n1.la&&a!=0) break;
		if(a!=0)
		{
			n.a[k]=a;
			a=0;
			if(i==numalen-n2.la) n.la++;
		}
	}
	if((j>numalen-n1.la)||(j==numalen-n1.la&&a!=0))
	{
		error=1;
		printf("位数溢出\n");
		return n;
	}
	n=postodec(n,t);
	n=carry(n);
	n.mis=n1.mis+n2.mis;
	if(n.mis==-2) n.mis=0;
	n=remzero(n);
	return n;
}

struct number divide(struct number n1,struct number n2)
{
	struct number n=initial();
	int k,t,temp;
	n.mis=n1.mis+n2.mis;
	if(n.mis==-2) n.mis=0;
	t=numcmp(n1,n2);
	if(numcmp(n2,charntonum("0"))==0)
	{
		printf("除0错误\n");
		error=1;
		return n;
	}
	if(numcmp(n2,charntonum("1"))==0)
	{
		n1.mis=n.mis;
		return n1;
	}
	if(t==0) return charntonum("1");
	if(t==1)
	{
		n.la=n1.la-n2.la+1;
		if(numcmp(n2,charntonum("1"))==2)
		{
			n.la=n1.la+countzero(n2)+1;
		}
	}
	if(t==2) n.la=1;
	k=numalen-n.la;
	while(k<=numalen-1&&t==1)
	{
		n.a[k]=9;
		temp=numcmp(n1,multiply(n,n2));
		if(error==1) return n;
		while(temp==2)
		{
			n.a[k]--;
			temp=numcmp(n1,multiply(n,n2));
			if(error==1) return n;
		}
		if(temp==0)
		{
			n=remzero(n);
			return n;
		}
		k++;
	}
	k=n2.la-n1.la-1;
	if(k<0) k=0;
	while(k<=numblen)
	{
		n.b[k]=9;
		n.lb=k+1;
		temp=numcmp(n1,multiply(n,n2));
		if(error==1) return n;
		while(temp==2)
		{
			n.b[k]--;
			temp=numcmp(n1,multiply(n,n2));
			if(error==1) return n;
		}
		if(temp==0) break;
		k++;
	}
	n=carry(n);
	n=remzero(n);
	return n;
}

struct number fac(struct number num)
{//求阶层
	struct number n=initial();
	if(num.lb!=0||num.mis==-1)
	{
		error=1;
		printf("小数或负数的阶层错误\n");
		return n;
	}
	if(numcmp(num,charntonum("0"))==0)
	{
		return charntonum("1");
	}
	n=num;
	while(numcmp(num,charntonum("1"))!=0)
	{
		num=minus(num,charntonum("1"));
		n=multiply(n,num);
		if(error==1) return n;
	}
	return n;
}

struct number powint(struct number n1,int n2)
{
	struct number n=initial();
	int i;
	if(n2>1000000)
	{
		error=1;
		printf("次方过高\n");
		return n;
	}
	n=charntonum("1");
	for(i=1;i<=n2;i++)
	{
		n=multiply(n,n1);
		if(error==1) return n;
	}
	return n;
}

struct number powe(struct number num)
{//num要小于1
	struct number n=initial();
	struct number temp=charntonum("1");
	int i=1;
	char c[10]="";
	while(numcmp(temp,n)!=0)
	{
		n=temp;
		inttocharn(i,c);
		temp=pownum(num,charntonum(c));
		temp=divide(temp,fac(charntonum(c)));
		temp=plus(n,temp);
		i++;
	}
	return n;
}

struct number pownum(struct number n1,struct number n2)
{
	struct number n=initial();
	struct number temp=initial();
	int t=0;
	if(numcmp(n1,charntonum("1"))==0)
	{
		if(n1.mis==0) return n1;
		else
		{
			if(n2.lb!=0)
			{
				error=1;
				printf("负数的小数次方错误\n");
				return n;
			}
			else
			{
				if(n2.a[numalen-1]%2==0) n1.mis=0;
				else n1.mis=-1;
				return n1;
			}
		}
	}
	if(numcmp(n1,charntonum("0"))==0)
	{
		if(numcmp(n2,charntonum("0"))==0)
		{
			error=1;
			printf("0^0无意义\n");
			return n;
		}
		if(n2.mis==-1)
		{
			error=1;
			printf("0的负数次方错误\n");
			return n;
		}
		return n1;
	}
	if(n2.mis==-1)
	{
		n2.mis=0;
		t=1;
	}
	if(n2.lb==0)
	{
		char c[charlen]="";
		numtocharn(n2,c);
		n=powint(n1,charntoint(c));
		if(error==1) return n;
		if(t==1) n=divide(charntonum("1"),n);
		return n;
	}
	if(n1.mis==-1)
	{
		error=1;
		printf("负数的小数次方错误\n");
		return n;
	}
	temp=getb(n2);
	temp=minus(n2,temp);
	n=pownum(n1,temp);
	if(error==1) return n;
	temp=getb(n2);
	n2=multiply(temp,ln(n1));
	temp=getb(n2);
	temp=minus(n2,temp);
	temp=pownum(charntonum(exp),temp);
	n=multiply(n,temp);
	temp=getb(n2);
	temp=powe(temp);
	n=multiply(n,temp);
	if(t==1) n=divide(charntonum("1"),n);
	return n;
}

struct number ln(struct number num)
{ //ln(x)=ln((1+y)/(1-y))=2(y+(1/3)y^3+(1/5)y^5+...)
	if(num.mis==-1||numcmp(num,charntonum("0"))==0)
	{
		error=1;
		printf("真数错误\n");
		return charntonum("0");
	}
	int t;
	t=numcmp(num,charntonum("5"));
	if(t==1)
	{
		num=divide(num,charntonum("10"));
		num=plus(ln(num),charntonum(ln10));
		return num;
	}
	t=numcmp(num,charntonum("0.5"));
	if(t==2)
	{
		num=multiply(num,charntonum("10"));
		num=minus(ln(num),charntonum(ln10));
		return num;
	}
	struct number n=charntonum("0");
	struct number temp1;
	struct number temp2;
	int i=3;
	char c[10]="";
	//y=(x-1)/(x+1)
	temp1=minus(num,charntonum("1"));
	temp2=plus(num,charntonum("1"));
	num=divide(temp1,temp2);
	temp1=num;
	while(numcmp(temp1,n)!=0)
	{
		n=temp1;
		inttocharn(i,c);
		temp2=powint(num,i);
		temp2=divide(temp2,charntonum(c));
		temp1=plus(temp1,temp2);
		i=i+2;
	}
	n=multiply(n,charntonum("2"));
	n=remzero(n);
	return n;
}

struct number lognum(struct number n1,struct number n2)
{
	struct number n=initial();
	if(n1.mis==-1||numcmp(n1,charntonum("0"))==0||numcmp(n1,charntonum("1"))==0)
	{
		error=1;
		printf("底数错误\n");
		return n;
	}
	if(n2.mis==-1||numcmp(n2,charntonum("0"))==0)
	{
		error=1;
		printf("真数错误\n");
		return n;
	}
	int i,tmp,t;
	if(n1.la>=10) n.la=0;
	if(n1.la<=9&&n1.la>=2) n.la=1;
	if(n1.la==1) n.la=2;
	for(i=numalen-n.la;i<=numalen-1;i++)
	{
		n.a[i]=9;
		tmp=numcmp(n2,pownum(n1,n));
		if(error==1) return n;
		t=numcmp(n2,charntonum("1"));
		if(t==1||t==0)
		{
			while(tmp==2)
			{
				n.a[i]--;
				tmp=numcmp(n2,pownum(n1,n));
				if(error==1) return n;
			}
		}
		if(t==2)
		{
			while(tmp==1)
			{
				n.a[i]--;
				tmp=numcmp(n2,pownum(n1,n));
				if(error==1) return n;
			}
		}
		if(tmp==0)
		{
			n=remzero(n);
			return n;
		}
	}
	n=ln(n2);
	if(error==1) return n;
	struct number temp=ln(n1);
	if(error==1) return n;
	n=divide(n,temp);
	carry(n);
	return n;
}

int findfn(char *ch,int n)
{
	int i;
	for(i=n-1;i>=0;i--)
	{
		if(ch[i]<'0'||ch[i]>'9')
		{
			if(ch[i]=='.') continue;
			if(i==0) return 0;
			if(ch[i-1]>='0'&&ch[i-1]<='9') return i+1;
			else return i;
		}
	}
	return 0;
}

int findbn(char *ch,int n)
{
	int i,len;
	len=strlen(ch);
	for(i=n+1;i<=len-1;i++)
	{
		if(ch[i]<'0'||ch[i]>'9')
		{
			if(i==n+1||ch[i]=='.') continue;
			return i-1;
		}
	}
	return len-1;
}

void reppm(char *ch)
{
	repcharn(ch,"+-","-");
	repcharn(ch,"--","+");
}

void calcutwo(char *ch,int n,int f,int b)
{
	struct number num1;
	struct number num2;
	struct number ans;
	char n1[charlen],n2[charlen],an[charlen];
	repcharn(ch," ","-");
	strcpy(n1,ch);
	strcpy(n2,ch);
	subcharn(n1,f,n-f);
	subcharn(n2,n+1,b-n);
	num1=charntonum(n1);
	num2=charntonum(n2);
	if(ch[n]=='+')
	{
		ans=plus(num1,num2);
	}
	if(ch[n]=='-')
	{
		ans=minus(num1,num2);
	}
	if(ch[n]=='*')
	{
		ans=multiply(num1,num2);
	}
	if(ch[n]=='/')
	{
		ans=divide(num1,num2);
	}
	if(ch[n]=='^')
	{
		ans=pownum(num1,num2);
	}
	if(ch[n]=='!')
	{
		ans=fac(num1);
	}
	if(ch[n]=='\'')
	{
		ans=lognum(num1,num2);
	}
	if(ch[n]=='n')
	{
		if(ch[n-1]=='l') ans=ln(num2);
	}
	if(error==1) return;
	char formula[charlen];
	numtocharn(ans,an);
	strcpy(formula,ch);
	subcharn(formula,f,b-f+1);
	ch[f]=' ';
	formula[0]=' ';
	repcharn(ch,formula,an);
	int len=strlen(ch);
	if(len>=charlen)
	{
		printf("式子溢出\n");
		error=1;
	}
}

void calculate(char *ch)
{
	int n,f,b;
	reppm(ch);
	
	n=findcharn(ch,"!");
	while(n!=-1)
	{
		f=findfn(ch,n);
		calcutwo(ch,n,f,n);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"!");
	}
	n=findcharn(ch,"^");
	while(n!=-1)
	{
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"^");
	}
	n=findcharn(ch,"'");
	while(n!=-1)
	{
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"'");
	}
	n=findcharn(ch,"ln");
	while(n!=-1)
	{
		b=findbn(ch,n+1);
		calcutwo(ch,n+1,n,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"'");
	}
	n=findcharn(ch,"/");
	while(n!=-1)
	{
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"/");
	}
	n=findcharn(ch,"*");
	while(n!=-1)
	{
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"*");
	}
	n=findcharn(ch,"-");
	while(n!=-1)
	{
		if(n==0)
		{
			ch[n]=' ';
			n=findcharn(ch,"-");
			if(n==-1) repcharn(ch," ","-");
			continue;
		}
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"-");
	}
	n=findcharn(ch,"+");
	while(n!=-1)
	{
		f=findfn(ch,n);
		b=findbn(ch,n);
		calcutwo(ch,n,f,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"+");
	}
}

void brac(char *ch)
{  //处理括号
	char c[charlen],tmp[charlen],chg[charlen];
	int i,j=0,len,a=-1,b=-1,t=0;
	len=strlen(ch);
	strcpy(chg,ch);
	for(i=0;i<=len-1;i++)
	{
		if(ch[i]=='(') t++;
		if(ch[i]==')') t--;
		if(t==1&&a==-1) a=i;
		if(a!=-1&&b==-1)
		{
			c[j]=ch[i];
			j++;
		}
		if(t==0&&a!=-1)
		{
			b=i;
			c[j]='\0';
			printf("%s\n",c);//包括括号
			strcpy(tmp,ch);
			subcharn(tmp,a+1,b-a-1);//括号里的，不包括括号
			brac(tmp);
			if(error==1) return;
			repcharn(chg,c,tmp);
			a=-1;b=-1;
			j=0;
			strcpy(c,"");
		}
	}
	strcpy(ch,chg);
	printf("%s\n",ch);
	calculate(ch);
}

int check(char *ch)
{
	int i,t=0,len,pass=1;
	if(strcmp(ch,"quit")==0)//退出
	{
		pass=2;
		return pass;
	}
	repcharn(ch," ","");//去空格
	char c[charlen];
	numtocharn(ans,c);
	repcharn(ch,"ans",c);  //替换ans
	len=strlen(ch);
	if(len==0)//检测式子的长度
	{
		pass=0;
		return pass;
	}
	if(len>=charlen)
	{
		pass=0;
		printf("输入过长\n");
		return pass;
	}
	for(i=0;i<=len-1;i++)//检测括号匹配
	{
		if(ch[i]=='(') t++;
		if(ch[i]==')') t--;
		if(t<0)
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
	}
	if(t!=0)
	{
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	t=t+findcharn(ch,"++")+1;//检测不能出现的符号
	t=t+findcharn(ch,"+*")+1;
	t=t+findcharn(ch,"+/")+1;
	t=t+findcharn(ch,"-+")+1;
	t=t+findcharn(ch,"-*")+1;
	t=t+findcharn(ch,"-/")+1;
	t=t+findcharn(ch,"*+")+1;
	t=t+findcharn(ch,"**")+1;
	t=t+findcharn(ch,"*/")+1;
	t=t+findcharn(ch,"/+")+1;
	t=t+findcharn(ch,"/*")+1;
	t=t+findcharn(ch,"//")+1;
	t=t+findcharn(ch,"()")+1;
	t=t+findcharn(ch,"(+")+1;
	t=t+findcharn(ch,"(*")+1;
	t=t+findcharn(ch,"(/")+1;
	t=t+findcharn(ch,")(")+1;
	t=t+findcharn(ch,"+)")+1;
	t=t+findcharn(ch,"-)")+1;
	t=t+findcharn(ch,"*)")+1;
	t=t+findcharn(ch,"/)")+1;
	t=t+findcharn(ch,"^)")+1;
	t=t+findcharn(ch,"^+")+1;
	t=t+findcharn(ch,"^*")+1;
	t=t+findcharn(ch,"^/")+1;
	t=t+findcharn(ch,"^^")+1;
	t=t+findcharn(ch,"(^")+1;
	t=t+findcharn(ch,"+^")+1;
	t=t+findcharn(ch,"-^")+1;
	t=t+findcharn(ch,"*^")+1;
	t=t+findcharn(ch,"/^")+1;
	t=t+findcharn(ch,"!(")+1;
	t=t+findcharn(ch,"!^")+1;
	t=t+findcharn(ch,"(!")+1;
	t=t+findcharn(ch,"+!")+1;
	t=t+findcharn(ch,"-!")+1;
	t=t+findcharn(ch,"*!")+1;
	t=t+findcharn(ch,"/!")+1;
	t=t+findcharn(ch,"^!")+1;
	t=t+findcharn(ch,"')")+1;
	t=t+findcharn(ch,"'+")+1;
	t=t+findcharn(ch,"'*")+1;
	t=t+findcharn(ch,"'/")+1;
	t=t+findcharn(ch,"'^")+1;
	t=t+findcharn(ch,"'!")+1;
	t=t+findcharn(ch,"''")+1;
	t=t+findcharn(ch,"('")+1;
	t=t+findcharn(ch,"+'")+1;
	t=t+findcharn(ch,"-'")+1;
	t=t+findcharn(ch,"*'")+1;
	t=t+findcharn(ch,"/'")+1;
	t=t+findcharn(ch,"^'")+1;
	t=t+findcharn(ch,"!'")+1;
	t=t+findcharn(ch,"n(")+1;
	t=t+findcharn(ch,"n)")+1;
	t=t+findcharn(ch,"n+")+1;
	t=t+findcharn(ch,"n*")+1;
	t=t+findcharn(ch,"n/")+1;
	t=t+findcharn(ch,"n^")+1;
	t=t+findcharn(ch,"n!")+1;
	t=t+findcharn(ch,"n'")+1;
	t=t+findcharn(ch,"nl")+1;
	t=t+findcharn(ch,")l")+1;
	t=t+findcharn(ch,"^l")+1;
	t=t+findcharn(ch,"!l")+1;
	t=t+findcharn(ch,"'l")+1;
	if(t>0)
	{
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	if(ch[0]=='+'||ch[0]=='*'||ch[0]=='/'||ch[0]=='.'||ch[0]==')'||ch[0]=='^'||ch[0]=='!'||
		ch[0]=='\'')
	{  //检测边缘符号
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	if(ch[len-1]=='+'||ch[len-1]=='-'||ch[len-1]=='*'||ch[len-1]=='/'||
		ch[len-1]=='.'||ch[len-1]=='('||ch[len-1]=='^'||ch[len-1]=='\''||
		ch[len-1]=='n')
	{
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	
	strcpy(c,ch);
	repcharn(c,"0","");  //检测小数点
	repcharn(c,"1","");
	repcharn(c,"2","");
	repcharn(c,"3","");
	repcharn(c,"4","");
	repcharn(c,"5","");
	repcharn(c,"6","");
	repcharn(c,"7","");
	repcharn(c,"8","");
	repcharn(c,"9","");
	t=findcharn(c,"..");
	if(t!=-1)
	{
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	t=0;
	strcpy(c,ch);
	while(t!=-1)
	{
		t=findcharn(c,".");
		if(t==-1) continue;
		if(c[t-1]<'0'||c[t-1]>'9'||c[t+1]<'0'||c[t+1]>'9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," ",".");
	t=0;
	while(t!=-1)  //检测左括号
	{
		t=findcharn(c,"(");
		if(t==-1) continue;
		if(t==0)
		{
			c[t]=' ';
			continue;
		}
		if(c[t-1]>='0'&&c[t-1]<='9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," ","(");
	t=0;
	while(t!=-1)  //检测右括号
	{
		t=findcharn(c,")");
		if(t==-1) continue;
		if(t==len-1) break;
		if(c[t+1]>='0'&&c[t+1]<='9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," ",")");
	t=0;
	while(t!=-1)  //检测阶层符号
	{
		t=findcharn(c,"!");
		if(t==-1) continue;
		if(t==len-1) break;
		if(c[t+1]>='0'&&c[t+1]<='9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," ","!");
	t=0;
	while(t!=-1)  //检测ln
	{
		t=findcharn(c,"ln");
		if(t==-1) continue;
		if(t==0)
		{
			c[t]=' ';
			continue;
		}
		if(c[t-1]>='0'&&c[t-1]<='9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," n","ln");

	repcharn(c,"(","");//检测剩余
	repcharn(c,")","");
	repcharn(c,"+","");
	repcharn(c,"-","");
	repcharn(c,"*","");
	repcharn(c,"/","");
	repcharn(c,".","");
	repcharn(c,"^","");
	repcharn(c,"!","");
	repcharn(c,"'","");
	repcharn(c,"ln","");
	len=strlen(c);
	if(len==0)
	{
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	for(i=0;i<=len-1;i++)
	{
		if(c[i]<'0'||c[i]>'9')
		{
			pass=0;
			printf("格式错误\n");
			return pass;
		}
	}
	return pass;
}

void show(struct number num)
{
	int i;
	if(num.mis==-1) printf("-");
	for(i=numalen-num.la;i<=numalen-1;i++)
	{
		printf("%d",num.a[i]);
	}
	if(num.lb!=0)
	{
		printf(".");
		for(i=0;i<=num.lb-1;i++)
		{
			if(i==numblen) break;
			printf("%d",num.b[i]);
		}
	}
	printf("\n");
}

void main()
{  //2015-12-17
	char s[charlen]="";
	ans=initial();
	printf("计算器      v%s\n\n",version);
	int pass=1;
	while(pass<=1)
	{
		printf("请输入计算式:\n");
		gets(s);
		pass=check(s);
		if(pass==1)
		{
			printf("\n");
			brac(s);
			if(error==0) ans=charntonum(s);
		}
		printf("\n");
		error=0;
	}
}