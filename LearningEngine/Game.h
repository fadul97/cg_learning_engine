/**********************************************************************************
// Game (Header File)
//
// Creation:	03/09/2022
// Update:		--/--/----
// Compiler:	Visual C++ 2022
//
// Description:	Abstract class to represent the game.
//				HOW TO USE: Create a derived class from Game and override Init(),
//							Update(), Draw() and Finalize() methods. To run the
//							game, pass the Game object to the Engine method Start().
//
**********************************************************************************/

#ifndef LE_GAME_H
#define LE_GAME_H

// ---------------------------------------------------------------------------------
// Includes

#include "Window.h"
#include "Input.h"

namespace LearningEngine
{
	// ---------------------------------------------------------------------------------
	// Structs

	struct Canvas
	{
		double width;														// Game width
		double height;														// Game height
		double centerX;														// Center of Game on X-axis
		double centerY;														// Center of Game on Y-axis
	};

	struct ViewPort
	{
		double left;														// coordinate x1
		double top;															// coordinate y1
		double right;														// coordinate x2
		double bottom;														// coordinate y2
	};

	class Game
	{
	public:
		ViewPort viewport;													// Visibile area of the Game

		Game();																// Constructor
		virtual ~Game();													// Destructor

		virtual void OnPause();												// Game pause

		static void SetSize(double width, double height);					// Set Game Window size
		
		static double GetWidth();											// Returns Game width
		static double GetHeight();											// Returns Game height
		static double GetCenterX();											// Returns center of Game on X-axis 
		static double GetCenterY();											// Returns center of Game on Y-axis 


		// Methods above are virtuals, that means they have to be
		// implemented in all Game objects

		virtual void Init() = 0;											// Game init
		virtual void Update() = 0;											// Game update
		virtual void Draw() = 0;											// Scene draw
		virtual void Finalize() = 0;										// Game finalize

	private:
		static Canvas world;												// Game area


	protected:
		static Window*& window;												// Game Window
		static Game*& game;													// Game which the object is in
		static double& gameTime;											// Last frame time

	};

	// ---------------------------------------------------------------------------------
	// Inline Functions - Getters

	// Returns Game width
	inline double Game::GetWidth()
	{ return world.width; }

	// Returns Game height
	inline double Game::GetHeight()
	{ return world.height; }

	// Returns center of Game on X-axis
	inline double Game::GetCenterX()
	{ return world.centerX; }

	// Returns center of Game on Y-axis
	inline double Game::GetCenterY()
	{ return world.centerY; }
}

#endif // !LE_GAME_H
