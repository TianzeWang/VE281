//
//  main.cpp
//  project3
//
//  Created by 季星佑 on 2017/10/27.
//  Copyright © 2017年 季星佑. All rights reserved.
//

#include <iostream>
#include <getopt.h>
#include "priority_queue.h"
#include "binary_heap.h"
#include "fib_heap.h"
#include "unsorted_heap.h"

using namespace std;

class point {
public:
    int x;
    int y;
    int cellweight=0;
    int pathcost=0;
    bool reached=false;
    point *predecessor=NULL;
    friend bool operator==(const point &p1,const point &p2)
    {
        return (p1.x==p2.x&&p1.y==p2.y&&p1.cellweight==p2.cellweight&&p1.pathcost==p2.pathcost&&p1.reached==p2.reached&&p1.predecessor==p2.predecessor);
    }
    friend bool operator<(const point &p1,const point &p2)
    {
        return p1.pathcost<p2.pathcost;
    }
    friend bool operator>(const point &p1,const point &p2)
    {
        return p1.pathcost>p2.pathcost;
    }
    struct compare_t
    {
        bool operator()(const point &a, const point &b) const
        {
            return (a.pathcost<b.pathcost)||((a.pathcost==b.pathcost)&&(a.x<b.x))||((a.pathcost==b.pathcost)&&(a.x==b.x)&&(a.y<b.y));
        }
    };
};

void trace_back_path(point *p);

int main(int argc,char* argv[])
{
    int width,height=0;
    cin>>width;
    cin>>height;
    int start_point_x,start_point_y,end_point_x,end_point_y;
    cin>>start_point_x>>start_point_y>>end_point_x>>end_point_y;
    point p_array[height][width];
    for(int h=0;h<height;++h)
    {
        for(int w=0;w<width;++w)
        {
            cin>>p_array[h][w].cellweight;
        }
    }
    for(int h=0;h<height;++h)
    {
        for(int w=0;w<width;++w)
        {
            p_array[h][w].x=w;
            p_array[h][w].y=h;
            p_array[h][w].pathcost=p_array[h][w].cellweight;
        }
    }
    bool verbose=false;
    string mode;
    while(true)
    {
        static struct option long_options[]=
        {
            {"verbose",no_argument,NULL,'v'},
            {"implementation",required_argument,NULL,'i'},
            {0, 0, 0, 0}
        };
        int c=getopt_long(argc,argv,"vi:",long_options,NULL);
        if(c==-1)
        {
            break;
        }
        if(c=='v')
        {
            verbose=true;
        }
        if(c=='i')
        {
            mode=optarg;
        }
    }
    priority_queue<point,point::compare_t> *PQ;
    if(mode=="BINARY")
    {
        PQ=new binary_heap<point,point::compare_t>();
    }
    else if(mode=="UNSORTED")
    {
        PQ=new unsorted_heap<point,point::compare_t>();
    }
    else if(mode=="FIBONACCI")
    {
        PQ=new fib_heap<point,point::compare_t>();
    }
    else
    {
        exit(0);
    }
    p_array[start_point_y][start_point_x].reached=true;
    PQ->enqueue(p_array[start_point_y][start_point_x]);
    int Step=0;
    while(PQ->empty()==false)
    {
        point C=PQ->dequeue_min();
        if(verbose==true)
        {
            cout<<"Step "<<Step<<endl;
            cout<<"Choose cell ("<<p_array[C.y][C.x].x<<", "<<p_array[C.y][C.x].y<<") with accumulated length "<<p_array[C.y][C.x].pathcost<<"."<<endl;
        }
        Step++;
        int delta_x[4]={1,0,-1,0};
        int delta_y[4]={0,1,0,-1};
        for(int i=0;i<4;++i)
        {
            int N_x=p_array[C.y][C.x].x+delta_x[i];
            int N_y=p_array[C.y][C.x].y+delta_y[i];
            if(N_x<0||N_x>width-1||N_y<0||N_y>height-1||p_array[N_y][N_x].reached==true)
            {
                continue;
            }
            p_array[N_y][N_x].pathcost=p_array[C.y][C.x].pathcost+p_array[N_y][N_x].cellweight;
            p_array[N_y][N_x].reached=true;
            p_array[N_y][N_x].predecessor=&p_array[C.y][C.x];
            if(p_array[end_point_y][end_point_x].x==p_array[N_y][N_x].x&&p_array[end_point_y][end_point_x].y==p_array[N_y][N_x].y)
            {
                if(verbose==true)
                {
                    cout<<"Cell ("<<p_array[N_y][N_x].x<<", "<<p_array[N_y][N_x].y<<") with accumulated length "<<p_array[N_y][N_x].pathcost<<" is the ending point."<<endl;
                }
                cout<<"The shortest path from ("<<p_array[start_point_y][start_point_x].x<<", "<<p_array[start_point_y][start_point_x].y<<") to ("<<p_array[end_point_y][end_point_x].x<<", "<<p_array[end_point_y][end_point_x].y<<") is "<<p_array[N_y][N_x].pathcost<<"."<<endl;
                cout<<"Path:"<<endl;
                trace_back_path(&p_array[end_point_y][end_point_x]);
                delete PQ;
                return 0;
            }
            else
            {
                PQ->enqueue(p_array[N_y][N_x]);
                if(verbose==true)
                {
                    cout<<"Cell ("<<p_array[N_y][N_x].x<<", "<<p_array[N_y][N_x].y<<") with accumulated length "<<p_array[N_y][N_x].pathcost<<" is added into the queue."<<endl;
                }
            }
        }
    }
    delete PQ;
    return 0;
}

void trace_back_path(point *p)
{
    if(p!=NULL)
    {
        trace_back_path(p->predecessor);
        cout<<"("<<p->x<<", "<<p->y<<")"<<endl;
    }
    else
    {
        return;
    }
}
