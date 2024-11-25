#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

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

    // ���Ըӽ� ��ü Ŭ����
    class SlotMachine {
    public:
        sf::RectangleShape body;
        sf::RectangleShape innerBox;
        sf::RectangleShape shadow1;
        sf::RectangleShape shadow2;
        sf::RectangleShape rectangle;
        sf::RectangleShape rectangleShadow;
        std::vector<sf::RectangleShape> lines;

        SlotMachine() {
            // ���Ըӽ� ��ü
            body.setSize(sf::Vector2f(800, 300));
            body.setFillColor(sf::Color(191, 236, 255));
            body.setOutlineColor(sf::Color(205, 193, 255));
            body.setOutlineThickness(5);
            body.setPosition(80, 180);

            // ���� �ڽ�
            innerBox.setSize(sf::Vector2f(780, 280));
            innerBox.setFillColor(sf::Color(255, 251, 243));
            innerBox.setOutlineColor(sf::Color(205, 193, 255));
            innerBox.setOutlineThickness(3);
            innerBox.setPosition(90, 190);

            // ��ü �׸��� 1
            shadow1.setSize(sf::Vector2f(820, 320));
            shadow1.setFillColor(sf::Color(217, 217, 217));
            shadow1.setPosition(75, 165);

            // ��ü �׸��� 2
            shadow2.setSize(sf::Vector2f(330, 30));
            shadow2.setFillColor(sf::Color(217, 217, 217));
            shadow2.setPosition(565, 465);

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

            // ���簢��
            rectangle.setSize(sf::Vector2f(750, 30));
            rectangle.setFillColor(sf::Color(227, 246, 255));
            rectangle.setOutlineColor(sf::Color(205, 193, 255));
            rectangle.setOutlineThickness(3);
            rectangle.setPosition(104, 485);

            // ���簢�� �׸���
            rectangleShadow.setSize(sf::Vector2f(750, 33));
            rectangleShadow.setFillColor(sf::Color(217, 217, 217));
            rectangleShadow.setPosition(117, 485);
        }

        // ȭ�鿡 �׸���
        void draw(sf::RenderWindow& window) {
            window.draw(shadow1);
            window.draw(shadow2);
            window.draw(body);
            window.draw(innerBox);
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
            leverBody.setPosition(930, 285);

            // ���� ������
            handle.setRadius(23);
            handle.setFillColor(sf::Color(203, 157, 240));
            handle.setOrigin(20, 20);
            handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

            // ���� ���� ����
            connection.setSize(sf::Vector2f(connectionWidth, 20));
            connection.setFillColor(sf::Color(240, 193, 225));
            connection.setPosition(880, 425);
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
                        isMouseOverLever = false;
                    }
                }

                // ������Ʈ
                lever.update(isMouseOverLever);

                // ȭ�鿡 �׸���
                window.clear(sf::Color::White);
                logo.draw(window);
                slotMachine.draw(window);
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
