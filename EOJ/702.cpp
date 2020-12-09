#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

struct FastIO{
    static const int S=1e7;
    int wpos;
    char wbuf[S];
    FastIO():wpos(0){}

    inline int xchar()
		{
			static char buf[S];
			static int len=0,pos=0;
			if(pos==len)
				pos=0,len=fread(buf,1,S,stdin);
			return buf[pos++];
		}

    inline int operator () ()
		{
			int c=xchar(),x=0,ng=0;
			while (!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline ll operator ! ()
		{
			int c=xchar(),ng=0;ll x=0;
			while(!isdigit(c)) ng|=(c=='-'),c=xchar();
			for(;isdigit(c);c=xchar()) x=x*10+c-'0';
			return ng?-x:x;
		}

    inline void wchar(int x)
		{
			if(wpos==S) fwrite(wbuf,1,S,stdout),wpos=0;
			wbuf[wpos++]=x;
		}

    inline void operator () (ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar('\n');
		}

    inline void space(ll x)
		{
			if (x<0) wchar('-'),x=-x;
			char s[24];
			int n=0;
			while(x||!n) s[n++]='0'+x%10,x/=10;
			while(n--) wchar(s[n]);
			wchar(' ');
		}

    inline void nextline() {wchar('\n');}

    ~FastIO() {if(wpos) fwrite(wbuf,1,wpos,stdout),wpos=0;}
}io;

const int MAXN=4E5+10;
const int MAXB=3000;

int n,q,a[MAXN],bl[MAXN],L[MAXN],R[MAXN],blksiz,totblk,lstv;
priority_queue<int> Q1[MAXB],Q2[MAXB];


void Clear(priority_queue<int> &pq){pq=priority_queue<int>();}

void build(int x,bool f)
{
	int l=L[x],r=R[x];
	if(f) Clear(Q1[x]);
	for(int i=l;i<=r;i++)
	{
		if(Q2[x].size()&&-Q2[x].top()<a[i])
		{
			int v=-Q2[x].top();
			Q2[x].pop();
			Q2[x].push(-a[i]);
			a[i]=v;
		}
		if(f) Q1[x].push(a[i]);
	}
	Clear(Q2[x]);
}

void solve(int l,int r,int v)
{
	if(bl[r]-bl[l]<=1)
	{
		for(int i=bl[l];i<=bl[r];i++)
			build(i,0);
		for(int i=l;i<=r;i++)
			if(a[i]>v)
				swap(a[i],v);
		for(int i=bl[l];i<=bl[r];i++)
			build(i,1);
	}
	else
	{
		build(bl[l],0);
		for(int i=l;i<=R[bl[l]];i++)
			if(a[i]>v)
				swap(a[i],v);
		build(bl[l],1);
		for(int i=bl[l]+1;i<bl[r];i++)
			if(Q1[i].top()>v)
			{
				int tmp=Q1[i].top();
				Q1[i].pop();
				Q1[i].push(v);
				Q2[i].push(-v);
				v=tmp;
			}
		build(bl[r],0);
		for(int i=L[bl[r]];i<=r;i++)
			if(a[i]>v)
				swap(a[i],v);
		build(bl[r],1);
	}
	lstv=v;
}

int main()
{
	int t=clock();
	n=io();q=io();
	blksiz=sqrt(n);
	for(int i=1;i<=n;i++)
	{
		a[i]=io();
		bl[i]=(i-1)/blksiz+1;
		if(!L[bl[i]]) L[bl[i]]=i;
		R[bl[i]]=i;
	}
	totblk=bl[n];
	for(int i=1;i<=totblk;i++)
		build(i,1);
	for(int i=1,l,r,v;i<=q;i++)
	{
		l=io();r=io();v=io();
		if(l<=r) solve(l,r,v);
		else solve(l,n,v),solve(1,r,lstv);
		io(lstv);
	}
}
