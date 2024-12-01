#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/System.hpp> 
#include <random>
#include <cstdlib>  // rand() �Լ� ����� ���� ���
#include <ctime>    // srand() �Լ� ����� ���� ���

using namespace std;

// �̹��� Ŭ����
class Logo {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Logo(const std::string& imagePath) {
        if (!texture.loadFromFile(imagePath)) {
            throw std::runtime_error("Failed to load image: " + imagePath);
        }

        sprite.setTexture(texture);
        sprite.setPosition(-450, -460);
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

// ���Ը� Ŭ����
class SlotReel {
public:
    sf::RectangleShape body;
    sf::RectangleShape innerBox;
    sf::RectangleShape shadow;
    std::vector<sf::RectangleShape> symbols;  // ���� ������ �ɺ���
    sf::Clock animationClock;                 // �ִϸ��̼� Ÿ�̸�
    bool isAnimating = false;                 // �ִϸ��̼� ����
    bool showInnerBoxColor = false;           // innerBox�� ���� ������ ǥ������ ����
    float scrollSpeed = 450.f;                // �ɺ� ��ũ�� �ӵ�
    int numSymbols = 4;                       // ���Կ� ǥ�õǴ� �ɺ� ����

    // ���� ������ ���� ������ �������� �Լ�
    sf::Color getCurrentColor() const {
        return innerBox.getFillColor(); 
    }

    SlotReel() {
        // ���� �� ��ü
        body.setSize(sf::Vector2f(500, 160));
        body.setFillColor(sf::Color(240, 193, 225));
        body.setOutlineColor(sf::Color(203, 157, 240));
        body.setOutlineThickness(5);
        body.setPosition(230, 185);

        // ���� �ڽ� (�ʱ� ���·� ��� ���̵��� ����)
        innerBox.setSize(sf::Vector2f(480, 140));
        innerBox.setFillColor(sf::Color(249, 245, 246));
        innerBox.setOutlineColor(sf::Color(203, 157, 240));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(240, 195);

        // ��ü �׸���
        shadow.setSize(sf::Vector2f(520, 175));
        shadow.setFillColor(sf::Color(217, 217, 217));
        shadow.setPosition(225, 170);

        for (int i = 0; i < numSymbols; ++i) {
            sf::RectangleShape symbol(sf::Vector2f(480, 40)); // �ɺ� ũ�� ����
            symbol.setPosition(240, 180 + i * 40);            // �ɺ� ��ġ ����
            symbol.setFillColor(sf::Color(249, 245, 246));
            symbols.push_back(symbol);
        }

    }

    // ���� ���� ���� �Լ�
    sf::Color getRandomColor() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        return sf::Color(dis(gen), dis(gen), dis(gen)); // ���� RGB �� ����
    }

    // �ɺ� �ִϸ��̼� ������Ʈ
    void update() {
        if (isAnimating) {
            // ���� �ð� �������� deltaTime ���
            float deltaTime = animationClock.getElapsedTime().asSeconds();

            // deltaTime�� 0���� �ʱ�ȭ�Ͽ� ���� ������Ʈ���� ��� ���� ����
            animationClock.restart();

            for (auto& symbol : symbols) {
                // �ɺ��� ���� ��ũ��
                symbol.move(0.f, -scrollSpeed * deltaTime);

                // ȭ�� ������ ������ �ٽ� �Ʒ��� ������
                if (symbol.getPosition().y + symbol.getSize().y < 236) {
                    symbol.setPosition(240, 180 + (numSymbols - 1) * 40); // �ٽ� �Ʒ��� ����
                    symbol.setFillColor(getRandomColor());               // �ɺ� ���� �������� ����
                }
            }
        }
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(shadow);
        window.draw(body);
        window.draw(innerBox);  // innerBox�� �ִϸ��̼� ���߿��� ��� ���̵���

        if (isAnimating) {
            // �ִϸ��̼� �߿��� �ɺ����� �׸���
            for (auto& symbol : symbols) {
                window.draw(symbol);
            }
        }
        else if (showInnerBoxColor) {
            // �ִϸ��̼��� ���� �Ŀ��� ���� ������ innerBox ���� ǥ�õǵ���
            window.draw(innerBox);  // innerBox�� ��� ���̰�
        }
    }

    // �ִϸ��̼� ����
    void startAnimation() {
        isAnimating = true;
        innerBox.setFillColor(sf::Color(249, 245, 246));
        showInnerBoxColor = false; // �ִϸ��̼� �߿��� innerBox ���� ���� ����
        animationClock.restart();
    }

    // �ִϸ��̼� ���� �� ���� ���� ��ȯ
    sf::Color stopAnimation() {
        isAnimating = false;
        sf::Color randomColor = getRandomColor(); // ���� ���� ����
        innerBox.setFillColor(randomColor);       // innerBox�� ���� ���� ����
        showInnerBoxColor = true;
        return randomColor;                      // ������ ���� ������ ��ȯ
    }
};

// ���Ըӽ� Ŭ����
class SlotMachine {
public:
    sf::RectangleShape body;
    sf::RectangleShape innerBox;
    sf::RectangleShape shadow1;
    sf::RectangleShape shadow2;
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangleShadow;
    std::vector<sf::RectangleShape> lines;
    std::vector<sf::RectangleShape> colorSections; // �� ������ ���� ���� ĭ�� (5��)
    int correctSectionIndex;  // �ùٸ� ������ ��ġ�� ������ �ε���  
    bool gameCleared = false;  // ���� Ŭ���� ����
    bool gameOver = false;     // ���� ���� ����

    SlotMachine() {
        // ���Ըӽ� ��ü
        body.setSize(sf::Vector2f(800, 300));
        body.setFillColor(sf::Color(191, 236, 255));
        body.setOutlineColor(sf::Color(205, 193, 255));
        body.setOutlineThickness(5);
        body.setPosition(80, 350);

        // ���� �ڽ�
        innerBox.setSize(sf::Vector2f(780, 280));
        innerBox.setFillColor(sf::Color(255, 251, 243));
        innerBox.setOutlineColor(sf::Color(205, 193, 255));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(90, 360);

        // ��ü �׸��� 1
        shadow1.setSize(sf::Vector2f(820, 320));
        shadow1.setFillColor(sf::Color(217, 217, 217));
        shadow1.setPosition(75, 335);

        // ��ü �׸��� 2
        shadow2.setSize(sf::Vector2f(330, 30));
        shadow2.setFillColor(sf::Color(217, 217, 217));
        shadow2.setPosition(565, 635);

        // �� ����
        const int lineCount = 4;
        const float lineWidth = 3; // �� �β�
        const float spacing = innerBox.getSize().x / (lineCount + 1); // �� ���� /  ���Ըӽ� �ڽ��� ���� ����

        for (int i = 0; i < lineCount; ++i) {
            sf::RectangleShape line(sf::Vector2f(lineWidth, innerBox.getSize().y)); // �ڽ� ���̿� ���߱�, ���Ʒ� ���� �߰�
            line.setFillColor(sf::Color(205, 193, 255));
            line.setPosition(
                innerBox.getPosition().x + spacing * (i + 1) - lineWidth / 2, // spacing: �� ������ ���� / - lineWidth / 2: ���� �β��� ����
                innerBox.getPosition().y // ���Ըӽ� �ڽ��� ���� ����
            );
            lines.push_back(line);
        }

        // ���� ä�� ���� ���� (�� ���� ����)
        srand(static_cast<unsigned int>(time(0))); // ���� �õ� ����

        for (int i = 0; i < lineCount + 1; ++i) {
            sf::RectangleShape section(sf::Vector2f(spacing, innerBox.getSize().y)); // �� ������ ũ�� ����
            section.setPosition(innerBox.getPosition().x + i * spacing, innerBox.getPosition().y);
            section.setFillColor(sf::Color(255, 251, 243));
            colorSections.push_back(section);
        }

        // ���簢��
        rectangle.setSize(sf::Vector2f(750, 30));
        rectangle.setFillColor(sf::Color(227, 246, 255));
        rectangle.setOutlineColor(sf::Color(205, 193, 255));
        rectangle.setOutlineThickness(3);
        rectangle.setPosition(104, 655);

        // ���簢�� �׸���
        rectangleShadow.setSize(sf::Vector2f(750, 33));
        rectangleShadow.setFillColor(sf::Color(217, 217, 217));
        rectangleShadow.setPosition(117, 655);

        // �ʱ� ���� ��ġ
        correctSectionIndex = -1; // �⺻���� �������� ���� ����
    }

    // ���� ���� ���� �Լ� (Ư�� ������ ����)
    sf::Color randomColorExcluding(const sf::Color& excludeColor) {
        sf::Color randomColor;
        do {
            int r = rand() % 256; // 0~255 ������ ���� ��
            int g = rand() % 256;
            int b = rand() % 256;
            randomColor = sf::Color(r, g, b);
        } while (randomColor == excludeColor); // ������ ����� �ߺ� �˻�
        return randomColor;
    }

    // ���� �����ϴ� �Լ� (������ ������ �� ȣ��)
    void update(const sf::Color& slotReelColor) {
        int targetIndex = rand() % colorSections.size(); // ���� ��ġ ����
        correctSectionIndex = targetIndex; // ���� ���� �ε��� ����

        for (int i = 0; i < colorSections.size(); ++i) {
            if (i == targetIndex) {
                // ���õ� ��ġ�� ���Ը� ���� ��ġ
                colorSections[i].setFillColor(slotReelColor);
            }
            else {
                // ������ �������� ���Ը� ������ ������ ���� ���� ��ġ
                colorSections[i].setFillColor(randomColorExcluding(slotReelColor));
            }
        }
    }

    // ������ Ŭ���� Ȯ�� �޼���
    bool isGameCleared() const {
        return gameCleared;
    }

    // ���� ���� Ȯ�� �޼���
    bool isGameOver() const {
        return gameOver;
    }

    // ���� ���� ���¸� �����ϴ� �޼���
    void setGameOver(bool over) {
        gameOver = over;
    }

    // ȭ��ǥ�� ���� ������ �����ߴ��� Ȯ��
    void checkArrowPosition(float arrowPositionX) {
        if (correctSectionIndex < 0 || correctSectionIndex >= colorSections.size()) {
            cout << "Error: correctSectionIndex is invalid!" << endl;
            return;
        } // ����

        // ���� ������ ���۰� �� X ��ǥ
        float sectionStartX = colorSections[correctSectionIndex].getPosition().x;
        float sectionEndX = sectionStartX + colorSections[correctSectionIndex].getSize().x;

        // ȭ��ǥ�� ���� ������ ��ġ�ϴ��� Ȯ��
        if (arrowPositionX >= sectionStartX && arrowPositionX <= sectionEndX) {
            cout << "Game Cleared! Colors match!" << endl;
            gameCleared = true; // ���� Ŭ���� ���·� ����
            gameOver = false;   // ���� ���� ���´� false
        }
        else {
            cout << "Game Over! Try again!" << endl;
            gameOver = true;    // ���� ���� ���·� ����
            gameCleared = false; // ���� Ŭ���� ���´� false
        }
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(shadow1);
        window.draw(shadow2);
        window.draw(body);
        window.draw(innerBox);

        // ���� ���� �׸���
        for (const auto& section : colorSections) {
            window.draw(section);
        }

        for (const auto& line : lines) {
            window.draw(line);
        }

        window.draw(rectangleShadow);
        window.draw(rectangle);
    }
};

// ȭ��ǥ Ŭ����
class Arrow {
public:
    sf::RectangleShape arrow;  // ȭ��ǥ ��ü
    sf::ConvexShape triangle;  // ȭ��ǥ ���� �ø� ����
    float speed = 500.0f;      // ȭ��ǥ �ӵ�
    float direction = 1.0f;    // ȭ��ǥ ���� (1�̸� ������, -1�̸� ����)
    sf::Vector2f position;     // ȭ��ǥ ��ġ

    // ȭ��ǥ�� X ��ġ ��ȯ
    float getPositionX() const {
        return arrow.getPosition().x;
    }

    Arrow() {
        // ȭ��ǥ ������ (�ܼ��� �簢�� ���)
        arrow.setSize(sf::Vector2f(20.0f, 25.0f)); // ũ�� ����
        arrow.setFillColor(sf::Color(205, 193, 255));
        arrow.setOrigin(arrow.getSize().x / 2, arrow.getSize().y / 2); // �߽��� �������� ȸ��
        arrow.setPosition(800, 628); // �ʱ� ��ġ ����

        // ���� ������ (ConvexShape ���)
        triangle.setPointCount(3);  // ����� �� ���� ������ ������
        triangle.setPoint(0, sf::Vector2f(0, -25));  // ���� (ũ�� Ű��)
        triangle.setPoint(1, sf::Vector2f(25, 15));  // ������ (ũ�� Ű��)
        triangle.setPoint(2, sf::Vector2f(-25, 15)); // ���� (ũ�� Ű��)
        triangle.setFillColor(sf::Color(205, 193, 255));
        triangle.setOrigin(0, 10); // ������ �������� �ٴڿ� ���߱�
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2);  // ȭ��ǥ ���� ��ġ
    }

    // ȭ��ǥ ������Ʈ �Լ�
    void update(float deltaTime) {

        // ȭ��ǥ ��ġ ������Ʈ
        position = arrow.getPosition();

        // ���ʰ� ���������� �ݺ��ϸ� �����̱�
        if (position.x <= 115 || position.x >= 845) {
            direction *= -1; // ���� ��ȯ
        }

        // ȭ��ǥ�� ��� �̵�
        position.x += speed * direction * deltaTime;  // deltaTime�� �̿��� �ӵ� ����
        arrow.setPosition(position);  // ���ο� ��ġ�� ����

        // ���� ȭ��ǥ�� ��ġ�� ���� ������Ʈ
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2); // ȭ��ǥ ���� ���� ��ġ ����
    }

    // ȭ��ǥ�� �ʱ� ��ġ�� �ǵ����� �Լ�
    void reset() {
        arrow.setPosition(800, 628); // �ʱ� ��ġ
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2); // ���� �ʱ� ��ġ�� �ǵ���
    }

    // ȭ�鿡 ȭ��ǥ�� ���� �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(arrow);    // ȭ��ǥ �׸���
        window.draw(triangle); // ���� �׸���
    }
};

// ���� Ŭ����
class Lever {
public:
    sf::RectangleShape leverBody, connection;
    sf::CircleShape handle;
    float angle = 0.0f; // ���� ȸ�� ���� ����
    float connectionWidth = 50.0f;
    sf::Clock animationClock;

    Lever() {
        // ���� ��ü
        leverBody.setSize(sf::Vector2f(20, 260));
        leverBody.setFillColor(sf::Color(240, 193, 225));
        leverBody.setOrigin(7.5f, 100);
        leverBody.setPosition(930, 455);

        // ���� ������
        handle.setRadius(23);
        handle.setFillColor(sf::Color(203, 157, 240));
        handle.setOrigin(20, 20);
        handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

        // ���� ���� ����
        connection.setSize(sf::Vector2f(connectionWidth, 20));
        connection.setFillColor(sf::Color(240, 193, 225));
        connection.setPosition(880, 595);
    }

    // ���� �ִϸ��̼�
    void update(bool isMouseOver) {
        float elapsedTime = animationClock.restart().asSeconds(); // ���� �ִϸ��̼��� ����Ǵ� ���� ����� �ð��� ������. (������ �󸶳� ȸ������ ���)
        if (isMouseOver) { // ���콺�� ���� ���� ���� ��
            // ������ ���� ����
            angle += 100.0f * elapsedTime; // ������ ȸ�� ����
            if (angle > 10.0f) angle = 10.0f; // ȸ�� ������ 10�� �̻����� Ŀ���� �ʵ��� ����
        }

        else {
            // ���� ���� ����
            angle -= 100.0f * elapsedTime;
            if (angle < 0.0f) angle = 0.0f;
        }

        // ���� ȸ�� �� ������ ��ġ ����
        leverBody.setRotation(angle);
        handle.setPosition(
            leverBody.getPosition().x + 100 * sin(angle * 3.14159 / 180), // lever.getPosition()�� ������ �߽� ��ġ ������
            leverBody.getPosition().y - 100 * cos(angle * 3.14159 / 180) // 3.14159 / 180 : ������ �������� ��ȯ�ϴ� �� (������ ����)
        );

        // ���� ������ ���� ���� ���� �ʺ� ����
        connectionWidth = 50.0f - (angle / 20.0f) * 30.0f; // ���� ������ ����Ͽ� �ʺ� �ٿ���
        connection.setSize(sf::Vector2f(connectionWidth, 20)); // ����
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(leverBody);
        window.draw(handle);
        window.draw(connection);
    }
};

// ��������
SlotReel slotReel;

// ���� Ŭ����
class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    SlotMachine slotMachine;
    Lever lever;
    Logo logo;
    Arrow arrow;
    int score = 0;
    sf::RectangleShape modalBackground;
    sf::Text scoreText;
    sf::Text modalText;
    sf::Font font;

public:
    // â ����
    Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close), logo("logo2.png") { // â �ִ�ȭ ��Ȱ��ȭ
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            throw runtime_error("Failed to load font");
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(900, 50);
        scoreText.setString("Score: 0");

        modalBackground.setSize(sf::Vector2f(400, 200));
        modalBackground.setFillColor(sf::Color(0, 0, 0, 200));
        modalBackground.setPosition(312, 284);

        modalText.setFont(font);
        modalText.setCharacterSize(24);
        modalText.setFillColor(sf::Color::White);
        modalText.setPosition(362, 334);
    }

    void run() {
        bool isMouseOverLever = false; 
        bool isFirstPull = true; // ó�� ���� ���
        bool isFirstReelAnimation = true; //ó�� ���Ը� �ִϸ��̼� �۵�
        bool showModal = false; // ��� ����

        // ���� ����
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // ���콺 �̵� �� ���� ���� �ִ��� Ȯ��
                if (event.type == sf::Event::MouseMoved) { // ���콺 ���� ��ġ Ȯ��
                    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y); // mousePos : ���콺�� �̵��� ���� ���� x, y ��ǥ
                    isMouseOverLever = lever.handle.getGlobalBounds().contains(mousePos); // contains(mousePos) : mousePos�� leverHandle�� ���� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
                }  // true or false

                // ���콺 ��ư�� ���� �� ���� ���� ���·� ��ȯ
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  ���콺 ��ư�� �������� �� & ���� ���콺 ��ư Ŭ��
                    if (isMouseOverLever && !slotMachine.isGameOver()) {
                        isMouseOverLever = false;

                        if (slotReel.isAnimating) {
                            sf::Color slotReelColor = slotReel.stopAnimation(); // �ִϸ��̼� ���� �� ���Ը� ���� ��ȯ
                            slotMachine.update(slotReelColor); // ��ȯ�� �������� ���Ըӽ� ������Ʈ
                            arrow.reset(); // ȭ��ǥ �ʱ�ȭ
                        }

                        else {
                            // ������ ó�� ��� ��
                            if (isFirstPull) {
                                isFirstPull = false;
                                arrow.reset(); // ȭ��ǥ �ʱ�ȭ
                            }
                            slotReel.startAnimation();  // ���� �� �ִϸ��̼� ����

                            // ù ��° ���� �� �ִϸ��̼� ���Ŀ��� checkArrowPosition �޼��� ����
                            if (!isFirstReelAnimation) {
                                slotMachine.checkArrowPosition(arrow.getPositionX()); // ȭ��ǥ ��ġ ���� ���� Ȯ�� �޼���
                                if (slotMachine.isGameCleared()) {
                                    score += 50; // ���� Ŭ���� �� ���� �߰�
                                    scoreText.setString("Score: " + to_string(score));
                                }
                                else if (slotMachine.isGameOver()) {
                                    showModal = true; // ���� ���� �� ��� ǥ��
                                    modalText.setString("Game Over!\nPress R to Retry\nH to Quit");
                                }
                            }

                            isFirstReelAnimation = false;  // ù ��° �ִϸ��̼��� �������� ǥ��
                        }
                    }
                }

                if (showModal) {
                    if (event.type == sf::Event::KeyPressed) {
                        slotMachine.setGameOver(false); // ���� ���� ���¸� false�� ����
                        if (event.key.code == sf::Keyboard::R) {
                            showModal = false;
                            arrow.reset();
                            score = 0;
                            scoreText.setString("Score: 0");                           
                        }
                        else if (event.key.code == sf::Keyboard::H) {
                            window.close();
                        }
                    }
                }
            }

            // DeltaTime ���
            float deltaTime = clock.restart().asSeconds();

            // ������ ó�� ��� �� && ���Ը� �ִϸ��̼��� �������� ���� ��
            if (!isFirstPull && !slotReel.isAnimating) {
                arrow.update(deltaTime); // ȭ��ǥ ������Ʈ
            }

            //������Ʈ
            lever.update(isMouseOverLever);
            slotReel.update();

            window.clear(sf::Color::White);
            logo.draw(window);
            slotMachine.draw(window);
            slotReel.draw(window);
            lever.draw(window);
            arrow.draw(window);
            window.draw(scoreText);

            if (showModal) {
                window.draw(modalBackground);
                window.draw(modalText);
            }

            window.display();
        }
    }
};

int main() {
    try {
        Game game;
        game.run();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    sf::Clock clock;
    float deltaTime = clock.restart().asSeconds();

    // Delta Time ���
    cout << "Delta Time: " << deltaTime << endl;
    return 0;
};