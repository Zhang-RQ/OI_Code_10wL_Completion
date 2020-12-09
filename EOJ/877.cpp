#include<bits/stdc++.h>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int MAXN=5E5+10;

vector<int> v[MAXN],pnt[MAXN];
int n,m,cnt=1,del[MAXN];
map<int,int> M[MAXN];
set<int> S[MAXN];
ll Ans,tAns[MAXN],sum;

void Insert_set(int id,int val)
{
	auto tmp=S[id].insert(val);
	if(!tmp.second)
		return;
	set<int>::iterator it1=tmp.first,it2=it1;
	--it1;++it2;
	if(!del[id])
		sum-=tAns[id];
	if(it2!=S[id].end())
		tAns[id]-=1ll*(*it2-*it1)*(n-*it2+1);
	tAns[id]+=1ll*(val-*it1)*(n-val+1);
	if(it2!=S[id].end())
		tAns[id]+=1ll*(*it2-val)*(n-*it2+1);
	if(!del[id])
		sum+=tAns[id];
}

void insert(const vector<int> &v,int id)
{
	int x=1;
	for(int i=1;i<v.size();i++)
	{
		if(!M[x][v[i]])
			M[x][v[i]]=++cnt,S[cnt].insert(0),pnt[i].push_back(cnt);
		x=M[x][v[i]];
		Insert_set(x,id);
	}
}

int merge(int x,int y)
{
	if(!x||!y)
		return x|y;
	if(S[x].size()<S[y].size())
		swap(x,y);
	for(int p:S[y])
		Insert_set(x,p);
	if(!del[y])
		sum-=tAns[y],del[y]=1;
	S[y].clear();
	for(auto e:M[y])
		M[x][e.first]=merge(M[x][e.first],e.second);
	M[y].clear();
	return x;
}

int main()
{
	scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++)
	{
		v[i].resize(m+1);
		for(int j=1;j<=m;j++)
			scanf("%d",&v[i][j]);
		insert(v[i],i);
	}
	int rt=1;del[1]=1;
	Ans+=sum;
	for(int i=1;i<m;i++)
	{
		rt=0;
		for(int p:pnt[i])
			if(!del[p])
				sum-=tAns[p],del[p]=1,rt=merge(rt,p);
		Ans+=sum;
	}
	printf("%lld\n",Ans);
}
