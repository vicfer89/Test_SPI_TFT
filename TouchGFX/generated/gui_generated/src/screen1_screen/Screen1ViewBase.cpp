/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>

Screen1ViewBase::Screen1ViewBase() :
    flexButtonCallback(this, &Screen1ViewBase::flexButtonCallbackHandler)
{

    __background.setPosition(0, 0, 320, 240);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    gauge1.setBackground(touchgfx::Bitmap(BITMAP_BLUE_GAUGES_ORIGINAL_GAUGE_BACKGROUND_STYLE_00_ID));
    gauge1.setPosition(69, -5, 251, 251);
    gauge1.setCenter(125, 125);
    gauge1.setStartEndAngle(-90, 90);
    gauge1.setRange(400, 4000);
    gauge1.setValue(400);
    gauge1.setNeedle(BITMAP_BLUE_NEEDLES_ORIGINAL_GAUGE_NEEDLE_STYLE_00_ID, 11, 55);
    gauge1.setMovingNeedleRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);
    gauge1.setSteadyNeedleRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);

    flexButton1.setBoxWithBorderPosition(0, 0, 69, 50);
    flexButton1.setBorderSize(5);
    flexButton1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(0, 102, 153), touchgfx::Color::getColorFromRGB(0, 153, 204), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(51, 102, 153));
    flexButton1.setPosition(0, 0, 69, 50);
    flexButton1.setAction(flexButtonCallback);

    flexButton2.setBoxWithBorderPosition(0, 0, 69, 50);
    flexButton2.setBorderSize(5);
    flexButton2.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(0, 102, 153), touchgfx::Color::getColorFromRGB(0, 153, 204), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(51, 102, 153));
    flexButton2.setPosition(0, 190, 69, 50);
    flexButton2.setAction(flexButtonCallback);

    add(__background);
    add(gauge1);
    add(flexButton1);
    add(flexButton2);
}

void Screen1ViewBase::setupScreen()
{

}

void Screen1ViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &flexButton1)
    {
        //Interaction2
        //When flexButton1 clicked show gauge1
        //Show gauge1
        gauge1.setVisible(true);
        gauge1.invalidate();
    }
    else if (&src == &flexButton2)
    {
        //Interaction1
        //When flexButton2 clicked hide gauge1
        //Hide gauge1
        gauge1.setVisible(false);
        gauge1.invalidate();
    }
}
