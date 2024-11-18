#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Simple Slot Machine");

    // 슬롯머신 본체
    sf::RectangleShape machineBody(sf::Vector2f(300, 400));
    machineBody.setFillColor(sf::Color(255, 200, 221)); // 연한 핑크
    machineBody.setOutlineColor(sf::Color(180, 150, 200)); // 연한 보라색
    machineBody.setOutlineThickness(5);
    machineBody.setPosition(362, 184);

    // 슬롯머신 얼굴 박스
    sf::RectangleShape faceBox(sf::Vector2f(230, 100));
    faceBox.setFillColor(sf::Color(245, 230, 240)); // 연한 분홍
    faceBox.setOutlineColor(sf::Color(200, 180, 210)); // 연한 보라
    faceBox.setOutlineThickness(3);
    faceBox.setPosition(397, 214);

    // 눈 추가
    sf::CircleShape leftEye(12);
    leftEye.setFillColor(sf::Color::White);
    leftEye.setPosition(430, 234);  // 왼쪽 눈 위치 조정

    sf::CircleShape leftPupil(5);
    leftPupil.setFillColor(sf::Color::Black);
    leftPupil.setPosition(438, 240); // 왼쪽 동공 위치 조정

    sf::CircleShape rightEye(12);
    rightEye.setFillColor(sf::Color::White);
    rightEye.setPosition(560, 234);  // 오른쪽 눈 위치 조정

    sf::CircleShape rightPupil(5);
    rightPupil.setFillColor(sf::Color::Black);
    rightPupil.setPosition(568, 240); // 오른쪽 동공 위치 조정

    // 코 추가
    sf::CircleShape nose(4);
    nose.setFillColor(sf::Color(200, 150, 200)); // 연보라
    nose.setPosition(505, 260);

    // 레버 본체
    sf::RectangleShape lever(sf::Vector2f(20, 180));
    lever.setFillColor(sf::Color(180, 150, 200)); // 연한 보라색
    lever.setOrigin(7.5f, 100); // 회전 중심 설정
    lever.setPosition(703, 288);

    // 레버 손잡이
    sf::CircleShape leverHandle(18);
    leverHandle.setFillColor(sf::Color(205, 115, 213)); // 빨간 손잡이
    leverHandle.setOrigin(16, 16); // 중심 설정
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // 레버 연결 직사각형
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // 직사각형 연결 도형
    leverConnection.setFillColor(sf::Color(180, 150, 200)); // 연한 보라색
    leverConnection.setPosition(665, 349);

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
        window.draw(machineBody);    // 슬롯머신 본체
        window.draw(faceBox);        // 얼굴 박스
        window.draw(leftEye);        // 왼쪽 눈
        window.draw(leftPupil);      // 왼쪽 동공
        window.draw(rightEye);       // 오른쪽 눈
        window.draw(rightPupil);     // 오른쪽 동공
        window.draw(nose);           // 코
        window.draw(lever);          // 레버
        window.draw(leverHandle);    // 레버 손잡이
        window.draw(leverConnection); // 레버와 머신본체를 연결하는 직사각형 도형
        window.display();
    }

    return 0;
}
