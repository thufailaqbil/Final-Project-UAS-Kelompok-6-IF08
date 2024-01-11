#include <sl.h>
#include <vector>
#include <random>
#include <string>
#include <cmath>
#include "entities.h"

bool isCollision(const Bird& b, const PipeUp& pu) {
	double dx = b.posX - pu.posX;
	double dy = b.posY - pu.posY;
	double distance = sqrt(pow(dx, 2.0) + pow(dy, 2));
	return (distance <= (pu.radius + b.radius));
}

bool isCollision(const Bird& b, const PipeDown& pd) {
	double dx = b.posX - pd.posX;
	double dy = b.posY - pd.posY;
	double distance = sqrt(pow(dx, 2.0) + pow(dy, 2));
	return (distance <= (pd.radius + b.radius));
}
void backsound()
{
	int bcksound = slLoadWAV("music.wav");
	slSoundLoop(bcksound);
}

int main()
{
	slWindow(1920, 1080, "Surf Bird", false);

	int imgBird = slLoadTexture("bird.png");
	int imgPipeUp = slLoadTexture("pipe.png");
	int imgPipeDown = slLoadTexture("pipedown.png");
	int imgBg = slLoadTexture("bg.png");
	int font = slLoadFont("text.otf");

	const double gravity = -0.1;
	std::random_device rd;
	int score = 0;


	Bird brd{ 200, 200, 0, 0, 200 };
	std::vector<PipeUp> pipeup;
	for (int i = 0; i < 1; ++i)
	{
		double x = 1920 + i * 128;
		double y = (rd() % 3 + 1) * 100;
		pipeup.push_back({ x, y, 15 }); 

		backsound();
	}
	std::vector<PipeDown> pipedown;
	for (int i = 0; i < 1; ++i)
	{
		double x = 1920 + i * 128;
		double y = (rd() % 3 + 1) * 100;
		pipedown.push_back({ x, y, 15 });
	}

	slSetFont(font, 30);
	bool gameOver = false;
	while (!slShouldClose())
	{
		slSetForeColor(1, 1, 1, 1);
		slSprite(imgBg, 960, 540, 1920, 1080);

		if (slGetMouseButton(SL_MOUSE_BUTTON_LEFT))
		{
			if (!gameOver)
			{
				brd.velY = 3;
			
			}
			else
			{
				for (int i = 0; i < pipeup.size(); ++i)
				{
					pipeup[i].posX = 1920 + i * 128;
					pipeup[i].posY = (rd() % 3 + 1) * 100;
				}
				for (int i = 0; i < pipedown.size(); ++i)
				{
					pipedown[i].posX = 1920 + i * 128;
					pipedown[i].posY = (rd() % 3 + 1) * 100;
				}
				score = 0;
				gameOver = false;
			}
		}
		
		if (!gameOver)
		{
			brd.velY += gravity;
			brd.posY += brd.velY;
			if (brd.posY <= 0)
			{
				brd.posY = 1080;
				brd.velY = 0;
			}

			for (PipeUp& pu : pipeup)
			{
				pu.posX -= 4;
				if (pu.posX <= -30)
				{
					pu.posX = 1920;
					pu.posY = (rd() % 3 + 1) * 100;
					score++;
				}
				if (isCollision(brd, pu))
					gameOver = true; 
			}
			for (PipeDown& pd : pipedown)
			{
				pd.posX -= 4;
				pd.posY = 1080;
				if (pd.posX <= -20)
				{
					pd.posX = 1920;
					pd.posY = (rd() % 3 + 1) * 100;
					score++;
				}
				if (isCollision(brd, pd))
					gameOver = true;
			}

			slSetForeColor(1, 1, 1, 1);
			slSprite(imgBird, brd.posX, brd.posY, 160, 120);

			slSetForeColor(1, 1, 1, 1);
			for (const PipeUp& pu : pipeup)
				slSprite(imgPipeUp, pu.posX, pu.posY, 190, 556);

			slSetForeColor(1, 1, 1, 1);
			for (const PipeDown& pd : pipedown)
				slSprite(imgPipeDown, pd.posX, pd.posY, 190, 556);
		}
		else
		{
			slSetForeColor(1, 0.5, 0.5, 1);
			slSetTextAlign(SL_ALIGN_CENTER);
			slText(960, 540, "Game Over");
			slText(960, 500, "Click to Continue");
		}

		slSetForeColor(1, 0.5, 0.5, 1);
		slSetTextAlign(SL_ALIGN_RIGHT);
		slText(1800, 100, "S u r f  B i r d");

		slSetTextAlign(SL_ALIGN_CENTER);
		slText(120, 980, std::to_string(score).c_str());

		slRender();
	}

	slClose();
	return 0;

}
	