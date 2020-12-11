#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

#include <Windows.h>

#include <discord.h>

#include "assets.h"

ULARGE_INTEGER jan1_1970LargeInt;

bool withinBounds(sf::RectangleShape rectangle, sf::Vector2f pos) {
    sf::Vector2f rectPos = rectangle.getPosition();
    sf::Vector2f rectSize = rectangle.getSize();

    return (rectPos.x >= pos.x)
        && (rectPos.y >= pos.y)
        && (rectPos.x + rectSize.x <= pos.x)
        && (rectPos.y + rectSize.y <= pos.y);
}

void setActivity(discord::Core*core, std::string details, std::string state, uint64_t since = 0) {
    discord::Activity activity{};

    activity.SetDetails(details.c_str());
    activity.SetState(state.c_str());

    activity.GetAssets().SetLargeImage("largeimage");
    activity.GetAssets().SetLargeText("FunkPlayer v0.0.1");

    activity.GetSecrets().SetJoin("");
    activity.GetSecrets().SetMatch("");
    activity.GetSecrets().SetSpectate("");

    activity.GetTimestamps().SetStart(since);
    activity.GetTimestamps().SetEnd(0);

    activity.SetType(discord::ActivityType::Listening);

    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        if (result == discord::Result::Ok) {
            std::cout << "Successfully set Discord Rich Presence." << std::endl;
        } else {
            std::cerr << "Failed to set Discord Rich Presence with error code " << (int) result << "." << std::endl;
        }
    });
}

std::uint64_t getUTC() {
    SYSTEMTIME now;
    GetSystemTime(&now);

    FILETIME nowFileTime;
    SystemTimeToFileTime(&now, &nowFileTime);

    ULARGE_INTEGER nowLargeInt;
    nowLargeInt.LowPart = nowFileTime.dwLowDateTime;
    nowLargeInt.HighPart = nowFileTime.dwHighDateTime;
    
    if (jan1_1970LargeInt.QuadPart > nowLargeInt.QuadPart) {
        std::cerr << "The epoch timestamp is larger than the timestamp from now. wth?" << std::endl;

        return 0;
    }

    return (nowLargeInt.QuadPart - jan1_1970LargeInt.QuadPart) / 10000000;
}

void pause(funk::Assets& assets, sf::Music& music) {
    if (music.getStatus() == sf::Music::Paused) {
        music.play();
        assets.play_spr.setTexture(assets.pause_tex);
    } else {
        music.pause();
        assets.play_spr.setTexture(assets.play_tex);
    }
}

int main(int argc, char** argv) {
    sf::RenderWindow window(sf::VideoMode(300, 125), "Funk", sf::Style::None);
    sf::Event event;

    sf::Music music;

    discord::Core* core{};

    bool windowMoving = false;
    sf::Vector2i dragStart;
    
    bool sliderDragging = false;
    bool volumeSliderDragging = false;

    float volume = 50;

    SetWindowPos(window.getSystemHandle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    SYSTEMTIME jan1_1970;
    jan1_1970.wDay = 1;
    jan1_1970.wDayOfWeek = 4;
    jan1_1970.wHour = 0;
    jan1_1970.wMilliseconds = 0;
    jan1_1970.wMinute = 0;
    jan1_1970.wMonth = 1;
    jan1_1970.wSecond = 1;
    jan1_1970.wYear = 1970;

    FILETIME jan1_1970FileTime;
    SystemTimeToFileTime(&jan1_1970, &jan1_1970FileTime);

    jan1_1970LargeInt.LowPart = jan1_1970FileTime.dwLowDateTime;
    jan1_1970LargeInt.HighPart = jan1_1970FileTime.dwHighDateTime;

    funk::Assets assets;

    sf::FloatRect progressbarG_bounding = assets.progressbarG_spr.getGlobalBounds();
    sf::FloatRect sliderknob_bounding = assets.sliderKnob_spr.getGlobalBounds();

    sf::FloatRect progressbarHitbox(progressbarG_bounding.left, sliderknob_bounding.top,
            progressbarG_bounding.width, sliderknob_bounding.height);
    
    sf::FloatRect volumebarG_bounding = assets.volumebarG_spr.getGlobalBounds();
    sf::FloatRect volumeSliderknob_bounding = assets.volumeSliderKnob_spr.getGlobalBounds();

    sf::FloatRect volumebarHitbox(volumebarG_bounding.left - 3, volumebarG_bounding.top - 3,
            volumebarG_bounding.width + 3, volumeSliderknob_bounding.height + 3);

    sf::Text songname;
    songname.setFont(assets.font);
    songname.setCharacterSize(20);
    songname.setFillColor(sf::Color::White);
    songname.setPosition(5, 2);

    discord::Result result = discord::Core::Create(786253943294001202, DiscordCreateFlags_NoRequireDiscord, &core);

    std::string filename = "C:\\Users\\sarah\\Music\\Kano - Yuudachi no Ribbon.wav";
    music.openFromFile(filename);
    music.setVolume(volume);
    music.play();

    songname.setString(filename.substr(filename.find_last_of('\\') + 1, filename.length() - filename.find_last_of('\\') - 5));

    setActivity(core, "Listening to music", songname.getString(), getUTC());

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            sf::Vector2i mousePos_i = sf::Mouse::getPosition(window);
            sf::Vector2f mousePos_f = (sf::Vector2f) mousePos_i;
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    } else if (event.key.code == sf::Keyboard::Space) {
                        pause(assets, music);
                    }

                    break;
                case sf::Event::MouseButtonPressed:
                    if (assets.close_spr.getGlobalBounds().contains(mousePos_f)) {
                        window.close();
                    } else if (assets.titlebar_boundary.contains(mousePos_i)) {
                        windowMoving = true;

                        dragStart = sf::Mouse::getPosition(window);
                    } else if (assets.play_spr.getGlobalBounds().contains(mousePos_f)) {
                        pause(assets, music);
                    } else if (progressbarHitbox.contains(mousePos_f)) {
                        music.pause();

                        sliderDragging = true;
                    } else if (assets.volumebarG_spr.getGlobalBounds().contains(mousePos_f)) {
                        volumeSliderDragging = true;

                        volume = 100.f - std::clamp<float>(mousePos_f.y - 2.f - 35.f, 0.f, 58.f) / 58.f * 100.f;

                        music.setVolume(volume);
                    }

                    break;
                case sf::Event::MouseButtonReleased:
                    if (windowMoving) {
                        windowMoving = false;
                    } else if (sliderDragging) {
                        float newProgress = std::clamp<float>(mousePos_f.x - 2.f - 103.f, 0.f, 184.f);
                        float newTime = music.getDuration().asSeconds() * newProgress / 184.f;

                        assets.sliderKnob_spr.setPosition(newProgress + 103.f, 96);

                        music.setPlayingOffset(sf::seconds(newTime));

                        music.play();

                        sliderDragging = false;
                    } else if (volumeSliderDragging) {
                        volume = 100.f - std::clamp<float>(mousePos_f.y - 2.f - 35.f, 0.f, 58.f) / 58.f * 100.f;

                        music.setVolume(volume);

                        volumeSliderDragging = false;
                    }

                    break;
                case sf::Event::MouseMoved:
                    if (assets.close_spr.getGlobalBounds().contains(mousePos_f)) {
                        assets.close_spr.setTexture(assets.closeSelected_tex);
                    } else {
                        assets.close_spr.setTexture(assets.closeUnselected_tex);
                    }

                    if (sliderDragging) {
                        float newProgress = std::clamp<float>(mousePos_f.x - 2.f - 103.f, 0.f, 184.f);

                        assets.sliderKnob_spr.setPosition(newProgress + 103.f, 96);
                    } else if (volumeSliderDragging) {
                        volume = 100.f - std::clamp<float>(mousePos_f.y - 2.f - 35.f, 0.f, 58.f) / 58.f * 100.f;

                        music.setVolume(volume);
                    }

                    break;
            }
        }

        if (windowMoving) {
            window.setPosition(sf::Mouse::getPosition() - dragStart);
        }
                    
        if (!sf::FloatRect((sf::Vector2f) window.getPosition(), (sf::Vector2f) window.getSize())
                .contains((sf::Vector2f) sf::Mouse::getPosition())) {
            assets.close_spr.setTexture(assets.closeUnselected_tex);
        }

        if (!sliderDragging) {
            int progress = music.getPlayingOffset().asSeconds() / music.getDuration().asSeconds() * 184;

            assets.progressbar_spr.setScale(progress, 1);
            assets.sliderKnob_spr.setPosition(103 + progress - 3, 96);
        } else {
            assets.progressbar_spr.setScale(assets.sliderKnob_spr.getPosition().x - 103.f, 1);
        }

        assets.volumebar_spr.setScale(1, volume / 100.f * 58.f);
        assets.volumebar_spr.setPosition(287, 35 + 58 - ((int) volume / 100.f * 58.f));
        assets.volumeSliderKnob_spr.setPosition(287 - 2, 35 + 58 - ((int) volume / 100.f * 58.f) - 2);

        window.clear(sf::Color(37, 37, 37, 255));

        window.draw(assets.titlebar_spr);
        window.draw(assets.progressbarG_spr);
        window.draw(assets.progressbar_spr);
        window.draw(assets.play_spr);
        window.draw(songname);
        window.draw(assets.sliderKnob_spr);
        window.draw(assets.close_spr);
        window.draw(assets.volumebarG_spr);
        window.draw(assets.volumebar_spr);
        window.draw(assets.volumeSliderKnob_spr);

        window.display();

        core->RunCallbacks();
    }
}