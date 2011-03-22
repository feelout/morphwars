#include "OrderPack.h"
#include "MouseSelector.h"
#include "Surface.h"
#include "Drawer.h"
#include "Logger.h"

using namespace Gui;
using namespace Core;

const int OrderPack::ORDER_BUTTON_WIDTH;
const int OrderPack::ORDER_BUTTON_HEIGHT;

OrderPack::OrderPack(Rect frame)
	: HBox(frame, 5)
{
}

void OrderPack::draw(Graphics::Surface *target)
{
	HBox::draw(target);
	Graphics::Drawer(target).drawRect(frame, RGBColor::WHITE);
}

