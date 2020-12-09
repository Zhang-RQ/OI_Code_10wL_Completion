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

const int MAXM=1E6+10;
const int MAXN=6E5+10;
const int MAXP=MAXN*23;

struct Line{
	int l,r,l1,r1,id,bl;
}l[MAXM];

struct edge_t{
	int v,nxt;
}Edge[(int)(MAXP*3.5)];

struct node_t{
	int l,r;
}t[MAXP];

struct opt{
	int pos,tp;
	ll val;//tp=2 remove,tp=0 insert,tp=1 query
	
	bool operator < (const opt &rhs) const {return pos<rhs.pos||(pos==rhs.pos&&tp<rhs.tp);}
}op[MAXN*4];

map<int,int> M;
int tv[MAXM*3],rt[MAXM*3];
int n,m,q,d[MAXP],val[MAXP],head[MAXP],cntl,cnt_e,cntp,totv,cnt,sp[MAXN],cntop,qu[MAXP];
ll f[MAXP],Ans[MAXN];
multiset<ll> S;

void add(int u,int v)
{
	Edge[++cnt_e]=(edge_t){v,head[u]};
	head[u]=cnt_e;
	++d[v];
}

void topo()
{
	int hd=1,tl=0;
	for(int i=1;i<=cntp;i++)
		if(!d[i])
			qu[++tl]=i;
	while(hd<=tl)
	{
		int x=qu[hd++];
		f[x]+=val[x];
		for(int i=head[x];i;i=Edge[i].nxt)
		{
			int v=Edge[i].v;
			f[v]=max(f[x],f[v]);
			if(!--d[v]) qu[++tl]=v;
		}
	}
}

void insert(int &x,int l,int r,int p,int y,int pnt)
{
	x=++cnt;
	t[x]=t[y];
	if(y) add(y+cntl,x+cntl);//?
	if(l==r) return add(pnt,x+cntl);
	int mid=(l+r)>>1;
	if(p<=mid) insert(t[x].l,l,mid,p,t[y].l,pnt);
	else insert(t[x].r,mid+1,r,p,t[y].r,pnt);
}

void adde(int x,int l,int r,int ql,int qr,int u)
{
	if(!x) return;
	if(ql<=l&&r<=qr) return add(x+cntl,u),void();
	int mid=(l+r)>>1;
	if(qr<=mid) adde(t[x].l,l,mid,ql,qr,u);
	else if(ql>mid) adde(t[x].r,mid+1,r,ql,qr,u);
	else adde(t[x].l,l,mid,ql,qr,u),adde(t[x].r,mid+1,r,ql,qr,u);
}

int main()
{
//	freopen("gaga.in","r",stdin);
//	freopen("gaga.out","w",stdout);
	n=io();m=io();q=io();
	for(int i=1,k,x,w;i<=m;i++)
	{
		k=io();x=io();w=io();
		if(k!=1)
		{
			if(x-k+1>0)
			{
				++cntl;
				l[cntl]=(Line){x-k+1,x,x-k+1,x-1,cntl,i};
				tv[++totv]=x-k+1;
				tv[++totv]=x;
				tv[++totv]=x-1;
				val[cntl]=w;
			}
			if(x+k-1<=n)
			{
				++cntl;
				l[cntl]=(Line){x,x+k-1,x+1,x+k-1,cntl,i};
				tv[++totv]=x;
				tv[++totv]=x+k-1;
				tv[++totv]=x+1;
				val[cntl]=w;
			}
		}
		else M[x]=w;
	}
	sort(tv+1,tv+1+totv);
	totv=unique(tv+1,tv+1+totv)-tv-1;
	for(int i=1;i<=cntl;i++)
		l[i].l=lower_bound(tv+1,tv+1+totv,l[i].l)-tv,
		l[i].r=lower_bound(tv+1,tv+1+totv,l[i].r)-tv,
		l[i].l1=lower_bound(tv+1,tv+1+totv,l[i].l1)-tv,
		l[i].r1=lower_bound(tv+1,tv+1+totv,l[i].r1)-tv;
	sort(l+1,l+1+cntl,[](const Line &lhs,const Line &rhs){return lhs.l1<rhs.l1;});
	int lstrt=0;
	for(int i=1,j=1;i<=totv;i++)
	{
		while(j<=cntl&&l[j].l1==i)
		{
			int nrt=0;
			insert(nrt,1,totv,l[j].r1,lstrt,l[j].id);
			lstrt=nrt;++j;
		}
		rt[i]=lstrt;
	}
	for(int i=1;i<=cntl;i++)
		adde(rt[l[i].l],1,totv,l[i].r,totv,l[i].id);
	for(int i=1;i<=cnt;i++)
	{
		if(t[i].l)
			add(t[i].l+cntl,i+cntl);
		if(t[i].r)
			add(t[i].r+cntl,i+cntl);
	}
	cntp=cntl+cnt;topo();
	sort(l+1,l+1+cntl,[](const Line &lhs,const Line &rhs){return lhs.id<rhs.id;});
	for(int i=1;i<=q;i++)
		sp[i]=io(),op[++cntop]=(opt){sp[i],1,i};
	for(int i=1;i<=cntl;i++)
	{
		op[++cntop]=(opt){tv[l[i].l1],0,f[i]};
		op[++cntop]=(opt){tv[l[i].r1],2,f[i]};
	}
	sort(op+1,op+1+cntop);
	S.insert(0);
	for(int i=1;i<=cntop;i++)
	{
		if(op[i].tp==0) S.insert(op[i].val);
		else if(op[i].tp==1) Ans[op[i].val]=max(Ans[op[i].val],*S.rbegin());
		else S.erase(S.find(op[i].val));
	}
	for(int i=1;i<=q;i++)
	{
		if(M.count(sp[i]))
			Ans[i]+=M[sp[i]];
		io(Ans[i]);
	}
}
