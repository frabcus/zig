#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <SDL_joystick.h>
#include <SDL_gamecontroller.h>




class Controller
{
public:
	Controller() 				{}
	virtual ~Controller()		{}
	virtual float	XAxis() = 0;		// left: -ve right +ve
	virtual float	YAxis() = 0;		// up: +ve down: -ve
	virtual bool	Button() = 0;
};

class SDLController;

class KeyboardController : public Controller
{
public:
	KeyboardController();
	~KeyboardController();
	virtual float	XAxis();
	virtual float	YAxis();
	virtual bool	Button();
private:
};




// autopilot!
class Autopilot : public Controller
{
public:
	Autopilot();
	virtual float	XAxis();
	virtual float	YAxis();
	virtual bool	Button();
private:
	float m_X;
	float m_Y;
};


// A meta-controller which sits on top of another controller.
// Only returns values when they have changed since the last check.
// Used for menus.
class LatchedController : public Controller
{
public:
	LatchedController( Controller& source );
	~LatchedController();
	virtual float	XAxis();
	virtual float	YAxis();
	virtual bool	Button();
private:
	Controller& m_Source;

	bool m_Button;

	enum { AUTOREPEAT=10 };
	int m_LeftCount;
	int m_RightCount;
	int m_UpCount;
	int m_DownCount;
};


// merges multiple controllers into one
class AggregateController : public Controller
{
public:
	AggregateController();
    virtual	~AggregateController();
	virtual float	XAxis();
	virtual float	YAxis();
	virtual bool	Button();

    void Add(Controller* src)
        { m_Sources.push_back(src); }
    void Remove(Controller* src);
private:
    std::vector<Controller*> m_Sources;
};


// Manager class for wrangling controllers
// handles attach/detach, and presents virtualised controlllers
// for gameplay and menu navigation
class ControllerMgr
{
public:
    ControllerMgr();
    ~ControllerMgr();

    // HandleJoyDevice/ControllerDevice events

    Controller& MenuController() { return m_MenuCtrl; }
    Controller& GameController() { return m_GameCtrl; }
protected:
    std::vector<SDLController*> m_Attached;
    KeyboardController m_KBCtrl;
    AggregateController m_GameCtrl;
    LatchedController m_MenuCtrl;
};





#endif	// CONTROLLER_H


