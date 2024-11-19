#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Simple Slot Machine");

    // 이미지 로드
    sf::Texture texture;
    if (!texture.loadFromFile("logo.png")) {  // 여기에 이미지 경로를 입력하세요.
        return -1;  // 이미지 로드를 실패한 경우 종료
    }

    sf::Sprite imageSprite;
    imageSprite.setTexture(texture);
    imageSprite.setPosition(-430, -400);  // 이미지가 화면의 최상단에 위치하도록 설정

    // 슬롯머신 본체 (입체감 추가)
    sf::RectangleShape machineBodyFront(sf::Vector2f(300, 400));
    machineBodyFront.setFillColor(sf::Color(209, 233, 246)); // 연한 핑크
    machineBodyFront.setOutlineColor(sf::Color(205, 193, 255)); // 연한 보라색
    machineBodyFront.setOutlineThickness(5);
    machineBodyFront.setPosition(362, 270);

    // 뒷면 (그림자 효과)
    sf::RectangleShape machineBodyBack(sf::Vector2f(300, 400));
    machineBodyBack.setFillColor(sf::Color(217, 217, 217));
    machineBodyBack.setPosition(375, 276); // 위치를 조금 이동시켜 그림자 느낌을 줌

    // 앞쪽 사각형 (입체감)
    sf::RectangleShape frontRectangle(sf::Vector2f(290, 100)); // 본체보다 작은 사각형
    frontRectangle.setFillColor(sf::Color(209, 233, 246)); // 연한 분홍
    frontRectangle.setOutlineColor(sf::Color(205, 193, 255)); // 좀 더 진한 보라색
    frontRectangle.setOutlineThickness(3);
    frontRectangle.setPosition(367, 435); // 본체와 거의 붙어 있지만 앞쪽으로 살짝 나와 보이도록

    // 앞쪽 사각형 그림자
    sf::RectangleShape frontRectangleShadow1(sf::Vector2f(295, 130)); // 그림자도 본체와 같은 크기
    frontRectangleShadow1.setFillColor(sf::Color(205, 193, 255)); // 약간 불투명한 색상
    frontRectangleShadow1.setPosition(364, 538); // 약간 아래로 오프셋을 주어 그림자 느낌을 살림

    // 슬롯머신 얼굴 박스
    sf::RectangleShape faceBox(sf::Vector2f(230, 100));
    faceBox.setFillColor(sf::Color(248, 232, 238)); // 연한 분홍
    faceBox.setOutlineColor(sf::Color(205, 193, 255)); // 연한 보라
    faceBox.setOutlineThickness(3);
    faceBox.setPosition(397, 300);

    // 눈 추가
    sf::CircleShape leftEye(12);
    leftEye.setFillColor(sf::Color::White);
    leftEye.setPosition(430, 324);  // 왼쪽 눈 위치 조정

    sf::CircleShape leftPupil(5);
    leftPupil.setFillColor(sf::Color::Black);
    leftPupil.setPosition(438, 330); // 왼쪽 동공 위치 조정

    sf::CircleShape rightEye(12);
    rightEye.setFillColor(sf::Color::White);
    rightEye.setPosition(560, 324);  // 오른쪽 눈 위치 조정

    sf::CircleShape rightPupil(5);
    rightPupil.setFillColor(sf::Color::Black);
    rightPupil.setPosition(568, 330); // 오른쪽 동공 위치 조정

    // 코 추가
    sf::CircleShape nose(4);
    nose.setFillColor(sf::Color(240, 168, 208)); // 연보라
    nose.setPosition(505, 340);

    // 동그라미 버튼 4개
    sf::CircleShape circleButton1(20);
    circleButton1.setFillColor(sf::Color(147, 223, 255)); // 빨간색
    circleButton1.setPosition(400, 380);

    sf::CircleShape circleButton2(20);
    circleButton2.setFillColor(sf::Color(205, 193, 255)); // 파란색
    circleButton2.setPosition(460, 380);

    sf::CircleShape circleButton3(20);
    circleButton3.setFillColor(sf::Color(255, 246, 227)); // 초록색
    circleButton3.setPosition(520, 380);

    sf::CircleShape circleButton4(20);
    circleButton4.setFillColor(sf::Color(255, 204, 234)); // 초록색
    circleButton4.setPosition(580, 380);

    sf::RectangleShape circleButton5(sf::Vector2f(40, 40));
    circleButton5.setFillColor(sf::Color(255, 204, 234)); // 빨간색
    circleButton5.setPosition(400, 480);

    sf::RectangleShape circleButton6(sf::Vector2f(40, 40));
    circleButton6.setFillColor(sf::Color(147, 223, 255)); // 파란색
    circleButton6.setPosition(460, 480);

    sf::RectangleShape circleButton7(sf::Vector2f(40, 40));
    circleButton7.setFillColor(sf::Color(205, 193, 255)); // 초록색
    circleButton7.setPosition(520, 480);

    sf::RectangleShape circleButton8(sf::Vector2f(40, 40));
    circleButton8.setFillColor(sf::Color(255, 246, 227)); // 초록색
    circleButton8.setPosition(580, 480);

    // 직사각형 구멍
    sf::RectangleShape rectangleHole(sf::Vector2f(100, 10)); // 직사각형 연결 도형
    rectangleHole.setFillColor(sf::Color(165, 148, 249)); // 연한 보라색
    rectangleHole.setPosition(460, 450);

    // 레버 본체
    sf::RectangleShape lever(sf::Vector2f(20, 180));
    lever.setFillColor(sf::Color(240, 193, 225)); // 연한 보라색
    lever.setOrigin(7.5f, 100); // 회전 중심 설정
    lever.setPosition(703, 388);

    // 레버 손잡이
    sf::CircleShape leverHandle(18);
    leverHandle.setFillColor(sf::Color(203, 157, 240)); // 빨간 손잡이
    leverHandle.setOrigin(16, 16); // 중심 설정
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // 레버 연결 직사각형
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // 직사각형 연결 도형
    leverConnection.setFillColor(sf::Color(240, 193, 225)); // 연한 보라색
    leverConnection.setPosition(665, 449);

    // 애니메이션 상태 변수
    bool isMouseOverLever = false;
    float leverAngle = 0.0f;
    float connectionWidth = 50.0f; // 초기 너비
    sf::Clock animationClock;

    // 게임 루프
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // 마우스 이동 시 레버 위에 있는지 확인
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                isMouseOverLever = leverHandle.getGlobalBounds().contains(mousePos);
            }

            // 마우스 버튼을 뗐을 때 복귀 상태로 전환
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMouseOverLever = false;
            }
        }

        // 레버 애니메이션
        if (isMouseOverLever) {
            // 레버를 당길 때
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle += 100.0f * elapsedTime; // 빠르게 당기기
            if (leverAngle > 10.0f)
                leverAngle = 10.0f;
        }
        else {
            // 레버 복귀
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle -= 100.0f * elapsedTime; // 빠르게 복귀
            if (leverAngle < 0.0f)
                leverAngle = 0.0f;
        }

        // 레버 회전 및 손잡이 위치 조정
        lever.setRotation(leverAngle);
        leverHandle.setPosition(
            lever.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180),
            lever.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180)
        );

        // 레버 각도에 따라 연결 직사각형 너비 변경
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; // 너비 감소
        leverConnection.setSize(sf::Vector2f(connectionWidth, 20));

        // 화면 그리기
        window.clear(sf::Color::White);
        window.draw(imageSprite);
        window.draw(machineBodyBack); // 슬롯머신 뒷면 (그림자)
        window.draw(machineBodyFront); // 슬롯머신 본체 (앞면)
        window.draw(frontRectangle); // 앞쪽 사각형 (입체감 추가)
        window.draw(frontRectangleShadow1); // 앞쪽 사각형 그림자1
        window.draw(circleButton1);  // 동그라미 버튼 1
        window.draw(circleButton2);  // 동그라미 버튼 2
        window.draw(circleButton3);  // 동그라미 버튼 3
        window.draw(circleButton4);  // 동그라미 버튼 4
        window.draw(circleButton5);  // 동그라미 버튼 1
        window.draw(circleButton6);  // 동그라미 버튼 2
        window.draw(circleButton7);  // 동그라미 버튼 3
        window.draw(circleButton8);  // 동그라미 버튼 4
        window.draw(faceBox);        // 얼굴 박스
        window.draw(leftEye);        // 왼쪽 눈
        window.draw(leftPupil);      // 왼쪽 동공
        window.draw(rightEye);       // 오른쪽 눈
        window.draw(rightPupil);     // 오른쪽 동공
        window.draw(nose);           // 코
        window.draw(rectangleHole);  // 직사각형 구멍
        window.draw(lever);          // 레버
        window.draw(leverHandle);    // 레버 손잡이
        window.draw(leverConnection); // 레버와 머신본체를 연결하는 직사각형 도형
        window.display();
    }

    return 0;
}
