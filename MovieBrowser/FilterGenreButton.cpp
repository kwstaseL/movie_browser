#include "FilterGenreButton.h"

//Updating all filter genre buttons
void FilterGenreButton::update()
{

	if (!m_visible)	//If the widget is invisible , return (+ if button is IDLE can also be added here, if we have a button that might not be invisible)
	{
		return;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	//Converting mouse_x,mouse_y to canvas units
	mouse_X = graphics::windowToCanvasX(ms.cur_pos_x);
	mouse_Y = graphics::windowToCanvasY(ms.cur_pos_y);

	//Checking if our mouse is inside the coordinates of our button
	if (contains(mouse_X, mouse_Y))
	{
		m_button_state = button_state_t::BUTTON_HIGHLIGHTED;

		m_highlighted = true;

		if (ms.button_left_pressed)	//Mouse is pressed
		{
			if (!requestFocus())	//If someone else is operating, return
			{
				return;
			}
			
			m_button_state = button_state_t::BUTTON_PRESSED;
			graphics::playSound(AssetsConst::ASSET_PATH + static_cast<std::string>("button.wav"), SOUND_VOLUME);
			m_action = true;	//Alert that the button was clicked
		}
		if (ms.button_left_released) //Button was pressed and now is released
		{
			m_button_state = button_state_t::BUTTON_IDLE;
			m_action = false;
			m_operating = false;
		}
	}
	else
	{
		m_highlighted = false;
	}

}

//Drawing our filter genre button
void FilterGenreButton::draw()
{
	if (!m_visible)	//If the widget is invisible , return
	{
		return;
	}

	//Drawing highlight background of the button.
	SETCOLOR(brush.fill_color, 0.8f * m_highlighted, 0.8f * m_highlighted, 0.8f * m_highlighted);
	brush.outline_opacity = 1.0f;
	graphics::drawRect(m_positionX, m_positionY + m_height, m_button_width + 0.1f, m_button_height + 0.1f, brush);

	//Drawing m_button_text which represents the buttons name ("Action","Drama"..)
	brush.fill_opacity = 1.0f;
	SETCOLOR(brush.fill_color, 1.0f, 1.0f, 1.0f);
	graphics::drawText(m_positionX - 0.45f, m_positionY + 0.125f + m_height, 0.4f, m_button_text, brush);


	//Drawing our button
	SETCOLOR(brush.fill_color, 0.0f, 0.0f, 0.0f);
	brush.texture = "";
	brush.fill_opacity = 1.0f;
	brush.outline_opacity = 0.5f;
	graphics::drawRect(m_positionX, m_positionY + m_height, m_button_width, m_button_height, brush);
}

/*
Checks if the given movie meets the requirements for filtering (checks if is already filtered or not by other widgets),
used to sychronize all filters with all widgets that can filter, together.
\param movie: a pointer to the movie to be checked
\return true if the movie meets the requirements else false
*/
bool FilterGenreButton::hasRequirements(const Movie* const movie) const
{
	if (movie)
	{
		for (const auto& filter : filterToBeChecked)
		{//If getWidgeState returns disabled, return false since this movie doesnt meet the requirementes for filtering
			if (movie->state_info.getWidgetState(filter) != WidgetEnums::WidgetFilterState::ENABLED)
			{
				return false;
			}
		}// Otherwise, if all the other widgets are enabled, check if it between the sliders years.
		return std::stoi(movie->getDate()) <= movie->state_info.getLastSelectedToYear() && std::stoi(movie->getDate()) >= movie->state_info.getLastSelectedFromYear();
	}
}

// Filters all movies by genre
// \param movie_list: a vector of all the movies
void FilterGenreButton::filterByGenre(const std::vector<Movie*>& movie_list)
{
	//If the genre map isn't created, create it 
	if (!genreMapCreated)
	{
		createGenreMap(movie_list);
	}
	//Inserting to the unordered set the button (text) that was pressed
	s_scanned_genres.insert(m_button_text);

	atLeastOneMovieHasGenres = false;
	/*
		For every movie, we disable it and turn genre filter state to false, we also reset the Genre count which holds
		all the current genres that are pressed by the user the movie has
	*/
	for (const auto& movie : movie_list)
	{
		movie->state_info.setDisabled(true);
		movie->state_info.updateWidgetState(WidgetEnums::WidgetFilters::GenreFilter, WidgetEnums::WidgetFilterState::DISABLED);
		movie->state_info.resetGenreCount();
	}

	//For all the genre buttons that where clicked
	for (const auto& genre : s_scanned_genres)
	{
		//For all movies inside the map with that specific genre
		for (const auto& movie : s_genreMap[genre])
		{
			if (!movie->state_info.isDisabled())
			{
				continue;
			}
			movie->state_info.addGenreCount(1);	//Adding +1 since it has the current genre
		}
	}

	for (const auto& movie : movie_list)
	{
		//If it has all genres that were pressed and it has requirements, then dont disable it, and set it that is it has all current genres that were pressed.
		if ((movie->state_info.getGenreCount() == (s_scanned_genres.size())) && hasRequirements(movie))
		{
			atLeastOneMovieHasGenres = true;
			movie->state_info.setDisabled(false);
			movie->state_info.updateWidgetState(WidgetEnums::WidgetFilters::GenreFilter, WidgetEnums::WidgetFilterState::ENABLED);
		}
	}

	//Because no movie has all genres that were pressed, we just filter based on the last genre that was pressed
	if (!atLeastOneMovieHasGenres)
	{
		clear();
		s_scanned_genres.insert(m_button_text);

		for (const auto& movie : s_genreMap[m_button_text])
		{
			if (hasRequirements(movie))
			{
				movie->state_info.setDisabled(false);
				movie->state_info.updateWidgetState(WidgetEnums::WidgetFilters::GenreFilter, WidgetEnums::WidgetFilterState::ENABLED);
			}
		}
	}
	releaseFocus();	//Releasing focus since operation is done
}


// Creates A map in which the key is a genre text ("Action","Drama"...) and the value is an unordered set of all the movies that have that specific genre.
// Its used map quickly filter movies by genre. 
void FilterGenreButton::createGenreMap(const std::vector<Movie*>& movie_list)
{
	for (const auto& movie : movie_list)
	{
		const std::vector<std::string> genres = movie->getGenres();
		for (const auto& genre : genres)
		{
			s_genreMap[genre].insert(movie);
		}
	}
	genreMapCreated = true;
}

// Resets the state of the button by clearing all the genres that where pressed.
void FilterGenreButton::clear()
{
	s_scanned_genres.clear();
}

//Performs the filtering action when the button is pressed
//\param movie_list: a vector of all the movies
void FilterGenreButton::takeAction(const std::vector<Movie*>& movie_list)
{
	filterByGenre(movie_list);
}


/* Constructs a new filter genre button
\param posX: the x coordinate of the filtergenrebutton's position
\param posY: the y coordinate of the filtergenrebutton's position
\param text: the text displayed on the button
*/
FilterGenreButton::FilterGenreButton(float posX, float posY, const std::string_view text, bool invisible)
	: Button(posX, posY, text)
{
	m_visible = !invisible;
	// Inserting the widgets this class needs to check before filtering the movies
	filterToBeChecked.push_back(WidgetEnums::WidgetFilters::TitleFilter);
}