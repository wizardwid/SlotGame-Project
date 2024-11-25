#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

// 이미지 클래스
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

        // 화면에 그리기
        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
        }
    };

    // 슬롯머신 본체 클래스
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
            // 슬롯머신 본체
            body.setSize(sf::Vector2f(800, 300));
            body.setFillColor(sf::Color(191, 236, 255));
            body.setOutlineColor(sf::Color(205, 193, 255));
            body.setOutlineThickness(5);
            body.setPosition(80, 180);

            // 내부 박스
            innerBox.setSize(sf::Vector2f(780, 280));
            innerBox.setFillColor(sf::Color(255, 251, 243));
            innerBox.setOutlineColor(sf::Color(205, 193, 255));
            innerBox.setOutlineThickness(3);
            innerBox.setPosition(90, 190);

            // 본체 그림자 1
            shadow1.setSize(sf::Vector2f(820, 320));
            shadow1.setFillColor(sf::Color(217, 217, 217));
            shadow1.setPosition(75, 165);

            // 본체 그림자 2
            shadow2.setSize(sf::Vector2f(330, 30));
            shadow2.setFillColor(sf::Color(217, 217, 217));
            shadow2.setPosition(565, 465);

            // 선 생성
            const int lineCount = 4;
            const float lineWidth = 3; // 선 두께
            const float spacing = innerBox.getSize().x / (lineCount + 1); // 선 간격 /  슬롯머신 박스의 가로 길이

            for (int i = 0; i < lineCount; ++i) {
                sf::RectangleShape line(sf::Vector2f(lineWidth, innerBox.getSize().y)); // 박스 높이에 맞추기, 위아래 간격 추가
                line.setFillColor(sf::Color(205, 193, 255));
                line.setPosition(
                    innerBox.getPosition().x + spacing * (i + 1) - lineWidth / 2, // spacing: 선 사이의 간격 / - lineWidth / 2: 선의 두께의 절반
                    innerBox.getPosition().y // 슬롯머신 박스의 세로 길이
                );
                lines.push_back(line);
            }

            // 직사각형
            rectangle.setSize(sf::Vector2f(750, 30));
            rectangle.setFillColor(sf::Color(227, 246, 255));
            rectangle.setOutlineColor(sf::Color(205, 193, 255));
            rectangle.setOutlineThickness(3);
            rectangle.setPosition(104, 485);

            // 직사각형 그림자
            rectangleShadow.setSize(sf::Vector2f(750, 33));
            rectangleShadow.setFillColor(sf::Color(217, 217, 217));
            rectangleShadow.setPosition(117, 485);
        }

        // 화면에 그리기
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

// 레버 클래스
class Lever {
    public:
        sf::RectangleShape leverBody, connection;
        sf::CircleShape handle;
        float angle = 0.0f; // 레버 회전 각도 변수
        float connectionWidth = 50.0f; 
        sf::Clock animationClock;

        Lever() {
            // 레버 본체
            leverBody.setSize(sf::Vector2f(20, 260));
            leverBody.setFillColor(sf::Color(240, 193, 225));
            leverBody.setOrigin(7.5f, 100);
            leverBody.setPosition(930, 285);

            // 레버 손잡이
            handle.setRadius(23);
            handle.setFillColor(sf::Color(203, 157, 240));
            handle.setOrigin(20, 20);
            handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

            // 레버 연결 도형
            connection.setSize(sf::Vector2f(connectionWidth, 20));
            connection.setFillColor(sf::Color(240, 193, 225));
            connection.setPosition(880, 425);
        }

        // 레버 애니메이션
        void update(bool isMouseOver) {
            float elapsedTime = animationClock.restart().asSeconds(); // 레버 애니메이션이 진행되는 동안 경과한 시간을 가져옴. (레버가 얼마나 회전할지 계산)
            if (isMouseOver) { // 마우스가 레버 위에 있을 때
                // 레버를 당기는 상태
                angle += 100.0f * elapsedTime; // 레버의 회전 각도
                if (angle > 10.0f) angle = 10.0f; // 회전 각도가 10도 이상으로 커지지 않도록 제한
            }

            else {
                // 레버 복귀 상태
                angle -= 100.0f * elapsedTime;
                if (angle < 0.0f) angle = 0.0f;
            }

            // 레버 회전 및 손잡이 위치 조정
            leverBody.setRotation(angle);
            handle.setPosition(
                leverBody.getPosition().x + 100 * sin(angle * 3.14159 / 180), // lever.getPosition()은 레버의 중심 위치 가져옴
                leverBody.getPosition().y - 100 * cos(angle * 3.14159 / 180) // 3.14159 / 180 : 각도를 라디안으로 변환하는 값 (수학적 공식)
            );

            // 레버 각도에 따라 연결 도형 너비 변경
            connectionWidth = 50.0f - (angle / 20.0f) * 30.0f; // 레버 각도에 비례하여 너비를 줄여줌
            connection.setSize(sf::Vector2f(connectionWidth, 20)); // 적용
        }

        // 화면에 그리기
        void draw(sf::RenderWindow& window) {
            window.draw(leverBody);
            window.draw(handle);
            window.draw(connection);
        }
};

// 게임 클래스
class Game {
    private:
        sf::RenderWindow window;
        SlotMachine slotMachine;
        Lever lever;
        Logo logo;

    public:
        // 창 생성
        Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close), logo("logo2.png") {} // 창 최대화 비활성화

        void run() {
            bool isMouseOverLever = false;

            // 게임 루프
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {

                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }

                    // 마우스 이동 시 레버 위에 있는지 확인
                    if (event.type == sf::Event::MouseMoved) { // 마우스 현재 위치 확인
                        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y); // mousePos : 마우스가 이동할 때의 현재 x, y 좌표
                        isMouseOverLever = lever.handle.getGlobalBounds().contains(mousePos); // contains(mousePos) : mousePos가 leverHandle의 영역 안에 있는지 확인하는 함수
                    }  // true or false

                    // 마우스 버튼을 뗐을 때 복귀 상태로 전환
                    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  마우스 버튼이 떨어졌을 때 & 왼쪽 마우스 버튼 클릭
                        isMouseOverLever = false;
                    }
                }

                // 업데이트
                lever.update(isMouseOverLever);

                // 화면에 그리기
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
