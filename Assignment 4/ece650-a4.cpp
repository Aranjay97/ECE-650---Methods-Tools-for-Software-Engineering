#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <string>
#include <list>
#include <stddef.h>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

struct Edge
{
    int src_vertx;
    int dst_vertx;
    
};

struct LitNew
{
    Minisat::Lit litC1;
    int i;
    int k;

};

std::vector<Edge> edgeParse(std::string ln_dat, int num);
int vertxParse(std::string ln_dat);
std::map<int,int> lineSame(std::vector<int> pt_vals);

int cmpre(const void *a, const void * b);
int maxValFind(std::map<int,int> pntFreq);
void FirstClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverVC);
void SecondClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr,std::unique_ptr<Minisat::Solver> &solverVC);
void ThirdClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr,std::unique_ptr<Minisat::Solver> &solverVC);
void FourthClause(std::vector<Edge> &grphDat,std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverVC);
void initialize_Lits(std::vector<std::vector<Minisat::Lit>> &listOfLits,int vertxNum, int lenVertxCvr,std::unique_ptr<Minisat::Solver> &solverTT);
int maxValAnalyze(int vertxNum, int freqMax,std::map<int,int> valFreq);

int main(int argc, char **argv) 
{
    
    std::vector<int> satSolve;
    std::vector<Edge> Edg_Dat;
    std::vector<int> pnt_Dat;
    int vertxNum;
    std::regex rVertx(R"([vV]\s*?[0-9]+)");
    std::regex rEdgLst(R"([eE]\s*?\{\s*(<\s*?[0-9]+\s?,\s*?[0-9]+\s*?>\s*?,\s*?)+(<\s*?[0-9]+\s*?,\s*?[0-9]+\s*?>)+\s*\})");
    std::regex rEdgFirst(R"([eE]\s*?\{\s*(<\s*?[0-9]+\s?,\s*?[0-9]+\s*?>\s*?)\})");
    std::regex rEdgLstN(R"([eE]\s*?\{\})");
    while (!std::cin.eof())
    {
        // std::vector<std::vector<Minisat::Lit>> listOfLits;
        std::unique_ptr<Minisat::Solver> ptr = std::unique_ptr<Minisat::Solver>(new Minisat::Solver());
        std::smatch rVertxStat,rEdgStat;
        std::smatch rVNone,rENone, rEOne;
        int valMax =0;

        std::string line;
        std::getline(std::cin, line);

        auto s1 = std::regex_match(line,rVertxStat,rVertx);
        auto s2 = std::regex_match(line,rEdgStat,rEdgLst);
        auto s5 = std::regex_match(line,rENone,rEdgLstN);
        auto s6 = std::regex_match(line,rEOne,rEdgFirst);
        std::map<int,int> valFreq;


        if (s1)
        {     
           vertxNum =  vertxParse(line);  
           vertxNum += 1;  
           
        }
        if (s2 || s6)
        {
            Edg_Dat = edgeParse(line,vertxNum);
            for (auto &e : Edg_Dat)
            {
               pnt_Dat.push_back(e.dst_vertx);
               //pnt_Dat.push_back(e.src_vertx);
            }
            for (auto &e : Edg_Dat)
            {
               //pnt_Dat.push_back(e.dst_vertx);
               pnt_Dat.push_back(e.src_vertx);
            }
            
        }
        if (s5)
        {
            std::cout<<std::endl;
        }
        if (!Edg_Dat.empty())
        {
            bool result = true;
            std::vector<std::vector<Minisat::Lit>> listOfLitsFinal(vertxNum);
            valFreq = lineSame(pnt_Dat);
            int maxVFreq = 0;
            maxVFreq = maxValFind(valFreq);
            valMax = maxValAnalyze(vertxNum, maxVFreq,valFreq);

            while (result == true)
            {
                std::vector<std::vector<Minisat::Lit>> listOfLits(vertxNum);
                
                initialize_Lits(listOfLits,vertxNum,valMax,ptr);
                FirstClause(listOfLits,vertxNum,valMax,ptr);
                SecondClause(listOfLits,vertxNum,valMax,ptr);
                ThirdClause(listOfLits,vertxNum,valMax,ptr);
                FourthClause(Edg_Dat,listOfLits,vertxNum,valMax,ptr);
                result = ptr->solve();
                ptr.reset(new Minisat::Solver());
                if (result == true)
                {
                    
                    valMax -= 1;
                    listOfLits.clear();
                }
            }
            
             
            if (!result)
            {
                valMax += 1;
                std::vector<std::vector<Minisat::Lit>> listOfLits(vertxNum);
                
                initialize_Lits(listOfLits,vertxNum,valMax,ptr);
                FirstClause(listOfLits,vertxNum,valMax,ptr);
                SecondClause(listOfLits,vertxNum,valMax,ptr);
                ThirdClause(listOfLits,vertxNum,valMax,ptr);
                FourthClause(Edg_Dat,listOfLits,vertxNum,valMax,ptr);
                result = ptr->solve();
                if (result)
                {
                    for (int i = 0; i < vertxNum; i++)
                    {
                        for (int j = 0; j < valMax; j++)
                        {
                            if (Minisat::toInt(ptr->modelValue(listOfLits[i][j])) == 0)
                            {
                                satSolve.push_back(i);
                            }
                        }                
                    }
                    qsort(&satSolve[0],satSolve.size(),sizeof(int),cmpre);
                    for (auto &e : satSolve)
                    {
                        std::cout<<e<<" ";
                    }
                    std::cout<<std::endl;
                    listOfLits.clear();
                }
                else
                {
                    std::cerr<<"Error: Not able to find the vertex cover for the Graph"<<std::endl;
                } 
            }
            

            //Clering important DS
            pnt_Dat.clear();
            satSolve.clear(); 
            Edg_Dat.clear();
            
            valFreq.clear();
            
        } 
    }
 return 0;
}

//Initialize Literals
/*
1. Number of atomic proposition  = n x k
*/
void initialize_Lits(std::vector<std::vector<Minisat::Lit>> &listOfLits,int vertxNum, int lenVertxCvr,std::unique_ptr<Minisat::Solver> &solverTT)
{
    for (int i = 0; i < vertxNum; i++)
    {
        for (int j = 0; j < lenVertxCvr; j++)
        {
            Minisat::Lit lt = Minisat::mkLit(solverTT->newVar());
            listOfLits[i].push_back(lt);
            
        }
        
    }
}

//Clause 1
/*
1. At least one vertex is the ith vertex in the vertex cover
2. No of clauses = k
*/
void FirstClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverTT)
{
    for (int i = 0; i < lenVertxCvr; i++)
    {
        Minisat::vec<Minisat::Lit> litCOne;

        for (int j = 0; j < numvertx; j++)
        {
            litCOne.push(listOfLits[j][i]);
        } 
        solverTT->addClause(litCOne);
        litCOne.clear();
    } 
}

//Clause 2
/*
1. No vertex can appear twice in a vertex cover
*/
void SecondClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverTT)
{
    for (int i = 0; i < numvertx; i++)
    {
        for (int j = 0; j < lenVertxCvr; j++)
        {
            for (int k = j+1; k < lenVertxCvr; k++)
            {
                solverTT->addClause(~listOfLits[i][j],~listOfLits[i][k]);
            }
        }   
    }
  
}
    
//Clause 3
/*
1. Dual of 2
*/
void ThirdClause(std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverTT)
{
    for (int i = 0; i < lenVertxCvr; i++)
    {
        for (int j = 0; j < numvertx-1; j++)
        {
            for (int k = j+1; k < numvertx; k++)
            {
                solverTT->addClause(~listOfLits[j][i],~listOfLits[k][i]);
            }
        }   
    }
  
}

//Clause 4
/*
1. Every edge is incident to at least one vertex in the vertex cover.
*/
void FourthClause(std::vector<Edge> &grphDat,std::vector<std::vector<Minisat::Lit>> &listOfLits,int numvertx, int lenVertxCvr, std::unique_ptr<Minisat::Solver> &solverTT)
{
    for (unsigned int i = 0; i < grphDat.size(); i++)
    {
        Minisat::vec<Minisat::Lit> litCOne;
        for (int j = 0; j < lenVertxCvr; j++)
        {
            litCOne.push(listOfLits[grphDat[i].src_vertx][j]);
            litCOne.push(listOfLits[grphDat[i].dst_vertx][j]);
        }
        solverTT->addClause(litCOne);
        litCOne.clear(); 
    }
}



int cmpre(const void *a, const void * b)
{
    return(*(int*)a - *(int *)b);
}

int maxValAnalyze(int vertxNum, int freqMax,std::map<int,int> valFreq)
{
    int freNumber[freqMax] = {};
    for (int i = 1; i <= freqMax ; i++)
    {
        int count =0;
        for (auto &e : valFreq)
        {
            if (e.second == i)
            {
                ++count;
            }  
        }
        freNumber[i-1] = count;
    }
    return (freqMax + ((vertxNum - freqMax)/freqMax));
}

int maxValFind(std::map<int,int> pntFreq)
{
    int a =1 ;
    for (auto &e : pntFreq)
    {
        if (e.second > a)
        {
            a = e.second;
        } 
    }
    return a;
}

std::vector<Edge> edgeParse(std::string ln_dat, int num)
{
        std::regex r4(R"([0-9]+)");
        std::regex r3(R"([eE]\s*?\{\s*(<\s*?[0-9]+\s?,\s*?[0-9]+\s*?>\s*?,\s*?)+(<\s*?[0-9]+\s*?,\s*?[0-9]+\s*?>)+\s*\})");
        std::regex r2(R"(\{\s*(<\s*?[0-9]+\s?,\s*?[0-9]+\s*?>\s*?,\s*?)+(<\s*?[0-9]+\s*?,\s*?[0-9]+\s*?>)+\s*\})");
        std::regex r5(R"([eE]\s*?\{\s*(<\s*?[0-9]+\s?,\s*?[0-9]+\s*?>\s*?)\})");
        std::regex rNew(R"(\s*?\<[0-9]+,[0-9]+\s*?\>)");
        std::smatch vPnt3;
        std::smatch vPntNw;
        std::smatch vPntTst, vPntTst2;
        std::vector<Edge> edgLst;

        auto vt = std::regex_match(ln_dat,vPntTst,r3);
        auto eOT = std::regex_match(ln_dat,vPntTst2,r5);

        if (vt || eOT )
        {
            while (std::regex_search(ln_dat,vPnt3,rNew))
            {
                //std::cout << vPnt3[0] << std::endl;
                std::string at = vPnt3[0];

                int i=1;
                Edge a;
                while (std::regex_search(at,vPntNw,r4))
                {
                    if (i%2 != 0)
                    {
                        //std::cout<< vPntNw[0] << std::endl;
                        std::string temp = vPntNw[0];
                        int tempin = std::stoi(temp);
                        a.src_vertx = tempin;
                    }
                    if (i%2 == 0)
                    {
                        //std::cout<< vPntNw[0] << std::endl;
                        std::string temp = vPntNw[0];
                        int tempin = std::stoi(temp);
                        a.dst_vertx = tempin;
                    }
                    i++;
                    at = vPntNw.suffix();
                }
                if (a.src_vertx == a.dst_vertx)
                {
                    std::cerr<<"Error: A Self loop edge identified inavlid edge information";
                    
                }
                if ((a.dst_vertx>=num)||(a.src_vertx >= num))
                {
                    std::cerr<<"Error: Invalid vertex information";
                    
                }
                
                else
                {
                    edgLst.push_back(a);
                }
                ln_dat = vPnt3.suffix();
            }
        }
        else
        {
            std::cerr<<"Error: Parsing data for edges";
            
        }
        
        return edgLst;
}

int vertxParse(std::string ln_dat)
{
    std::regex r4(R"([0-9]+)");
    std::regex re(R"([vV]\s*?[0-9]+)");
    std::smatch vPOints;
    std::smatch vPnt3;
    std::string at;
    auto vt = std::regex_match(ln_dat,vPOints,re);
    if (vt)
    {
        std::regex_search(ln_dat,vPnt3,r4);
        at = vPnt3[0];
    }
    else
    {
        std::cerr<<"Error: Parsing Vertex Data";
       
    }  
    return std::stoi(at);
}

std::map<int,int> lineSame(std::vector<int> pt_vals)
{
    std::vector<int> xValues;
    std::map<int,int> freqX;
    for (auto &e : pt_vals)
    {
        auto result = freqX.insert(std::pair<int,int>(e,1));
        if (result.second == false)
        {
            result.first->second++;
        }     
    }
    return freqX; 
}