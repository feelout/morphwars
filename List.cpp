#include "List.h"
#include "FontSystem.h"
#include "Logger.h"
#include "Drawer.h"

using namespace Gui;

const int List::ITEM_HEIGHT;

List::List(Rect frame)
	: Widget(frame), topItem(0), currentItem(-1)//, clickedCb(NULL), itemChangedCb(NULL)
{
	Utility::Logger::getInstance()->log("List::List(Rect)\n");

	frameUpdated();
}

List::List(Rect frame, std::vector<std::string> &content)
	: Widget(frame), topItem(0)//, clickedCb(NULL), itemChangedCb(NULL)
{
	items.assign(content.begin(), content.end());
	currentItem = items.size() - 1;

	frameUpdated();
}

void List::frameUpdated()
{
	Utility::Logger::getInstance()->log("List frame updated\n");
	maxItems = frame.h / ITEM_HEIGHT;
}

void List::setCurrentItem(int n)
{
	Utility::Logger::getInstance()->log("Current: %i of %i\nTop: %i, Max: %i\n", n, items.size(), topItem, maxItems);
	if(n >= items.size()) return;

	currentItem = n;

	/*if(itemChangedCb)
		itemChangedCb(items[n]);*/
	currentItemChanged(n);

	if( (currentItem == topItem + maxItems) && (currentItem < items.size()) )
	{
		Utility::Logger::getInstance()->log("Scrolling\n");
		topItem++;
	}
}

/*void List::setOnItemClicked(ListItemClickCallback cb)
{
	clickedCb = cb;
}

void List::setOnCurrentItemChanged(ListCurrentItemChangedCallback cb)
{
	itemChangedCb = cb;
}*/

bool List::mouseMoved(int x, int y)
{
	Utility::Logger::getInstance()->log("List::mouseMoved\n");
	if(!frame.isPointInRect(x, y))
		return false;

	setCurrentItem((y - frame.y) / ITEM_HEIGHT);
	return true;
}

bool List::mouseLMBClicked(int x, int y)
{
	if(!frame.isPointInRect(x, y))
		return false;

	itemSelected(currentItem);
	/*if(clickedCb)
		clickedCb(items[currentItem]);*/
	return true;
}

void List::draw(Graphics::Surface *target)
{
	int lastItem = (items.size() >= maxItems) ? (topItem + maxItems) : (items.size());

	//Utility::Logger::getInstance()->log("Drawing list: current = %i, last = %i\n", currentItem, lastItem);
	//
	
	Graphics::Drawer drawer(target);

	RGBColor color;
	for(int i=topItem; i < lastItem; ++i)
	{
		if(i == currentItem)
		{
			color = RGBColor(30, 80, 120);
			//FIXME: why doesn`t RGBColor::GREEN work?
		}
		else
		{
			color = RGBColor(30, 30, 30);
			//color = RGBColor::YELLOW;
		}
		drawer.fillRect(Rect(frame.x, frame.y+i*ITEM_HEIGHT, frame.w, ITEM_HEIGHT), color);
		drawer.drawRect(Rect(frame.x, frame.y+i*ITEM_HEIGHT, frame.w, ITEM_HEIGHT), RGBColor::WHITE);

		Graphics::FontSystem::getInstance()->print(target, items[i], frame.x, frame.y+i*ITEM_HEIGHT, RGBColor(255,0,0));	
	}

	drawer.drawRect(frame, RGBColor::WHITE);
}
