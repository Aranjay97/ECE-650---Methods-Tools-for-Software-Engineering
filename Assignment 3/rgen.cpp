#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<getopt.h>
#include<time.h>
#include<cmath>
#include<map>

std::ifstream urandom("/dev/random");
int randXY_cord(int max,int min);
std::string rand_name();
int rand_un_num(int max, int min);
void wrt(std::string msg, FILE* ostream);
clock_t start,end;
double cput_used;


struct point
{
   int ptX =0;
   int ptY =0;
   float pX;
   float pY;
};

struct path
{
  point x;
  point y;
};

class Road
{
private:

public:
    std::vector<point> road_pts;
    std::vector<path> road_pth;
    std::string road_name;
    int duplicates_exist;
    int line_pos_similar;
    std::string add_cmd;
    std::string remove_cmd;
    point nw_pt;
    path nw_path;
    Road();

};

Road::Road()
{
      this->road_pts = {};
      this->road_pth = {};
}


void roadPathGen(Road &x);
int duplicateCheck(Road &x);
int overlapCheck(Road &x,Road &y);
int selfIntersectionCheck(Road &x);
std::string roadAddCmdGen(Road &x);
std::vector<point> randPoints(int numOfLineSegs,int maxN);
point intersecPoints(path line1, path line2);
std::vector<Road> randRoadSetGen(int s, int n, int l, int c);
bool roadIntersect(path pOne, path pTwo);
float clkCheck(point x, point y, point c);
float lenFind(point x, point y);
bool btwStreet(path pOne, path pTwo);
int lineSame(Road &x);

const char* program_name;
char* S;
char* N;
char* L;
char* C;

void print_usage(char *programName)
{
    std::cout<<"Usage : "<<std::endl;
    std::cout<<programName<<std::endl;
    std::cout<<"-h get some help "<<std::endl;
    std::cout<<"-s to enter number of streets "<<std::endl;
    std::cout<<"-n to enter number of line-segments in street"<<std::endl;
    std::cout<<"-l to enter wait period"<<std::endl;
    std::cout<<"-c to enter Co-oridnate data range"<<std::endl;
}

int main(int argc, char **argv)
{
    //std::vector<Road> ttlRoads;
    std::vector<int> specs;
    int Sinp =10;
    int Ninp =5;
    int Linp =5;
    int Cinp =20;
    int errCode = 0;

    //int childStatus;
    int next_option;
    const char* const short_options = "hs:n:l:c:";

    const struct option long_options[] = {
        {"help", 0, NULL, 'h'},
        {"sVal", 1, NULL, 's'},
        {"nVal", 1, NULL, 'n'},
        {"lVal", 1, NULL, 'l'},
        {"cVal", 1, NULL, 'c'}
    };

    program_name = argv[0];
    while ((next_option = getopt_long(argc,argv,short_options,long_options,NULL))!=-1)
    {
        switch (next_option)
        {
        case 'h':
            print_usage(argv[0]);
            break;
        case 's':
            S = optarg;
            if (S != "")
            {
                Sinp = atoi(S);
                if (Sinp<2)
                {
                    errCode = 1;
                    std::cerr<<"Error: Enter No of street values greater than 2"<<std::endl;
                    exit(1);
                }
            }
            else
            {
                Sinp =10;
            }
            break;
        case 'n':
            N = optarg;
            if (N != "")
            {
                Ninp = atoi(N);
                if (Ninp < 1)
                {
                   errCode =2;
                   std::cerr<<"Error: Enter No of line segments values greater than 1"<<std::endl;
                   exit(1);
                }
            }
            else
            {
                Ninp =5;
            }
            break;
        case 'l':
            L = optarg;
            if (L != "")
            {
                Linp = atoi(L);
                if (Linp < 5)
                {
                    errCode =3;
                    std::cerr<<"Error: Enter delay values greater than 5"<<std::endl;
                    exit(1);
                }
            }
            else
            {
                Linp =5;
            }
            break;
        case 'c':
            C = optarg;
            if (C != "")
            {
                Cinp = atoi(C);
                if (Cinp < 1)
                {
                    errCode=4;
                    std::cerr<<"Error: Enter co-ordinate values greater than 1"<<std::endl;
                    exit(1);
                }
            }
            else
            {
                Cinp =20;
            }
            break;
        default:
            Sinp = 10;
            Ninp = 5;
            Linp = 5;
            Cinp = 20;
            break;
        }
    }

    int slpTime = rand_un_num(Linp,5);
    while (true)
    {
        if (errCode > 0)
        {
            std::cerr<<"Error: Wrong street spec for random street generation "<<std::endl;
            break;
        }
        start = clock();

        auto finalRoads = randRoadSetGen(Sinp, Ninp, Linp,Cinp);
        if (!finalRoads.empty())
        {

            for (unsigned i = 0; i < finalRoads.size(); i++)
            {
                std::cout<<finalRoads[i].add_cmd<<std::endl;
            }
            std::cout<<"gg"<<std::endl;
            usleep(slpTime*1000000);
            for (unsigned i = 0; i < finalRoads.size(); i++)
            {
                 std::cout<<finalRoads[i].remove_cmd<<std::endl;
            }
            finalRoads.clear();
        }
        else
        {
            std::cerr<<"Error: No streets specified to generate graph"<<std::endl;
        }
        end = clock();
        //std::cout<<"Time Used : "<<((double)(end-start))/CLOCKS_PER_SEC<<std::endl;
    }
    urandom.close();
    return 0;
}


void roadPathGen(Road &x)
{
    for (unsigned int i = 0; i < x.road_pts.size(); i++)
    {
        if (i+1 < x.road_pts.size() )
        {
            x.nw_path.x = x.road_pts[i];
            x.nw_path.y = x.road_pts[i+1];
            x.road_pth.push_back(x.nw_path);
        }
    }
}

int duplicateCheck(Road &x)
{
    int valStat = 0;
    for (unsigned int i = 0; i < x.road_pts.size(); i++)
    {
        for (unsigned int j = i+1 ; j <= (x.road_pts.size()-1) ; j++)
        {
            if((x.road_pts[i].ptX == x.road_pts[j].ptX) && (x.road_pts[i].ptY == x.road_pts[j].ptY))
            {
                valStat += 1;
            }
        }
    }

    return valStat;
}

int lineSame(Road &x)
{
    std::vector<int> xVal,yVal;
    std::map<int,int> freqX, freqY;
    int totalNum =0;
    int dX =0;
    int dY =0;
    for (unsigned int j = 1; j < x.road_pts.size(); j++)
    {
        xVal.push_back(x.road_pts[j].ptX);
        yVal.push_back(x.road_pts[j].ptY);

    }
    for (auto &elem : xVal)
    {
        auto result = freqX.insert(std::pair<int,int>(elem,1));
        if (result.second == false)
        {
            result.first->second++;
        }

    }

    for (auto &elem : yVal)
    {
        auto result2 = freqY.insert(std::pair<int,int>(elem,1));
        if (result2.second == false)
        {
            result2.first->second++;
        }

    }

    for (auto &elem : freqX)
    {
        if (elem.second >= 3)
        {
            dX = elem.first;
        }
    }

    for (auto &elem : freqY)
    {
        if (elem.second >= 3)
        {
            dY = elem.first;
        }
    }
    for (unsigned int k = 0; k < x.road_pts.size(); k++)
    {
       if ((dX == x.road_pts[k].ptX)||(dY == x.road_pts[k].ptY))
       {
           totalNum += 1;
       }

    }

    xVal.clear();
    yVal.clear();
    freqX.clear();
    freqY.clear();
    return totalNum;

}

int selfIntersectionCheck(Road &x)
{
    int doesntIntersect = 0;
    int roadPntPos=-1;
    point intersecPts;
    intersecPts.ptX =0;
    intersecPts.ptY =0;
    for (unsigned int i = 0; i < x.road_pth.size(); i++)
    {
        for (unsigned int j = i+1; j <= x.road_pth.size()-1; j++)
        {
            if (roadIntersect(x.road_pth[i], x.road_pth[j]) || btwStreet(x.road_pth[i],x.road_pth[j]))
            {
                intersecPts = intersecPoints(x.road_pth[i],x.road_pth[j]);
                doesntIntersect =0;
                for (unsigned k = 0; k < x.road_pts.size(); k++)
                {
                    if ((intersecPts.pX ==x.road_pts[k].ptX) && (intersecPts.pY == x.road_pts[k].ptY))
                    {
                     doesntIntersect +=1 ;
                    }
                }
                if (doesntIntersect == 0)
                {
                    roadPntPos += 1;
                }
            }
        }
    }
    return roadPntPos;
}

//Check street Paths

bool roadIntersect(path pOne, path pTwo)
{
    return (clkCheck(pOne.x,pTwo.x,pTwo.y) != clkCheck(pOne.y,pTwo.x,pTwo.y)) &&(clkCheck(pOne.x,pOne.y,pTwo.x)!=clkCheck(pOne.x,pOne.y,pTwo.y));
}

float clkCheck(point x, point y, point c)
{
    return ((c.ptY - x.ptY)*(y.ptX - x.ptX)) > ((y.ptY-x.ptY)*(c.ptX-x.ptX));
}

float lenFind(point x, point y)
{
    float length;
    length = sqrt(((y.ptX - x.ptX)*(y.ptX - x.ptX))+((y.ptY - x.ptY)*(y.ptY - x.ptY)));
    return length;
}

bool btwStreet(path pOne, path pTwo)
{
    if ((lenFind(pOne.x,pTwo.x) + lenFind(pOne.y,pTwo.x) == lenFind(pOne.x,pOne.y)) ||
       (lenFind(pOne.x,pTwo.y) + lenFind(pOne.y,pTwo.y) == lenFind(pOne.x,pOne.y)) ||
       (lenFind(pTwo.x,pOne.x) + lenFind(pTwo.y,pOne.x) == lenFind(pTwo.x,pTwo.y)) ||
       (lenFind(pTwo.x,pTwo.y) + lenFind(pTwo.y,pOne.x) == lenFind(pTwo.x,pTwo.y)))
    {
       return true;
    }
    else
    {
        return false;
    }


}

std::string rand_name()
{

    if (urandom.fail())
    {
        std::cerr << "Error: Cannot open /dev/urandom \n";
        return NULL;
    }
    char ch = 'x';
    std::string name= "";
    for (int i = 0; i < 3; i++)
    {
        urandom.read(&ch, 1);
        int val =0;
        val = (65 + (((unsigned int)ch)%(90-65+1)));
        name = name + (char)val;
    }


    return name;
}

int randXY_cord(int max, int min)
{
    if (urandom.fail())
    {
        std::cerr << "Error: Cannot open /dev/urandom \n";
        return 1;
    }
    unsigned int ch = 1;
    urandom.read((char*)&ch, 1);
    int val =0;
    val = (min + (((unsigned int)ch)%(max-min+1)));
    return val;


}

int rand_un_num(int max, int min)
{
    if (urandom.fail())
    {
        std::cerr << "Error: Cannot open /dev/urandom \n";
        return 1;
    }
    unsigned int ch =1;
    urandom.read((char*)&ch, 1);
    int val =0;
    if (max == 10)
    {
        val = 5;
    }
    else
    {
        val = (min + (((unsigned int)ch)%(max-min+1)));
    }
    return val;
}

std::string roadAddCmdGen(Road &x)
{
    std::string add_cmd = "add ";
    char start = '"';

    add_cmd = add_cmd + start + x.road_name + start;

    std::string xycord,spc_btw_cords;
     for (unsigned int i = 0; i < x.road_pts.size(); i++){
         xycord = '(' + std::to_string(x.road_pts[i].ptX) + ',' + std::to_string(x.road_pts[i].ptY) + ')';
         spc_btw_cords = spc_btw_cords + ' ' + xycord;
     }

     add_cmd = add_cmd + spc_btw_cords;

     return add_cmd;
}

std::string generateStreetremoveCmd(Road &x)
{
    std::string remove_cmd = "rm ";
    char start = '"';

    remove_cmd = remove_cmd + start + x.road_name + start;
    return remove_cmd;

}

std::vector<point> randPoints(int numOfLineSegs,int maxN)
{
    point nw_pt;
    std::vector<point> sPoints;
    for (int i = 0; i < numOfLineSegs+1; i++)
    {
        nw_pt.ptX = randXY_cord(maxN,-maxN);
        nw_pt.ptY = randXY_cord(maxN,-maxN);
        sPoints.push_back(nw_pt);
    }
    return sPoints;
}

std::vector<Road> randRoadSetGen(int s, int n, int l, int c)
{

    int numOfRoads=rand_un_num(s,2);
    int numOfLineSegs=rand_un_num(n,1);
    std::string nwName = "";
    std::vector<Road> ttlRoads = {};
    for (int j = 0; j < numOfRoads; j++)
    {
        Road *x = new Road();
        nwName = rand_name();
        for (int i = 0; i < numOfLineSegs+1; i++)
        {
            x->nw_pt.ptX = randXY_cord(c,-c);
            x->nw_pt.ptY = randXY_cord(c,-c);
            x->road_pts.push_back(x->nw_pt);
        }
        x->road_name = nwName;
        roadPathGen(*x);
        x->duplicates_exist = duplicateCheck(*x);
        x->line_pos_similar = lineSame(*x);
        //Check for self intersec
        if (numOfLineSegs >1)
        {
            int sPos = selfIntersectionCheck(*x);
            if ((sPos >=0) ||(x->duplicates_exist>0) || (x->line_pos_similar>= 3))
            {
                int tries = 25;
                while (tries != 0)
                {
                    auto newSPoints = randPoints(numOfLineSegs,c);
                    x->road_pts.clear();
                    x->road_pts = newSPoints;
                    x->duplicates_exist =duplicateCheck(*x);
                    x->line_pos_similar = lineSame(*x);
                    x->road_pth.clear();
                    roadPathGen(*x);
                    if ((selfIntersectionCheck(*x) >= 0) ||(x->duplicates_exist >0)||(x->line_pos_similar>= 3) )
                    {
                        tries -= 1;
                        if(tries ==0 )
                        {
                            std::cerr<<"Error: Unable to generate streets after 25 attempts \n";
                            exit(1);
                        }
                    }
                    else
                    {
                        break;
                    }
                }

            }

        }
        x->add_cmd = roadAddCmdGen(*x);
        x->remove_cmd = generateStreetremoveCmd(*x);
        ttlRoads.push_back(*x);
    }
    return ttlRoads;

}

point intersecPoints(path line1, path line2)
{
    point intersec;
    int x1 = line1.x.ptX;
    int y1 = line1.x.ptY;
    int x2 = line1.y.ptX;
    int y2 = line1.y.ptY;
    int x3 = line2.x.ptX;
    int y3 = line2.x.ptY;
    int x4 = line2.y.ptX;
    int y4 = line2.y.ptY;

    float xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    float xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4));
    intersec.pX = xnum / xden;

    float ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    float yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    intersec.pY = ynum / yden;

    return intersec;

}
