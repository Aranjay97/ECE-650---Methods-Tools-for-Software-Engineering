#include<bits/stdc++.h>
#include<iostream>
#include<stdio.h>
#include<regex>
#include<sstream>

using namespace std;

class Graph
{
    vector<int> Gr[1000];
public:
    void insertEdge(int m, int n);
    void BFS(int vertx, int src, int dst);
    void vectorClear();
};

void Graph::insertEdge(int m, int n)
{
    Gr[m].push_back(n);
    Gr[n].push_back(m);
}

void Graph::vectorClear()
{
    for(int i=0; i<1000; i++)
    {
        Gr[i].clear();
    }
}

int convert_regex_int(string input,int iterator)
{
    regex e(R"(\d+)");
    sregex_iterator iter(input.begin(), input.end(), e);
    sregex_iterator end;

    stringstream strstrm((*iter)[iterator]);
    int match_vertx;
    strstrm>> match_vertx;
    return match_vertx;
}

void Graph::BFS(int vertx, int src, int dst)
{
    int visited[1000] = {0};
    int d[1000], p[1000];
    visited[src]=1;
    d[src]=0;
    p[src]=-1;
    queue<int> q;
    q.push(src);
    while(!q.empty())
    {
        int v = q.front();
        q.pop();
        for(int u: Gr[v])
        {
            if(!visited[u])
            {
                visited[u]=1;
                q.push(u);
                d[u]=d[v]+1;
                p[u]=v;
            }
        }
    }

    vector<int> path;

    if (visited[dst]==0)
    {
        cout<<"Error: No path exists";
    }
    else
    {
        int x = dst;
        while(x!=-1)
        {
            path.push_back(x);
            x=p[x];
        }

        reverse(path.begin(),path.end());

    }
    for(unsigned int i=0;i<path.size();i++)
    {
        if(i==path.size()-1)
            cout<<path[i];
        else
            cout<<path[i]<<"-";
    }
    cout<<endl;
}



int main()
{
    string input;

    int vertx = 0;
    Graph gr;

    while(!cin.eof())
    {
        getline (cin,input);
        switch (input[0]) {
            case 'V':
            {
                vertx = convert_regex_int(input,0);
                if (vertx<=1)
                {
                    cout<<"Error: There should be more than 1 vertices"<<endl;
                    vertx=0;
                }
                else
                    break;
            }
                break;
            case 'E':
            {
                gr.vectorClear();
                int vertx1, vertx2;
                regex e(R"(\d+)");
                sregex_iterator iter(input.begin(), input.end(), e);
                sregex_iterator end;
                while(iter != end)
                {
                    stringstream strstrm1((*iter)[0]);
                    strstrm1 >> vertx1;

                    ++iter;

                    stringstream strstrm2((*iter)[0]);
                    strstrm2 >> vertx2;

                    if (vertx1 <= vertx && vertx2 <= vertx && vertx1!=0 && vertx2!=0)
                        gr.insertEdge(vertx1,vertx2);
                    else
                    {
                        cout << "Error: Invalid Edges as the vertex entered is undefined."<<endl;
                        vertx=0;
                        break;
                    }
                    ++iter;
                }
            }
                break;
            case 's':
            {
                regex e(R"(\d+)");
                sregex_iterator iter(input.begin(), input.end(), e);
                sregex_iterator end;

                int vertx1, vertx2;

                stringstream strstrm1((*iter)[0]);

                ++iter;

                stringstream strstrm2((*iter)[0]);

                strstrm1 >> vertx1;
                strstrm2 >> vertx2;

                if (vertx1==vertx2){
                    cout <<"Error: Source and destination are same"<<endl;
                    continue;
                }


                if (vertx1 <= vertx && vertx2 <= vertx && vertx1!=0 && vertx2!=0)
                    gr.BFS(vertx,vertx1,vertx2);
                else
                {
                    cout << "Error: The vertex entered is undefined or no valid graph"<<endl;
                    break;
                }
            }
                break;
        }
    }
}
