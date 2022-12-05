#include "Movie.h"

Movie::Movie(const std::string_view name, const std::string_view desc, const std::string_view age, const std::string_view image)

	: m_name(name), m_description(desc), m_age_restriction(age),m_image(image)

{
}

void Movie::draw()
{
	//This function will draw a Movie to the screen
	graphics::Brush br2;

	//This is for our glow when doing mouse over movie

	float glow{ 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 100) * m_highlighted };

	//Hightlight movie when mouse over movie

	float h{ 0.2f * m_highlighted + glow * 0.5f };	//If we mouse over a movie, then it gets highlighted

	//Drawing our highlight/glow

	SETCOLOR(br2.fill_color, h, h, h);
	br2.outline_opacity = 0.0f;

	graphics::drawRect(m_pos[0], m_pos[1], MovieConst::Movie_Banner_Width+0.125, MovieConst::Movie_Banner_Height+0.125, br2);


	//Drawing our Movie

	br.outline_opacity = 0.1f;

	br.texture = AssetsConst::ASSET_PATH + static_cast<std::string>(m_image);
	graphics::drawRect(m_pos[0],m_pos[1], MovieConst::Movie_Banner_Width, MovieConst::Movie_Banner_Height, br);
	graphics::resetPose();



}

void Movie::update()
{

	graphics::MouseState ms;
	graphics::getMouseState(ms);	//Fills our ms state, ms inside has states of our mouse , button left pressed, button left down..

	float mx{ graphics::windowToCanvasX(ms.cur_pos_x) };
	float my{ graphics::windowToCanvasY(ms.cur_pos_y) };

	//Going with my mouse to movie images , mouse_x and mouse_y is on the image if they are on movie's "zone"

	
		if (contains(mx, my)) {

			if (PlaySound)
			{
				graphics::playSound(AssetsConst::ASSET_PATH + static_cast<std::string>("browse.wav"), 0.4f);
			}
			setHighlight(true);
			DisplayInfo();
			PlaySound = false;
		}
		else
		{
			PlaySound = true;
			setHighlight(false);

		}

}

bool Movie::contains(float x, float y) const
{
	return FunctionsConst::distance(x, y,m_pos[0],m_pos[1]) < MovieConst::Movie_Banner_Height*MovieConst::Movie_Banner_Width*0.14f;

}

const std::string& Movie::getName() const
{
	return m_name;
}

const std::string& Movie::getDesc() const
{
	return m_description;
}

void Movie::DisplayInfo()
{
	graphics::Brush br;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15, CanvasConst::CANVAS_HEIGHT / 1.35, 1.0f,getName(), br);


	char info[80];
	sprintf_s(info, " ", getDesc());
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15, CanvasConst::CANVAS_HEIGHT / 1.09, 0.5f, getDesc(), br);


}
