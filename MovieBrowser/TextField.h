#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "Widget.h"
#include <stack>
#include <deque>

// Indicates which filtering can be done on the textfield
// Added if we ever want to filter something other than title,protagnoist_director on the TextField.

namespace TextFieldFiltering 
{
    //Enum represents if the widget filter is enabled or disabled. 
    enum class FilterBy
    {
        TITLE_PROTAGONIST_DIRECTOR,
        TITLE,
        PROTAGONIST,
        DIRECTOR
    };
}

/*
Represents a TextField, which in our program has a text ("Search Movie/Dir/Prot"), and when pressed user can provide a title of a movie or a director or a prodagonist.
The TextButton class is derived from the Widget class, which provids a common interface for interacting with different types of widgets.
When filtering, we also take as consideration all the other widgets that may have filtered the movies (filtergenre,sliders state).
*/
class TextField : public Widget
{
private:

    /*
    * Continuously draws the text field.
    */
    void draw() override;

    /*
     * Continuously updates the text field.
     */
    void update() override;

    /*
   * Checks if the given movie meets the specified requirements for filtering.
   * \param movie The movie to check.
   * \return true if the movie has the filtered genre and is between the 2 years specified by the slider, false otherwise.
   */
    bool hasRequirements(const Movie* const movie) const;

    /*
     * Searches for movies by title or director using the given list of movies.
     * \param movie_list The list of movies to search through.
     */
    void searchByTitleProtDir(const std::vector<Movie*>& movie_list);

    // Clears all variables of the textfield
    void clear() override;

    /*
     * Check if the mouse is inside the coordinates of the text field.
     * \param x The x coordinate of the mouse.
     * \param y The y coordinate of the mouse.
     * \return true if the mouse is in the coordinates of textfield
 */
    bool contains(float x, float y) const;

    /*
     * Takes a list of movies and performs some actions on them.
     * \param movie_list The list of movies to perform actions on.
     */
    void takeAction(const std::vector<Movie*>& movie_list) override;

    float getHeightOffset() const  override { return m_height_offset; }

private:
    // Constants that store the height and width of the text field.
    static constexpr float m_Textfield_height{ 0.5f };
    static constexpr float m_Textfield_width{ 4.0f };

    // Represents the filter which the textfield will filter
    const TextFieldFiltering::FilterBy m_textfield_filter;

    // Represents the speed that the user can write on the textfield
    static constexpr int m_textfield_speed{ 8 };

    // Represents the maximum words that the user can write on the textfield
    static constexpr int max_textfield_words{ 25 };

    // Deque that stores all the characters the user gave as input, we decided to use a deque because it
    // allows fast insertion and deletion at both its beginning and its end (O(1)).
    std::deque<char> m_characters{};

    // Contains a constant value to specify what text field it is ("Search Movie").
    const std::string m_text{};

    // Keeps track if the text field is full or not.
    bool isFull{ false };

    //Represents if the text that is drawn inside the textfield is invisible or not.
    bool isTextInvisible{ false };

    // Variable that stores whether the user has typed in the text field.
    bool typed{ false };

    // This variable acts like a "timer", when the timer surpasses a default value, we want to set mtyped to false, so user can type again
    int timer{ -1 };

    // We use a stack to keep in the out of sight words (that the user can't see) when the textfield gets overflowed with words
    // we keep the words that the user can't see in a stack.
    // When the user presses "BACKSPACE" we insert one-by-one each character in the front of the whole word that is kept in the deque.
    std::stack<char> outofsight_characters{};

    // A vector indicating all widgets this class needs to check, if they have filtered the movies previously.
   // This is used in order to sychronize all filters together. (Also watch MovieState.h + hasRequirements)
    std::vector<WidgetEnums::WidgetFilters> filterToBeChecked{};

    //This represents how much height will the textfield need in order to appear when the dock comes down
    static constexpr float m_height_offset{ 4.0f };

public:
    /*
     * Constructs a new text field at the given position with the given text.
     * \param posX The x position of the text field.
     * \param posY The y position of the text field.
     * \param text The text to display in the text field.
     */
    TextField(float posX, float posY, const std::string_view text, const TextFieldFiltering::FilterBy& filter, bool invisible = true);
    virtual ~TextField() = default;
};

#endif