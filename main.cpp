#include <wx/wx.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

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
  wxTimer::Start(20);
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
  printf("FILE POINTER OBTAINED\n");
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
  static int r1 = 120;
  static int r2 = 50;
  static int r3 = 100;
  static int r4 = 80;
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

  x0 = 200;
  y0 = 200;
  x1 = x0 + r1;
  y1 = y0;
  x2 = x0 + r2*cos(alpha);
  y2 = y0 + r2*sin(alpha);
  x3 = x1 + r4*cos(beta);  
  y3 = y1 + r4*sin(beta);

  double conrod = sqrt((y3-y2)*(y3-y2)+(x3-x2)*(x3-x2));
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


