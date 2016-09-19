#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <map>
#include <stack>
#include <queue>

using namespace std;
typedef pair<int,int> pos;
int n=0,bsize=0;
int blocksindx=0,blocksindy=0;
map<int,int> rows[30],cols[30],blocks[30];
bool findans=false;
bool vis[30][30][30];
vector<pair<vector<int>,pos> > def;
struct cmp
{
    bool operator()(pair<vector<int>,pos> a, pair<vector<int>,pos> b)
    {
        return a.first.size() > b.first.size();
    }
};
priority_queue<pair<vector<int>,pos>,vector<pair<vector<int>,pos> >,cmp> pq;

bool check(int x,int y,int v,int board[][30])
{
    int bx,by;
    for(int i=0; i<n; i++) if(board[x][i]==v) return false;
    for(int i=0; i<n; i++) if(board[i][y]==v) return false;
    for(int i=1; i<=bsize; i++) if(x<bsize*i)
        {
            bx=i;
            break;
        }
    for(int i=1; i<=bsize; i++) if(y<bsize*i)
        {
            by=i;
            break;
        }
    for(int i=bsize*(bx-1); i<bsize*bx; i++)
    {
        for(int j=bsize*(by-1); j<bsize*by; j++) if(board[i][j]==v) return false;
    }
    return true;
}
void unsolvingBox(int board[][30])
{
    pq = priority_queue<pair<vector<int>,pos>,vector<pair<vector<int>,pos> >,cmp>(); //clear pq
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            vector<int> range;
            if(board[i][j]==0)
            {
                for(int k=1; k<=n; k++)
                {
                    if(check(i,j,k,board))
                    {
                        range.push_back(k);
                    }
                }
                pq.push(make_pair(range,pos(i,j)));
            }
        }
    }
}
bool presolve(int board[][30])
{

    while(!pq.empty())
    {
        if(pq.top().first.size()==0) return false;
        else if(pq.top().first.size()>1) break;
        else if(pq.top().first.size()==1)
        {
            int value=pq.top().first.front();
            int tmpx=pq.top().second.first,tmpy=pq.top().second.second;
            board[tmpx][tmpy]=value;
            unsolvingBox(board);
        }
    }
    return true;
}
int count_=0;
void backtracking(int board[][30])
{
    ///found the answer
    if(findans) return;
    if(pq.empty())
    {
            for(int i=0; i<n; i++)
            {
                for(int j=0; j<n; j++)
                {
                    if(j) printf(" ");
                    printf("%d",board[i][j]);
                }
                printf("\n");
            }
            findans=true;
        return;
    }
    ///start guessing
    vector<int> rangev = pq.top().first;
    priority_queue<pair<vector<int>,pos>,vector<pair<vector<int>,pos> >,cmp> tmppq;
    int posx=pq.top().second.first,posy=pq.top().second.second;
    pq.pop();
    tmppq=pq;
    int orgb[30][30]= {0};
    memcpy(orgb,board,sizeof(orgb));

    for(int i=0; i<rangev.size(); i++)
    {

        board[posx][posy]=rangev[i];
        unsolvingBox(board);
        vis[posx][posy][rangev[i]]=true;
        if(!presolve(board))
        {
            memcpy(board,orgb,sizeof(orgb));
            pq=tmppq;//return origin pq
            vis[posx][posy][rangev[i]]=false;
            continue;
        }

        backtracking(board);
        memcpy(board,orgb,sizeof(orgb));
        vis[posx][posy][rangev[i]]=false;
        pq=tmppq;//return origin pq
    }
}
int main()
{
    string str="";
    bool first=false;
    int num[30][30];

    while(getline(cin,str))
    {
        if(first) printf("\n");
        first=true;

        ///-------------initial variables
        findans=false;
        int ans[30][30];
        ///-------------read files
        stringstream ss(str);
        n=0;
        while(ss>>num[0][n]) n++;
        bsize=sqrt(n);
        bool testr[30][30],testc[30][30],wrong=false;
        memset(testr,false,sizeof(testr));
        memset(testc,false,sizeof(testc));

        for(int i=1; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                int tmp;
                cin >> tmp;
                num[i][j]=tmp;
                if(tmp!=0 && (testr[i][tmp] || testc[j][tmp]))
                {
                    wrong=true;
                }
                testr[i][tmp]=true;
                testc[j][tmp]=true;
            }
        }
        if(wrong)
        {
            printf("NO\n\n");
            string line;
            getline(cin,line);
            continue;
        }
        ///--------------set original board
        unsolvingBox(num);
        presolve(num);
        backtracking(num);
        string line;
        getline(cin,line);
        if(!findans) printf("NO\n");
    }
    return 0;
}
