/*
****************************************
******** Crank Rocker Simulator ********
*
* 2014 Shohei Aoki, All rights reserved.
*
****************************************
*/

#include <wx/wx.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <wx/slider.h>

using namespace std;

const int ID_SLIDER1 = 1;
const int ID_SLIDER2 = 2;
const int ID_SLIDER3 = 3;
const int ID_SLIDER4 = 4;

class MyFrame;

class MyTimer : public wxTimer
{
public:
    MyTimer(MyFrame* frame);
    void Notify();
    void start();
private:
    MyFrame* frame; 
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
    void paintEvent(wxPaintEvent& event);
    void render(wxDC& dc);
    void onClose(wxCloseEvent& evt);
    ~MyFrame();
    MyTimer *timer;
    FILE *fp;
    wxSlider *slider1,*slider2,*slider3,*slider4,*slider5;
    void OnScroll1(wxScrollEvent& event);
    void OnScroll2(wxScrollEvent& event);
    void OnScroll3(wxScrollEvent& event);
    void OnScroll4(wxScrollEvent& event);
    void OnScroll5(wxScrollEvent& event);
    int pos1,pos2,pos3,pos4,pos5;
    int r1,r2,r3,r4,speed;
    DECLARE_EVENT_TABLE();
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    MyFrame* frame;
};

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
  cout << "MyApp initializing" << endl;
  frame = new MyFrame();
  frame->Show();
  return true;
}

MyTimer::MyTimer(MyFrame* frame) : wxTimer()
{
 MyTimer::frame = frame;
}

void MyTimer::Notify()
{
  frame->Refresh();
}

void MyTimer::start()
{
  wxTimer::Start(10);
}

MyFrame::MyFrame()
    : wxFrame(NULL,-1,wxT("CrankRockerSimulator"),wxPoint(50,50),wxSize(600,400))
{
  cout << "get into constructer" << endl;
  timer = new MyTimer(this);
  cout << "timer constructed" << endl;
  //Show();
  cout << "starting timer " << endl;
  timer->start();
  printf("get file pointer\n");
  fp = fopen("plot.dat","w");
  if(fp==NULL) exit(0);
  printf("File pointer obtained\n");
  wxPanel *panel = new wxPanel(this);
  slider1 = new wxSlider(panel,ID_SLIDER1,120,0,200,wxPoint(10,20),wxSize(200,-1));
  slider2 = new wxSlider(panel,ID_SLIDER2,50,0,200,wxPoint(10,50),wxSize(200,-1));
  slider3 = new wxSlider(panel,ID_SLIDER3,100,0,200,wxPoint(10,80),wxSize(200,-1));
  slider4 = new wxSlider(panel,ID_SLIDER4,80,0,200,wxPoint(10,110),wxSize(200,-1));
  r1 = 120;
  r2 = 50;
  r3 = 100;
  r4 = 80;
  Connect(ID_SLIDER1,wxEVT_COMMAND_SLIDER_UPDATED,
          wxScrollEventHandler(MyFrame::OnScroll1));
  Connect(ID_SLIDER2,wxEVT_COMMAND_SLIDER_UPDATED,
          wxScrollEventHandler(MyFrame::OnScroll2));
  Connect(ID_SLIDER3,wxEVT_COMMAND_SLIDER_UPDATED,
          wxScrollEventHandler(MyFrame::OnScroll3));
  Connect(ID_SLIDER4,wxEVT_COMMAND_SLIDER_UPDATED,
          wxScrollEventHandler(MyFrame::OnScroll4));
}

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
EVT_CLOSE(MyFrame::onClose)
EVT_PAINT(MyFrame::paintEvent)
END_EVENT_TABLE()

void MyFrame::paintEvent(wxPaintEvent& event)
{
  wxPaintDC dc(this);
  render(dc);
}

/*****************************
*** GEOMETORY DESCRIPTION ***
*****************************
P2   r3  P3       Pn = (xn,yn)
  --------
  \       \
r2 \       \ r4
    \       \
     --------
    P0  r1   P1
*****************************/

void MyFrame::render(wxDC& dc)
{
  static int alpha_deg = 0;
  static double x0,y0,x1,y1,x2,y2,x3,y3,alpha,alpha1,alpha2,beta;

  alpha_deg += 2;
  if(alpha_deg >360) alpha_deg=0;
  alpha = (double)alpha_deg*M_PI/180;
  alpha1 = acos((r1-r2*cos(alpha))/sqrt(pow(r1,2)+pow(r2,2)-2*r1*r2*cos(alpha)));
  alpha2 = acos((pow(r1,2)+pow(r2,2)-pow(r3,2)+pow(r4,2)-2*r1*r2*cos(alpha))/(2*r4*sqrt(pow(r1,2)+pow(r2,2)-2*r1*r2*cos(alpha))));
  //fprintf(fp,"%f %f\n",-1.0*alpha,alpha2);
  //cout << "alpha1: " << alpha1 << ", alpha2: " << alpha2 << endl;
  //cout << "content: " << (pow(r1,2)+pow(r2,2)-pow(r3,2)+pow(r4,2)-2*r1*r2*cos(alpha))/(2*r4*sqrt(pow(r1,2)+pow(r2,2)-2*r1*r2*cos(alpha))) << endl;
  if(alpha_deg < 180){
      //cout << "0 < a < 180" << endl;
      beta = M_PI - (alpha1 - alpha2); 
  } else {
      //cout << "180 < a < 360" << endl;
      beta = M_PI + (alpha1 + alpha2); 
  }

  x0 = 300;
  y0 = 200;
  x1 = x0 + r1;
  y1 = y0;
  x2 = x0 + r2*cos(alpha);
  y2 = y0 + r2*sin(alpha);
  x3 = x1 + r4*cos(beta);  
  y3 = y1 + r4*sin(beta);

  //double conrod = sqrt((y3-y2)*(y3-y2)+(x3-x2)*(x3-x2));
  //cout << "conrod length: " << conrod << endl;
  //cout << "alpha_deg: " << alpha_deg << ", x2: " << x2 << ", y2: " << y2 << endl;

  dc.SetBackground(*wxWHITE_BRUSH);
  dc.Clear();
  dc.SetPen(wxPen(wxColor(0,0,0),2));
  //dc.DrawLine(x0,y0,x1,y1);
  dc.DrawLine(x0,y0,x2,y2);
  dc.DrawLine(x2,y2,x3,y3);
  dc.DrawLine(x1,y1,x3,y3);
}

void MyFrame::onClose(wxCloseEvent& event)
{
  timer->Stop();
  event.Skip();
}

MyFrame::~MyFrame()
{
  delete timer;
  fclose(fp);
}

void MyFrame::OnScroll1(wxScrollEvent& event)
{
  pos1 = slider1->GetValue();
  r1 = pos1;
  cout << pos1 << endl;
}

void MyFrame::OnScroll2(wxScrollEvent& event)
{
  pos2 = slider2->GetValue();
  cout << pos2 << endl;
}

void MyFrame::OnScroll3(wxScrollEvent& event)
{
  pos3 = slider3->GetValue();
  r3 = pos3;
  cout << pos3 << endl;
}

void MyFrame::OnScroll4(wxScrollEvent& event)
{
  pos4 = slider4->GetValue();
  r4 = pos4;
  cout << pos4<< endl;
}

void MyFrame::OnScroll5(wxScrollEvent& event)
{
  pos5 = slider5->GetValue();
  speed = pos5;
  cout << pos5 << endl;
}

