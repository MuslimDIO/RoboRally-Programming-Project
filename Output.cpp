#include "Output.h"

#include "Input.h"

#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////


Output::Output()
{
	// Initialize user interface parameters
	UI.InterfaceMode = MODE_DESIGN;

	// Widths and Heights

	UI.StatusBarHeight = 60;
	UI.CommandsBarHeight = 100;
	UI.CommandItemWidth = 70;
	UI.ToolBarHeight = 55;
	UI.MenuItemWidth = 55;

	UI.width = 1815; // make it divisible by NumHorizontalCells
	UI.height = 815;
	UI.wx = 5;
	UI.wy = 5;

	UI.CellWidth = UI.width / NumHorizontalCells;
	UI.CellHeight = (UI.height - UI.ToolBarHeight - UI.StatusBarHeight - UI.CommandsBarHeight) / NumVerticalCells;

	// Pen Colors of messages of status bar and players' info
	UI.MsgColor = DARKRED;
	UI.PlayerInfoColor = DARKSLATEBLUE;

	// Background Colors of toolbar and statusbar
	UI.ToolBarColor = WHITE;
	UI.StatusBarColor = LIGHTGRAY;
	UI.CommandBarColor = BLACK;

	// Line Colors of the borders of each cell
	UI.GridLineColor = WHITE;

	// Cell Color if Empty & Cell Number Font & Color
	UI.CellColor = LIGHTSLATEBLUE;
	UI.CellColor_DangerZone = DARKRED;
	UI.CellColor_WaterPit = BLUE;
	UI.CellNumFont = 13;
	UI.CellNumColor = UI.GridLineColor;

	// Belt Line Width and Color
	UI.BeltLineWidth = 6;
	UI.BeltColor = DARKSLATEBLUE;

	// The X and Y Offsets of the Space BEFORE Drawing the Belt (offset from the start X and Y of the Cell)
	UI.BeltXOffset = (UI.CellWidth - 2 * UI.BeltLineWidth) / 5;
	UI.BeltYOffset = (UI.CellHeight / 4) * 3;

	// Flag and Flag Pole Colors
	UI.FlagPoleWidth = 4;
	UI.FlagPoleHeight = UI.CellHeight / 2;
	UI.FlagWidth = UI.CellWidth / 4;
	UI.FlagHeight = UI.FlagPoleHeight / 2;

	UI.FlagColor = RED;
	UI.FlagPoleColor = GHOSTWHITE;

	// Commands X and Y Coordinates
	UI.SpaceBetweenCommandsSlots = 10;
	UI.AvailableCommandsXOffset = (UI.CommandItemWidth + UI.SpaceBetweenCommandsSlots) * 6;

	// Colors of the 2 Players
	UI.PlayerColors[0] = GOLD;
	UI.PlayerColors[1] = DARKSLATEBLUE;
	/*UI.PlayerColors[2] = KHAKI;
	UI.PlayerColors[3] = CHOCOLATE;*/

	UI.LaserColor = TOMATO;

	// Create the output window
	pWind = CreateWind(UI.width + 15, UI.height, UI.wx, UI.wy);

	// Change the title
	pWind->ChangeTitle("RoboRally");

	// Create the toolbar, grid area and status bar
	CreateDesignModeToolBar();
	ClearGridArea();
	ClearStatusBar();
	ClearCommandsBar();
}

//////////////////////////////////////////////////////////////////////////////////////////

window *Output::CreateWind(int w, int h, int x, int y) const
{
	// The Window Object is created inside the Ouput class
	window *pW = new window(w, h, x, y);
	return pW;
}

//////////////////////////////////////////////////////////////////////////////////////////

Input *Output::CreateInput() const
{
	// The Input Object is created inside the Output class
	Input *pIn = new Input(pWind);
	return pIn;
}

//======================================================================================//
//								Some Utility Functions										//
//======================================================================================//

int Output::GetCellStartX(const CellPosition &cellPos) const
{
	/// TODO: implement the following function as described in Output.h file

	if (cellPos.IsValidCell())
	{
		return cellPos.HCell() * UI.CellWidth;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

int Output::GetCellStartY(const CellPosition &cellPos) const
{
	/// TODO: implement the following function as described in Output.h file

	if (cellPos.IsValidCell())
	{
		return cellPos.VCell() * UI.CellHeight + UI.ToolBarHeight;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::ClearToolBar() const
{
	// Clear by drawing a rectangle filled with toolbar background color
	pWind->SetPen(UI.ToolBarColor, 1);
	pWind->SetBrush(UI.ToolBarColor);
	pWind->DrawRectangle(0, 0, UI.width, UI.ToolBarHeight);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::DrawTriangle(int triangleCenterX, int triangleCenterY, int triangleHeight, int triangleWidth, Direction direction, color triangleColor, drawstyle style, int penWidth) const
{
	int x1, y1, x2, y2, x3, y3;

	/// Calculate the coordiantes of the 3 vertices of the triangle based on the passed parameters
	/// DONE

	switch (direction)
	{
	case UP:
		x1 = triangleCenterX - triangleWidth / 2;
		y1 = triangleCenterY + triangleHeight / 2;
		x2 = triangleCenterX + triangleWidth / 2;
		y2 = triangleCenterY + triangleHeight / 2;
		x3 = triangleCenterX;
		y3 = triangleCenterY - triangleHeight / 2;
		break;
	case DOWN:
		x1 = triangleCenterX - triangleWidth / 2;
		y1 = triangleCenterY - triangleHeight / 2;
		x2 = triangleCenterX + triangleWidth / 2;
		y2 = triangleCenterY - triangleHeight / 2;
		x3 = triangleCenterX;
		y3 = triangleCenterY + triangleHeight / 2;
		break;
	case LEFT:

		y1 = triangleCenterY - triangleHeight / 2;
		y2 = triangleCenterY + triangleHeight / 2;
		x1 = triangleCenterX + triangleWidth / 2;
		x2 = triangleCenterX + triangleWidth / 2;
		x3 = triangleCenterX - triangleWidth / 2;
		y3 = triangleCenterY;

		break;
	case RIGHT:

		y1 = triangleCenterY - triangleHeight / 2;
		y2 = triangleCenterY + triangleHeight / 2;
		x1 = triangleCenterX - triangleWidth / 2;
		x2 = triangleCenterX - triangleWidth / 2;
		x3 = triangleCenterX + triangleWidth / 2;
		y3 = triangleCenterY;

		break;

	default:
		return;
		break;
	}
	pWind->SetPen(triangleColor, penWidth);
	pWind->SetBrush(triangleColor);
	pWind->DrawTriangle(x1, y1, x2, y2, x3, y3, style);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::DrawImageInCell(const CellPosition &cellPos, string a_image, int width, int height) const
{
	/// DONE: Validate the cell position
	VALIDATE_CELL(cellPos);
	int x = GetCellStartX(cellPos) + UI.CellWidth / 4;
	int y = GetCellStartY(cellPos) + UI.CellHeight / 4;

	image l_img(a_image);
	pWind->DrawImage(l_img, x, y, width, height);

	/// DONE: Complete the implementation of this function
}

//======================================================================================//
//								Interface Functions										//
//======================================================================================//

void Output::ClearStatusBar() const
{
	// Clear drawing a rectangle filled with statusbar background color
	pWind->SetPen(UI.StatusBarColor, 1);
	pWind->SetBrush(UI.StatusBarColor);
	pWind->DrawRectangle(0, UI.height - UI.StatusBarHeight, UI.width, UI.height);
}

void Output::ClearCommandsBar() const
{
	// Clear drawing a rectangle filled with command bar background color
	pWind->SetPen(UI.CommandBarColor, 1);
	pWind->SetBrush(UI.CommandBarColor);
	pWind->DrawRectangle(0, UI.height - UI.StatusBarHeight - UI.CommandsBarHeight, UI.width, UI.height - UI.StatusBarHeight);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::ClearGridArea() const
{
	// Draw each cell in the Grid: ( NumVerticalCells * NumberHorizontalCells )
	for (int i = 0; i < NumVerticalCells; i++)
	{
		for (int j = 0; j < NumHorizontalCells; j++)
		{
			CellPosition cellPos(i, j);
			DrawCell(cellPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::CreateDesignModeToolBar() const
{
	UI.InterfaceMode = MODE_DESIGN;

	ClearToolBar(); // Ensure the toolbar is cleared before drawing

	// Prepare List of images for each menu item
	string MenuItemImages[DESIGN_ITM_COUNT];

	// Map each menu item to its corresponding image

	MenuItemImages[ITM_EXIT] = "images\\Menu_Exit.jpg";
	MenuItemImages[ITM_SWITCH_TO_PLAY_MODE] = "images\\Menu_SwitchToGame.jpg";
	MenuItemImages[ITM_SET_FLAG_CELL] = "images\\Menu_SetFlag.jpg";
	MenuItemImages[ITM_ADD_ANTENNA] = "images\\Menu_AddAntenna.jpg";
	MenuItemImages[ITM_ADD_BELT] = "images\\Menu_AddBelt.jpg";
	MenuItemImages[ITM_ADD_WATER_PIT] = "images\\Menu_AddWaterPit.jpg";
	MenuItemImages[ITM_ADD_DANGER_ZONE] = "images\\Menu_AddDangerZone.jpg";
	MenuItemImages[ITM_ADD_WORKSHOP] = "images\\Workshop.jpg";
	MenuItemImages[ITM_COPY] = "images\\Menu_Copy.jpg";
	MenuItemImages[ITM_CUT] = "images\\Menu_Cut.jpg";
	MenuItemImages[ITM_PASTE] = "images\\Menu_Paste.jpg";
	MenuItemImages[ITM_DELETE] = "images\\Menu_Delete.jpg";
	MenuItemImages[ITM_SAVE_GRID] = "images\\Menu_Save.jpg";
	MenuItemImages[ITM_LOAD_GRID] = "images\\Menu_Load.jpg";
	MenuItemImages[ITM_ADD_ROTATINGGEAR] = "images\\Gear_Clockwise.jpg";
	// Draw menu items one image at a time
	for (int i = 0; i < DESIGN_ITM_COUNT; i++)
	{

		pWind->DrawImage(MenuItemImages[i], i * UI.MenuItemWidth, 0, UI.MenuItemWidth, UI.ToolBarHeight);
	}
}


void Output::CreatePlayModeToolBar() const
{
	UI.InterfaceMode = MODE_PLAY;

	ClearToolBar(); // Clear existing toolbar to prevent overlapping icons

	// Prepare the list of images for each menu item
	string MenuItemImages[PLAY_ITM_COUNT];

	// Map each PLAY_MODE_ITEM to an image path
	MenuItemImages[ITM_SWITCH_TO_DESIGN_MODE] = "images\\Menu_SwitchToGrid.jpg";
	MenuItemImages[ITM_EXECUTE_COMMANDS] = "images\\Menu_ExecuteCommands.jpg";
	MenuItemImages[ITM_SELECT_COMMAND] = "images\\Menu_SelectCommand.jpg";
	MenuItemImages[ITM_NEW_GAME] = "images\\Menu_NewGame.jpg";
	MenuItemImages[ITM_USE_CONSUMABLE] = "images\\Menu_UseConsumable.jpg";
	MenuItemImages[ITM_REBOOT_REPAIR] = "images\\Menu_RebootRepair.jpg";
	MenuItemImages[ITM_EXIT_PLAY_MODE] = "images\\Menu_Exit.jpg";

	for (int i = 0; i < PLAY_ITM_COUNT; i++)
	{
		pWind->DrawImage(MenuItemImages[i], i * UI.MenuItemWidth, 0, UI.MenuItemWidth, UI.ToolBarHeight);
	}
}

void Output::CreateCommandsBar(Command savedCommands[], int savedCommandsCount, Command availableCommands[], int availableCommandsCount) const
{
	// Clear the commands bar area
	ClearCommandsBar();
	UI.InterfaceMode = MODE_PLAY;

	// Prepare images for all possible commands
	string CommandItemImages[COMMANDS_COUNT];
	CommandItemImages[NO_COMMAND] = "images\\CommandSlot-grey.jpg";
	CommandItemImages[MOVE_FORWARD_ONE_STEP] = "images\\MoveForwardCard.jpg";
	CommandItemImages[MOVE_FORWARD_TWO_STEPS] = "images\\MoveForwardTwice.jpg";
	CommandItemImages[MOVE_FORWARD_THREE_STEPS] = "images\\3-Steps-Forward.jpg";
	CommandItemImages[MOVE_BACKWARD_ONE_STEP] = "images\\MoveBackwardCard.jpg";

    CommandItemImages[MOVE_BACKWARD_TWO_STEPS] = "images\\2-Steps-Back.jpg";
	CommandItemImages[MOVE_BACKWARD_THREE_STEPS] = "images\\3stepsBack.jpg";

	CommandItemImages[ROTATE_LEFT] = "images\\RotateLeftCard.jpg";
	CommandItemImages[ROTATE_RIGHT] = "images\\RotateRightCard.jpg";
	CommandItemImages[USE_TOOLKIT] = "images\\ToolkitCard.jpg";
	CommandItemImages[USE_HACK_DEVICE] = "images\\HackDeviceCard.jpg";
	// Add more commands as needed with proper image file paths

	// Draw saved commands in the saved commands area
	DrawSavedCommands(savedCommands, savedCommandsCount, CommandItemImages);

	// Draw available commands in the available commands area
	DrawAvailableCommands(availableCommands, availableCommandsCount, CommandItemImages);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::DrawSavedCommands(Command savedCommands[], int savedCommandsCount, string CommandItemImages[]) const
{
	if (UI.InterfaceMode == MODE_DESIGN)
		return;

	int spaceBetweenSlots = 10;
	for (int i = 0; i < savedCommandsCount; ++i)
	{
		int x = i * (UI.CommandItemWidth + spaceBetweenSlots);
		int y = UI.height - UI.StatusBarHeight - UI.CommandsBarHeight;
		pWind->DrawImage(CommandItemImages[savedCommands[i]], x, y, UI.CommandItemWidth, UI.CommandsBarHeight);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::DrawAvailableCommands(Command availableCommands[], int availableCommandsCount, string CommandItemImages[]) const
{
	if (UI.InterfaceMode == MODE_DESIGN)
		return;

	// Define the size and spacing for the available commands
	int availableCommandWidth = UI.CommandItemWidth / 2;
	int availableCommandHeight = UI.CommandsBarHeight / 2;

	// Define the starting position for the available commands
	int startX = UI.AvailableCommandsXOffset;
	int startY = UI.height - UI.StatusBarHeight - UI.CommandsBarHeight;

	// Draw the "Available Commands" text above the command slots
	pWind->SetPen(WHITE);
	pWind->SetFont(UI.CellNumFont, BOLD, BY_NAME, "Arial");
	string availableCommandsText = "Available Commands";
	int textWidth = 0, textHeight = 0; // to be used in the next line as they are passed by reference
	pWind->GetStringSize(textWidth, textHeight, availableCommandsText);
	pWind->DrawString(startX, startY, availableCommandsText);

	UI.AvailableCommandsYOffset = textHeight + 10; // vertical space between the start of the command bar and the cards of available commands
												   // will be used in detecting selected command from the user click
	for (int i = 0; i < availableCommandsCount; ++i)
	{
		int x = startX + i * (availableCommandWidth);
		int y = startY + UI.AvailableCommandsYOffset; // Adjust the Y position to be below the text

		// Draw the command slot (assuming you have an image for the available commands)
		if (availableCommands[i] != NO_COMMAND)
			pWind->DrawImage(CommandItemImages[availableCommands[i]], x, y, availableCommandWidth, availableCommandHeight);

		// Draw the command number below the card
		pWind->SetPen(UI.CellNumColor);
		pWind->SetFont(UI.CellNumFont, BOLD | ITALICIZED, BY_NAME, "Arial");
		int w = 0, h = 0;
		pWind->GetIntegerSize(w, h, i + 1);
		int numX = x + (availableCommandWidth - w) / 2;
		int numY = y + availableCommandHeight + 5; // Adjust the Y position to be below the card
		pWind->DrawInteger(numX, numY, i + 1);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::PrintMessage(string msg) const // Prints a message on status bar
{
	ClearStatusBar(); // First clear the status bar from any previous writing

	// Set pen and font before drawing the string on the window
	pWind->SetPen(UI.MsgColor);
	pWind->SetFont(18, BOLD, BY_NAME, "Verdana");
	pWind->DrawString(10, UI.height - (int)(UI.StatusBarHeight / 1.3), msg);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::PrintPlayersInfo(string info)
{
	/// DONE: Clear what was written on the toolbar
	CreatePlayModeToolBar();
	// One of the correct ways to implement the above TODO is to call CreatePlayModeToolBar();
	// to clear what was written in the player info (there are other ways too � You are free to use any)

	// Set the pen and font before drawing the string on the window
	pWind->SetPen(UI.PlayerInfoColor);
	pWind->SetFont(20, BOLD, BY_NAME, "Verdana");

	int w = 0, h = 0;

	/// DONE: Calculate the Width and Height of the string if drawn using the current font
	//       (Use GetStringSize() window function) and set the "w" and "h" variables with its width and height
	pWind->GetStringSize(w, h, info);
	// Set the start X & Y coordinate of drawing the string
	int x = UI.width - w - 20;			// space 20 before the right-side of the window
										// ( - w ) because x is the coordinate of the start point of the string (upper left)
	int y = (UI.ToolBarHeight - h) / 2; // in the Middle of the toolbar height

	/// DONE: Draw the string "info" in the specified location (x, y)
	pWind->DrawString(x, y, info);
}

//======================================================================================//
//			         			Game Drawing Functions   								//
//======================================================================================//

void Output::DrawCell(const CellPosition &cellPos, color cellColor) const
{
	// Get the Cell Number (from 1 to NumVerticalCells*NumHorizontalCells) and the X & Y of its upper left corner
	// if (!cellPos.IsValidCell())
	// 	return;
	VALIDATE_CELL(cellPos);
	int cellNum = cellPos.GetCellNum();
	int cellStartX = GetCellStartX(cellPos);
	int cellStartY = GetCellStartY(cellPos);

	// ----- 1- Draw the cell itself (background) --> Filled Rectangle -----
	pWind->SetPen(UI.GridLineColor, 1);

	pWind->SetBrush(cellColor);

	/// TODO: Draw the Cell Rectangle using the appropriate coordinates ///DONE:?????????///
	//       using cellStartX, cellStartY, UI.CellWidth, UI.CellHeight
	pWind->DrawRectangle(cellStartX, cellStartY, cellStartX + UI.CellWidth, cellStartY + UI.CellHeight);

	// ----- 2- Draw the CELL number (the small number at the bottom right of the cell) -----
	pWind->SetPen(UI.CellNumColor);
	pWind->SetFont(UI.CellNumFont, BOLD, BY_NAME, "Verdana");

	int w = 0, h = 0;

	/// TODO: Get the Width and Height of the Cell Number if written using the current font
	//       (Use GetIntegerSize() window function) and set the "w" and "h" variables with its width and height
	/// DONE:?????????????//
	pWind->GetIntegerSize(w, h, cellNum);

	// Calculate X & Y coordinate of the start point of writing the card number (upper left point of the cell num)
	int x = cellStartX + (UI.CellWidth - w - 1);  // space 1 from the end of the cell width
												  // ( - w ) because x is for the start point of cell num (num's left corner)
	int y = cellStartY + (UI.CellHeight - h - 1); // space 1 from the end of the cell height
												  // ( - w ) because y is for the start point of cell num (num's upper corner)

	/// TODO: Draw the cell number in the x and y location

	pWind->DrawInteger(x, y, cellNum);
}

//////////////////////////////////////////////////////////////////////////////////////////

void Output::DrawPlayer(const CellPosition &cellPos, int playerNum, color playerColor, Direction direction) const
{
	/// TODO: Validate the cell position and the playerNum, if not valid return
	VALIDATE_CELL(cellPos);
	if (playerNum < 0 || playerNum > 1)
		return;

	// Get the X & Y coordinates of the start point of the cell (its upper left corner)
	int cellStartX = GetCellStartX(cellPos);
	int cellStartY = GetCellStartY(cellPos);

	// Calculate the Radius of the Player's Triangle
	int radius = UI.CellWidth / 14; // proportional to cell width

	// Calculate the horizontal space before drawing players triangles (space from the left border of the cell)
	int ySpace = UI.CellHeight / 6; // proportional to cell height

	// Note: Players' Triangles Locations depending on "playerNum" is as follows:
	// Player_0
	// Player_1

	// Calculate the Y coordinate of the center of the player's triangle (based on playerNum)
	int y = cellStartY + ySpace + radius + 2;
	if (playerNum == 1)
		y += 2 * (radius + 2); // because playerNum 1 is drawn in the second row of triangles

	// Calculate the X coordinate of the center of the player's triangle (based on playerNum)
	int x = cellStartX + UI.BeltXOffset + radius + 4; // UI.BeltXOffset is used to draw players' triangles
													  // AFTER the Belt start vertical line (assuming there is a belt)
													  // for not overlapping with belts

	/// TODO: Draw the player triangle in center(x,y) and filled with the playerColor passed to the function
	DrawTriangle(x, y, radius * 2, radius * 2, direction, playerColor, FILLED, 2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Output::DrawLaser(const CellPosition& fromCellPos, const CellPosition& toCellPos) const
{
	int fromStartX = GetCellStartX(fromCellPos);
	int fromStartY = GetCellStartY(fromCellPos);
	int endStartX = GetCellStartX(toCellPos);
	int endStartY = GetCellStartY(toCellPos);

	int fromLaserX;
	int endLaserX;
	int fromLaserY;
	int endLaserY;

	if (fromCellPos.VCell() == toCellPos.VCell() || fromCellPos.HCell() == toCellPos.HCell())
	{
		fromLaserX = fromStartX + (UI.CellWidth / 2);
		endLaserX = endStartX + (UI.CellWidth / 2);
		fromLaserY = fromStartY + (UI.CellHeight / 2);
		endLaserY = endStartY + (UI.CellHeight / 2);
		pWind->SetPen(UI.LaserColor, UI.BeltLineWidth);
		pWind->DrawLine(fromLaserX, fromLaserY, endLaserX, endLaserY);
	}


}

void Output::ClearLaser(const CellPosition& fromCellPos, const CellPosition& toCellPos) const
{
	int fromStartX = GetCellStartX(fromCellPos);
	int fromStartY = GetCellStartY(fromCellPos);
	int endStartX = GetCellStartX(toCellPos);
	int endStartY = GetCellStartY(toCellPos);

	int fromLaserX;
	int endLaserX;
	int fromLaserY;
	int endLaserY;

	if (fromCellPos.VCell() == toCellPos.VCell() || fromCellPos.HCell() == toCellPos.HCell())
	{
		fromLaserX = fromStartX + (UI.CellWidth / 2);
		endLaserX = endStartX + (UI.CellWidth / 2);
		fromLaserY = fromStartY + (UI.CellHeight / 2);
		endLaserY = endStartY + (UI.CellHeight / 2);
		pWind->SetPen(UI.CellColor, UI.BeltLineWidth);
		pWind->DrawLine(fromLaserX, fromLaserY, endLaserX, endLaserY);
	}


}
//////////////////////////////////////////////////////////////////////////
void Output::DrawBelt(const CellPosition &fromCellPos, const CellPosition &toCellPos) const
{
	// Validate the fromCell and toCell
	if ((!fromCellPos.IsValidCell() || !toCellPos.IsValidCell()) ||									// check Cells Validity
		((fromCellPos.GetCellNum() == toCellPos.GetCellNum()) || fromCellPos.GetCellNum() == 1) ||	// check that belt doesnt start and end in same cell
		((fromCellPos.HCell() != toCellPos.HCell()) && (fromCellPos.VCell() != toCellPos.VCell()))) // check that the cells either have common H or V so that the belt is either vertical or horizontal
		return;

	// Get the start X and Y coordinates of the upper left corner of the fromCell and toCell
	int fromCellStartX = GetCellStartX(fromCellPos);
	int fromCellStartY = GetCellStartY(fromCellPos);
	int toCellStartX = GetCellStartX(toCellPos);
	int toCellStartY = GetCellStartY(toCellPos);

	// Calculate the belt's starting and ending positions
	int beltFromCellX = fromCellStartX + (UI.CellWidth / 2) + UI.BeltXOffset;
	int beltToCellX = toCellStartX + (UI.CellWidth / 2) + UI.BeltXOffset;
	int beltFromCellY = fromCellStartY + UI.BeltYOffset;
	int beltToCellY = toCellStartY + UI.BeltYOffset;

	// Set pen color and width
	pWind->SetPen(UI.BeltColor, UI.BeltLineWidth);

	// Draw the belt line
	pWind->DrawLine(beltFromCellX, beltFromCellY, beltToCellX, beltToCellY);

	// Calculate the center of the belt line
	int l_centerX = (beltFromCellX + beltToCellX) / 2;
	int l_centerY = (beltFromCellY + beltToCellY) / 2;

	// Determine the belt's direction and draw the triangle pointing to the direction
	Direction l_TriDirection;
	if (fromCellPos.HCell() < toCellPos.HCell())
		l_TriDirection = RIGHT;
	else if (fromCellPos.HCell() > toCellPos.HCell())
		l_TriDirection = LEFT;
	else if (fromCellPos.VCell() < toCellPos.VCell())
		l_TriDirection = DOWN;
	else
		l_TriDirection = UP;

	// Triangle dimensions
	int triangleWidth = UI.CellWidth / 4;
	int triangleHeight = UI.CellHeight / 4;

	// Draw the direction triangle at the center of the belt line
	DrawTriangle(l_centerX, l_centerY, triangleHeight, triangleWidth, l_TriDirection, UI.BeltColor, FILLED, 1);
}

//////////////////////////////////////////////////////////////////////////////////////////
#if 0
void Output::DrawFlag(const CellPosition &cellPos) const
{
	// TODO: Validate the cell position
    
	// Get the X and Y coordinates of the start point of the cell (its upper left corner)
	int cellStartX = GetCellStartX(cellPos);
	int cellStartY = GetCellStartY(cellPos);

	// TODO: Draw the flag as a line with a triangle connected to it directed to right

	// TODO: 1. Draw the flag pole (the line)
	int flagPoleStartX = cellStartX + UI.CellWidth / 2;
	int flagPoleStartY = cellStartY + UI.CellHeight / 4;

	// 		 2. Draw the flag (the triangle)
}
#endif

void Output::DrawFlag(const CellPosition &cellPos) const
{
	/// DONE: Validate the cell position
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	// Get the X and Y coordinates of the start point of the cell (its upper left corner)
	int cellStartX = GetCellStartX(cellPos);
	int cellStartY = GetCellStartY(cellPos);

	// Calculate flag pole coordinates
	int flagPoleStartX = cellStartX + UI.CellWidth / 2;	   // X coordinate of the flag pole
	int flagPoleStartY = cellStartY + UI.CellHeight / 4;   // Y coordinate for top of the flag pole
	int flagPoleEndY = flagPoleStartY + UI.FlagPoleHeight; // Y coordinate for bottom of the flag pole

	// Draw the flag pole (vertical line)
	pWind->SetPen(UI.FlagPoleColor, UI.FlagPoleWidth);
	pWind->DrawLine(flagPoleStartX, flagPoleStartY, flagPoleStartX, flagPoleEndY);

	// Calculate flag (triangle) vertices
	int triangleBaseX = flagPoleStartX;
	int triangleBaseY = flagPoleStartY;
	int triangleTipX = triangleBaseX + UI.FlagWidth;		// Tip of the triangle to the right
	int triangleTipY = triangleBaseY + (UI.FlagHeight / 2); // Center of the triangle vertically

	// Vertices of the triangle
	int x1 = triangleBaseX;					// Left vertex (attached to the pole)
	int y1 = triangleBaseY;					// Top vertex
	int x2 = triangleBaseX;					// Left vertex (attached to the pole)
	int y2 = triangleBaseY + UI.FlagHeight; // Bottom vertex
	int x3 = triangleTipX;					// Right vertex (tip of the flag)
	int y3 = triangleTipY;					// Center of the flag

	// Draw the flag (triangle)

	pWind->SetPen(UI.FlagColor, 1);
	pWind->SetBrush(UI.FlagColor);
	pWind->DrawTriangle(x1, y1, x2, y2, x3, y3, FILLED);
}

void Output::DrawRotatingGear(const CellPosition &cellPos, bool clockwise) const
{
	// Validate the cell position using the macro
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	// Choose the appropriate image based on the gear's direction
	string gearImage = clockwise ? "images\\Gear_Clockwise.jpg" : "images\\Gear_CounterClockwise.jpg";

	// Define the width and height of the gear image relative to the cell size
	int gearWidth = UI.CellWidth / 2;	// Scaled to half the cell's width
	int gearHeight = UI.CellHeight / 2; // Scaled to half the cell's height

	// Use DrawImageInCell to place the gear in the specified cell
	DrawImageInCell(cellPos, gearImage, gearWidth, gearHeight);
}

void Output::DrawAntenna(const CellPosition &cellPos) const
{
	/// TODO: Validate the cell position
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	int Width = UI.CellWidth / 2;	// Scaled to half the cell's width
	int Height = UI.CellHeight / 2; // Scaled to half the cell's height

	// Use DrawImageInCell to place the gear in the specified cell
	DrawImageInCell(cellPos, "images\\Menu_AddAntenna.jpg", Width, Height);
	/// TODO: Draw the antenna image in the cell
}

void Output::DrawWorkshop(const CellPosition &cellPos) const
{
	// TODO: Validate the cell position
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	// TODO: Draw the workshop image in the cell
	int Width = UI.CellWidth / 2;	// Scaled to half the cell's width
	int Height = UI.CellHeight / 2; // Scaled to half the cell's height

	// Use DrawImageInCell to place the gear in the specified cell
	DrawImageInCell(cellPos, "images\\Workshop.jpg", Width, Height);
}

void Output::DrawDangerZone(const CellPosition &cellPos) const
{
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	DrawCell(cellPos, UI.CellColor_DangerZone);
}

void Output::DrawWaterPit(const CellPosition &cellPos) const
{
	VALIDATE_CELL(cellPos);
	VALIDATE_CELL_NUM(cellPos);
	DrawCell(cellPos, UI.CellColor_WaterPit);
}

Output::~Output()
{
	// deallocating the window object
	delete pWind;
}
