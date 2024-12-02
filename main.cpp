#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp> 
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <random>
#include <unordered_map>
#include <string>
#include <cstdlib>  // rand() �Լ� ����� ���� ���
#include <ctime>    // srand() �Լ� ����� ���� ���

using namespace std;

// ùȭ�� �̹��� Ŭ����
class fLogo {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    fLogo(const string& imagePath) {
        if (!texture.loadFromFile(imagePath)) {
            throw runtime_error("Failed to load image: " + imagePath);
        }
        sprite.setTexture(texture);
        sprite.setPosition(-440, -440);
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

// ùȭ�� ���Ըӽ� Ŭ����
class fSlotMachine {
private:
    sf::RectangleShape bodyFront, bodyBack, faceBox, frontRectangle, frontRectangleShadow1;
    sf::CircleShape leftEye, leftPupil, rightEye, rightPupil, nose;
    vector<sf::CircleShape> bigCircles;
    vector<sf::CircleShape> smallHexagons;
    vector<sf::RectangleShape> rectangleButtons;
    vector<sf::Color> colors;

public:
    fSlotMachine() {
        // ���Ըӽ� ��ü
        bodyFront.setSize(sf::Vector2f(300, 400));
        bodyFront.setFillColor(sf::Color(209, 233, 246));
        bodyFront.setOutlineColor(sf::Color(205, 193, 255));
        bodyFront.setOutlineThickness(5);
        bodyFront.setPosition(362, 270);

        // �޸� (�׸��� ȿ��)
        bodyBack.setSize(sf::Vector2f(320, 420));
        bodyBack.setFillColor(sf::Color(217, 217, 217));
        bodyBack.setPosition(357, 255);

        // ���� �簢��
        frontRectangle.setSize(sf::Vector2f(290, 100));
        frontRectangle.setFillColor(sf::Color(209, 233, 246));
        frontRectangle.setOutlineColor(sf::Color(205, 193, 255));
        frontRectangle.setOutlineThickness(3);
        frontRectangle.setPosition(367, 435);

        // ���� �簢�� �׸���
        frontRectangleShadow1.setSize(sf::Vector2f(295, 130));
        frontRectangleShadow1.setFillColor(sf::Color(205, 193, 255));
        frontRectangleShadow1.setPosition(364, 538);

        // ���Ըӽ� �� �ڽ�
        faceBox.setSize(sf::Vector2f(230, 100));
        faceBox.setFillColor(sf::Color(248, 232, 238));
        faceBox.setOutlineColor(sf::Color(205, 193, 255));
        faceBox.setOutlineThickness(3);
        faceBox.setPosition(397, 300);

        // ��
        // �� 
        leftEye.setRadius(12);
        leftEye.setFillColor(sf::Color::White);
        leftEye.setPosition(430, 324);

        leftPupil.setRadius(5);
        leftPupil.setFillColor(sf::Color::Black);
        leftPupil.setPosition(438, 330);

        rightEye.setRadius(12);
        rightEye.setFillColor(sf::Color::White);
        rightEye.setPosition(560, 324);

        rightPupil.setRadius(5);
        rightPupil.setFillColor(sf::Color::Black);
        rightPupil.setPosition(568, 330);

        // ��
        nose.setRadius(4);
        nose.setFillColor(sf::Color(240, 168, 208));
        nose.setPosition(505, 340);

        // ū ���׶��
        vector<sf::Color> bigCircleColors = {
            sf::Color(147, 223, 255),
            sf::Color(205, 193, 255),
            sf::Color(255, 246, 227),
            sf::Color(255, 204, 234)
        };
        for (int i = 0; i < 4; ++i) {
            sf::CircleShape circle(20);
            circle.setFillColor(bigCircleColors[i]);
            circle.setPosition(402 + i * 60, 380);
            bigCircles.push_back(circle);
        }

        // ���� ������
        for (int i = 0; i < 4; ++i) {
            sf::CircleShape hexagon(7, 6);
            hexagon.setFillColor(sf::Color(218, 172, 255));
            hexagon.setPosition(460 + i * 30, 450);
            smallHexagons.push_back(hexagon);
        }

        // ��ư
        // ��ư ���� �迭
        colors = {
            sf::Color(147, 223, 255),
            sf::Color(205, 193, 255),
            sf::Color(255, 246, 227),
            sf::Color(255, 204, 234)
        };

        // �� ��ư�� ������ ����
        for (int i = 0; i < 4; ++i) {
            sf::RectangleShape button(sf::Vector2f(40, 40));
            button.setPosition(402 + i * 60, 480);
            button.setFillColor(colors[i]);
            rectangleButtons.push_back(button); // ��ư�� ���Ϳ� �߰�
        }
    }

    // ���� ������Ʈ �Լ�
    void updateColors() {
        // colors �迭�� ù ��° ���� ���������� �̵���Ŵ (�������� ȸ��)
        rotate(colors.rbegin(), colors.rbegin() + 1, colors.rend());

        // �� ��ư�� ������ ���� ������Ʈ
        for (int i = 0; i < 4; ++i) {
            rectangleButtons[i].setFillColor(colors[i]);
        }
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(bodyBack);
        window.draw(bodyFront);
        window.draw(frontRectangle);
        window.draw(frontRectangleShadow1);
        for (const auto& circle : bigCircles) window.draw(circle);
        window.draw(faceBox);
        window.draw(leftEye);
        window.draw(leftPupil);
        window.draw(rightEye);
        window.draw(rightPupil);
        window.draw(nose);
        for (const auto& hexagon : smallHexagons) window.draw(hexagon);
        for (const auto& button : rectangleButtons) window.draw(button);
    }
};

// ùȭ�� ���� Ŭ����
class fLever {
public:
    sf::RectangleShape leverBody, connection;
    sf::CircleShape handle;
    float leverAngle = 0.0f;    // ���� ȸ�� ���� ����
    float connectionWidth = 50.0f; // �ʱ� �ʺ�
    sf::Clock animationClock;

public:
    fLever() {
        leverBody.setSize(sf::Vector2f(20, 195));
        leverBody.setFillColor(sf::Color(240, 193, 225));
        leverBody.setOrigin(7.5f, 100);
        leverBody.setPosition(703, 373);

        handle.setRadius(18);
        handle.setFillColor(sf::Color(203, 157, 240));
        handle.setOrigin(16, 16);
        handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

        connection.setSize(sf::Vector2f(50, 20));
        connection.setFillColor(sf::Color(240, 193, 225));
        connection.setPosition(665, 448);
    }

    // ���� �ִϸ��̼�
    void update(bool isMouseOver) {
        float elapsedTime = animationClock.restart().asSeconds(); // ���� �ִϸ��̼��� ����Ǵ� ���� ����� �ð��� ������. (������ �󸶳� ȸ������ ���)
        if (isMouseOver) { // ���콺�� ���� ���� ���� ��
            // ������ ���� ����
            leverAngle += 100.0f * elapsedTime; // ������ ȸ�� ����
            if (leverAngle > 10.0f) leverAngle = 10.0f; // ȸ�� ������ 10�� �̻����� Ŀ���� �ʵ��� ����
        }

        else {
            // ���� ���� ����
            leverAngle -= 100.0f * elapsedTime;
            if (leverAngle < 0.0f) leverAngle = 0.0f;
        }

        // ���� ȸ�� �� ������ ��ġ ����
        leverBody.setRotation(leverAngle);
        handle.setPosition(
            leverBody.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180), // lever.getPosition()�� ������ �߽� ��ġ ������
            leverBody.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180) // 3.14159 / 180 : ������ �������� ��ȯ�ϴ� �� (������ ����)
        );

        // ���� ������ ���� ���� ���� �ʺ� ����
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; // ���� ������ ����Ͽ� �ʺ� �ٿ���
        connection.setSize(sf::Vector2f(connectionWidth, 20)); // ����
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(leverBody);
        window.draw(handle);
        window.draw(connection);
    }
};

// �̹��� Ŭ����
class Logo {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Logo(const string& imagePath) {
        if (!texture.loadFromFile(imagePath)) {
            throw runtime_error("Failed to load image: " + imagePath);
        }

        sprite.setTexture(texture);
        sprite.setPosition(-480, -455);
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
    vector<sf::RectangleShape> symbols;  // ���� ������ �ɺ���
    sf::Clock animationClock;            // �ִϸ��̼� Ÿ�̸�
    bool isAnimating = false;            // �ִϸ��̼� ����
    bool showInnerBoxColor = false;      // innerBox�� ���� ������ ǥ������ ����
    float scrollSpeed = 450.f;           // �ɺ� ��ũ�� �ӵ�
    int numSymbols = 4;                  // ���Կ� ǥ�õǴ� �ɺ� ����

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
        body.setPosition(231, 185);

        // ���� �ڽ� (�ʱ� ���·� ��� ���̵��� ����)
        innerBox.setSize(sf::Vector2f(480, 140));
        innerBox.setFillColor(sf::Color(249, 245, 246));
        innerBox.setOutlineColor(sf::Color(203, 157, 240));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(241, 195);

        // ��ü �׸���
        shadow.setSize(sf::Vector2f(520, 175));
        shadow.setFillColor(sf::Color(217, 217, 217));
        shadow.setPosition(226, 170);

        for (int i = 0; i < numSymbols; ++i) {
            sf::RectangleShape symbol(sf::Vector2f(480, 40)); // �ɺ� ũ�� ����
            symbol.setPosition(240, 180 + i * 40);            // �ɺ� ��ġ ����
            symbol.setFillColor(sf::Color(249, 245, 246));
            symbols.push_back(symbol);
        }

    }

    // ���� ���� ���� �Լ�
    sf::Color getRandomColor() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, 255);

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
                    symbol.setPosition(241, 180 + (numSymbols - 1) * 40); // �ٽ� �Ʒ��� ����
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
    vector<sf::RectangleShape> lines;         // ������ ������ ��
    vector<sf::RectangleShape> colorSections; // �� ������ ���� ���� ĭ�� (5��)
    vector<sf::Color> generatedColors;        // ������ ������ ����
    float colorVariance = 500.0f;             // ���� ���� ����
    int correctSectionIndex = -1;             // �ùٸ� ������ ��ġ�� ������ �ε���  
    bool gameCleared = false;                 // ���� Ŭ���� ����
    bool gameOver = false;                    // ���� ���� ����

    SlotMachine() {
        // ���Ըӽ� ��ü
        body.setSize(sf::Vector2f(800, 300));
        body.setFillColor(sf::Color(191, 236, 255));
        body.setOutlineColor(sf::Color(205, 193, 255));
        body.setOutlineThickness(5);
        body.setPosition(81, 350);

        // ���� �ڽ�
        innerBox.setSize(sf::Vector2f(780, 280));
        innerBox.setFillColor(sf::Color(255, 251, 243));
        innerBox.setOutlineColor(sf::Color(205, 193, 255));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(91, 360);

        // ��ü �׸��� 1
        shadow1.setSize(sf::Vector2f(820, 320));
        shadow1.setFillColor(sf::Color(217, 217, 217));
        shadow1.setPosition(76, 335);

        // ��ü �׸��� 2
        shadow2.setSize(sf::Vector2f(330, 30));
        shadow2.setFillColor(sf::Color(217, 217, 217));
        shadow2.setPosition(566, 635);

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
        rectangle.setPosition(105, 655);

        // ���簢�� �׸���
        rectangleShadow.setSize(sf::Vector2f(750, 33));
        rectangleShadow.setFillColor(sf::Color(217, 217, 217));
        rectangleShadow.setPosition(118, 655);
    }

    // ���� ���� �ʱ�ȭ
    void resetColorVariance() {
        colorVariance = 500.0f;
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
    sf::RectangleShape arrow;
    sf::ConvexShape triangle;
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
        arrow.setPosition(801, 628); // �ʱ� ��ġ ����

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
        if (position.x <= 116 || position.x >= 846) {
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
        window.draw(arrow);
        window.draw(triangle);
    }
};

// �ְ� ��� Ŭ����
class HighScore {
public:
    int highScore;

    HighScore() : highScore(0) {
        loadHighScore(); // ��ü�� ������ ��, ���Ͽ��� �ְ� ������ �ҷ���
    }

    // �ְ� ���� �ҷ�����
    void loadHighScore() {
        ifstream file("C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\highScore.txt"); // ���� ���, �б� ���� ��Ʈ��(ifstream) ����
        if (file.is_open()) { // ���� ����
            file >> highScore; // ���Ͽ��� �ְ� ������ �о ������(highScore) ����
            file.close(); // ������ ����
        }
    }

    // �ְ� ���� ����
    void saveHighScore() {
        ofstream file("C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\highScore.txt"); // ���� ���, ���� ���� ��Ʈ��(ofStream) ����
        if (file.is_open()) { // ���� ����
            file << highScore; // highScore ���� ���Ͽ� ����
            file.close(); // ���� ����
        }
    }

    // �ְ� ���� ���
    int getHighScore() {
        return highScore; // ���� �ְ� ������ ��ȯ
    }

    // �ְ� ���� ����
    void update(int score) {
        if (score > highScore) { // �� ������ ���� �ְ� �������� ũ�� ����
            highScore = score;
            saveHighScore(); // ���ο� �ְ����� ����
        }
    }
};

// �ؽ�Ʈ Ŭ����
class Text {
public:
    sf::Text scoreText;
    sf::Text timerText;
    sf::Text comboText;
    sf::Text highScoreText;
    sf::Font font;
    bool showComboText = false; // �޺� �ؽ�Ʈ ǥ�� ����
    sf::Clock comboClock;       // �޺� �ؽ�Ʈ ǥ�� �ð� 

    // �Ű����� ������ (��Ʈ ��θ� ���޹��� �� �ֵ��� ��)
    Text(const string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            throw runtime_error("Failed to load font");
        }

        // �ְ� ���� �ؽ�Ʈ 
        highScoreText.setFont(font);
        highScoreText.setCharacterSize(24);
        highScoreText.setFillColor(sf::Color(203, 157, 240));
        highScoreText.setStyle(sf::Text::Bold);
        highScoreText.setPosition(801, 35);
        highScoreText.setString("High Score: 0");

        // ���� �ؽ�Ʈ
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color(149, 189, 255));
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setPosition(801, 65);
        scoreText.setString("Score: 0");

        // ���� �ð� �ؽ�Ʈ 
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color(250, 218, 122));
        timerText.setStyle(sf::Text::Bold);
        timerText.setPosition(801, 95);
        timerText.setString("Time: 0s");

        // �޺� �ؽ�Ʈ
        comboText.setFont(font);
        comboText.setString("Combo Bonus!");
        comboText.setCharacterSize(50);
        comboText.setFillColor(sf::Color(255, 241, 0));
        comboText.setStyle(sf::Text::Bold);
        comboText.setPosition(311, 230);
    }

    // ���� ������Ʈ
    void updateScore(int score) {
        scoreText.setString("Score: " + to_string(score));
    }

    // ���� �ð� ������Ʈ
    void updateTime(float time) {
        timerText.setString("Time: " + to_string(static_cast<int>(time)) + "s"); // static_cast : float�� int�� ��ȯ
    }

    // �ְ� ���� ����
    void updateHScore(int highScore) {
        highScoreText.setString("High Score: " + to_string(highScore));
    }

    // �޺� �ؽ�Ʈ ǥ��
    void showCombo() {
        showComboText = true;
        comboClock.restart(); // ǥ�� �ð� �ʱ�ȭ
    }

    // �޺� �ؽ�Ʈ ������Ʈ (�ð� ������� ���� ó��)
    void updateCombo() {
        if (showComboText && comboClock.getElapsedTime().asSeconds() > 0.1f) {
            showComboText = false;
        }
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(highScoreText);
        window.draw(scoreText);
        window.draw(timerText);
        if (showComboText) {
            window.draw(comboText);
        }
    }
};

// ��� Ŭ����
class Modal {
public:
    sf::RectangleShape modalBackground;
    sf::Text modalText;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Font font;

    // �Ű����� ������ (��Ʈ ��θ� ���޹��� �� �ֵ��� ��)
    Modal(const string& fontPath) {
        if (!font.loadFromFile(fontPath)) {  // ��ȣ ����
            throw runtime_error("Failed to load font");
        }

        modalBackground.setSize(sf::Vector2f(400, 200)); // ����� ũ��
        modalBackground.setFillColor(sf::Color(79, 23, 135, 200)); // ��� �� (������)
        modalBackground.setPosition(313, 294); // ����� ��ġ

        modalText.setFont(font);
        modalText.setCharacterSize(24);
        modalText.setFillColor(sf::Color::White);
        modalText.setPosition(358, 393);

        // ������ �ؽ�Ʈ
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(358, 333);

        // �ְ� ���� �ؽ�Ʈ 
        highScoreText.setFont(font);
        highScoreText.setCharacterSize(24);
        highScoreText.setFillColor(sf::Color::White);
        highScoreText.setPosition(358, 363);
    }

    // ��� �ؽ�Ʈ ����
    void setModalText(const string& text) {
        modalText.setString(text);
    }

    // ���� ���� ����
    void setCurrentScore(int score) {
        scoreText.setString("Score: " + to_string(score));
    }

    // �ְ� ���� ����
    void setHighScore(int highScore) {
        highScoreText.setString("High Score: " + to_string(highScore));
    }

    // ȭ�鿡 �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(modalBackground);
        window.draw(modalText);
        window.draw(scoreText);
        window.draw(highScoreText);
    }
};

// ���� Ŭ����
class Sound {
private:
    // ���� ���ۿ� ���� ��ü�� �����ϴ� �ؽø�
    unordered_map<string, sf::SoundBuffer> soundBuffers; // ���� ������ �޸𸮿� �ε��ϴ� ��ü
    unordered_map<string, sf::Sound> sounds; // sf::SoundBuffer�� �ε�� ���带 ������ ����ϴ� ��ü
    sf::Music backgroundMusic; // ��� ���ǿ� �ݺ� ���

public:
    // ���� �ε� �޼���
    bool loadSound(const string& name, const string& filePath) { // ��� ���� ������ �ε�
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filePath)) {
            cerr << "Failed to load sound: " << filePath << endl;
            return false;
        }
        soundBuffers[name] = buffer; // �ε�� ���� ���۸� soundBuffers �ؽøʿ� name Ű�� ���� / buffer : ���� ������ �޸𸮿� �ε��� ����
        sounds[name].setBuffer(soundBuffers[name]); //  sf::Sound ��ü�� �ش� ���� ���۸� ����
        return true;
    }

    // ��� ���� �ε�
    bool loadBackgroundMusic(const string& filePath) {
        if (!backgroundMusic.openFromFile(filePath)) {
            std::cerr << "Failed to load background music: " << filePath << endl;
            return false;
        }
        return true;
    }

    // ���� ���
    void playSound(const string& name) { // �־��� �̸��� ���带 ���
        sounds[name].play();
    }

    // ��� ���� ���
    void playBackgroundMusic(bool loop = true) { // ��� ������ �����
        backgroundMusic.setLoop(loop);
        backgroundMusic.play();
    }

    // ���� ����
    void stopSound(const std::string& name) { // �־��� �̸��� ���带 ����
        sounds[name].stop();
    }

    // ��� ���� ����
    void stopBackgroundMusic() {
        backgroundMusic.stop();
    }

    // ���� ����
    void setSoundVolume(const string& name, float volume) {
        sounds[name].setVolume(volume); // 0~100
    }

    void setBackgroundMusicVolume(float volume) {
        backgroundMusic.setVolume(volume); // 0~100
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
        leverBody.setPosition(931, 455);

        // ���� ������
        handle.setRadius(23);
        handle.setFillColor(sf::Color(203, 157, 240));
        handle.setOrigin(20, 20);
        handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

        // ���� ���� ����
        connection.setSize(sf::Vector2f(connectionWidth, 20));
        connection.setFillColor(sf::Color(240, 193, 225));
        connection.setPosition(881, 595);
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
SlotReel slotReel; // ���� ������ ���
bool isIndex = true; // ùȭ������ �ƴ��� ����

// ����2 Ŭ����
class Game2 {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    SlotMachine slotMachine;
    Lever lever;
    Logo logo;
    Arrow arrow;
    Sound sound;
    HighScore highScore;
    Text text;
    Modal modal;
    int nScore = 0; // ����
    int comboCount = 0; // ���� Ŭ���� ī��Ʈ

public:
    // â ����
    Game2() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close), // â �ִ� ��Ȱ��ȭ
        logo("logo2.png"), text("C:\\Windows\\Fonts\\arial.ttf"),
        modal("C:\\Windows\\Fonts\\arial.ttf") {
        // ���� �ε�
        if (!sound.loadSound("reel", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\slot2_reel_spin_1.wav") ||
            !sound.loadSound("lever", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\Tiny Button Push Sound.mp3") ||
            !sound.loadSound("bonus", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\collect-points-190037.mp3") ||
            !sound.loadSound("fail", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\fail-234710.mp3") ||
            !sound.loadBackgroundMusic("C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\funny-bgm-240795.mp3"))
        {
            throw runtime_error("Failed to load sound files");
        }

        // ������� ���� �� ���� ����
        sound.setBackgroundMusicVolume(50);
        sound.playBackgroundMusic();
    } // â �ִ�ȭ ��Ȱ��ȭ

    void run() {
        bool isMouseOverLever = false; // ���콺�� ���� ���� �ִ��� ����
        bool isFirstPull = true; // ó�� ���� ���
        bool isFirstReelAnimation = true; //ó�� ���Ը� �ִϸ��̼� �۵�
        bool showModal = false; // ��� ����
        bool showIndex = false;         // index ���̱� ����
        float elapsedTime = 0.0f; // ȭ��ǥ�� �����̱� ������ �� ��� �ð�

        // ���� ����
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                // ù ���� �ְ��� �ҷ�����
                text.updateHScore(highScore.getHighScore());

                // ���콺 �̵� �� ���� ���� �ִ��� Ȯ��
                if (event.type == sf::Event::MouseMoved) { // ���콺 ���� ��ġ Ȯ��
                    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y); // mousePos : ���콺�� �̵��� ���� ���� x, y ��ǥ
                    isMouseOverLever = lever.handle.getGlobalBounds().contains(mousePos); // contains(mousePos) : mousePos�� leverHandle�� ���� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
                }  // true or false

                // ���콺 ��ư�� ���� �� ���� ���� ���·� ��ȯ
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  ���콺 ��ư�� �������� �� & ���� ���콺 ��ư Ŭ��
                    if (isMouseOverLever && !slotMachine.isGameOver()) {
                        sound.playSound("lever"); // ���� �Ҹ� ���
                        if (slotReel.isAnimating) {
                            sound.stopSound("reel"); // ���Ը� �Ҹ� ����
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
                            sound.playSound("reel"); // ���Ը� �Ҹ� ���

                            // ù ��° ���� �� �ִϸ��̼� ���Ŀ��� checkArrowPosition �޼��� ����
                            if (!isFirstReelAnimation) {
                                slotMachine.checkArrowPosition(arrow.getPositionX()); // ȭ��ǥ ��ġ ���� ���� Ȯ�� �޼���
                                if (slotMachine.isGameCleared()) {
                                    comboCount++; // Ŭ���� ī��Ʈ ���� 
                                    // 3�� ����� �� ���ʽ� ���� �߰�
                                    if (comboCount % 3 == 0) {
                                        sound.playSound("bonus"); // ���ʽ� �Ҹ� ���
                                        nScore += 100;         // �޺� ���ʽ� ���� �߰�
                                        text.showCombo();      // �޺� �ؽ�Ʈ ǥ��
                                    }
                                    else nScore += 50; // ���� Ŭ���� �� ���� �߰�
                                    text.updateCombo();
                                    text.updateScore(nScore); // ���� ������Ʈ
                                    highScore.update(nScore); // �ְ� ��� ������Ʈ
                                    text.updateHScore(highScore.getHighScore()); // �ְ� ��� �޾ƿͼ� ȭ�鿡 ����
                                    arrow.increaseSpeed(); // ȭ�� �ӵ� ����
                                }
                                else if (slotMachine.isGameOver()) {
                                    showModal = true; // ���� ���� �� ��� ǥ��
                                    sound.stopSound("reel"); // ���Ը� �Ҹ� ����
                                    sound.playSound("fail"); // ���� �Ҹ� ���
                                    modal.setModalText("Game Over!\nPress R to Retry");
                                    modal.setCurrentScore(nScore); // ������ 
                                    modal.setHighScore(highScore.getHighScore()); // �ְ� ���� 
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
                            showModal = false; // ��� ����
                            elapsedTime = 0.0f; // ��� �ð� �ʱ�ȭ
                            text.updateTime(elapsedTime); // ���� �ð� ������Ʈ
                            arrow.reset(); // ȭ��ǥ ��ġ �ʱ�ȭ
                            nScore = 0;  // ���� �ʱ�ȭ
                            comboCount = 0; // �޺� ī��Ʈ �ʱ�ȭ
                            text.showComboText = false; // �޺� �ؽ�Ʈ �ȶ߰�
                            slotMachine.resetColorVariance(); // ���� ���� �ʱ�ȭ
                            arrow.resetSpeed(); // ���ǵ� �ʱ�ȭ
                            slotReel.startAnimation(); // ���Ը� �ִϸ��̼� ����
                            sound.playSound("reel"); // ���Ը� �Ҹ� ���
                        }
                        else{
                            window.close();  // ���� ���� â�� ����
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
                    modal.setModalText("Game Over!\nPress R to Retry");
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

// ����1 Ŭ����
class Game1 {
private:
    sf::RenderWindow window;
    fSlotMachine fslotMachine;
    fLever flever;
    fLogo flogo;
    Sound sound;
    sf::Clock clock; // �ð��� ����� ���� ����

public:
    // â ����
    Game1() : window(sf::VideoMode(1024, 768), "Index", sf::Style::Close), flogo("logo.png") { // â �ִ�ȭ ��Ȱ��ȭ
        // ���� �ε�
        if (!sound.loadSound("lever", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\Tiny Button Push Sound.mp3") ||
            !sound.loadBackgroundMusic("C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Desktop\\SlotGame-Project\\Sound\\doki-doki-crafting-club-194811.mp3"))
        {
            throw runtime_error("Failed to load sound files");
        }

        // ������� ���� �� ���� ����
        sound.setBackgroundMusicVolume(50);
        sound.playBackgroundMusic();
    } 

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
                    isMouseOverLever = flever.handle.getGlobalBounds().contains(mousePos); // contains(mousePos) : mousePos�� leverHandle�� ���� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
                }  // true or false

                // ���콺 ��ư�� ���� �� ���� ���·� ��ȯ
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  ���콺 ��ư�� �������� �� & ���� ���콺 ��ư Ŭ��
                    sound.playSound("lever"); // ���� �Ҹ� ���
                    isIndex = false; // �� �� ���� ùȭ�� �ƴ�
                    sound.stopBackgroundMusic(); // ������� ����
                    window.close();
                }
            }

            // ������Ʈ
            flever.update(isMouseOverLever);

            // 0.3�ʸ��� �� ��ȭ��Ű��
            if (clock.getElapsedTime() >= sf::seconds(0.3f)) { // 0.3�� ������
                fslotMachine.updateColors();  // ��ư ������ ������Ʈ
                clock.restart(); // �ð踦 �����Ͽ� 0.3�ʸ� �ٽ� ī��Ʈ ����
            }

            // ȭ�鿡 �׸���
            window.clear(sf::Color::White);
            flogo.draw(window);
            fslotMachine.draw(window);
            flever.draw(window);
            window.display();
        }

        // Game2 ����
        if (!isIndex) {
            Game2 game2; // Game2 ��ü ����
            game2.run(); // Game2 ����
        }
    }
};

int main() {
    try {
        Game1 game1;
        game1.run();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
    return 0;
};