#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/System.hpp> 
#include <random>
#include <cstdlib>  // rand() 함수 사용을 위한 헤더
#include <ctime>    // srand() 함수 사용을 위한 헤더

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

// 슬롯릴 클래스
class SlotReel {
public:
    sf::RectangleShape body;
    sf::RectangleShape innerBox;
    sf::RectangleShape shadow;
    std::vector<sf::RectangleShape> symbols;  // 슬롯 내부의 심볼들
    sf::Clock animationClock;                 // 애니메이션 타이머
    bool isAnimating = false;                 // 애니메이션 상태
    bool showInnerBoxColor = false;           // innerBox에 랜덤 색상을 표시할지 여부
    float scrollSpeed = 450.f;                // 심볼 스크롤 속도
    int numSymbols = 4;                       // 슬롯에 표시되는 심볼 개수

    // 슬롯 릴에서 현재 색상을 가져오는 함수
    sf::Color getCurrentColor() const {
        return innerBox.getFillColor();
    }

    SlotReel() {
        // 슬롯 릴 본체
        body.setSize(sf::Vector2f(500, 160));
        body.setFillColor(sf::Color(240, 193, 225));
        body.setOutlineColor(sf::Color(203, 157, 240));
        body.setOutlineThickness(5);
        body.setPosition(230, 185);

        // 내부 박스 (초기 상태로 계속 보이도록 설정)
        innerBox.setSize(sf::Vector2f(480, 140));
        innerBox.setFillColor(sf::Color(249, 245, 246));
        innerBox.setOutlineColor(sf::Color(203, 157, 240));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(240, 195);

        // 본체 그림자
        shadow.setSize(sf::Vector2f(520, 175));
        shadow.setFillColor(sf::Color(217, 217, 217));
        shadow.setPosition(225, 170);

        for (int i = 0; i < numSymbols; ++i) {
            sf::RectangleShape symbol(sf::Vector2f(480, 40)); // 심볼 크기 설정
            symbol.setPosition(240, 180 + i * 40);            // 심볼 위치 설정
            symbol.setFillColor(sf::Color(249, 245, 246));
            symbols.push_back(symbol);
        }

    }

    // 랜덤 색상 생성 함수
    sf::Color getRandomColor() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 255);

        return sf::Color(dis(gen), dis(gen), dis(gen)); // 랜덤 RGB 값 생성
    }

    // 심볼 애니메이션 업데이트
    void update() {
        if (isAnimating) {
            // 현재 시간 기준으로 deltaTime 계산
            float deltaTime = animationClock.getElapsedTime().asSeconds();

            // deltaTime을 0으로 초기화하여 다음 업데이트에서 계속 새로 시작
            animationClock.restart();

            for (auto& symbol : symbols) {
                // 심볼을 위로 스크롤
                symbol.move(0.f, -scrollSpeed * deltaTime);

                // 화면 밖으로 나가면 다시 아래로 보내기
                if (symbol.getPosition().y + symbol.getSize().y < 236) {
                    symbol.setPosition(240, 180 + (numSymbols - 1) * 40); // 다시 아래로 설정
                    symbol.setFillColor(getRandomColor());               // 심볼 색상도 랜덤으로 변경
                }
            }
        }
    }

    // 화면에 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(shadow);
        window.draw(body);
        window.draw(innerBox);  // innerBox는 애니메이션 도중에도 계속 보이도록

        if (isAnimating) {
            // 애니메이션 중에는 심볼들을 그린다
            for (auto& symbol : symbols) {
                window.draw(symbol);
            }
        }
        else if (showInnerBoxColor) {
            // 애니메이션이 끝난 후에는 랜덤 색상이 innerBox 위에 표시되도록
            window.draw(innerBox);  // innerBox만 계속 보이게
        }
    }

    // 애니메이션 시작
    void startAnimation() {
        isAnimating = true;
        innerBox.setFillColor(sf::Color(249, 245, 246));
        showInnerBoxColor = false; // 애니메이션 중에는 innerBox 랜덤 색상 숨김
        animationClock.restart();
    }

    // 애니메이션 정지 및 랜덤 색상 반환
    sf::Color stopAnimation() {
        isAnimating = false;
        sf::Color randomColor = getRandomColor(); // 랜덤 색상 생성
        innerBox.setFillColor(randomColor);       // innerBox에 랜덤 색상 설정
        showInnerBoxColor = true;
        return randomColor;                      // 생성된 랜덤 색상을 반환
    }
};

// 슬롯머신 클래스
class SlotMachine {
public:
    sf::RectangleShape body;
    sf::RectangleShape innerBox;
    sf::RectangleShape shadow1;
    sf::RectangleShape shadow2;
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangleShadow;
    vector<sf::RectangleShape> lines;
    vector<sf::RectangleShape> colorSections; // 각 공간을 위한 색상 칸들 (5개)
    vector<sf::Color> generatedColors; // 생성된 색상을 추적
    float colorVariance = 500.0f;  // 색상 범위 변수
    int correctSectionIndex = -1;  // 올바른 색상이 위치한 구간의 인덱스  
    bool gameCleared = false;  // 게임 클리어 여부
    bool gameOver = false;     // 게임 오버 여부

    SlotMachine() {
        // 슬롯머신 본체
        body.setSize(sf::Vector2f(800, 300));
        body.setFillColor(sf::Color(191, 236, 255));
        body.setOutlineColor(sf::Color(205, 193, 255));
        body.setOutlineThickness(5);
        body.setPosition(80, 350);

        // 내부 박스
        innerBox.setSize(sf::Vector2f(780, 280));
        innerBox.setFillColor(sf::Color(255, 251, 243));
        innerBox.setOutlineColor(sf::Color(205, 193, 255));
        innerBox.setOutlineThickness(3);
        innerBox.setPosition(90, 360);

        // 본체 그림자 1
        shadow1.setSize(sf::Vector2f(820, 320));
        shadow1.setFillColor(sf::Color(217, 217, 217));
        shadow1.setPosition(75, 335);

        // 본체 그림자 2
        shadow2.setSize(sf::Vector2f(330, 30));
        shadow2.setFillColor(sf::Color(217, 217, 217));
        shadow2.setPosition(565, 635);

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

        // 색을 채울 공간 생성 (선 사이 구간)
        srand(static_cast<unsigned int>(time(0))); // 랜덤 시드 설정

        for (int i = 0; i < lineCount + 1; ++i) {
            sf::RectangleShape section(sf::Vector2f(spacing, innerBox.getSize().y)); // 각 구간의 크기 설정
            section.setPosition(innerBox.getPosition().x + i * spacing, innerBox.getPosition().y);
            section.setFillColor(sf::Color(255, 251, 243));
            colorSections.push_back(section);
        }

        // 직사각형
        rectangle.setSize(sf::Vector2f(750, 30));
        rectangle.setFillColor(sf::Color(227, 246, 255));
        rectangle.setOutlineColor(sf::Color(205, 193, 255));
        rectangle.setOutlineThickness(3);
        rectangle.setPosition(104, 655);

        // 직사각형 그림자
        rectangleShadow.setSize(sf::Vector2f(750, 33));
        rectangleShadow.setFillColor(sf::Color(217, 217, 217));
        rectangleShadow.setPosition(117, 655);
    }

    // 색상 범위를 초기화하는 setter 함수
    void setColorVariance(float variance) {
        colorVariance = variance;
    }

    // 색상 범위 제어
    sf::Color randomColorExcluding(const sf::Color& excludeColor) {
        sf::Color randomColor;
        bool colorExists; // 존재하는 컬러인지 추적
        do { // 생성된 색상이 excludeColor와 동일한 경우를 피함
            // 색상 범위를 colorVariance 범위에 맞게 조절하여 랜덤 색상 생성
            int r = rand() % (int)colorVariance + excludeColor.r - (int)(colorVariance / 2); // excludeColor의 해당 색상 값을 기준으로 생성
            int g = rand() % (int)colorVariance + excludeColor.g - (int)(colorVariance / 2); // 0부터 colorVariance - 1까지의 값을 반환
            int b = rand() % (int)colorVariance + excludeColor.b - (int)(colorVariance / 2); // excludeColor rgb를 중심으로 값이 ±(colorVariance / 2) 범위 내에서 생성

            // 색상 범위 보정
            r = max(0, min(255, r)); // RGB 값이 0에서 255 사이로 제한
            g = max(0, min(255, g));
            b = max(0, min(255, b));

            randomColor = sf::Color(r, g, b);
            colorExists = find(generatedColors.begin(), generatedColors.end(), randomColor) != generatedColors.end(); // 이미 생성된 색상인지 검사
            // find 함수 : 찾고자 하는 요소가 컨테이너 안에 존재하는지 여부를 확인 -> 해당 요소를 가리키는 iterator 반환
        } while (randomColor == excludeColor || colorExists); // 제외할 색상과 중복 검사(randomColor가 excludeColor와 동일한지 확인)
        return randomColor;
    }

    // 색을 변경하는 함수 (레버를 눌렀을 때 호출)
    void update(const sf::Color& slotReelColor) {
        int targetIndex = rand() % colorSections.size(); // 랜덤 위치 선택
        correctSectionIndex = targetIndex; // 정답 구간 인덱스 저장

        for (int i = 0; i < colorSections.size(); ++i) {
            if (i == targetIndex) {
                // 선택된 위치에 슬롯릴 색상 배치
                colorSections[i].setFillColor(slotReelColor);
            }
            else {
                // 나머지 공간에는 슬롯릴 색상을 제외한 랜덤 색상 배치
                colorSections[i].setFillColor(randomColorExcluding(slotReelColor));
            }
        }
        // 게임 진행에 따라 색상 범위 축소
        if (colorVariance > 100.0f) {
            colorVariance -= 10.0f; // 최소값 100까지 축소
        }
    }

    // 게임이 클리어 확인 메서드
    bool isGameCleared() const {
        return gameCleared;
    }

    // 게임 오버 확인 메서드
    bool isGameOver() const {
        return gameOver;
    }

    // 게임 오버 상태를 설정하는 메서드
    void setGameOver(bool over) {
        gameOver = over;
    }

    // 화살표가 정답 구간에 도달했는지 확인
    void checkArrowPosition(float arrowPositionX) {
        if (correctSectionIndex < 0 || correctSectionIndex >= colorSections.size()) {
            cout << "Error: correctSectionIndex is invalid!" << endl;
            return;
        } // 에러

        // 정답 구간의 시작과 끝 X 좌표
        float sectionStartX = colorSections[correctSectionIndex].getPosition().x;
        float sectionEndX = sectionStartX + colorSections[correctSectionIndex].getSize().x;

        // 화살표가 정답 구간에 위치하는지 확인
        if (arrowPositionX >= sectionStartX && arrowPositionX <= sectionEndX) {
            cout << "Game Cleared! Colors match!" << endl;
            gameCleared = true; // 게임 클리어 상태로 설정
            gameOver = false;   // 게임 오버 상태는 false
        }
        else {
            cout << "Game Over! Try again!" << endl;
            gameOver = true;    // 게임 오버 상태로 설정
            gameCleared = false; // 게임 클리어 상태는 false
        }
    }

    // 화면에 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(shadow1);
        window.draw(shadow2);
        window.draw(body);
        window.draw(innerBox);

        // 색상 공간 그리기
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

// 화살표 클래스
class Arrow {
public:
    sf::RectangleShape arrow;  // 화살표 본체
    sf::ConvexShape triangle;  // 화살표 위에 올릴 세모
    float speed = 400.0f;      // 화살표 속도
    float direction = 1.0f;    // 화살표 방향 (1이면 오른쪽, -1이면 왼쪽)
    sf::Vector2f position;     // 화살표 위치

    // 화살표의 X 위치 반환
    float getPositionX() const {
        return arrow.getPosition().x;
    }

    Arrow() {
        // 화살표 디자인 (단순한 사각형 모양)
        arrow.setSize(sf::Vector2f(20.0f, 25.0f)); // 크기 설정
        arrow.setFillColor(sf::Color(205, 193, 255));
        arrow.setOrigin(arrow.getSize().x / 2, arrow.getSize().y / 2); // 중심을 기준으로 회전
        arrow.setPosition(800, 628); // 초기 위치 설정

        // 세모 디자인 (ConvexShape 사용)
        triangle.setPointCount(3);  // 세모는 세 개의 점으로 구성됨
        triangle.setPoint(0, sf::Vector2f(0, -25));  // 위쪽 (크기 키움)
        triangle.setPoint(1, sf::Vector2f(25, 15));  // 오른쪽 (크기 키움)
        triangle.setPoint(2, sf::Vector2f(-25, 15)); // 왼쪽 (크기 키움)
        triangle.setFillColor(sf::Color(205, 193, 255));
        triangle.setOrigin(0, 10); // 세모의 기준점을 바닥에 맞추기
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2);  // 화살표 위에 배치
    }

    // 화살표 업데이트 함수
    void update(float deltaTime) {

        // 화살표 위치 업데이트
        position = arrow.getPosition();

        // 왼쪽과 오른쪽으로 반복하며 움직이기
        if (position.x <= 115 || position.x >= 845) {
            direction *= -1; // 방향 전환
        }

        // 화살표를 계속 이동
        position.x += speed * direction * deltaTime;  // deltaTime을 이용한 속도 조절
        arrow.setPosition(position);  // 새로운 위치로 갱신

        // 세모도 화살표의 위치에 맞춰 업데이트
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2); // 화살표 위에 세모 위치 갱신
    }

    // 스피드 증가
    void increaseSpeed() {
        speed += 30.0f; // 화살표 속도 증가시킴 (클리어마다 속도 증가)
    }

    // 스피드 초기화
    void resetSpeed() {
        speed = 400.0f;  // 기본 속도로 초기화
    }

    // 화살표를 초기 위치로 되돌리는 함수
    void reset() {
        arrow.setPosition(800, 628); // 초기 위치
        triangle.setPosition(arrow.getPosition().x, arrow.getPosition().y - arrow.getSize().y / 2); // 세모도 초기 위치로 되돌림
    }

    // 화면에 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(arrow);    // 화살표 그리기
        window.draw(triangle); // 세모 그리기
    }
};

class Text {
public:
    sf::Text scoreText;   // 스코어 텍스트
    sf::Text timerText;  // 제한 시간 텍스트
    sf::Font font;
    sf::Text comboText; // 콤보 텍스트
    bool showComboText = false; // 콤보 텍스트 표시 여부
    sf::Clock comboClock;   // 콤보 텍스트 표시 시간 관리

    // 매개변수 생성자 (폰트 경로를 전달받을 수 있도록 함)
    Text(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {
            throw std::runtime_error("Failed to load font");
        }

        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(900, 50); // 화면에서 점수의 위치
        scoreText.setString("Score: 0");

        // 제한 시간 텍스트 설정
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::Black);
        timerText.setPosition(900, 80);  // 시간 표시 위치 (점수 밑)
        timerText.setString("Time: 0s");

        // 콤보 텍스트 초기화
        comboText.setFont(font); // font는 기존 폰트 객체
        comboText.setString("Combo Bonus!");
        comboText.setCharacterSize(50);
        comboText.setFillColor(sf::Color::Yellow);
        comboText.setStyle(sf::Text::Bold);
        comboText.setPosition(400, 300); // 화면 중앙에 표시
    }

    // 점수 업데이트
    void updateScore(int score) {
        scoreText.setString("Score: " + to_string(score));
    }

    // 제한 시간 업데이트
    void updateTime(float time) {
        timerText.setString("Time: " + to_string(static_cast<int>(time)) + "s");
    }

    // 콤보 텍스트 표시
    void showCombo() {
        showComboText = true;
        comboClock.restart(); // 표시 시간 초기화
    }

    // 콤보 텍스트 업데이트 (시간 기반으로 숨김 처리)
    void updateCombo() {
        if (showComboText && comboClock.getElapsedTime().asSeconds() > 0.5f) {
            showComboText = false;
        }
    }

    // 화면에 그리기
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

    // 매개변수 생성자 (폰트 경로를 전달받을 수 있도록 함)
    Modal(const std::string& fontPath) {
        if (!font.loadFromFile(fontPath)) {  // 괄호 수정
            throw std::runtime_error("Failed to load font");
        }

        modalBackground.setSize(sf::Vector2f(400, 200)); // 모달의 크기
        modalBackground.setFillColor(sf::Color(0, 0, 0, 200)); // 배경 색 (반투명)
        modalBackground.setPosition(312, 284); // 모달의 위치

        modalText.setFont(font);
        modalText.setCharacterSize(24);
        modalText.setFillColor(sf::Color::White);
        modalText.setPosition(362, 334); // 텍스트 위치
    }

    // 모달 텍스트 설정
    void setModalText(const std::string& text) {
        modalText.setString(text);
    }

    // 화면에 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(modalBackground);
        window.draw(modalText);
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
        leverBody.setPosition(930, 455);

        // 레버 손잡이
        handle.setRadius(23);
        handle.setFillColor(sf::Color(203, 157, 240));
        handle.setOrigin(20, 20);
        handle.setPosition(leverBody.getPosition().x, leverBody.getPosition().y - 100);

        // 레버 연결 도형
        connection.setSize(sf::Vector2f(connectionWidth, 20));
        connection.setFillColor(sf::Color(240, 193, 225));
        connection.setPosition(880, 595);
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

// 전역변수
SlotReel slotReel;

// 게임 클래스
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
    int comboCount = 0; // 게임 클리어 카운트

public:
    // 창 생성
    Game() : window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close),
        logo("logo2.png"), text("C:\\Windows\\Fonts\\arial.ttf"),
        modal("C:\\Windows\\Fonts\\arial.ttf") {} // 창 최대화 비활성화

    void run() {
        bool isMouseOverLever = false;
        bool isFirstPull = true; // 처음 레버 당김
        bool isFirstReelAnimation = true; //처음 슬롯릴 애니메이션 작동
        bool showModal = false; // 모달 띄우기
        float elapsedTime = 0.0f; // 화살표가 움직이기 시작한 후 경과 시간

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

                // 마우스 버튼을 뗐을 때 레버 복귀 상태로 전환
                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  마우스 버튼이 떨어졌을 때 & 왼쪽 마우스 버튼 클릭
                    if (isMouseOverLever && !slotMachine.isGameOver()) {
                        if (slotReel.isAnimating) {
                            sf::Color slotReelColor = slotReel.stopAnimation(); // 애니메이션 정지 및 슬롯릴 색상 반환
                            slotMachine.update(slotReelColor); // 반환된 색상으로 슬롯머신 업데이트
                            arrow.reset(); // 화살표 초기화
                        }

                        else {
                            // 레버를 처음 당길 때
                            if (isFirstPull) {
                                isFirstPull = false;
                                arrow.reset(); // 화살표 초기화
                            }
                            elapsedTime = 0.0f; // 경과 시간 초기화
                            slotReel.startAnimation();  // 슬롯 릴 애니메이션 시작

                            // 첫 번째 슬롯 릴 애니메이션 이후에만 checkArrowPosition 메서드 수행
                            if (!isFirstReelAnimation) {
                                slotMachine.checkArrowPosition(arrow.getPositionX()); // 화살표 위치 정답 구간 확인 메서드
                                if (slotMachine.isGameCleared()) {
                                    comboCount++; // 클리어 카운트 증가 
                                    // 3의 배수일 때 보너스 점수 추가
                                    if (comboCount % 3 == 0) {
                                        nScore += 100;         // 콤보 보너스 점수 추가
                                        text.showCombo();      // 콤보 텍스트 표시
                                    }
                                    else nScore += 50; // 게임 클리어 시 점수 추가
                                    text.updateCombo();
                                    text.updateScore(nScore); // 점수 업데이트
                                    arrow.increaseSpeed();
                                }
                                else if (slotMachine.isGameOver()) {
                                    showModal = true; // 게임 오버 시 모달 표시
                                    modal.setModalText("Game Over!\nPress R to Retry\nH to Quit");
                                }
                            }
                            isFirstReelAnimation = false;  // 첫 번째 애니메이션이 끝났음을 표시
                        }
                    }
                }
                // 모달 표시
                if (showModal) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::R) {
                            slotMachine.setGameOver(false); // 게임 오버 상태를 false로 설정
                            showModal = false;
                            elapsedTime = 0.0f; // 경과 시간 초기화
                            text.updateTime(elapsedTime); // 제한 시간 업데이트
                            arrow.reset();
                            nScore = 0;  // 점수 초기화
                            text.updateScore(nScore); // 점수 업데이트
                            slotMachine.setColorVariance(500.0f); // 색상 범위 초기화
                            comboCount = 0;
                            arrow.resetSpeed(); // 스피드 초기화
                            slotReel.startAnimation();
                        }
                        else if (event.key.code == sf::Keyboard::H) {
                            window.close(); // 'H' 눌렀을 때 창 닫기
                        }
                    }
                }
            }

            // DeltaTime 계산
            float deltaTime = clock.restart().asSeconds();

            // 레버를 처음 당긴 후 && 슬롯릴 애니메이션이 움직이지 않을 때 && 모달이 안보일 때
            if (!isFirstPull && !slotReel.isAnimating && !showModal) {
                arrow.update(deltaTime); // 화살표 업데이트

                // 경과 시간 누적
                elapsedTime += deltaTime;
                text.updateTime(elapsedTime); // 제한 시간 업데이트

                // 3초가 지나면 게임 오버
                if (elapsedTime >= 4.0f) {
                    slotMachine.setGameOver(true);
                    modal.setModalText("Game Over!\nPress R to Retry\nH to Quit");
                    showModal = true;
                }
            }

            //업데이트
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

    // Delta Time 출력
    cout << "Delta Time: " << deltaTime << endl;
    return 0;
};