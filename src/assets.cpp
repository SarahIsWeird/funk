#include "assets.h"

funk::Assets::Assets() {
    titlebar_boundary = *new sf::IntRect(0, 0, 300, 30);
    titlebar_img.create(titlebar_boundary.width, titlebar_boundary.height, sf::Color(25, 25, 25));
    titlebar_tex.loadFromImage(titlebar_img);
    titlebar_spr.setTexture(titlebar_tex);

    progressbar_img.create(1, 3, sf::Color::White);
    progressbar_tex.loadFromImage(progressbar_img);
    progressbar_spr.setTexture(progressbar_tex);
    progressbar_spr.setPosition(106, 98);

    progressbarG_img.create(1, 3, sf::Color(25, 25, 25));
    progressbarG_tex.loadFromImage(progressbarG_img);
    progressbarG_spr.setTexture(progressbarG_tex);
    progressbarG_spr.setPosition(106, 98);
    progressbarG_spr.setScale(184, 1);

    play_tex.loadFromFile("assets\\play.png");
    pause_tex.loadFromFile("assets\\pause.png");
    play_spr.setTexture(pause_tex);
    play_spr.setPosition(14, 45);

    sliderKnob_tex.loadFromFile("assets\\slider_knob.png");
    sliderKnob_spr.setTexture(sliderKnob_tex);
    sliderKnob_spr.setPosition(103, 96);

    closeUnselected_tex.loadFromFile("assets\\close_unselected.png");
    closeSelected_tex.loadFromFile("assets\\close_selected.png");
    close_spr.setTexture(closeUnselected_tex);
    close_spr.setPosition(270, 0);

    volumebar_img.create(3, 1, sf::Color::White);
    volumebar_tex.loadFromImage(volumebar_img);
    volumebar_spr.setTexture(volumebar_tex);

    volumebarG_img.create(3, 58, sf::Color(25, 25, 25));
    volumebarG_tex.loadFromImage(volumebarG_img);
    volumebarG_spr.setTexture(volumebarG_tex);
    volumebarG_spr.setPosition(287, 35);
    volumeSliderKnob_spr.setTexture(sliderKnob_tex);

    font.loadFromFile("assets\\segoeuil.ttf");
}