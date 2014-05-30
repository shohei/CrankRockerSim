#include <wx/wx.h>
#include <iostream>

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
  timer = new MyTimer(this);
  Show();
  timer->start();
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

void MyFrame::render(wxDC& dc)
{
  static int theta = 0;
  theta += 1;
  if(theta>360) theta=0;
  static double x,y;
  static int origin = 200;
  x = origin + 50*cos(-1.0*(double)theta*3.14/180);
  y = origin + 50*sin(-1.0*(double)theta*3.14/180);
  cout << "theta: " << theta << ", x: " << x << ", y: " << y << endl;

  dc.SetBackground(*wxWHITE_BRUSH);
  dc.Clear();
  dc.SetPen(wxPen(wxColor(0,0,0),2));
  dc.DrawLine(origin,origin,x,y);
}

void MyFrame::onClose(wxCloseEvent& event)
{
  timer->Stop();
  event.Skip();
}

MyFrame::~MyFrame()
{
  delete timer;
}


