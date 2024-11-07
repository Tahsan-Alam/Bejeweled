/*******************
Name: Tahsan Ul Alam
Date: 10/29/24
********************/

/**************************************
Description: This is a C++ game called Bejeweled. This code is from FamTrinli. More details are in the
README.md file.

Some pieces of code have been edited.

Project Part 1:
A class Piece is created.
Getters and setters are implemented
All member variables are set to private
All member variables are initialized with default values in the constructor
Made necessary changes in the code to make it work with the getter and setter methods

Project Part 2:
Sound is added
Font is added
GameOver page with instruction is added
Top 5 High Scores are added in game over page
Functionality added to stop and restart the game
Welcome page is added with game obejective and
instructions
Score and timer are added

Font source:
https://www.1001freefonts.com/phelya.font
Designer Name:	madeDeduk
License: Personal Use only

Sound source:
https://pixabay.com/sound-effects/success-1-6297/
Artist: Lezek_Szary
License: Pixabay Content license (Personal Use only)
**************************************/
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <string>
#include <iostream>
using namespace sf;

int ts = 54; //tile size
Vector2i offset(48,24); // Moving sprite to x-offset by 48 units and y-offset by 24 units

class Piece
{
public:

	// all setters
	void setX(const int xVal)
	{
		x = xVal;
	}
	void setY(const int yVal)
	{
		y = yVal;
	}
	void setCol(const int colVal)
	{
		col = colVal;
	}
	void setRow(const int rowVal)
	{
		row = rowVal;
	}
	void setKind(const int types)
	{
		kind = types;
	}
	void setMatch(const int count)
	{
		match = count;
	}
	void setAlpha(const int opacity)
	{
		alpha = opacity;
	}

	// all getters
	int getX() const
	{
		return x;
	}
	int getY() const
	{
		return y;
	}
	int getCol() const
	{
		return col;
	}
	int getRow() const
	{
		return row;
	}
	int getKind() const
	{
		return kind;
	}
	int getMatch() const
	{
		return match;
	}
	int getAlpha() const
	{
		return alpha;
	}

	Piece()
	{
		setX(0);
		setY(0);
		setCol(0);
		setRow(0);
		setKind(0);
		setMatch(0);
		setAlpha(255);
	}

private:
	int x; // x coordinate of pixel
	int y; // y coordinate of pixel
	int col; // column val of grid
	int row; // row val of grid
	int kind; // type of gems. There are 7 types
	int match;  // to track if we have a match
	int alpha; // stores opacity value
};

// 2D array with 10 rows and 10 columns
Piece grid[10][10];

// This function swaps two gems' position by swapping their row and column values
// Input: p1 and p2 are Piece objects pass by value to swap
// Output: updated grid of pieces

void swap(Piece p1,Piece p2)
{
	// swap column values
	int tmp = p1.getCol();
	p1.setCol(p2.getCol());
	p2.setCol(tmp);

	// swap column values
	tmp = p1.getRow();
	p1.setRow(p2.getRow());
	p2.setRow(tmp);

	grid[p1.getRow()][p1.getCol()]=p1; // assigning p1 to new row and col values
	grid[p2.getRow()][p2.getCol()]=p2; // assigning p2 to new row and col values
}

// this function is created to avoid repeatition of the code
// this function is called at the begining of the game and
// after player restart the game to reset the game
// This function takes int numOfGems which gets incremented in each level
// by 1.
void gameTable(const int & numOfGems)
{
	for (int i=1;i<=8;i++) // looping through rows
	{
		for (int j=1;j<=8;j++) // looping through columns
		{
			// generating 3 random numbers to assign random gem in each grid
			grid[i][j].setKind(rand()%numOfGems);

			// assigning current gem's column number
			grid[i][j].setCol(j);

			// assigning current gem's row number
			grid[i][j].setRow(i);

			//calculates pixel coordinate of x for the gem based on its column num
			// grid[i][j].x = j*ts;
			grid[i][j].setX(j*ts);

			// calculate pixel coordinate of y for the gem based on its row num
			grid[i][j].setY(i*ts);
		}
	}
}

// This function takes type Text variable, type Font variable,
//  x(int) and y(int) position of the text, type Color variable,
// font size of the text (int)
// and set up the variable to make the text properly
void createText(sf::Text & currentText, const sf:: Font & currentFont,
				const int & x, const int &y,
                const sf::Color & currentColor,
                const int & charSize)
{
	currentText.setFont(currentFont);
	currentText.setFillColor(currentColor);
	currentText.setCharacterSize(charSize);
	currentText.setPosition(x,y);
}

std::vector<int> scores(5); // to store top 5 High Scores

// This function takes score of the game
// and check if the score is greater than the scores
// the vector has already. If the score is greater than those
// score then it reassign the score and update the vector
// with top 5 high scores
void highScores(const int & score)
{
	bool isGreater = false;
	int i = 0;
	while(!isGreater)
	{
		if(scores[i] == score)
		{
			isGreater == true;
		}
		else if(scores[i] < score)
		{
			scores[i] = score;
			isGreater = true;
		}
		else
		{
			i++;
		}
	}
}
int main()
{
    // Seeds the random number generator with the current time.
    // time(0) ensures different set of random numbers each time
    srand(time(0));

    // to draw on the window
    // size of the window sets to 740 x 480 pixels
    // title of the window is  Math-3 Game!
    RenderWindow app(VideoMode(740,480), "Match-3 Game!");
    //setting Frame rate limit to 60 FPS for better performance
    app.setFramerateLimit(60);

    Texture t1,t2;

	int GemsNum = 3; // at level 1 there are 3 unique gems
	gameTable(GemsNum);

    //loading background texture
	// the program terminates if it cannot open the file
    if(!t1.loadFromFile("images/background.png"))
    {
        return EXIT_FAILURE;
    }
    //loading gems texture
	// the program terminates if it cannot open the file
    if(!t2.loadFromFile("images/gems.png"))
    {
        return EXIT_FAILURE;
    }

	// loading Sound
	sf::SoundBuffer buffer;
	if(!buffer.loadFromFile("sounds/GemsMatch.ogg"))
	{
		return -1;
	}
	sf::Sound sound;
	sound.setBuffer(buffer);

	// loading font
	sf::Font font;
	if(!font.loadFromFile("fonts/Phelya.ttf"))
	{
		return -1;
	}

	sf:: Text welcome; // Welcome page text
	createText(welcome, font, 200,100, Color::White, 50);
	welcome.setString("Welcome to Bejeweled!");

	sf:: Text rules; // Objective of the game text
	createText(rules, font, 50, 200, Color::White, 20);
	rules.setString("Objective\n"
				 "\n1. Match three or more gems of the same color\n"
				 "    in a row or column to clear them and earn points.\n"
				 "2. The goal is to make as many matches as possible\n"
				 "   to score the highest points before the timer runs out\n"
	             "3. There are 4 levels. "
	             " Getting 100 points each time will increase the game level\n"
				 "\n             Press Enter to play !!!");

	sf::Text text; // Score text
	createText(text,font,500,400,Color::Blue,50);
	text.setString("Score: 0");

	sf:: Text countdown; // Countdown Text
	createText(countdown,font,500,350,Color::Green, 50);

	sf:: Text gameOver; // GAME OVER text
	createText(gameOver,font,200,30,Color::White,50);

	sf:: Text instruction; // Instruction in the Game Over page
	createText(instruction,font,100,400,Color::White,50);

	Text levels; // level text
	createText(levels,font,500, 300,Color::Red,50);
	levels.setStyle(sf::Text::Bold);
	levels.setString("Level 1");

	Text fiveScores; // Top 5 High Scores text
	createText(fiveScores,font,50,100,Color::White,40);

	sf::Time totalTime = sf::seconds(300); // total time of the game
	sf::Time passedTime = sf::seconds(0.01f); // time to substract from total time in each loop iteration
    // Sprite Object named background for texture t1
    Sprite background(t1);
    // Sprite object named gems for texture t2
    Sprite gems(t2);

    int x0; // store x pos  of first gem
	int y0; // store y pos of first gem
	int x; // store x pos of second gem
	int y; // store y pos of second gem
    int click=0; // keep track of number of mouse click
    Vector2i pos; // store position of gem when it is clicked
    bool isSwap=false; // to check if swapping between two gems happened
    bool isMoving=false; // to check if moving happened
	int point = 0;// game score
	bool isGameStart = false; // to start and stop game
	bool isEnabled = false; // to enable 2nd if statement in the event loop
	int level = 1; // game level
	bool isTimeOut = false; // if time is 0s

	// while window is open
    while (app.isOpen())
    {
    	// Welcome page
    	// game is stopped
    	// isEnabled set to true to prompt to 2nd
    	// if statement of the event loop
    	if(!isGameStart)
    	{
    		app.clear(Color::Black);
    		app.draw(welcome);
    		app.draw(rules);
    		app.display();
    		isEnabled = true;
    	}

        Event e;
        while (app.pollEvent(e))
        {
        	// if user close the window
            if (e.type == Event::Closed)
            {
                app.close();
            }

        	// if player press enter isGameStart becomes true which
        	// starts the game
        	if(e.type == Event::KeyPressed && isEnabled)
        	{
        		if(e.key.code == Keyboard::Enter)
        		{
        			isGameStart = true;
        		}
        	}

        	// if time is 0s and player pressed r to restart the game
        	if(e.type == Event::KeyPressed && totalTime.asSeconds() == 0)
        	{
        		if(e.key.code == Keyboard::R)
        		{
        			isTimeOut = false;
        			totalTime = sf::seconds(300);
        			passedTime = sf::seconds(0.01f);
        			click=0;
        			isSwap=false;
        			isMoving=false;
        			point = 0;
        			text.setString("Score: 0");
        			GemsNum = 3;
        			level = 1;
        			levels.setString("Level 1");
        			gameTable(GemsNum);
        		}
        	}

        	// if user click mouse button
			if (e.type == Event::MouseButtonPressed)
			{
				// if user clicked left mouse button
				// and check if swaping and moving happened
				// if not then increment click variable by 1
			    if (e.key.code == Mouse::Left)
			    {
			        if (!isSwap && !isMoving)
			        {
			            click++;
			        }

			    	// get the position of mouse click
			        pos = Mouse::getPosition(app)-offset;
			    }
			}
        }

    	// if player pressed Enter to play the game or
    	// pressed r to restart the game
		if(isGameStart && !isTimeOut)
			{
				// if total time is less than 10
				// then text becomes red as a warning
				if(totalTime.asSeconds() <= 10.0)
    			{
    				countdown.setFillColor(Color::Red);
    			}

				// if total time is greater than 0
				// then time gets substracted to get the
				// remaining time
    			if(totalTime.asSeconds() > 0)
    			{
    				totalTime -= passedTime ;
    				countdown.setString(std::to_string(totalTime.asSeconds()));
    			}

				// mouse click
				if (click==1)
				{
					x0=pos.x/ts+1; // assign x pos of first gem
					y0=pos.y/ts+1; // assing y pos of first gem
				}
				if (click==2)
				{
   					x=pos.x/ts+1; // assign x pos of second gem
					y=pos.y/ts+1; // assign y pos of second gem

					// if distance between first selection and second
					// selection is 1 then swapping happens and isSwap
					// becomes true, click resets to 0
					// else click is assigned to 1
					if (abs(x-x0)+abs(y-y0)==1)
					{
      					swap(grid[y0][x0],grid[y][x]);
      					isSwap=1;
      					click=0;
					}
					else
					{
						click=1;
					}
				}

				//Match finding

    			for(int i=1;i<=8;i++) // looping through rows
				{
					for(int j=1;j<=8;j++) // looping through columns
					{
						// checking vertical matching
						// if three gems in vertical position are the same
						// if they are same then match is incremented by 1
	   					if (grid[i][j].getKind()==grid[i+1][j].getKind())
	   					{
	   						if (grid[i][j].getKind()==grid[i-1][j].getKind())
	   						{
	   							for(int n=-1;n<=1;n++)
	   							{
	   								grid[i+n][j].setMatch(grid[i+n][j].getMatch()+ 1);

	   							}
	   						}
	   					}

						// checking horizontal matching
						// if three gems in horizontal position are the same
						// if they are same then match is incremented by 1
	   					if (grid[i][j].getKind()==grid[i][j+1].getKind())
	   					{
	   						if (grid[i][j].getKind()==grid[i][j-1].getKind())
	   						{
	   							for(int n=-1;n<=1;n++)
	   							{
	   								grid[i][j+n].setMatch(grid[i][j+n].getMatch()+1);
	   							}
	   						}
	   					}
					}
				}

				//Moving animation
				isMoving=false;
				for (int i=1;i<=8;i++) // looping through rows
				{
   					for (int j=1;j<=8;j++) // looping through columns
   					{
   						// to store current grid position
   						Piece &p = grid[i][j];

   						// x pos of gem
   						int dx;

   						// y pos of gem
   						int dy;

   						for(int n=0;n<4;n++)   // 4 - speed
   						{
   							// moving to target x position
   							// dx = p.x-p.col*ts;
   							dx = p.getX() - p.getCol()*ts;

   							// moving to target y position
   							dy = p.getY()-p.getRow()*ts;

   							// if dx is not zero
   							// then it goes to the target position
   							if (dx)
   							{
   								// p.x-=dx/abs(dx);
   								p.setX(p.getX() - dx/abs(dx));
   							}

   							// if dy is not zero
   							// then it goes to the target position
   							if (dy)
   							{
   								// p.y-=dy/abs(dy);
   								p.setY(p.getY() - dy/abs(dy));
   							}
   						}

   						// if dx or dy is 1
   						// then isMoving is assigned to true
   						if (dx||dy)
   						{
   							isMoving=1;
   						}
   					}
				}


    			//Deleting animation
				if (!isMoving)
				{
   					for (int i=1;i<=8;i++) // looping through rows
   					{
   						for (int j=1;j<=8;j++) // looping through columns
   						{
   							// if there is a match then
   							// then it fades out the gems
   							// and assign isMoving to true
   							if (grid[i][j].getMatch())
   							{
   								if (grid[i][j].getAlpha()>10)
   								{
   									grid[i][j].setAlpha(grid[i][j].getAlpha() -10);
   									isMoving=true;
   								}
   							}
   						}
   					}
				}

				//Get score
				int score=0;
				for (int i=1;i<=8;i++)
				{
   					for (int j=1;j<=8;j++)
   					{
   						score+=grid[i][j].getMatch();
   					}
				}

				//Second swap if no match
				if (isSwap && !isMoving)
				{
					if (!score)
					{
						swap(grid[y0][x0],grid[y][x]);
	   					isSwap=0;
					}
				}

				//Update grid
				if (!isMoving)
				{
					// It checks if there is any match in the grid
					// if there is no match then it breaks the loop

					for(int i=8;i>0;i--)
					{
      					for(int j=1;j<=8;j++)
      					{
      						if (grid[i][j].getMatch())
      						{
      							for(int n=i;n>0;n--)
      							{
      								if (!grid[n][j].getMatch())
      								{
      									swap(grid[n][j],grid[i][j]);
      									break;
      								}
      							}
      						}
      					}
					}

					// if there is a match then it updates the grid
					for(int j=1;j<=8;j++)
					{
      					for(int i=8,n=0;i>0;i--)
      					{
      						if (grid[i][j].getMatch())
      						{
      							sound.play(); // play sound after getting a match

      							// if point is greater than 0 and divisible by
      							// 100. Each level is 100 scores
      							if(point > 0 && point % 100 == 0)
      							{
      								// if unique number of gems is lesser than 7
      								if(GemsNum < 8)
      								{
      									GemsNum += 1;
      									level += 1;
      									levels.setString("Level " + std::to_string(level));
      									gameTable(GemsNum); // upgrade the level difficulty
      								}
      				  			}
      							point += 1; // score is incremented by 1 for each gem in the matching
      							text.setString("Score: " + std::to_string(point));
      							grid[i][j].setKind(rand()%7);
      							grid[i][j].setY(-ts*n++);
      							grid[i][j].setMatch(0);
      							grid[i][j].setAlpha(255);
      						}
      					}
					}
				}

				//////draw///////

				app.draw(background);
    			app.draw(text); // to draw Score
    			app.draw(countdown); // to draw timer
				app.draw(levels); // to draw level
				for (int i=1;i<=8;i++) // looping through rows
				{
					for (int j=1;j<=8;j++) // looping through columns
					{
						// current grid position p
						Piece p = grid[i][j];

						// setting texture to specific dimensions
						gems.setTextureRect( IntRect(p.getKind()*49,0,49,49));

						// setting colors to gems
						gems.setColor(Color(255,255,255,p.getAlpha()));

						//setting position of gems
						gems.setPosition(p.getX(),p.getY());

						// moving gems
						gems.move(offset.x-ts,offset.y-ts);
						app.draw(gems);
					}
				}

				// if the time is 0s it changes the page to
				// game over page and stop the game
    			if(totalTime.asSeconds() == 0.0)
    			{
    				isTimeOut = true;
    				sound.stop();
    				highScores(point);
    				app.clear(sf::Color::Black);
    				gameOver.setString("Game Over!!!");
    				fiveScores.setString("High Scores\n"
							 "1. " + std::to_string(scores[0]) +"\n"
										   "2. " + std::to_string(scores[1])+"\n"
											   "3. " + std::to_string(scores[2])+"\n"
												"4. " + std::to_string(scores[3])+"\n"
											  "5. " + std::to_string(scores[4]) + "\n");

    				instruction.setString("Press R to play again");
    				app.draw(gameOver); // to draw Game Over
    				app.draw(fiveScores); // to draw top 5 high scores
    				app.draw(instruction); // to draw instruction in the Game over page
    			}
    			app.display();
			}
		}
	return 0;
}

