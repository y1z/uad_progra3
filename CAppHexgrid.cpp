#include "CAppHexgrid.h"
// ---------------------------- Constructors and destructor --------------------------------//

CAppHexgrid::CAppHexgrid()
	:CApp()
{
}

CAppHexgrid::CAppHexgrid(uint16_t ScreenWidth, uint16_t ScreenHight)
	:CApp(ScreenWidth, ScreenHight)
{
}

CAppHexgrid::CAppHexgrid(uint16_t ScreenWidth, uint16_t ScreenHight, 
												 uint32_t HexGridWidth, uint32_t hexGridhight)
	: CApp(ScreenWidth, ScreenHight),
	m_HexGridHight(hexGridhight),
	m_HexGridWidth(HexGridWidth)
{
}

CAppHexgrid::~CAppHexgrid()
{
	if (mptr_Grid != nullptr)
	{
		delete mptr_Grid;
	}
}

 //----------------------------------- Functions-------------------------------------------------//

void CAppHexgrid::initialize()
{
	
}

void CAppHexgrid::update(double TimeDelta){

}

void CAppHexgrid::run()
{
	if(canRun())
	{
		mptr_Grid = new CGrid(getOpenGLRenderer(), getGameWindow(),
													this->m_HexGridHight * this->m_HexGridWidth
													, this);
		mptr_Grid->m_hexGridHight = this->m_HexGridHight;
		mptr_Grid->m_hexGridWidth = this->m_HexGridWidth;

		mptr_Grid->Start();
	}

}
/*!
Calls the render function from CGrid 
because that's the only way i found to 
follow the specification given to me about how to 
organize the class hierarchy
*/
void CAppHexgrid::render(){
	mptr_Grid->Render();
}


bool CAppHexgrid::initializeMenu(){

	return false;
}