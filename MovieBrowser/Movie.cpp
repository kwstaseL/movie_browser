#include "Movie.h"
#include <cstring>


// Constructor for Movie class. Initializes member variables (desc,name,year..), sets the position of the information box, and also creates the filterstates of all the widgets needed.
Movie::Movie(const std::string_view name, const std::string_view desc, const std::string_view image, const std::string_view year, const std::string_view dir, const std::vector<std::string>& prot, const std::vector<std::string>& genre)
	: m_name{ name }, m_description{ desc }, m_image{ image }, m_production_year{ year }, m_director{ dir }, m_genres{ genre }, m_protagonists{ prot }
{
	//Setting the position of the information box to the center of the canvas
	informationBox.setPosX(CanvasConst::CANVAS_WIDTH / 2);
	informationBox.setPosY(CanvasConst::CANVAS_HEIGHT / 2);

	//Creates the Widget Filter States, that each movie has and is filtered upon
	state_info.insertNewWidgetState(WidgetEnums::WidgetFilters::GenreFilter, WidgetEnums::WidgetFilterState::ENABLED);
	state_info.insertNewWidgetState(WidgetEnums::WidgetFilters::TitleFilter, WidgetEnums::WidgetFilterState::ENABLED);
}

//Function that draws a movie on the screen
void Movie::draw()
{
	if (state_info.isDisabled())
	{
		return;
	}

	const float BASE_GLOW{ 0.5f };
	const float GLOW_AMPLITUDE{ 0.5f };
	const float GLOW_PERIOD{ 100.0f };
	const float HIGHLIGHT_STRENGTH{ 0.2f };
	const float HIGHLIGHT_SCALE{ 0.5f };

	// Calculate glow effect for when the mouse is hovering over the movie.
	float glow = BASE_GLOW + GLOW_AMPLITUDE * sinf(graphics::getGlobalTime() / GLOW_PERIOD) * m_highlighted;
	float highlight = HIGHLIGHT_STRENGTH * m_highlighted + glow * HIGHLIGHT_SCALE;


	
	// Draw highlight/glow effect.
	const float BANNER_PADDING = 0.125f;

	SETCOLOR(brush2.fill_color, highlight, highlight, highlight);
	brush2.outline_opacity = 0.0f;
	graphics::drawRect(m_pos[0], m_pos[1], MovieConst::Movie_Banner_Width + BANNER_PADDING, MovieConst::Movie_Banner_Height + BANNER_PADDING, brush2);


	//Drawing our Movie image
	brush3.outline_opacity = 0.1f;
	brush3.texture = AssetsConst::ASSET_PATH + static_cast<std::string>(m_image);
		graphics::drawRect(m_pos[0], m_pos[1], MovieConst::Movie_Banner_Width, MovieConst::Movie_Banner_Height, brush3);

}

// Function that updates the movie on the screen.
void Movie::update()
{
	if (state_info.isDisabled())
	{
		return;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	float mx{ graphics::windowToCanvasX(ms.cur_pos_x) };
	float my{ graphics::windowToCanvasY(ms.cur_pos_y) };

	// If the mouse is within the coordinates of the movie.
	if (contains(mx, my)) {

		if (state_info.isInformationVisible())
		{
			if (playSound)
			{
				graphics::playSound(AssetsConst::ASSET_PATH + static_cast<std::string>("browse.wav"), SOUND_VOLUME);
			}

			setHighlight(true);
			displayMovieDetails();
			playSound = false;

		}
		//If mouse contains movie, and mouse button left button is pressed,update its state to pressed.
		if (ms.button_left_pressed && state_info.isInformationVisible())
		{
			state_info.setClickTriggered(true);
		}
		//If left button is released , update its state to not pressed.
		if (ms.button_left_released)
		{
			state_info.setClickTriggered(false);
		}

	}
	else
	{
		playSound = true;
		setHighlight(false);
	}


	//If mouse doesn't contain anymore and mouse is released then again we update its state to not pressed
	if (ms.button_left_released)
	{
		state_info.setClickTriggered(false);
	}

}

/*
 * Determines if the given coordinates of mouse x,y are insde the coordinates of the Movies Frame.
 * \param mouse_x: the x coordinate of the mouse.
 * \param mouse_y: the y coordinate of the mouse.
 * \return True if the given coordinates of mouse x,y is inside the coordinates of the Movies Frame.
 */
bool Movie::contains(float mouse_x, float mouse_y) const
{
	return (mouse_x > m_pos[0] - MovieConst::Movie_Banner_Width / 2 && mouse_x < m_pos[0] + MovieConst::Movie_Banner_Width / 2 
		&& mouse_y > m_pos[1] - MovieConst::Movie_Banner_Height / 2 && mouse_y < m_pos[1] + MovieConst::Movie_Banner_Height / 2);
}


// Function that returns a description lines vector which represents all the lines that should be drawen to the canvas line-by-line.
// Based on the whole description of the movie.

const std::vector<std::string> Movie::createDescription()
{
	const int MAX_LINE_WORDS = 80;
	std::string description = getDesc();	//Getting the description of the movie
	char* tokens = &description[0];
	char* context;
	tokens = strtok_s(tokens, " ", &context);
	std::vector<std::string> words;	//Creating vector which stores all the words separated

	// Here we are iterating to all the words of the description and tokenize them based on the " "
	while (tokens != NULL) {
		words.push_back(tokens);
		tokens = strtok_s(nullptr, " ", &context);
	}

	std::vector<std::string> description_lines;	//This vector has all the lines of the description
	std::string line;

	for (const auto& word : words) {	// For all the words in our words vector

		//Here we check if adding a word would exceed the 90 which represents the maximum length of the line on each row.
		if (line.length() + word.length() + 1 > MAX_LINE_WORDS) {

			//Adds the current line to the lines vector
			description_lines.push_back(line);
			//and clears the line so a new line can start
			line.clear();
		}
		// Here, it adds the current word we are iterating allong with a space to this particular line
		line += word + "  ";
	}
	//Pushes back the last current line
	description_lines.push_back(line);

	return description_lines;
}


// Function that displays movie state_information on the "starting screen" 
void Movie::displayMovieDetails()
{

	graphics::Brush br;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);

	//Drawing the Movies Name
	const int NAME_FONT_OFFSET = 1.0f;
	graphics::setFont("OpenSans-Semibold.ttf");
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f - NAME_FONT_OFFSET, CanvasConst::CANVAS_HEIGHT / 1.35f, 1.0f, getName(), br);

	//Drawing the Movies Date
	graphics::setFont("OpenSans-Regular.ttf");
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f, CanvasConst::CANVAS_HEIGHT / 1.20f, 0.5f, "Year:      " + getDate(), br);

	//Drawing the Movies Directors
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f, CanvasConst::CANVAS_HEIGHT / 1.15f, 0.5f, "Director:      " + getDir(), br);
	graphics::setFont("OpenSans-Regular.ttf");

	//For all protagonists, draw the protagonist
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f, CanvasConst::CANVAS_HEIGHT / 1.10f, 0.5f, "Protagonists:      ", br);
	float LABEL_OFFSET{ 3.5f };
	for (const auto& protagonist : m_protagonists)
	{
		graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f + LABEL_OFFSET, CanvasConst::CANVAS_HEIGHT / 1.10f, 0.5f, protagonist, br);
		LABEL_OFFSET += protagonist.size() / 3.0f;
	}

	//For all genres, draw the genre
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f - 1.0f, CanvasConst::CANVAS_HEIGHT / 1.28f, 0.5f, "Genre:      ", br);
	LABEL_OFFSET = 1.5f;
	for (const auto& genre : m_genres)
	{
		graphics::drawText(CanvasConst::CANVAS_WIDTH / 15.0f + LABEL_OFFSET, CanvasConst::CANVAS_HEIGHT / 1.28, 0.5f, genre, br);
		LABEL_OFFSET += genre.size() / 3.0f;
	}

}

// Function used to draw state_information about a movie on the screen when the movie is clicked
void Movie::drawMovieInformation()
{
	graphics::Brush brush4;

	//Drawing our background image
	// Set the brush properties for the background
	brush.fill_opacity = 0.5f;
	brush.outline_opacity = 0.0f;
	brush.texture = AssetsConst::ASSET_PATH + static_cast<std::string>(AssetsConst::BACKGROUND);
	graphics::drawRect(CanvasConst::CANVAS_WIDTH / 2, CanvasConst::CANVAS_HEIGHT / 2, CanvasConst::CANVAS_WIDTH, CanvasConst::CANVAS_HEIGHT, brush);

	//Drawing outside box of movie state_information
	SETCOLOR(brush4.fill_color, 0.0f, 0.0f, 0.0f);
	brush4.texture = "";
	brush4.fill_opacity = 0.7f;
	brush4.outline_opacity = 0.3f;
	graphics::drawRect(informationBox.getPosX(), informationBox.getPosY(), CanvasConst::CANVAS_WIDTH / 1.2f, CanvasConst::CANVAS_HEIGHT / 1.2f, brush4);

	// Draw the title of the movie
	graphics::setFont("OpenSans-Semibold.ttf");
	graphics::Brush br;
	SETCOLOR(br.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.6f, CanvasConst::CANVAS_HEIGHT / 6.0f, 1.0f, getName(), br);

	// Draw the image of the movie
	float WIDTH_OFFSET{ 0.7f };
	br.outline_opacity = 0.5f;
	br.texture = AssetsConst::ASSET_PATH + static_cast<std::string>(m_image);
	graphics::drawRect(CanvasConst::CANVAS_WIDTH / 5.5f, CanvasConst::CANVAS_HEIGHT / 3.8f, MovieConst::Movie_Banner_Width + WIDTH_OFFSET, MovieConst::Movie_Banner_Height + WIDTH_OFFSET, br);

	// Set the font and brush properties for the description
	graphics::setFont("OpenSans-Semibold.ttf");
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 8.0f, CanvasConst::CANVAS_HEIGHT / 2.0f, 0.5f, "Description: ", br);
	graphics::setFont("OpenSans-Light.ttf");


	// In this process, we split the description into multiple lines if it is too long to fit on a single line
	//------------------------------------------------------------------------------------------------
	if (!descriptionCreated)
	{
		description_lines = createDescription();
	}

	//For all lines in the lines vector we have made, draws the lines. Every time a line is drawn, we add a height so the next line can go under the previous line drawen.
	float text_height_offset{ 0.0f };
	for (const auto& line : description_lines)
	{
		graphics::drawText(CanvasConst::CANVAS_WIDTH / 8.0f, CanvasConst::CANVAS_HEIGHT / 1.65f+ text_height_offset, 0.5f, line, br);
		text_height_offset += .7f;
	}

	//------------------------------------------------------------------------------------------------

	//Drawing year of movie
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.4f + getName().size() / 2.5f + 1.2f, CanvasConst::CANVAS_HEIGHT / 6.25f, 0.7f, +"(" + getDate() + ")", br);
	graphics::setFont("OpenSans-Light.ttf");

	// Draw the director of the movie
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.5f, CanvasConst::CANVAS_HEIGHT / 3.3f, 0.5f, "-Directors:   " + getDir(), br);

	//Draw all protagonists of the movie
	WIDTH_OFFSET = 3.5f;
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.5f, CanvasConst::CANVAS_HEIGHT / 2.7f, 0.5f, "-Protagonists: ", br);

	for (const auto& protagonist : m_protagonists)
	{
		graphics::setFont("OpenSans-Light.ttf");
		graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.5f + WIDTH_OFFSET, CanvasConst::CANVAS_HEIGHT / 2.7f, 0.5f, protagonist, br);
		WIDTH_OFFSET += protagonist.size() / 3.5f;
	}

	//Draw all Genres of the movie
	WIDTH_OFFSET = 3.5f;
	graphics::drawText(CanvasConst::CANVAS_WIDTH / 3.6f, CanvasConst::CANVAS_HEIGHT / 4.5f, 0.5f, "Genre: ", br);
	for (const auto& genre : m_genres)
	{
		graphics::setFont("OpenSans-Light.ttf");
		graphics::drawText(CanvasConst::CANVAS_WIDTH / 4.7f + WIDTH_OFFSET, CanvasConst::CANVAS_HEIGHT / 4.5f, 0.5f, genre, br);
		WIDTH_OFFSET += genre.size() / 3.5f;
	}

}

const std::string& Movie::getName() const
{
	return m_name;
}

const std::string& Movie::getDesc() const
{
	return m_description;
}

const std::string& Movie::getDate() const
{
	return m_production_year;
}

const std::string& Movie::getDir() const
{
	return m_director;
}

const std::vector<std::string>& Movie::getGenres() const
{
	return m_genres;
}

const std::vector<std::string>& Movie::getProtagonists() const
{
	return m_protagonists;
}