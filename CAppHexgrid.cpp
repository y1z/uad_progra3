#include "CAppHexgrid.h"
// ---------------------------- Constructors and destructor --------------------------------//

CAppHexgrid::CAppHexgrid()
	:CApp()
{


}

CAppHexgrid::CAppHexgrid(uint16_t Width, uint16_t Hight)
	:CApp(Width,Hight)
{

}

CAppHexgrid::~CAppHexgrid()
{
	if (ptr_Grid != nullptr)
	{
		delete ptr_Grid;
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
		CGrid *ptr_Grid = new CGrid(getOpenGLRenderer(), getGameWindow(), this->m_HexGridHight * this->m_HexGridWidth,*this);
		ptr_Grid->m_GridHight = this->m_HexGridHight;
		ptr_Grid->m_GridWidth = this->m_HexGridWidth;

		ptr_Grid->Start();

	
	}


}
/*!
*/
void CAppHexgrid::render(){
	ptr_Grid->Render();

}


bool CAppHexgrid::initializeMenu(){


	return false;
}