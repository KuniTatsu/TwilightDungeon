#pragma once
class Item;

class Inventory
{
public:
	Inventory();
	~Inventory();

	Item* inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	void AddInventory(Item* item);

	void DrawInventory(int x,int y);

private:

};

