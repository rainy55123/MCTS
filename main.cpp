#include "windows.h"
#include "conio.h"
#include <cv.h>
#include <highgui.h>
#include "tree.h"
#include<iostream>
#include<cstdlib>
using namespace std;
enum{X,O};

bool isEnd(int map[9]);
void DrawMap(int map[9]);
void onMouse(int Event,int x,int y,int flags,void* param );


IplImage *Image1;
CvPoint PointArray[4],VertexOne,VertexThree,TextPosition;
CvScalar Color_White,Color_Red,Color_Black;
int PolyVertexNumber,board[9]={0,0,0,0,0,0,0,0,0};
int Thickness=5;
int Shift,scale,order=1,Type=0;
double FScale=10;
Tree Tree;

int main()
{
    int x,y,i,j;
    cvNamedWindow("Test",1);
    DrawMap(board);
    cvShowImage("Test",Image1);
    cvSetMouseCallback("Test",onMouse,NULL);
    cvWaitKey(0);
    
    return 0;
}

bool isEnd(int map[9])
{
     if(map[0]==map[1]&map[1]==map[2]&map[0]!=0)
     {
           return true;                           
     }
     else if(map[3]==map[4]&map[4]==map[5]&map[3]!=0)
     {
           return true;                           
     }
     else if(map[6]==map[7]&map[7]==map[8]&map[6]!=0)
     {
           return true;                           
     }
     else if(map[0]==map[3]&map[3]==map[6]&map[0]!=0)
     {
           return true;                           
     }
     else if(map[1]==map[4]&map[4]==map[7]&map[1]!=0)
     {
           return true;                           
     }
     else if(map[2]==map[5]&map[5]==map[8]&map[2]!=0)
     {
           return true;                           
     }
     else if(map[0]==map[4]&map[4]==map[8]&map[0]!=0)
     {
           return true;                           
     }
     else if(map[2]==map[4]&map[4]==map[6]&map[2]!=0)
     {
           return true;                           
     }
     else if(map[0]!=0&&map[1]!=0&&map[2]!=0&&map[3]!=0&&map[4]!=0&&map[5]!=0&&map[6]!=0&&map[7]!=0&&map[8]!=0)
     {
           cout<<"平手\n案ENTER結束"<<endl;
           Type=2; 
           return true;
     }
     return false;
}

void DrawMap(int map[9])
{
     int x,y;
     CvFont Font1=cvFont(FScale,Thickness);
     CvSize ImageSize1 = cvSize(800,600);
     Image1 = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);
     CvScalar Color_Red=CV_RGB(255,0,0);
     CvScalar Color_White=CV_RGB(255,255,255);
     CvScalar Color_Black=CV_RGB(0,0,0);
     PolyVertexNumber=4;
     PointArray[0]=cvPoint(0,0);
     PointArray[1]=cvPoint(800,0);
     PointArray[2]=cvPoint(800,600);
     PointArray[3]=cvPoint(0,600);
     cvFillConvexPoly(Image1,PointArray,PolyVertexNumber,Color_White,CV_AA,Shift);
     PointArray[0]=cvPoint(600,35);
     PointArray[1]=cvPoint(750,35);
     PointArray[2]=cvPoint(750,185);
     PointArray[3]=cvPoint(600,185);
     cvFillConvexPoly(Image1,PointArray,PolyVertexNumber,Color_Red,CV_AA,Shift);
     TextPosition = cvPoint(620,165);
     if(Type==0)
     {
         cvPutText(Image1,"O",TextPosition,&Font1,Color_Black);        
     }
     else
     {
         cvPutText(Image1,"X",TextPosition,&Font1,Color_Black);      
     }
     for(x=0;x<3;x++)
     {
         for(y=0;y<3;y++)
         {
                      if(map[y+x*3]==1)
                      {
                            TextPosition = cvPoint(70+y*150,165+x*150);
                            cvPutText(Image1,"O",TextPosition,&Font1,Color_Black);
                      }
                      else if(map[y+x*3]==2)
                      {
                            TextPosition = cvPoint(70+y*150,165+x*150);
                            cvPutText(Image1,"X",TextPosition,&Font1,Color_Black);
                      }
                      VertexOne=cvPoint(50+y*150,35+x*150);
                      VertexThree=cvPoint(200+y*150,185+x*150);   
                      cvRectangle(Image1,VertexOne,VertexThree,Color_Black,Thickness,CV_AA,Shift);        
         }            
     }
}


void onMouse(int Event,int x,int y,int flags,void* param ) 
{

    int px,py,target;
    if(Event==1)
    {
          if(x>=50&&x<500&&y>=35&&y<485)
          {
                 px=(x-50)/150;
                 py=(y-35)/150;
                 if(board[px+3*py]==0)
                 {
                       board[px+3*py]=Type+1;   
                       if(!isEnd(board))
                       {
                             Tree.MCTS(board,3000,Type);  
                             if(isEnd(board))
                             {
                                   DrawMap(board);
                                   cvShowImage("Test",Image1);
                                   if(Type!=2)
                                        cout<<"結束,電腦勝利\n案ENTER結束"<<endl; 
                                   cvDestroyWindow("Test");           
                             }       
                       }
                       else
                       {
                            DrawMap(board);
                            cvShowImage("Test",Image1);
                            if(Type!=2)
                                 cout<<"結束,玩家勝利\n案ENTER結束"<<endl; 
                            cvDestroyWindow("Test");
                       }   
                 }          
          }      
          else if(x>=600&&x<=750&&y>=35&&y<=185)
          {
                 for(int i=0;i<9;i++)
                     board[i]=0;
                 if(Type==0)
                 {
                     Tree.MCTS(board,1000,Type);
                     Type=1;
                 }
                 else 
                 {
                     Type=0;
                 }
          }
    }
    DrawMap(board);
    cvShowImage("Test",Image1);
}
