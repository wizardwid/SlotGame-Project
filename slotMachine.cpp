#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
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
    }

    // ���� ���� ���� �Լ�
    sf::Color randomColor() {
        int r = rand() % 256; // 0~255 ������ ���� ��
        int g = rand() % 256;
        int b = rand() % 256;
        return sf::Color(r, g, b);
    }

    // ���� �����ϴ� �Լ� (������ ������ �� ȣ��)
    void update(const sf::Color& slotReelColor) {
        int targetIndex = rand() % colorSections.size(); // ���� ��ġ ����

        for (int i = 0; i < colorSections.size(); ++i) {
            if (i == targetIndex) {
                // ���õ� ��ġ�� ���Ը� ���� ��ġ
                colorSections[i].setFillColor(slotReelColor);
            }
            else {
                // ������ �������� ���� ���� ��ġ
                colorSections[i].setFillColor(randomColor());
            }
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
    SlotMachine slotMachine;
    Lever lever;
    Logo logo;

public:
    // â ����
    Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close), logo("logo2.png") {} // â �ִ�ȭ ��Ȱ��ȭ

    void run() {
        bool isMouseOverLever = false;

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

                // ���콺 ��ư�� ���� �� ���� ���·� ��ȯ
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  ���콺 ��ư�� �������� �� & ���� ���콺 ��ư Ŭ��
                    if (isMouseOverLever) {
                        isMouseOverLever = false;

                        if (slotReel.isAnimating) {
                            slotReel.stopAnimation(); // ���Ը� �ִϸ��̼� ���� 
                            sf::Color slotReelColor = slotReel.stopAnimation(); // �ִϸ��̼� ���� �� ���Ը� ���� ��ȯ
                            slotMachine.update(slotReelColor); // ��ȯ�� �������� ���Ըӽ� ������Ʈ
                        }

                        else {
                            slotReel.startAnimation(); // ���Ը� �ִϸ��̼� ����
                        }
                    }
                }
            }

            // ������Ʈ
            lever.update(isMouseOverLever);
            slotReel.update();

            // ȭ�鿡 �׸���
            window.clear(sf::Color::White);
            logo.draw(window);
            slotMachine.draw(window);
            slotReel.draw(window);
            lever.draw(window);
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
    return 0;
}