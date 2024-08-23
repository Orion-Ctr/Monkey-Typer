#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float WORD_SPEED = 0.01;
const int INITIAL_LIVES = 3;

struct Word {
    std::string text;
    sf::Text sfText;
    float x;
    float y;
};

void loadWords(std::vector<std::string>& words) {
    // Load words from a file or define them here
    words = {"ball", "pass", "future", "pjc", "coding", "hard", "pain", "tears", "stress", "pressure", "money", "space", "helicopter", "hotdog", "zabka", "brains", "professor", "error", "loading", "network", "career", "time", "crazy"};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Typing Game");

    sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Error loading font\n";
        return 1;
    }

    std::vector<std::string> wordList;
    loadWords(wordList);

    std::vector<Word> activeWords;
    std::string input;
    int score = 0;
    int lives = INITIAL_LIVES;
    float spawnTimer = 0;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(sf::Color::Red);
    livesText.setPosition(10, 40);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !input.empty()) {
                    input.pop_back();
                } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
                    input += static_cast<char>(event.text.unicode);
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        spawnTimer += deltaTime;

        if (spawnTimer > 2.0f) {
            spawnTimer = 0;
            Word newWord;
            newWord.text = wordList[rand() % wordList.size()];
            newWord.sfText.setFont(font);
            newWord.sfText.setString(newWord.text);
            newWord.sfText.setCharacterSize(24);
            newWord.sfText.setFillColor(sf::Color::Green);
            newWord.x = 0;
            newWord.y = rand() % (WINDOW_HEIGHT - 50);
            newWord.sfText.setPosition(newWord.x, newWord.y);
            activeWords.push_back(newWord);
        }

        for (auto& word : activeWords) {
            word.x += WORD_SPEED;
            word.sfText.setPosition(word.x, word.y);
        }

        for (auto it = activeWords.begin(); it != activeWords.end();) {
            if (it->x > WINDOW_WIDTH) {
                lives--;
                it = activeWords.erase(it);
                if (lives == 0) {
                    std::ofstream highscoreFile("highscores.txt", std::ios::app);
                    if (highscoreFile.is_open()) {
                        std::string playerName;
                        std::cout << "Game Over! Enter your name: ";
                        std::cin >> playerName;
                        highscoreFile << playerName << " " << score << "\n";
                        highscoreFile.close();
                    }
                    window.close();
                }
            } else {
                ++it;
            }
        }

        for (auto it = activeWords.begin(); it != activeWords.end();) {
            if (input == it->text) {
                score++;
                input.clear();
                it = activeWords.erase(it);
            } else {
                ++it;
            }
        }

        scoreText.setString("Score: " + std::to_string(score));
        livesText.setString("Lives: " + std::to_string(lives));

        window.clear();
        for (const auto& word : activeWords) {
            window.draw(word.sfText);
        }
        window.draw(scoreText);
        window.draw(livesText);
        window.display();
    }

    return 0;
}
