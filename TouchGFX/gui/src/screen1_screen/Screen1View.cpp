#include <gui/screen1_screen/Screen1View.hpp>
#include <touchgfx/Color.hpp>

extern volatile uint16_t CO2_Value;
extern volatile uint16_t TVOC_Value;

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
	Unicode::snprintfFloat(textArea1Buffer, TEXTAREA1_SIZE, "%.0f", (float) CO2_Value);


	Unicode::snprintfFloat(textArea2Buffer, TEXTAREA2_SIZE, "%.0f", (float) TVOC_Value);


	compute_CO2_range();
	compute_TVOC_range();

	boxWithBorder1.invalidate();
	textArea1.invalidate();

	boxWithBorder1_1.invalidate();
	textArea2.invalidate();
}


void Screen1View::compute_CO2_range(void)
{
	if(CO2_Value < 1000)
	{
		boxWithBorder1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
	}
	else if ( (CO2_Value >= 1000) ||  (CO2_Value < 2000))
	{
		boxWithBorder1.setColor(touchgfx::Color::getColorFromRGB(0xFF, 0xA5, 0x00));
	}
	else if ( CO2_Value > 2000 )
	{
		boxWithBorder1.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
	}
}

void Screen1View::compute_TVOC_range(void)
{
	if(TVOC_Value < 500)
	{
		boxWithBorder1_1.setColor(touchgfx::Color::getColorFromRGB(0, 255, 0));
	}
	else if ( (TVOC_Value >= 500) ||  (TVOC_Value < 1000))
	{
		boxWithBorder1_1.setColor(touchgfx::Color::getColorFromRGB(0xFF, 0xA5, 0x00));
	}
	else if ( TVOC_Value > 1000 )
	{
		boxWithBorder1_1.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
	}
}
