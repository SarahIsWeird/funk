#if !defined(ASSETS_H)
#define ASSETS_H

#include <SFML/Graphics.hpp>

namespace funk {

class Assets {
public:
    Assets();

    sf::IntRect titlebar_boundary;
    sf::Image titlebar_img;
    sf::Texture titlebar_tex;
    sf::Sprite titlebar_spr;

    sf::Image progressbar_img;
    sf::Texture progressbar_tex;
    sf::Sprite progressbar_spr;

    sf::Image progressbarG_img;
    sf::Texture progressbarG_tex;
    sf::Sprite progressbarG_spr;

    sf::Texture play_tex;
    sf::Texture pause_tex;
    sf::Sprite play_spr;

    sf::Texture sliderKnob_tex;
    sf::Sprite sliderKnob_spr;

    sf::Texture closeUnselected_tex;
    sf::Texture closeSelected_tex;
    sf::Sprite close_spr;

    sf::Image volumebar_img;
    sf::Texture volumebar_tex;
    sf::Sprite volumebar_spr;

    sf::Image volumebarG_img;
    sf::Texture volumebarG_tex;
    sf::Sprite volumebarG_spr;
    sf::Sprite volumeSliderKnob_spr;

    sf::Font font;
};

}; // namespace funk

#endif // ASSETS_H
