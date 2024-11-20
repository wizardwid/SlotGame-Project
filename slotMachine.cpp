#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close); // 창 최대화 비활성화

    // 이미지 로드
    sf::Texture texture;
    if (!texture.loadFromFile("logo2.png")) {
        return -1;  // 이미지 로드를 실패한 경우 종료
    }

    sf::Sprite imageSprite;
    imageSprite.setTexture(texture);
    imageSprite.setPosition(-440, -460);  // 이미지가 화면 최상단에 위치하도록 설정

    // 뒷면 (그림자 효과)
    sf::RectangleShape machineBodyBack(sf::Vector2f(820, 320));
    machineBodyBack.setFillColor(sf::Color(217, 217, 217));
    machineBodyBack.setPosition(75, 195);

    // 슬롯머신 몸체
    sf::RectangleShape machineBody(sf::Vector2f(800, 300));
    machineBody.setFillColor(sf::Color(191, 236, 255));
    machineBody.setOutlineColor(sf::Color(205, 193, 255));
    machineBody.setOutlineThickness(5);
    machineBody.setPosition(80, 210);

    // 슬롯머신 안 박스
    sf::RectangleShape machineBox(sf::Vector2f(780, 280));
    machineBox.setFillColor(sf::Color(255, 251, 243));
    machineBox.setOutlineColor(sf::Color(205, 193, 255));
    machineBox.setOutlineThickness(3);
    machineBox.setPosition(90, 220);

    // 선 생성
    const int lineCount = 4;
    const float lineWidth = 3;  // 선 두께
    const float spacing = machineBox.getSize().x / (lineCount + 1);  // 선 간격
    sf::RectangleShape lines[lineCount];

    for (int i = 0; i < lineCount; ++i) {
        lines[i].setSize(sf::Vector2f(lineWidth, machineBox.getSize().y)); // 박스 높이에 맞추기, 위아래 간격 추가
        lines[i].setFillColor(sf::Color(205, 193, 255));
        lines[i].setPosition(
            machineBox.getPosition().x + spacing * (i + 1) - lineWidth / 2,
            machineBox.getPosition().y// 위쪽에서 약간 내려오게 설정
        );
    }

    // 직사각형1
    sf::RectangleShape rectangle1(sf::Vector2f(750, 30)); // 직사각형
    rectangle1.setFillColor(sf::Color(227, 246, 255));
    rectangle1.setPosition(104, 515);

    // 직사각형2
    sf::RectangleShape rectangle2(sf::Vector2f(30, 50)); // 직사각형
    rectangle2.setFillColor(sf::Color(205, 193, 255));
    rectangle2.setPosition(474, 515);

    // 동그라미 (랜덤 색깔 나오는)
    sf::CircleShape circle(100);
    circle.setFillColor(sf::Color(250, 185, 255));
    circle.setPosition(390, 560);

    // 레버 본체
    sf::RectangleShape lever(sf::Vector2f(20, 260));
    lever.setFillColor(sf::Color(240, 193, 225));
    lever.setOrigin(7.5f, 100); // 회전 중심 설정
    lever.setPosition(930, 315);

    // 레버 손잡이
    sf::CircleShape leverHandle(23);
    leverHandle.setFillColor(sf::Color(203, 157, 240));
    leverHandle.setOrigin(20, 20); // 중심 설정
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // 레버 연결 직사각형
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // 직사각형 연결 도형
    leverConnection.setFillColor(sf::Color(240, 193, 225));
    leverConnection.setPosition(880, 455);

    // 애니메이션 상태 변수
    bool isMouseOverLever = false;
    float leverAngle = 0.0f;    // 레버 회전 각도 변수
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
            if (event.type == sf::Event::MouseMoved) {  // 마우스 현재 위치 확인
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);    // mousePos : 마우스가 이동할 때의 현재 x, y 좌표
                isMouseOverLever = leverHandle.getGlobalBounds().contains(mousePos);    // contains(mousePos) : mousePos가 leverHandle의 영역 안에 있는지 확인하는 함수
            }  // true or false

            // 마우스 버튼을 뗐을 때 복귀 상태로 전환
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  마우스 버튼이 떨어졌을 때 & 왼쪽 마우스 버튼 클릭
                isMouseOverLever = false;
            }
        }

        // 레버 애니메이션
        if (isMouseOverLever) { // 마우스가 레버 위에 있을 때
            // 레버를 당길 때
            float elapsedTime = animationClock.restart().asSeconds();   // 레버 애니메이션이 진행되는 동안 경과한 시간을 가져옴. (레버가 얼마나 회전할지 계산)
            leverAngle += 100.0f * elapsedTime; // 레버의 회전 각도
            if (leverAngle > 10.0f) // 회전 각도가 10도 이상으로 커지지 않도록 제한
                leverAngle = 10.0f;
        }
        else {  // 마우스가 레버 위에 없을 때
            // 레버 복귀
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle -= 100.0f * elapsedTime;
            if (leverAngle < 0.0f)
                leverAngle = 0.0f;
        }

        // 레버 회전 및 손잡이 위치 조정
        lever.setRotation(leverAngle);
        leverHandle.setPosition(
            lever.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180),  // lever.getPosition()은 레버의 중심 위치 가져옴
            lever.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180)   // 3.14159 / 180 : 각도를 라디안으로 변환하는 값 (수학적 공식)

        );

        // 레버 각도에 따라 연결 직사각형 너비 변경
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; //  레버 각도에 비례하여 너비를 줄여줌
        leverConnection.setSize(sf::Vector2f(connectionWidth, 20)); // 적용

        // 화면 그리기
        window.clear(sf::Color::White); 
        window.draw(imageSprite);       // 로고2
        window.draw(machineBodyBack);   // 슬롯머신 몸체 그림자
        window.draw(machineBody);       // 슬롯머신 몸체
        window.draw(machineBox);        // 슬롯머신 안 박스
        for (int i = 0; i < lineCount; ++i) {
            window.draw(lines[i]);      // 각 선 그리기
        }
        window.draw(rectangle1);     // 직사각형1
        window.draw(rectangle2);     // 직사각형2
        window.draw(circle);     // 직사각형2
        window.draw(lever);          // 레버
        window.draw(leverHandle);    // 레버 손잡이
        window.draw(leverConnection); // 레버와 머신본체를 연결하는 직사각형 도형
        window.display(); 
    }

    return 0;
}
