#include "TextField.h"
#include <algorithm>
#include <cctype>

void TextField::draw()
{
	if (!m_visible)
	{
		return;
	}

	graphics::Brush br;

	SETCOLOR(br.fill_color, 0.5f, 0.5f, 0.5f);
	graphics::drawRect(m_positionX, m_positionY + m_height, 4.0f, 0.5f, br);

	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(m_positionX - 3.0f, m_positionY + 4.1f, 0.3f, m_text, br);
}

void TextField::update()
{
	m_height += 0.008f * graphics::getDeltaTime();

	if (m_height > 4.0f)
	{
		m_height = 4.0f;
	}

	if (!m_visible)
	{
		m_height = 0.0f;
		return;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	mouse_X = graphics::windowToCanvasX(ms.cur_pos_x);
	mouse_Y = graphics::windowToCanvasY(ms.cur_pos_y);


	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		if (!m_typed)
		{
			m_counter = 0;
			characters.push_back('a');
			iterator++;
			m_typed = true;
			m_action = true;
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_B))
	{
		if (!m_typed)
		{
			m_counter = 0;
			characters.push_back('b');
			iterator++;
			m_typed = true;
			m_action = true;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_C))
	{
		if (!m_typed)
		{
			m_counter = 0;
			characters.push_back('c');
			iterator++;
			m_typed = true;
			m_action = true;
		}
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		if (!m_typed)
		{
			m_counter = 0;
			characters.push_back('c');
			iterator++;
			m_typed = true;
			m_action = true;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE))
	{
		if (!characters.empty())
		{
			m_counter = 0;
			characters.pop_back();
			iterator--;
		}
	}

	if (!characters.empty())
	{
		std::string string(characters.begin(), characters.end());
		graphics::drawText(m_positionX - 1.9f, m_positionY + 4.1f, 0.3f, string, brush);
	}

	m_counter++;
	if (m_counter > 20)
		m_typed = false;
}

void TextField::takeAction(const std::vector<Movie*>& movie_list)
{

}

bool TextField::contains(float x, float y) const
{
	return FunctionsConst::distance(x, y, m_positionX, m_positionY);
}

void TextField::deleteText() {

	characters.clear();
}

TextField::TextField(float posX, float posY, const std::string_view text)
	:Widget(posX, posY), m_text{ text }
{}