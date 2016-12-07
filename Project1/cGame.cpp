/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"
// writing on a text file
#include <iostream>
#include <fstream>
using namespace std;

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theAreaClicked = { 0, 0 };

	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/exit button.png", "Images/Controls buttons.png", "Images/score button.png", "Images/menu button.png", "Images/start button.png", "Images/save button.png", "Images/settings button.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = QUIT;
	// Store the textures
	textureName = { "OtherCar1", "OtherCar2","thePlayer","theBackground"};
	texturesToUse = { "Images\\car 2.png", "Images\\car 3.png", "Images\\car.png", "Images\\background.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "squishy", "Gameover", };
	fontsToUse = { "Fonts/squishy.ttf", "Fonts/GameOver.ttf", };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "HIGHWAY DODGER!!!", "Main menu", "Score : ", "Game Over!!!", "would you like to save your score", "Use the up and down arrow keys to move up and down" };
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 0, 0, 0  }, { 100, 100, 100, 100 }));
	theTextureMgr->addTexture("Menu", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, gameTextList[1], SOLID, { 0, 0, 0, 0 }, { 1000, 100,50, 50 }));
	theTextureMgr->addTexture("score", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 0, 0, 0, 0 }, { 1000, 100, 100, 100 }));
	theTextureMgr->addTexture("Game over", theFontMgr->getFont("Gameover")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 0, 0, 0, 0 }, { 400, 400, 50, 50 }));
	theTextureMgr->addTexture("Save?", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 0, 0, 0, 0 }, { 600, 600, 50, 50 }));
	theTextureMgr->addTexture("controls", theFontMgr->getFont("Gameover")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 0, 0, 0, 0 }, { 600, 600, 50, 50 }));
	// Load game sounds
	soundList = { "theme", "crash" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/Music.wav", "Audio/Crash.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//thePlayer.setSpritePos({ 50, 350 });
	thePlayer.setSpritePos({ 0, 20350 });
	thePlayer.setTexture(theTextureMgr->getTexture("thePlayer"));
	thePlayer.setSpriteDimensions(theTextureMgr->getTexture("thePlayer")->getTWidth(), theTextureMgr->getTexture("thePlayer")->getTHeight());
	thePlayer.setPlayerSpeed({ 0, 0 });

	// Create vector array of textures

	for (int astro = 0; astro < 8; astro++)
	{

			theOtherCars.push_back(new cOtherCars);
			int xPosition = 100 * (rand() % 5 + 1) + 500;
			int yPosition = 100 * (rand() % 5 + 1);
			if (yPosition > 600) { yPosition = 580; }
			theOtherCars[astro]->setSpritePos({ xPosition, yPosition });
			theOtherCars[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
			int randOtherCar = rand() % 2;
			theOtherCars[astro]->setTexture(theTextureMgr->getTexture(textureName[randOtherCar]));
			theOtherCars[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randOtherCar])->getTWidth(), theTextureMgr->getTexture(textureName[randOtherCar])->getTHeight());
			theOtherCars[astro]->setOtherCarVelocity({ 6.0f, 6.0f });
			theOtherCars[astro]->setActive(true);

			if (theGameState == PLAYING)
			{
				int randOtherCar = rand() % 2;
				theOtherCars[astro]->setTexture(theTextureMgr->getTexture(textureName[randOtherCar]));
				theOtherCars[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randOtherCar])->getTWidth(), theTextureMgr->getTexture(textureName[randOtherCar])->getTHeight());
				theOtherCars[astro]->setOtherCarVelocity({ 6.0f, 6.0f });
				theOtherCars[astro]->setActive(true);
			}

	}

	reset = false;
	/*write to file 
	int main() {
		ofstream myfile("example.txt");
		if (myfile.is_open())
		{
			myfile << "This is a line.\n";
			myfile.close();
		}
		else cout << "Unable to open file";
		myfile.close();
		}

REad the file
#include <string>
using namespace std;

int main () {
string line;
ifstream myfile ("example.txt");
if (myfile.is_open())
{
while ( getline (myfile,line) )
{
cout << line << '\n';
}
myfile.close();
}

else cout << "Unable to open file";

return 0;
}


	*/

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		/*if (theGameState != EXIT);
		{*/
			this->render(theSDLWND, theRenderer);
		/*}*/
		//cout << "state : " <<theGameState << endl;
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		//for rendering the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("Title");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("Menu");
		pos = { 600, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("controls");
		pos = { 290, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);


		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 300 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 450, 400 });
		theButtonMgr->getBtn("instructions_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("instructions_btn")->getSpritePos(), theButtonMgr->getBtn("instructions_btn")->getSpriteScale());
		theButtonMgr->getBtn("instructions_btn")->setSpritePos({ 450, 500 });

	}
	break;
	case PLAYING:
	{
		//thePlayer.setSpritePos({ 50, 350 });

		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render each OtherCar in the vector array
		for (int draw = 0; draw < theOtherCars.size(); draw++)
		{
			theOtherCars[draw]->render(theRenderer, &theOtherCars[draw]->getSpriteDimensions(), &theOtherCars[draw]->getSpritePos(), theOtherCars[draw]->getSpriteRotAngle(), &theOtherCars[draw]->getSpriteCentre(), theOtherCars[draw]->getSpriteScale());
		}
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// render the player
		thePlayer.render(theRenderer, &thePlayer.getSpriteDimensions(), &thePlayer.getSpritePos(), thePlayer.getSpriteRotAngle(), &thePlayer.getSpriteCentre(), thePlayer.getSpriteScale());
		//render the buttons
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 675 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	
		//render score texture
		theTextureMgr->addTexture("score", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, scorestr.c_str(), SOLID, { 0, 0, 0, 0 }, { 1000, 100, 100, 100 }));
		tempTextTexture = theTextureMgr->getTexture("score");
		pos = { 550, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);


	}
	break;
	case EXIT:
	{
		//for rendering the background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		tempTextTexture = theTextureMgr->getTexture("Game over");
		pos = { 400, 100, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 450, 600 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 450, 700 });

		//render the score
		theTextureMgr->addTexture("score", theFontMgr->getFont("squishy")->createTextTexture(theRenderer, scorestr.c_str(), SOLID, { 0, 0, 0, 0 }, { 1000, 100, 100, 100 }));
		tempTextTexture = theTextureMgr->getTexture("score");
		pos = { 400, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

	}
	break;

	case RESTART:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());

		score == 0;
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("play_btn")->setSpritePos({ 450, 300 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 450, 500 });


	}
	default:
		break;
	}


	SDL_RenderPresent(theRenderer);
	
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{

	switch (theGameState)
		{


		case MENU:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, EXIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
			if (theGameState == PLAYING)
			{
				thePlayer.setSpritePos({ 70, 350 });
				//cout << "Setting sprite position on button click" << endl;
				score = 0;
				reset = true;
			}

		}
		break;
		case PLAYING:
		{
			score += 1;
			scorestr = gameTextList[2] + to_string(score);
			theTextureMgr->deleteTexture("score");

			for (vector<cOtherCars*>::iterator OtherCarIterator = theOtherCars.begin(); OtherCarIterator != theOtherCars.end(); ++OtherCarIterator)
	{
		if ((*OtherCarIterator)->collidedWith(&(*OtherCarIterator)->getBoundingRect(), &thePlayer.getBoundingRect()))
		{
			// if a collsion happens the player is taken to the game over screen
			theGameState = EXIT;
			theSoundMgr->getSnd("crash")->play(0);
			//thePlayer.setSpritePos({ 50, 650 });
			(&thePlayer)->update(deltaTime);
			theAreaClicked = { 0, 0 };
			reset = true;

			break;
		}
	}
			// Update the visibility and position of each obsticle

			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, EXIT, theAreaClicked);
			vector<cOtherCars*>::iterator OtherCarIterator = theOtherCars.begin();

			while (OtherCarIterator != theOtherCars.end())
			{
				if ((*OtherCarIterator)->isActive() == false)
				{
					OtherCarIterator = theOtherCars.erase(OtherCarIterator);
				}
				else
				{
					if (theGameState == PLAYING)
					{
						(*OtherCarIterator)->update(deltaTime);
					}
					++OtherCarIterator;
				}
			}
			/*(&thePlayer)->playerScore += 1;*/
			//cout << "Current Score : " << (&thePlayer)->playerScore << endl;
			// Update the players position
			(&thePlayer)->update(deltaTime);
			//cout << "updating" << endl;
		}
		break;

		case EXIT:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, ENDGAME, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
			reset = true;

		}
		break;

		//case ENDGAME:
		//{

		//}
		}

	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/

	//(*bulletIterartor)->update(deltaTime);
	if (reset == true)
	{

		for (int astro = 0; astro < 8; astro++)
		{
			int xPosition = 100 * (rand() % 5 + 1) + 500;
			int yPosition = 100 * (rand() % 5 + 1);
			if (yPosition > 600) { yPosition = 580; }
			theOtherCars[astro]->setSpritePos({ xPosition, yPosition });
			theOtherCars[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });

		}
		(&thePlayer)->update(deltaTime);

		reset = false;
	}


}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					//cout << "red at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;
					if (thePlayer.getSpritePos().y < 600)
					{
						thePlayer.setSpriteTranslation({ 0, -5 });
					}
				}
				break;

				case SDLK_UP:
				{
					//cout << "red up at position - x: " << thePlayer.getBoundingRect().x << " y: " << thePlayer.getBoundingRect().y << endl;
					if (thePlayer.getSpritePos().y > 100)
					{
					thePlayer.setSpriteTranslation({ 0, 5 });
					}
				}
				break;

	default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

