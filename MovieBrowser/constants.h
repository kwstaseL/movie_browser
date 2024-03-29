#ifndef CONSTANTS_H
#define CONSTANTS_H

//Function which takes in graphics::Brush and fills it with color
#define SETCOLOR(c,r,g,b) {c[0] = r,c[1] = g; c[2] = b;}  

#define SOUND_VOLUME 0.5

namespace constants
{

    namespace WINDOW
    {

        inline constexpr const char* const WINDOW_NAME{ "Movie Browser" };

    }

    namespace CANVAS
    {
        inline constexpr int CANVAS_WIDTH{ 28 };
        inline constexpr int CANVAS_HEIGHT{ 16 };
    }

    namespace MOVIE_SIZES
    {
        inline constexpr float Movie_Banner_Height{ 4.15f };
        inline constexpr float Movie_Banner_Width{ 3.5f };
    }

    namespace ASSETS
    {
        inline constexpr const char* const ASSET_PATH{ ".\\assets\\" };

        inline constexpr const char* const HARRY_POTTER{ "HarryPotter.png" };
        inline constexpr const char* const GODFATHER{ "GodFather.png" };
        inline constexpr const char* const TERMINATOR{ "Terminator.png" };
        inline constexpr const char* const BACKGROUND{ "Background.png" };
        inline constexpr const char* const ANEWHOPE{ "ANewHope.png" };
        inline constexpr const char* const MYNEIGHBORTOTORO{ "MyNeighborTotoro.png" };
        inline constexpr const char* const PULPFICTION{ "PulpFiction.png" };
        inline constexpr const char* const SCHINDLERSLIST{ "SchindlersList.png" };
        inline constexpr const char* const SPIRITEDAWAY{ "SpiritedAway.png" };
        inline constexpr const char* const MIDSOMMAR{ "MidSommar.png" };
        inline constexpr const char* const LORDOFTHERINGS{ "LordOfTheRings.png" };

        inline constexpr const char* const DOCK{ "dock.png" };

    }

    namespace FUNCTIONS
    {
        inline float distance(float x1, float y1, float x2, float y2)
        {
            float dx{ x1 - x2 };
            float dy{ y1 - y2 };
            return sqrt(dx * dx + dy * dy);
        }

    }

}

namespace WindowConst = constants::WINDOW;
namespace CanvasConst = constants::CANVAS;
namespace MovieConst = constants::MOVIE_SIZES;
namespace AssetsConst = constants::ASSETS;
namespace FunctionsConst = constants::FUNCTIONS;



#endif