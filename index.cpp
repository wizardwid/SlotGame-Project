#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// 이미지 클래스
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
        sprite.setPosition(-440, -440);
    }

    // 화면에 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

// 슬롯머신 클래스
class SlotMachine {
private:
    sf::RectangleShape bodyFront, bodyBack, faceBox, frontRectangle, frontRectangleShadow1;
    sf::CircleShape leftEye, leftPupil, rightEye, rightPupil, nose;
    vector<sf::CircleShape> bigCircles;
    vector<sf::CircleShape> smallHexagons;
    vector<sf::RectangleShape> rectangleButtons;
    vector<sf::Color> colors;

public:
    SlotMachine() {
        // 슬롯머신 본체
        bodyFront.setSize(sf::Vector2f(300, 400));
        bodyFront.setFillColor(sf::Color(209, 233, 246));
        bodyFront.setOutlineColor(sf::Color(205, 193, 255));
        bodyFront.setOutlineThickness(5);
        bodyFront.setPosition(362, 270);

        // 뒷면 (그림자 효과)
        bodyBack.setSize(sf::Vector2f(320, 420));
        bodyBack.setFillColor(sf::Color(217, 217, 217));
        bodyBack.setPosition(357, 255);

        // 앞쪽 사각형
        frontRectangle.setSize(sf::Vector2f(290, 100));
        frontRectangle.setFillColor(sf::Color(209, 233, 246));
        frontRectangle.setOutlineColor(sf::Color(205, 193, 255));
        frontRectangle.setOutlineThickness(3);
        frontRectangle.setPosition(367, 435);

        // 앞쪽 사각형 그림자
        frontRectangleShadow1.setSize(sf::Vector2f(295, 130));
        frontRectangleShadow1.setFillColor(sf::Color(205, 193, 255));
        frontRectangleShadow1.setPosition(364, 538);

        // 슬롯머신 얼굴 박스
        faceBox.setSize(sf::Vector2f(230, 100));
        faceBox.setFillColor(sf::Color(248, 232, 238));
        faceBox.setOutlineColor(sf::Color(205, 193, 255));
        faceBox.setOutlineThickness(3);
        faceBox.setPosition(397, 300);

        // 얼굴
        // 눈 
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

        // 코
        nose.setRadius(4);
        nose.setFillColor(sf::Color(240, 168, 208));
        nose.setPosition(505, 340);

        // 큰 동그라미
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

        // 작은 육각형
        for (int i = 0; i < 4; ++i) {
            sf::CircleShape hexagon(7, 6);
            hexagon.setFillColor(sf::Color(218, 172, 255));
            hexagon.setPosition(460 + i * 30, 450);
            smallHexagons.push_back(hexagon);
        }

        // 버튼
        // 버튼 색깔 배열
        colors = {
            sf::Color(147, 223, 255),
            sf::Color(205, 193, 255),
            sf::Color(255, 246, 227),
            sf::Color(255, 204, 234)
        };

        // 각 버튼에 색상을 적용
        for (int i = 0; i < 4; ++i) {
            sf::RectangleShape button(sf::Vector2f(40, 40));
            button.setPosition(402 + i * 60, 480);
            button.setFillColor(colors[i]);
            rectangleButtons.push_back(button); // 버튼을 벡터에 추가
        }
    }

    // 색상 업데이트 함수
    void updateColors() {
        // colors 배열의 첫 번째 색을 마지막으로 이동시킴 (왼쪽으로 회전)
        rotate(colors.rbegin(), colors.rbegin() + 1, colors.rend());

        // 각 버튼의 색상을 새로 업데이트
        for (int i = 0; i < 4; ++i) {
            rectangleButtons[i].setFillColor(colors[i]);
        }
    }

    // 화면에 그리기
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

// 사운드 클래스
class Sound {
private:
    // 사운드 버퍼와 사운드 객체를 매핑하는 해시맵
    unordered_map<string, sf::SoundBuffer> soundBuffers; // 사운드 파일을 메모리에 로드하는 객체
    unordered_map<string, sf::Sound> sounds; // sf::SoundBuffer로 로드된 사운드를 실제로 재생하는 객체
    sf::Music backgroundMusic; // 배경 음악용 반복 재생

public:
    // 사운드 로드 메서드
    bool loadSound(const string& name, const string& filePath) { // 배경 음악 파일을 로드
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filePath)) {
            cerr << "Failed to load sound: " << filePath << endl;
            return false;
        }
        soundBuffers[name] = buffer; // 로드된 사운드 버퍼를 soundBuffers 해시맵에 name 키로 저장 / buffer : 사운드 파일을 메모리에 로드한 버퍼
        sounds[name].setBuffer(soundBuffers[name]); //  sf::Sound 객체에 해당 사운드 버퍼를 연결
        return true;
    }

    // 배경 음악 로드
    bool loadBackgroundMusic(const string& filePath) {
        if (!backgroundMusic.openFromFile(filePath)) {
            std::cerr << "Failed to load background music: " << filePath << endl;
            return false;
        }
        return true;
    }

    // 사운드 재생
    void playSound(const string& name) { // 주어진 이름의 사운드를 재생
        sounds[name].play();
    }

    // 배경 음악 재생
    void playBackgroundMusic(bool loop = true) { // 배경 음악을 재생합
        backgroundMusic.setLoop(loop);
        backgroundMusic.play();
    }

    // 사운드 정지
    void stopSound(const std::string& name) { // 주어진 이름의 사운드를 정지
        sounds[name].stop();
    }

    // 배경 음악 정지
    void stopBackgroundMusic() {
        backgroundMusic.stop();
    }

    // 볼륨 조정
    void setSoundVolume(const string& name, float volume) {
        sounds[name].setVolume(volume); // 0~100
    }

    void setBackgroundMusicVolume(float volume) {
        backgroundMusic.setVolume(volume); // 0~100
    }
};

// 레버 클래스
class Lever {
public:
    sf::RectangleShape leverBody, connection;
    sf::CircleShape handle;
    float leverAngle = 0.0f;    // 레버 회전 각도 변수
    float connectionWidth = 50.0f; // 초기 너비
    sf::Clock animationClock;

public:
    Lever() {
        leverBody.setSize(sf::Vector2f(20, 180));
        leverBody.setFillColor(sf::Color(240, 193, 225));
        leverBody.setOrigin(7.5f, 100);
        leverBody.setPosition(703, 388);

        handle.setRadius(18);
        handle.setFillColor(sf::Color(203, 157, 240));
        handle.setOrigin(16, 16);
        handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

        connection.setSize(sf::Vector2f(50, 20));
        connection.setFillColor(sf::Color(240, 193, 225));
        connection.setPosition(665, 449);
    }

    // 레버 애니메이션
    void update(bool isMouseOver) {
        float elapsedTime = animationClock.restart().asSeconds(); // 레버 애니메이션이 진행되는 동안 경과한 시간을 가져옴. (레버가 얼마나 회전할지 계산)
        if (isMouseOver) { // 마우스가 레버 위에 있을 때
            // 레버를 당기는 상태
            leverAngle += 100.0f * elapsedTime; // 레버의 회전 각도
            if (leverAngle > 10.0f) leverAngle = 10.0f; // 회전 각도가 10도 이상으로 커지지 않도록 제한
        }

        else {
            // 레버 복귀 상태
            leverAngle -= 100.0f * elapsedTime;
            if (leverAngle < 0.0f) leverAngle = 0.0f;
        }

        // 레버 회전 및 손잡이 위치 조정
        leverBody.setRotation(leverAngle);
        handle.setPosition(
            leverBody.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180), // lever.getPosition()은 레버의 중심 위치 가져옴
            leverBody.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180) // 3.14159 / 180 : 각도를 라디안으로 변환하는 값 (수학적 공식)
        );

        // 레버 각도에 따라 연결 도형 너비 변경
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; // 레버 각도에 비례하여 너비를 줄여줌
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
    Sound sound;
    Lever lever;
    Logo logo;
    sf::Clock clock; // 시간이 경과한 양을 추적

public:
    // 창 생성
    Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close), logo("logo.png") {
        // 사운드 로드
        if (!sound.loadSound("lever", "C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Downloads\\Tiny Button Push Sound.mp3") ||
            !sound.loadBackgroundMusic("C:\\Users\\ungesxy.BOOK-CCUUTHN80B.000\\Downloads\\doki-doki-crafting-club-194811.mp3"))
        {
            throw runtime_error("Failed to load sound files");
        }

        // 배경음악 루프 및 볼륨 설정
        sound.setBackgroundMusicVolume(50);
        sound.playBackgroundMusic();
    } // 창 최대화 비활성화

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
                    sound.playSound("lever"); // 레버 소리 재생
                    isMouseOverLever = false;
                }
            }

            // 업데이트
            lever.update(isMouseOverLever);

            // 0.3초마다 색 변화시키기
            if (clock.getElapsedTime() >= sf::seconds(0.3f)) { // 0.3초 지나면
                slotMachine.updateColors();  // 버튼 색상을 업데이트
                clock.restart(); // 시계를 리셋하여 0.3초를 다시 카운트 시작
            }

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
