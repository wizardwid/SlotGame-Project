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
    vector<sf::RectangleShape> lines;
    vector<sf::RectangleShape> colorSections; // �� ������ ���� ���� ĭ�� (5��)
    vector<sf::Color> generatedColors; // ������ ������ ����
    float colorVariance = 500.0f;  // ���� ���� ����
    int correctSectionIndex = -1;  // �ùٸ� ������ ��ġ�� ������ �ε���  
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
    }

    // ���� ������ �ʱ�ȭ�ϴ� setter �Լ�
    void setColorVariance(float variance) {
        colorVariance = variance;
    }

    // ���� ���� ����
    sf::Color randomColorExcluding(const sf::Color& excludeColor) {
        sf::Color randomColor;
        bool colorExists; // �����ϴ� �÷����� ����
        do { // ������ ������ excludeColor�� ������ ��츦 ����
            // ���� ������ colorVariance ������ �°� �����Ͽ� ���� ���� ����
            int r = rand() % (int)colorVariance + excludeColor.r - (int)(colorVariance / 2); // excludeColor�� �ش� ���� ���� �������� ����
            int g = rand() % (int)colorVariance + excludeColor.g - (int)(colorVariance / 2); // 0���� colorVariance - 1������ ���� ��ȯ
            int b = rand() % (int)colorVariance + excludeColor.b - (int)(colorVariance / 2); // excludeColor rgb�� �߽����� ���� ��(colorVariance / 2) ���� ������ ����

            // ���� ���� ����
            r = max(0, min(255, r)); // RGB ���� 0���� 255 ���̷� ����
            g = max(0, min(255, g));
            b = max(0, min(255, b));

            randomColor = sf::Color(r, g, b);
            colorExists = find(generatedColors.begin(), generatedColors.end(), randomColor) != generatedColors.end(); // �̹� ������ �������� �˻�
            // find �Լ� : ã���� �ϴ� ��Ұ� �����̳� �ȿ� �����ϴ��� ���θ� Ȯ�� -> �ش� ��Ҹ� ����Ű�� iterator ��ȯ
        } while (randomColor == excludeColor || colorExists); // ������ ����� �ߺ� �˻�(randomColor�� excludeColor�� �������� Ȯ��)
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
        // ���� ���࿡ ���� ���� ���� ���
        if (colorVariance > 100.0f) {
            colorVariance -= 10.0f; // �ּҰ� 100���� ���
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
    float speed = 400.0f;      // ȭ��ǥ �ӵ�
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

    // ���ǵ� ����
    void increaseSpeed() {
        speed += 30.0f; // ȭ��ǥ �ӵ� ������Ŵ (Ŭ����� �ӵ� ����)
    }

    // ���ǵ� �ʱ�ȭ
    void resetSpeed() {
        speed = 400.0f;  // �⺻ �ӵ��� �ʱ�ȭ
    }

    // ȭ��ǥ�� �ʱ� ��ġ�� �ǵ����� �Լ�
    void reset() {
        arrow.setPosition(800, 628); // �ʱ� ��ġ
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2); // ���� �ʱ� ��ġ�� �ǵ���
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(arrow);    // ȭ��ǥ �׸���
        window.draw(triangle); // ���� �׸���
    }
};

class Text {
public:
    sf::Text scoreText;   // ���ھ� �ؽ�Ʈ
    sf::Text timerText;  // ���� �ð� �ؽ�Ʈ
    sf::Font font;
    sf::Text comboText; // �޺� �ؽ�Ʈ
    bool showComboText = false; // �޺� �ؽ�Ʈ ǥ�� ����
    sf::Clock comboClock;   // �޺� �ؽ�Ʈ ǥ�� �ð� ����

    // �Ű����� ������ (��Ʈ ��θ� ���޹��� �� �ֵ��� ��)
    Text(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            throw std::runtime_error("Failed to load font");
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(900, 50); // ȭ�鿡�� ������ ��ġ
        scoreText.setString("Score: 0");

        // ���� �ð� �ؽ�Ʈ ����
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::Black);
        timerText.setPosition(900, 80);  // �ð� ǥ�� ��ġ (���� ��)
        timerText.setString("Time: 0s");

        // �޺� �ؽ�Ʈ �ʱ�ȭ
        comboText.setFont(font); // font�� ���� ��Ʈ ��ü
        comboText.setString("Combo Bonus!");
        comboText.setCharacterSize(50);
        comboText.setFillColor(sf::Color::Yellow);
        comboText.setStyle(sf::Text::Bold);
        comboText.setPosition(400, 300); // ȭ�� �߾ӿ� ǥ��
    }

    // ���� ������Ʈ
    void updateScore(int score) {
        scoreText.setString("Score: " + to_string(score));
    }

    // ���� �ð� ������Ʈ
    void updateTime(float time) {
        timerText.setString("Time: " + to_string(static_cast<int>(time)) + "s");
    }

    // �޺� �ؽ�Ʈ ǥ��
    void showCombo() {
        showComboText = true;
        comboClock.restart(); // ǥ�� �ð� �ʱ�ȭ
    }

    // �޺� �ؽ�Ʈ ������Ʈ (�ð� ������� ���� ó��)
    void updateCombo() {
        if (showComboText && comboClock.getElapsedTime().asSeconds() > 0.5f) {
            showComboText = false;
        }
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(scoreText);
        window.draw(timerText);
        if (showComboText) {
            window.draw(comboText);
        }
    }
};

class Modal {
public:
    sf::RectangleShape modalBackground;
    sf::Text modalText;
    sf::Font font;

    // �Ű����� ������ (��Ʈ ��θ� ���޹��� �� �ֵ��� ��)
    Modal(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {  // ��ȣ ����
            throw std::runtime_error("Failed to load font");
        }

        modalBackground.setSize(sf::Vector2f(400, 200)); // ����� ũ��
        modalBackground.setFillColor(sf::Color(0, 0, 0, 200)); // ��� �� (������)
        modalBackground.setPosition(312, 284); // ����� ��ġ

        modalText.setFont(font);
        modalText.setCharacterSize(24);
        modalText.setFillColor(sf::Color::White);
        modalText.setPosition(362, 334); // �ؽ�Ʈ ��ġ
    }

    // ��� �ؽ�Ʈ ����
    void setModalText(const std::string& text) {
        modalText.setString(text);
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(modalBackground);
        window.draw(modalText);
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
    Text text;
    Modal modal;
    int nScore=0;
    int comboCount = 0; // ���� Ŭ���� ī��Ʈ

public:
    // â ����
    Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close),
        logo("logo2.png"), text("C:\\Windows\\Fonts\\arial.ttf"),
        modal("C:\\Windows\\Fonts\\arial.ttf") {} // â �ִ�ȭ ��Ȱ��ȭ

    void run() {
        bool isMouseOverLever = false;
        bool isFirstPull = true; // ó�� ���� ���
        bool isFirstReelAnimation = true; //ó�� ���Ը� �ִϸ��̼� �۵�
        bool showModal = false; // ��� ����
        float elapsedTime = 0.0f; // ȭ��ǥ�� �����̱� ������ �� ��� �ð�

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
                            elapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
                            slotReel.startAnimation();  // ���� �� �ִϸ��̼� ����

                            // ù ��° ���� �� �ִϸ��̼� ���Ŀ��� checkArrowPosition �޼��� ����
                            if (!isFirstReelAnimation) {
                                slotMachine.checkArrowPosition(arrow.getPositionX()); // ȭ��ǥ ��ġ ���� ���� Ȯ�� �޼���
                                if (slotMachine.isGameCleared()) {
                                    comboCount++; // Ŭ���� ī��Ʈ ���� 
                                    // 3�� ����� �� ���ʽ� ���� �߰�
                                    if (comboCount % 3 == 0) {
                                        nScore += 100;         // �޺� ���ʽ� ���� �߰�
                                        text.showCombo();      // �޺� �ؽ�Ʈ ǥ��
                                    }
                                    else nScore += 50; // ���� Ŭ���� �� ���� �߰�
                                    text.updateCombo();
                                    text.updateScore(nScore); // ���� ������Ʈ
                                    arrow.increaseSpeed();
                                }
                                else if (slotMachine.isGameOver()) {
                                    showModal = true; // ���� ���� �� ��� ǥ��
                                    modal.setModalText("Game Over!\nPress R to Retry\nH to Quit");
                                }
                            }
                            isFirstReelAnimation = false;  // ù ��° �ִϸ��̼��� �������� ǥ��
                        }
                    }
                }
                // ��� ǥ��
                if (showModal) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::R) {
                            slotMachine.setGameOver(false); // ���� ���� ���¸� false�� ����
                            showModal = false;
                            elapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
                            text.updateTime(elapsedTime); // ���� �ð� ������Ʈ
                            arrow.reset();
                            nScore = 0;  // ���� �ʱ�ȭ
                            text.updateScore(nScore); // ���� ������Ʈ
                            slotMachine.setColorVariance(500.0f); // ���� ���� �ʱ�ȭ
                            comboCount = 0;
                            arrow.resetSpeed(); // ���ǵ� �ʱ�ȭ
                            slotReel.startAnimation();
                        }
                        else if (event.key.code == sf::Keyboard::H) {
                            window.close(); // 'H' ������ �� â �ݱ�
                        }
                    }
                }
            }

            // DeltaTime ���
            float deltaTime = clock.restart().asSeconds();

            // ������ ó�� ��� �� && ���Ը� �ִϸ��̼��� �������� ���� �� && ����� �Ⱥ��� ��
            if (!isFirstPull && !slotReel.isAnimating && !showModal) {
                arrow.update(deltaTime); // ȭ��ǥ ������Ʈ

                // ��� �ð� ����
                elapsedTime += deltaTime;
                text.updateTime(elapsedTime); // ���� �ð� ������Ʈ

                // 3�ʰ� ������ ���� ����
                if (elapsedTime >= 4.0f) {
                    slotMachine.setGameOver(true);
                    modal.setModalText("Game Over!\nPress R to Retry\nH to Quit");
                    showModal = true;
                }
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
            text.draw(window);
            if (showModal) {
                modal.draw(window);
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