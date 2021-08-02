#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    bool songSelected = false;
    bool playlistWordSelected = false;

    //setting the sf objects before entering loop for updating window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Playlist finder!");
    sf::Event event;
    sf::Font font;
    if (!font.loadFromFile("TimesNewerRoman-Italic.otf"))//this font was taken from https://timesnewerroman.com/ and is open-source
        return EXIT_FAILURE;

    string songInput;
    string playlistWordInput;
    sf::Text title("Welcome! Fill out at least one of the below fields and click run to find a playlist!", font, 18);
    sf::Text title1("Song to search for:", font, 13);
    sf::Text inputsong("", font, 13);
    sf::Text title2("Playlist word to search for:", font, 13);
    sf::Text inputPlaylistWord("", font, 13);
    sf::Text search("Search", font, 15);
    sf::RectangleShape inputBox(sf::Vector2f(200,20));
    sf::RectangleShape inputBox1(sf::Vector2f(200, 20));
    sf::RectangleShape searchButton(sf::Vector2f(50, 20));
    title.setFillColor(sf::Color::Black);
    title1.setFillColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setFillColor(sf::Color::White);
    inputsong.setFillColor(sf::Color::Black);
    title2.setFillColor(sf::Color::Black);
    inputBox1.setOutlineThickness(2);
    inputBox1.setOutlineColor(sf::Color::Black);
    inputBox1.setFillColor(sf::Color::White);
    inputPlaylistWord.setFillColor(sf::Color::Black);
    searchButton.setOutlineThickness(1);
    searchButton.setOutlineColor(sf::Color::Black);
    searchButton.setFillColor(sf::Color::Transparent);
    search.setFillColor(sf::Color::Black);

    while (window.isOpen())
    {
        title.setPosition(0, 0);
        title1.setPosition(0, 25);
        inputBox.setPosition(2, 45);
        inputsong.setPosition(3, 45);
        title2.setPosition(0, 70);
        inputBox1.setPosition(2, 90);
        inputPlaylistWord.setPosition(3, 90);
        searchButton.setPosition(150, 130);
        search.setPosition(153,130);

        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                //if window is being closed
                if (event.type == sf::Event::Closed)
                    window.close();

                //to check when a text box/rectangle is being pressed
                if (event.type == sf::Event::MouseButtonPressed) {
                    auto position = sf::Mouse::getPosition(window);
                    if (inputBox.getGlobalBounds().contains(sf::Vector2f(position))) {
                        inputBox.setOutlineColor(sf::Color::Blue);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        searchButton.setFillColor(sf::Color::White);
                        songSelected = true;
                        playlistWordSelected = false;
                    }
                    else if (inputBox1.getGlobalBounds().contains(sf::Vector2f(position))) {
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Blue);
                        searchButton.setFillColor(sf::Color::White);
                        playlistWordSelected = true;
                        songSelected = false;
                    }
                    else if(searchButton.getGlobalBounds().contains(sf::Vector2f(position))) {
                        searchButton.setFillColor(sf::Color::Blue);
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        playlistWordSelected = false;
                        songSelected = false;

                        if (songInput.compare("") == 0 && playlistWordInput.compare("") != 0) {//only playlist field has input
                            //search for playlist
                        }
                        else if (playlistWordInput.compare("") == 0 && songInput.compare("") != 0) {//only song field has input
                            //search for song
                        }
                        else if () {//both are filled out
                            //search for song
                            //search for playlist in retrieved ones
                        }
                        else {//neither filled out
                            //do nothing
                        }
                    }
                    else {//click on empty space in window
                        inputBox.setOutlineColor(sf::Color::Black);
                        inputBox1.setOutlineColor(sf::Color::Black);
                        searchButton.setFillColor(sf::Color::White);
                        songSelected = false;
                        playlistWordSelected = false;
                    }
                }

                //if text is being entered
                if (event.type == sf::Event::TextEntered) {
                    //if song box is selected
                    if (event.text.unicode < 128 && songSelected) {
                        if (event.text.unicode == 8) //backspace
                            songInput = songInput.substr(0, songInput.size() - 1);
                        else
                            songInput += event.text.unicode;

                        inputsong.setString(songInput);
                    }
                    //if playlistWord box is selected
                    else if (event.text.unicode < 128 && playlistWordSelected) {
                        if (event.text.unicode == 8) //backspace
                            playlistWordInput = playlistWordInput.substr(0, playlistWordInput.size() - 1);
                        else
                            playlistWordInput += event.text.unicode;

                        inputPlaylistWord.setString(playlistWordInput);
                    }
                }
            }

            window.clear(sf::Color::White);
            window.draw(title);
            window.draw(title1);
            window.draw(inputBox);
            window.draw(inputsong);
            window.draw(title2);
            window.draw(inputBox1);
            window.draw(inputPlaylistWord);
            window.draw(searchButton);
            window.draw(search);
            window.display();
        }
    }

    return 0;
}