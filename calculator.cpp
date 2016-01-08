/*
	yuer
*/

#include<stdio.h>
#include"mycharn.h"

#define version "0.5.0"
int numalen=100;
int numblen=20;
//numalen要大于numblen*4

#define exp "2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742"
#define pai "3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798"
#define ln10 "2.30258509299404568401799145468436420760110148862877297603332790096757260967735248023599720508959829834"

struct number
{
	int mis;  //负号
	int *a;
	int la;
	int *b;
	int lb;
};

struct number minus(struct number n1,struct number n2);
struct number ln(struct number num);
void show(struct number num);

struct number initial()
{
	struct number num;
	int i;
	num.mis=0;
	num.a=(int*)malloc(numalen*4);
	for(i=0;i<=numalen-1;i++)
	{
		num.a[i]=0;
	}
	num.la=0;
	num.b=(int*)malloc((numblen+1)*4);
	for(i=0;i<=numblen;i++)
	{
		num.b[i]=0;
	}
	num.lb=0;
	return num;
}

struct number ini(struct number num)
{
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

void freenum(struct number num)
{
	free(num.a);
	free(num.b);
}

struct number ans;
int error=0;

struct number clonenum(struct number num)
{
	struct number n=initial();
	int i;
	n.mis=num.mis;
	for(i=0;i<=numalen-1;i++)
	{
		n.a[i]=num.a[i];
	}
	n.la=num.la;
	for(i=0;i<=numblen;i++)
	{
		n.b[i]=num.b[i];
	}
	n.lb=num.lb;
	return n;
}

struct number charntonum(char *ch,struct number num)
{
	int i=0,n,len;
	num=ini(num);
	char *c=(char*)malloc(charnlen);
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
		if(len-n-1>numblen+1) len=n+numblen+2;
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
	free(c);
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

struct number remzero(struct number num)
{
	int i,t=0;
	for(i=0;i<=numalen-1;i++)
	{
		if(num.a[i]>0) break;
	}
	if(i==numalen) i=numalen-1;
	num.la=numalen-i;
	for(i=numblen;i>=0;i--)
	{
		if(num.b[i]>0) break;
	}
	num.lb=i+1;
	return num;
}

struct number dectopos(struct number num)
{//去掉小数点
	if(num.lb==0) return num;
	num.la=num.la+num.lb;
	if(num.la>numalen)
	{
		error=1;
		return num;
	}
	int i,j;
	j=numalen-num.la-num.lb;
	for(i=numalen-num.la;i<=numalen-1;i++)
	{
		num.a[j]=num.a[i];
		j++;
	}
	for(i=0;i<=num.lb-1;i++)
	{
		num.a[j]=num.b[i];
		num.b[i]=0;
		j++;
	}
	num.lb=0;
	num=remzero(num);
	return num;
}

struct number postodec(struct number num,int lb)
{//还原小数点
	if(lb==0) return num;
	int i,j;
	j=0;
	for(i=numalen-lb;i<=numalen-1;i++)
	{
		if(j<=numblen)
		{
			num.b[j]=num.a[i];
			j++;
		}
		num.a[i]=0;
	}
	num.lb=j;
	j=numalen-1;
	for(i=numalen-lb-1;i>=numalen-num.la;i--)
	{
		num.a[j]=num.a[i];
		num.a[i]=0;
		j--;
	}
	num.la=numalen-j;
	if(num.la==0) num.la=1;
	num=remzero(num);
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

struct number plus(struct number n1,struct number n2)
{
	struct number n;
	if(n1.mis==-1)
	{
		n1.mis=0;
		n=minus(n2,n1);
		return n;
	}
	n=initial();
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
			freenum(n1);
			freenum(n2);
			printf("位数溢出\n");
			return n;
		}
		n.a[numalen-n.la-1] = a;
		n.la++;
	}
	n=remzero(n);
	freenum(n1);
	freenum(n2);
	return n;
}

struct number minus(struct number n1,struct number n2)
{
	struct number n;
	if(n1.mis==-1)
	{
		n1.mis=0;
		n=plus(n1,n2);
		if(error==1) return n;
		n.mis=-1;
		return n;
	}
	n=initial();
	int i,l,a=0,t;
	struct number temp;
	t=numcmp(n1,n2);
	if(t==0)
	{
		freenum(n1);
		freenum(n2);
		return charntonum("0",n);
	}
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
	freenum(n1);
	freenum(n2);
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
		freenum(n1);
		freenum(n2);
		printf("位数溢出\n");
		return n;
	}
	n=charntonum("0",n);
	if(numcmp(n1,n)==0||numcmp(n2,n)==0)
	{
		freenum(n1);
		freenum(n2);
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
		freenum(n1);
		freenum(n2);
		error=1;
		printf("位数溢出\n");
		return n;
	}
	n=postodec(n,t);
	n.mis=n1.mis+n2.mis;
	if(n.mis==-2) n.mis=0;
	n=remzero(n);
	freenum(n1);
	freenum(n2);
	return n;
}

struct number divide(struct number n1,struct number n2)
{
	struct number n=initial();
	int k,t,temp;
	if(numcmp(n2,charntonum("0",n))==0)
	{
		freenum(n1);
		freenum(n2);
		printf("除0错误\n");
		error=1;
		return n;
	}
	struct number nt=initial();
	nt=charntonum("1",nt);
	n.mis=n1.mis+n2.mis;
	if(n.mis==-2) n.mis=0;
	if(numcmp(n2,nt)==0)
	{
		freenum(n);
		freenum(nt);
		freenum(n2);
		n1.mis=n.mis;
		return n1;
	}
	t=numcmp(n1,n2);
	if(t==0)
	{
		freenum(n1);
		freenum(n2);
		freenum(n);
		nt.mis=n.mis;
		return nt;
	}
	if(t==1)
	{
		n.la=n1.la-n2.la+1;
		if(numcmp(n2,nt)==2)
		{
			n.la=n1.la+countzero(n2)+1;
		}
	}
	freenum(nt);
	struct number np1;
	struct number np2;
	if(t==2) n.la=1;
	k=numalen-n.la;
	while(k<=numalen-1&&t==1)
	{
		n.a[k]=9;
		np1=clonenum(n);
		np2=clonenum(n2);
		nt=multiply(np1,np2);
		temp=numcmp(n1,nt);
		freenum(nt);
		if(error==1)
		{
			freenum(n1);
			freenum(n2);
			return n;
		}
		while(temp==2)
		{
			n.a[k]--;
			np1=clonenum(n);
			np2=clonenum(n2);
			nt=multiply(np1,np2);
			temp=numcmp(n1,nt);
			freenum(nt);
			if(error==1)
			{
				freenum(n1);
				freenum(n2);
				return n;
			}
		}
		if(temp==0)
		{
			n=remzero(n);
			freenum(n1);
			freenum(n2);
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
		np1=clonenum(n);
		np2=clonenum(n2);
		nt=multiply(np1,np2);
		temp=numcmp(n1,nt);
		freenum(nt);
		if(error==1)
		{
			freenum(n1);
			freenum(n2);
			return n;
		}
		while(temp==2)
		{
			n.b[k]--;
			np1=clonenum(n);
			np2=clonenum(n2);
			nt=multiply(np1,np2);
			temp=numcmp(n1,nt);
			freenum(nt);
			if(error==1)
			{
				freenum(n1);
				freenum(n2);
				return n;
			}
		}
		if(temp==0) break;
		k++;
	}
	n=remzero(n);
	freenum(n1);
	freenum(n2);
	return n;
}

struct number fac(struct number num)
{//求阶层
	if(num.lb!=0||num.mis==-1)
	{
		error=1;
		printf("小数或负数的阶层错误\n");
		return num;
	}
	struct number nt=initial();
	nt=charntonum("0",nt);
	if(numcmp(num,nt)==0)
	{
		nt=charntonum("1",nt);
		freenum(num);
		return nt;
	}
	struct number n;
	n=clonenum(num);
	struct number np;
	nt=charntonum("1",nt);
	while(numcmp(num,nt)!=0)
	{
		np=clonenum(nt);
		num=minus(num,np);
		np=clonenum(num);
		n=multiply(n,np);
		if(error==1) break;
	}
	freenum(num);
	freenum(nt);
	return n;
}

struct number powint(struct number n1,int n2)
{
	if(n2>1000000)
	{
		error=1;
		printf("次方过高\n");
		return n1;
	}
	struct number n=initial();
	struct number nt;
	int i;
	n=charntonum("1",n);
	for(i=1;i<=n2;i++)
	{
		nt=clonenum(n1);
		n=multiply(n,nt);
		if(error==1) break;
	}
	freenum(n1);
	return n;
}

struct number powe(struct number num)
{//num要小于1
	//e^x=1+x+x^2/2!+x^3/3!...
	struct number n=initial();
	struct number temp=initial();
	temp=charntonum("1",temp);
	struct number nt=initial();
	struct number np;
	int i=1;
	char c[10]="";
	while(numcmp(temp,n)!=0)
	{
		freenum(n);
		n=clonenum(temp);
		freenum(temp);
		inttocharn(i,c);
		nt=charntonum(c,nt);
		np=clonenum(num);
		temp=powint(np,i);
		np=clonenum(nt);
		temp=divide(temp,fac(np));
		np=clonenum(n);
		temp=plus(np,temp);
		i++;
	}
	freenum(temp);
	freenum(nt);
	freenum(num);
	return n;
}

struct number pownum(struct number n1,struct number n2)
{
	struct number temp=initial();
	temp=charntonum("1",temp);
	int t=0;
	if(numcmp(n1,temp)==0)
	{
		if(n1.mis==0)
		{
			freenum(temp);
			freenum(n2);
			return n1;
		}
		else
		{
			if(n2.lb!=0)
			{
				freenum(n1);
				freenum(n2);
				error=1;
				printf("负数的小数次方错误\n");
				return temp;
			}
			else
			{
				if(n2.a[numalen-1]%2==0) n1.mis=0;
				else n1.mis=-1;
				freenum(n2);
				freenum(temp);
				return n1;
			}
		}
	}
	temp=charntonum("0",temp);
	if(numcmp(n1,temp)==0)
	{
		if(numcmp(n2,temp)==0)
		{
			freenum(n1);
			freenum(n2);
			error=1;
			printf("0^0无意义\n");
			return temp;
		}
		if(n2.mis==-1)
		{
			freenum(n1);
			freenum(n2);
			error=1;
			printf("0的负数次方错误\n");
			return temp;
		}
		freenum(n2);
		freenum(temp);
		return n1;
	}
	if(n2.mis==-1)
	{
		n2.mis=0;
		t=1;
	}
	struct number n;
	temp=charntonum("1",temp);
	if(n2.lb==0)
	{
		char c[15]="";
		numtocharn(n2,c);
		n=powint(n1,charntoint(c));
		if(error==1)
		{
			freenum(n2);
			freenum(temp);
			return n;
		}
		if(t==1) 
		{
			n=divide(temp,n);
			temp=initial();
		}
		freenum(temp);
		freenum(n2);
		return n;
	}
	if(n1.mis==-1)
	{
		freenum(n1);
		freenum(n2);
		error=1;
		printf("负数的小数次方错误\n");
		return temp;
	}
	//x^y = x^a*x^b = x^a*e^blnx = x^a*e^c*e^d
	struct number nt;
	freenum(temp);
	temp=getb(n2);
	nt=clonenum(n2);
	temp=minus(nt,temp);
	nt=clonenum(n1);
	n=pownum(nt,temp);
	if(error==1)
	{
		freenum(n1);
		freenum(n2);
		return n;
	}
	temp=getb(n2);
	freenum(n2);
	nt=clonenum(n1);
	n2=multiply(temp,ln(nt));
	temp=getb(n2);
	nt=clonenum(n2);
	temp=minus(nt,temp);
	nt=initial();
	nt=charntonum(exp,nt);
	temp=pownum(nt,temp);
	n=multiply(n,temp);
	temp=getb(n2);
	temp=powe(temp);
	n=multiply(n,temp);
	if(t==1)
	{
		nt=initial();
		nt=charntonum("1",nt);
		n=divide(nt,n);
	}
	freenum(n1);
	freenum(n2);
	return n;
}

struct number ln(struct number num)
{ //ln(x)=ln((1+y)/(1-y))=2(y+y^3/3+y^5/5+...)
	struct number temp=initial();
	temp=charntonum("0",temp);
	if(num.mis==-1||numcmp(num,temp)==0)
	{
		freenum(num);
		error=1;
		printf("真数错误\n");
		return temp;
	}
	int t=0,k=0;
	struct number nt;
	temp=charntonum("5",temp);
	while(numcmp(num,temp)==1)
	{
		t=1;
		nt=initial();
		nt=charntonum("10",nt);
		num=divide(num,nt);
		k++;
	}
	temp=charntonum("0.5",temp);
	while(numcmp(num,temp)==2)
	{
		t=2;
		nt=initial();
		nt=charntonum("10",nt);
		num=multiply(num,nt);
		k++;
	}
	struct number n=initial();
	struct number temp1;
	struct number temp2;
	struct number np;
	int i=3;
	char c[10]="";
	//y=(x-1)/(x+1)
	temp=charntonum("1",temp);
	nt=clonenum(num);
	np=clonenum(temp);
	temp1=minus(nt,np);
	nt=clonenum(num);
	np=clonenum(temp);
	temp2=plus(nt,np);
	freenum(num);
	num=divide(temp1,temp2);
	temp1=clonenum(num);
	while(numcmp(temp1,n)!=0)
	{
		freenum(n);
		n=clonenum(temp1);
		inttocharn(i,c);
		nt=clonenum(num);
		temp2=powint(nt,i);
		temp=charntonum(c,temp);
		temp2=divide(temp2,temp);
		temp=initial();
		temp1=plus(temp1,temp2);
		i=i+2;
	}
	temp=charntonum("2",temp);
	n=multiply(n,temp);
	if(t==1)
	{
		for(i=1;i<=k;i++)
		{
			nt=initial();
			nt=charntonum(ln10,nt);
			n=plus(n,nt);
		}
	}
	if(t==2)
	{
		for(i=1;i<=k;i++)
		{
			nt=initial();
			nt=charntonum(ln10,nt);
			n=minus(n,nt);
		}
	}
	n=remzero(n);
	freenum(num);
	freenum(temp1);
	return n;
}

struct number lognum(struct number n1,struct number n2)
{
	struct number n=initial();
	struct number temp=initial();
	n=charntonum("0",n);
	temp=charntonum("1",temp);
	if(n1.mis==-1||numcmp(n1,n)==0||numcmp(n1,temp)==0)
	{
		freenum(n1);
		freenum(n2);
		freenum(temp);
		error=1;
		printf("底数错误\n");
		return n;
	}
	if(n2.mis==-1||numcmp(n2,n)==0)
	{
		freenum(n1);
		freenum(n2);
		freenum(temp);
		error=1;
		printf("真数错误\n");
		return n;
	}
	if(numcmp(n2,temp)==0)
	{
		freenum(n1);
		freenum(n2);
		freenum(temp);
		return n;
	}
	struct number nt;
	int i,tmp,t,p;
	for(i=1;i<=100;i++)
	{
		freenum(temp);
		nt=clonenum(n1);
		temp=powint(nt,i);
		if(error==1)
		{
			error=0;
			break;
		}
		tmp=numcmp(n2,temp);
		temp=charntonum("1",temp);
		t=numcmp(n2,temp);
		if(t==1&&tmp==2) break;
		if(t==2&&tmp==1) break;
		p=numcmp(n1,temp);
		if(t==1&&p==2) break;
		if(t==2&&p==1) break;
		if(tmp==0)
		{
			freenum(n1);
			freenum(n2);
			freenum(temp);
			char c[15];
			inttocharn(i,c);
			n=charntonum(c,n);
			return n;
		}
	}
	freenum(n);
	freenum(temp);
	n=ln(n2);
	temp=ln(n1);
	n=divide(n,temp);
	return n;
}

struct number sinnum(struct number num)
{
	struct number temp=initial();
	temp=charntonum("360",temp);
	struct number nt;
	while(num.mis==-1)
	{
		nt=clonenum(temp);
		num=plus(num,nt);
	}
	while(numcmp(num,temp)==1||numcmp(num,temp)==0)
	{
		nt=clonenum(temp);
		num=minus(num,nt);
	}
	int t=0;
	temp=charntonum("180",temp);
	if(numcmp(num,temp)==0)
	{
		temp=charntonum("0",temp);
		freenum(num);
		return temp;
	}
	if(numcmp(num,temp)==1)
	{
		t=1;
		nt=clonenum(temp);
		num=minus(num,nt);
	}
	temp=charntonum("150",temp);
	if(numcmp(num,temp)==0)
	{
		temp=charntonum("0.5",temp);
		freenum(num);
		if(t==1) temp.mis=-1;
		return temp;
	}
	temp=charntonum("90",temp);
	if(numcmp(num,temp)==0)
	{
		temp=charntonum("1",temp);
		freenum(num);
		if(t==1) temp.mis=-1;
		return temp;
	}
	temp=charntonum("30",temp);
	if(numcmp(num,temp)==0)
	{
		temp=charntonum("0.5",temp);
		freenum(num);
		if(t==1) temp.mis=-1;
		return temp;
	}
	//sinx=x-x^3/3!+x^5/5!...
	struct number n=initial();
	temp=charntonum("180",temp);
	nt=clonenum(temp);
	num=divide(num,nt);
	temp=charntonum(pai,temp);
	num=multiply(num,temp);
	temp=clonenum(num);
	int i=3,j=1;
	char c[10]="";
	while(numcmp(temp,n)!=0)
	{
		freenum(n);
		n=clonenum(temp);
		freenum(temp);
		inttocharn(i,c);
		nt=clonenum(num);
		temp=powint(nt,i);
		nt=initial();
		nt=charntonum(c,nt);
		temp=divide(temp,fac(nt));
		nt=clonenum(n);
		if(j%2==1) temp=minus(nt,temp);
		if(j%2==0) temp=plus(nt,temp);
		i=i+2;j++;
	}
	if(t==1) n.mis=-1;
	freenum(temp);
	freenum(num);
	return n;
}

struct number cosnum(struct number num)
{
	struct number temp=initial();
	temp=charntonum("360",temp);
	struct number nt;
	while(num.mis==-1)
	{
		nt=clonenum(temp);
		num=plus(num,nt);
	}
	temp=charntonum("90",temp);
	num=minus(temp,num);
	num=sinnum(num);
	return num;
}

struct number tannum(struct number num)
{
	struct number temp;
	struct number n;
	struct number nt;
	nt=clonenum(num);
	temp=cosnum(nt);
	nt=initial();
	nt=charntonum("0",nt);
	if(numcmp(nt,temp)==0)
	{
		freenum(num);
		freenum(temp);
		error=1;
		printf("tan90无意义\n");
		return nt;
	}
	freenum(nt);
	n=sinnum(num);
	n=divide(n,temp);
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
	struct number num1=initial();
	struct number num2=initial();
	struct number answer;
	char *n1=(char*)malloc(charnlen);
	char *n2=(char*)malloc(charnlen);
	repcharn(ch," ","-");
	strcpy(n1,ch);
	strcpy(n2,ch);
	subcharn(n1,f,n-f);
	subcharn(n2,n+1,b-n);
	num1=charntonum(n1,num1);
	num2=charntonum(n2,num2);
	if(ch[n]=='+')
	{
		answer=plus(num1,num2);
	}
	if(ch[n]=='-')
	{
		answer=minus(num1,num2);
	}
	if(ch[n]=='*')
	{
		answer=multiply(num1,num2);
	}
	if(ch[n]=='/')
	{
		answer=divide(num1,num2);
	}
	if(ch[n]=='^')
	{
		answer=pownum(num1,num2);
	}
	if(ch[n]=='!')
	{
		freenum(num2);
		answer=fac(num1);
	}
	if(ch[n]=='\'')
	{
		answer=lognum(num1,num2);
	}
	if(ch[n]=='n')
	{
		freenum(num1);
		if(ch[n-1]=='l') answer=ln(num2);
		if(ch[n-1]=='i') answer=sinnum(num2);
		if(ch[n-1]=='a') answer=tannum(num2);
	}
	if(ch[n]=='s')
	{
		freenum(num1);
		answer=cosnum(num2);
	}
	free(n1);
	free(n2);
	if(error==1)
	{
		freenum(answer);
		return;
	}
	char *an=(char*)malloc(charnlen);
	char *formula=(char*)malloc(charnlen);
	numtocharn(answer,an);
	strcpy(formula,ch);
	subcharn(formula,f,b-f+1);
	ch[f]=' ';
	formula[0]=' ';
	repcharn(ch,formula,an);
	free(an);
	free(formula);
	freenum(answer);
	int len=strlen(ch);
	if(len>=charnlen)
	{
		printf("式子溢出\n");
		error=1;
	}
}

void calculate(char *ch)
{
	int n,f,b;
	reppm(ch);
	
	n=findcharn(ch,"ln");
	while(n!=-1)
	{
		b=findbn(ch,n+1);
		calcutwo(ch,n+1,n,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"ln");
	}
	n=findcharn(ch,"sin");
	while(n!=-1)
	{
		b=findbn(ch,n+2);
		calcutwo(ch,n+2,n,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"sin");
	}
	n=findcharn(ch,"cos");
	while(n!=-1)
	{
		b=findbn(ch,n+2);
		calcutwo(ch,n+2,n,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"cos");
	}
	n=findcharn(ch,"tan");
	while(n!=-1)
	{
		b=findbn(ch,n+2);
		calcutwo(ch,n+2,n,b);
		if(error==1) return;
		reppm(ch);
		printf("= %s\n",ch);
		n=findcharn(ch,"tan");
	}
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
	char *c=(char*)malloc(charnlen);
	char *tmp=(char*)malloc(charnlen);
	char *chg=(char*)malloc(charnlen);
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
			if(error==1)
			{
				free(c);
				free(tmp);
				free(chg);
				return;
			}
			repcharn(chg,c,tmp);
			a=-1;b=-1;
			j=0;
			strcpy(c,"");
		}
	}
	strcpy(ch,chg);
	printf("%s\n",ch);
	free(c);
	free(tmp);
	free(chg);
	calculate(ch);
}

struct number carry(struct number num)
{
	int i;
	if(num.lb==numblen+1)
	{
		if(num.b[numblen]>=5)
		{
			char *s=(char*)malloc(numblen+3);
			strcpy(s,"0.");
			for(i=1;i<=numblen-1;i++) strcat(s,"0");
			strcat(s,"1");
			struct number nt=initial();
			nt=charntonum(s,nt);
			num=plus(num,nt);
			free(s);
		}
		num.b[numblen]=0;
		num.lb--;
		num=remzero(num);
	}
	return num;
}

int check(char *ch)
{
	int i,t=0,len,pass=1;
	if(strcmp(ch,"quit")==0)//退出
	{
		pass=2;
		return pass;
	}
	if(strcmp(ch,"numalen")==0)//设置整数长度
	{
		printf("请输入整数长度:");
		char dec[10];
		int de;
		gets(dec);
		de=charntoint(dec);
		pass=0;
		if(de<100||de>40000)
		{
			printf("设置不正确，请输入100~40000之间的数\n");
		}
		else
		{
			freenum(ans);
			free(ch);
			numalen=de;
			charnlen=numalen*10;
			if(numblen*5>numalen) numblen=numalen/5;
			ans=initial();
			printf("设置成功\n");
			pass=-1;
		}
		return pass;
	}
	if(strcmp(ch,"numblen")==0)//设置小数精确度
	{
		printf("请输入小数精确度:");
		char dec[10];
		int de;
		gets(dec);
		de=charntoint(dec);
		pass=0;
		if(de<1||de>100)
		{
			printf("设置不正确，请输入1~100之间的数\n");
		}
		else
		{
			freenum(ans);
			free(ch);
			numblen=de;
			if(numblen*5>numalen)
			{
				numalen=numblen*5;
				charnlen=numalen*10;
			}
			ans=initial();
			printf("设置成功\n");
			pass=-1;
		}
		return pass;
	}
	repcharn(ch," ","");//去空格
	char *c=(char*)malloc(charnlen);
	numtocharn(ans,c);
	repcharn(ch,"ans",c);  //替换ans
	strcpy(c,exp);
	repcharn(ch,"e",subcharn(c,0,numblen+3));  //替换e
	strcpy(c,pai);
	repcharn(ch,"pi",subcharn(c,0,numblen+3));  //替换π
	len=strlen(ch);
	if(len==0)//检测式子的长度
	{
		free(c);
		pass=0;
		return pass;
	}
	if(len>=charnlen)
	{
		free(c);
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
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
	}
	if(t!=0)
	{
		free(c);
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
	t=t+findcharn(ch,"n)")+1;
	t=t+findcharn(ch,"n+")+1;
	t=t+findcharn(ch,"n*")+1;
	t=t+findcharn(ch,"n/")+1;
	t=t+findcharn(ch,"n^")+1;
	t=t+findcharn(ch,"n!")+1;
	t=t+findcharn(ch,"n'")+1;
	t=t+findcharn(ch,"nl")+1;
	t=t+findcharn(ch,")l")+1;
	t=t+findcharn(ch,"!l")+1;
	t=t+findcharn(ch,"ns")+1;
	t=t+findcharn(ch,")s")+1;
	t=t+findcharn(ch,"!s")+1;
	t=t+findcharn(ch,"s)")+1;
	t=t+findcharn(ch,"s+")+1;
	t=t+findcharn(ch,"s*")+1;
	t=t+findcharn(ch,"s/")+1;
	t=t+findcharn(ch,"s^")+1;
	t=t+findcharn(ch,"s!")+1;
	t=t+findcharn(ch,"s'")+1;
	t=t+findcharn(ch,"sl")+1;
	t=t+findcharn(ch,"ss")+1;
	t=t+findcharn(ch,"sc")+1;
	t=t+findcharn(ch,")c")+1;
	t=t+findcharn(ch,"!c")+1;
	t=t+findcharn(ch,"nc")+1;
	t=t+findcharn(ch,"nt")+1;
	t=t+findcharn(ch,")t")+1;
	t=t+findcharn(ch,"!t")+1;
	t=t+findcharn(ch,"st")+1;
	if(t>0)
	{
		free(c);
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	if(ch[0]=='+'||ch[0]=='*'||ch[0]=='/'||ch[0]=='.'||ch[0]==')'||ch[0]=='^'||ch[0]=='!'||
		ch[0]=='\'')
	{  //检测边缘符号
		free(c);
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	if(ch[len-1]=='+'||ch[len-1]=='-'||ch[len-1]=='*'||ch[len-1]=='/'||
		ch[len-1]=='.'||ch[len-1]=='('||ch[len-1]=='^'||ch[len-1]=='\''||
		ch[len-1]=='n'||ch[len-1]=='s')
	{
		free(c);
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
		free(c);
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
			free(c);
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
			free(c);
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
			free(c);
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
			free(c);
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
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," n","ln");
	t=0;
	while(t!=-1)  //检测sin
	{
		t=findcharn(c,"sin");
		if(t==-1) continue;
		if(t==0)
		{
			c[t]=' ';
			continue;
		}
		if(c[t-1]>='0'&&c[t-1]<='9')
		{
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," in","sin");
	t=0;
	while(t!=-1)  //检测cos
	{
		t=findcharn(c,"cos");
		if(t==-1) continue;
		if(t==0)
		{
			c[t]=' ';
			continue;
		}
		if(c[t-1]>='0'&&c[t-1]<='9')
		{
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," os","cos");
	t=0;
	while(t!=-1)  //检测tan
	{
		t=findcharn(c,"tan");
		if(t==-1) continue;
		if(t==0)
		{
			c[t]=' ';
			continue;
		}
		if(c[t-1]>='0'&&c[t-1]<='9')
		{
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
		c[t]=' ';
	}
	repcharn(c," an","tan");

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
	repcharn(c,"sin","");
	repcharn(c,"cos","");
	repcharn(c,"tan","");
	len=strlen(c);
	if(len==0)
	{
		free(c);
		pass=0;
		printf("格式错误\n");
		return pass;
	}
	for(i=0;i<=len-1;i++)
	{
		if(c[i]<'0'||c[i]>'9')
		{
			free(c);
			pass=0;
			printf("格式错误\n");
			return pass;
		}
	}
	free(c);
	return pass;
}

void show(struct number num)
{
	int i;
	printf("= ");
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
			printf("%d",num.b[i]);
		}
	}
	printf("\n");
}

void main()
{  //2015-12-17
	char *s=(char*)malloc(charnlen);
	ans=initial();
	printf("计算器      v%s\n\n",version);
	int pass=1;
	while(pass<=1)
	{
		printf("请输入计算式:\n");
		gets(s);
		pass=check(s);
		if(pass==-1) s=(char*)malloc(charnlen);
		if(pass==1)
		{
			printf("\n");
			brac(s);
			if(error==0)
			{
				ans=charntonum(s,ans);
				ans=carry(ans);
				show(ans);
			}
		}
		printf("\n");
		error=0;
	}
	free(s);
	freenum(ans);
}