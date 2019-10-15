#include "HS_Dial_Widget.h"

HS_Dial_Widget::HS_Dial_Widget(TFT_eSPI TFT)
{
	cpuLoadDial = new TFT_eSprite(&TFT);
	cpuLoadNeedle = new TFT_eSprite(&TFT);
	// Create the dial Sprite
	cpuLoadDial->setColorDepth(8);       // Size is odd (i.e. 91) so there is a centre pixel at 45,45
	cpuLoadDial->createSprite(91, 91);   // 8bpp requires 91 * 91 = 8281 bytes
	cpuLoadDial->setPivot(45, 45);       // set pivot in middle of dial Sprite

	CreateNeedle();
}

HS_Dial_Widget::~HS_Dial_Widget()
{

}

void HS_Dial_Widget::DrawDialScale(TFT_eSPI TFT, int16_t start_angle, int16_t end_angle, int16_t increment, uint16_t ringColor)
{
	// Draw dial outline
	cpuLoadDial->fillSprite(TFT_TRANSPARENT);           // Fill with transparent colour
	cpuLoadDial->fillCircle(45, 45, 43, ringColor);  // Draw dial outer

	// Hijack the use of the needle Sprite since that has not been used yet!

	TFT_eSprite tickSprite = TFT_eSprite(&TFT);
	tickSprite.setColorDepth(8);
	tickSprite.createSprite(3, 3);     // 3 pixels wide, 3 high
	tickSprite.fillSprite(TFT_WHITE);  // Fill with white
	tickSprite.setPivot(1, 43);        //  Set pivot point x to the Sprite centre and y to marker radius

	for (int16_t angle = start_angle; angle <= end_angle; angle += increment) {
		tickSprite.pushRotated(cpuLoadDial, angle); // Sprite is used to make scale markers
		yield(); // Avoid a watchdog time-out
	}
}


void HS_Dial_Widget::CreateNeedle()
{
	cpuLoadNeedle->setColorDepth(8);
	cpuLoadNeedle->createSprite(11, 49); // create the needle Sprite 11 pixels wide by 49 high

	cpuLoadNeedle->fillSprite(TFT_BLACK);          // Fill with black

	// Define needle pivot point
	uint16_t piv_x = cpuLoadNeedle->width() / 2;   // x pivot of Sprite (middle)
	uint16_t piv_y = cpuLoadNeedle->height() - 10; // y pivot of Sprite (10 pixels from bottom)
	cpuLoadNeedle->setPivot(piv_x, piv_y);         // Set pivot point in this Sprite

	// Draw the red needle with a yellow tip
	// Keep needle tip 1 pixel inside dial circle to avoid leaving stray pixels
	cpuLoadNeedle->fillRect(piv_x - 1, 2, 3, piv_y + 8, TFT_RED);
	cpuLoadNeedle->fillRect(piv_x - 1, 2, 3, 5, TFT_YELLOW);

	// Draw needle centre boss
	cpuLoadNeedle->fillCircle(piv_x, piv_y, 5, TFT_MAROON);
	cpuLoadNeedle->drawPixel(piv_x, piv_y, TFT_WHITE);     // Mark needle pivot point with a white pixel
}

void HS_Dial_Widget::PlotDial(int16_t x, int16_t y, int16_t angle, String label, float val)
{
	DrawEmptyDial(label, val);

	// Push a rotated needle Sprite to the dial Sprite, with black as transparent colour
	cpuLoadNeedle->pushRotated(cpuLoadDial, angle, TFT_BLACK); // dial is the destination Sprite

	// Push the resultant dial Sprite to the screen, with transparent colour
	cpuLoadDial->pushSprite(x, y, TFT_TRANSPARENT);
}

void HS_Dial_Widget::DrawEmptyDial(String label, float val)
{
	// Draw black face
	cpuLoadDial->fillCircle(45, 45, 40, TFT_BLACK);
	cpuLoadDial->drawPixel(45, 45, TFT_WHITE);        // For demo only, mark pivot point with a while pixel

	cpuLoadDial->setTextDatum(TC_DATUM);              // Draw dial text
	cpuLoadDial->drawString(label, 45, 15, 2);
	cpuLoadDial->drawFloat(val, 1, 45, 60, 2);
}