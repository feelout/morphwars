#include "List.h"
#include "FontSystem.h"

using namespace Gui;

const int List::ITEM_HEIGHT;

List::List(Rect frame)
	: Widget(frame), topItem(0), currentItem(-1)
{
}

List::List(Rect frame, std::vector<std::string> &content)
	: Widget(frame), topItem(0)
{
	items.assign(content.begin(), content.end());
	currentItem = items.size() - 1;
}

void List::frameUpdated()
{
	maxItems = frame.h / ITEM_HEIGHT;
}

void List::mouseMoved(int x, int y)
{
	if(!frame.isPointInRect(x, y))
		return;

	currentItem = (y - frame.y) / ITEM_HEIGHT;

	//FIXME: check scrolling
	if( (currentItem == topItem + maxItems) && (currentItem < items.size()) )
	{
		topItem++;
	}
}

void List::mouseLMBClicked(int x, int y)
{
	if(!frame.isPointInRect(x, y))
		return;

	itemSelected(currentItem);
}

void List::draw(Graphics::Drawer *target)
{
	int lastItem = (items.size() >= maxItems) ? (topItem + maxItems) : (items.size());

	RGBColor color;
	for(int i=topItem; i < lastItem; ++i)
	{
		//FIXME: Write item`s string itself
		if(i == currentItem)
		{
			color = RGBColor(30, 80, 120);
		}
		else
		{
			color = RGBColor(30, 30, 30);
		}
		target->fillRect(Rect(frame.x, frame.y+i*ITEM_HEIGHT, frame.w, ITEM_HEIGHT), color);

		Graphics::FontSystem::getInstance()->print(target, items[i], frame.x, frame.y+i*ITEM_HEIGHT, RGBColor(255,0,0));	
	}
}
