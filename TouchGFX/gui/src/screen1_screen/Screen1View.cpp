#include <gui/screen1_screen/Screen1View.hpp>

extern volatile uint16_t CO2_Value;

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
	gauge1.updateValue(CO2_Value, 0);
	gauge1.invalidate();
}
