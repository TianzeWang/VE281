//
//  main.cpp
//  testcase
//
//  Created by 季星佑 on 2017/11/5.
//  Copyright © 2017年 季星佑. All rights reserved.
//

#include <iostream>
#include <time.h>
using namespace std;

int main(int argc, const char * argv[])
{
    srand(time(0));
    int width=30;
    int height=width;
    int sx=0;
    int sy=0;
    int ex=width-1;
    int ey=height-1;
    int map[height][width];
    for(int i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
            map[i][j]=rand()%10;
        }
    }
    cout<<width<<" "<<height<<endl;
    cout<<sx<<" "<<sy<<endl;
    cout<<ex<<" "<<ey<<endl;
    for(int i=0;i<height;++i)
    {
        for(int j=0;j<width;++j)
        {
            cout<<map[i][j]<<" ";
        }
        cout << endl;
    }
    return 0;
}
