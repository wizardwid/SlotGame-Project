#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(1024, 768), "index", sf::Style::Close); // 창 최대화 비활성화

    // 이미지 로드
    sf::Texture texture;
    if (!texture.loadFromFile("logo.png")) {  
        return -1;  // 이미지 로드를 실패한 경우 종료
    }

    sf::Sprite imageSprite;
    imageSprite.setTexture(texture);
    imageSprite.setPosition(-440, -440);  // 이미지가 화면 최상단에 위치하도록 설정

    // 슬롯머신 본체 (입체감 추가)
    sf::RectangleShape machineBodyFront(sf::Vector2f(300, 400));
    machineBodyFront.setFillColor(sf::Color(209, 233, 246));
    machineBodyFront.setOutlineColor(sf::Color(205, 193, 255)); 
    machineBodyFront.setOutlineThickness(5);
    machineBodyFront.setPosition(362, 270);

    // 뒷면 (그림자 효과)
    sf::RectangleShape machineBodyBack(sf::Vector2f(300, 400));
    machineBodyBack.setFillColor(sf::Color(217, 217, 217));
    machineBodyBack.setPosition(375, 276); 

    // 앞쪽 사각형 (입체감)
    sf::RectangleShape frontRectangle(sf::Vector2f(290, 100));
    frontRectangle.setFillColor(sf::Color(209, 233, 246));
    frontRectangle.setOutlineColor(sf::Color(205, 193, 255)); 
    frontRectangle.setOutlineThickness(3);
    frontRectangle.setPosition(367, 435); 

    // 앞쪽 사각형 그림자
    sf::RectangleShape frontRectangleShadow1(sf::Vector2f(295, 130)); 
    frontRectangleShadow1.setFillColor(sf::Color(205, 193, 255)); 
    frontRectangleShadow1.setPosition(364, 538); 

    // 슬롯머신 얼굴 박스
    sf::RectangleShape faceBox(sf::Vector2f(230, 100));
    faceBox.setFillColor(sf::Color(248, 232, 238));
    faceBox.setOutlineColor(sf::Color(205, 193, 255));
    faceBox.setOutlineThickness(3);
    faceBox.setPosition(397, 300);

    // 눈 추가
    sf::CircleShape leftEye(12);
    leftEye.setFillColor(sf::Color::White);
    leftEye.setPosition(430, 324);

    sf::CircleShape leftPupil(5);
    leftPupil.setFillColor(sf::Color::Black);
    leftPupil.setPosition(438, 330); 

    sf::CircleShape rightEye(12);
    rightEye.setFillColor(sf::Color::White);
    rightEye.setPosition(560, 324); 

    sf::CircleShape rightPupil(5);
    rightPupil.setFillColor(sf::Color::Black);
    rightPupil.setPosition(568, 330); 

    // 코 추가
    sf::CircleShape nose(4);
    nose.setFillColor(sf::Color(240, 168, 208)); 
    nose.setPosition(505, 340);

    // 큰 동그라미 4개
    sf::CircleShape bigCircle1(20);
    bigCircle1.setFillColor(sf::Color(147, 223, 255));
    bigCircle1.setPosition(402, 380);

    sf::CircleShape bigCircle2(20);
    bigCircle2.setFillColor(sf::Color(205, 193, 255));
    bigCircle2.setPosition(462, 380);

    sf::CircleShape bigCircle3(20);
    bigCircle3.setFillColor(sf::Color(255, 246, 227));
    bigCircle3.setPosition(522, 380);

    sf::CircleShape bigCircle4(20);
    bigCircle4.setFillColor(sf::Color(255, 204, 234));
    bigCircle4.setPosition(582, 380);

    // 작은 육각형 4개
    sf::CircleShape smallPentagon1(7, 6);
    smallPentagon1.setFillColor(sf::Color(218, 172, 255));
    smallPentagon1.setPosition(460, 450);

    sf::CircleShape smallPentagon2(7, 6);
    smallPentagon2.setFillColor(sf::Color(218, 172, 255));
    smallPentagon2.setPosition(490, 450);

    sf::CircleShape smallPentagon3(7, 6);
    smallPentagon3.setFillColor(sf::Color(218, 172, 255));
    smallPentagon3.setPosition(520, 450);

    sf::CircleShape smallPentagon4(7, 6);
    smallPentagon4.setFillColor(sf::Color(218, 172, 255));
    smallPentagon4.setPosition(550, 450);

    // 버튼 색깔 배열
    std::vector<sf::Color> colors = {
        sf::Color(147, 223, 255),  
        sf::Color(205, 193, 255), 
        sf::Color(255, 246, 227),
        sf::Color(255, 204, 234)  
    };

    // 4개의 직사각형 버튼 생성
    sf::RectangleShape rectangleButton1(sf::Vector2f(40, 40));
    rectangleButton1.setPosition(402, 480);
    rectangleButton1.setFillColor(colors[0]);

    sf::RectangleShape rectangleButton2(sf::Vector2f(40, 40));
    rectangleButton2.setPosition(462, 480);
    rectangleButton2.setFillColor(colors[1]);

    sf::RectangleShape rectangleButton3(sf::Vector2f(40, 40));
    rectangleButton3.setPosition(522, 480);
    rectangleButton3.setFillColor(colors[2]);

    sf::RectangleShape rectangleButton4(sf::Vector2f(40, 40));
    rectangleButton4.setPosition(582, 480);
    rectangleButton4.setFillColor(colors[3]);

    // 타이머
    sf::Clock clock;    // 시간이 경과한 양을 추적
    const sf::Time cycleTime = sf::seconds(0.3f); // 0.3초마다 색 변화
    sf::Time elapsedTime = sf::Time::Zero;  // 경과 시간 추적 변수

    // 직사각형 구멍
    sf::RectangleShape rectangleHole(sf::Vector2f(100, 10)); // 직사각형
    rectangleHole.setFillColor(sf::Color(165, 148, 249));
    rectangleHole.setPosition(461, 550);

    // 레버 본체
    sf::RectangleShape lever(sf::Vector2f(20, 180));
    lever.setFillColor(sf::Color(240, 193, 225));
    lever.setOrigin(7.5f, 100); // 회전 중심 설정
    lever.setPosition(703, 388);

    // 레버 손잡이
    sf::CircleShape leverHandle(18);
    leverHandle.setFillColor(sf::Color(203, 157, 240)); 
    leverHandle.setOrigin(16, 16); // 중심 설정
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // 레버 연결 직사각형
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // 직사각형 연결 도형
    leverConnection.setFillColor(sf::Color(240, 193, 225)); 
    leverConnection.setPosition(665, 449);

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

        // 시간 갱신
        elapsedTime += clock.restart(); // 누적된 시간 갱신

        // 색상 순환을 일정 시간마다만 갱신
        if (elapsedTime >= cycleTime) { // 0.3초보다 크면 순환 시작
            elapsedTime -= cycleTime; // 초과 시간을 보정

            // 색상 순환 (한 칸씩 오른쪽으로 이동)
            sf::Color lastColor = colors.back();    // lastColor : 마지막 색상
            for (int i = colors.size() - 1; i > 0; --i) {
                colors[i] = colors[i - 1];  // 계속 회전 오른쪽으로 밀어냄
            }
            colors[0] = lastColor;  // 마지막 색상을 배열의 첫 번째 위치로 이동

            // 버튼 색상 업데이트
            rectangleButton1.setFillColor(colors[0]);
            rectangleButton2.setFillColor(colors[1]);
            rectangleButton3.setFillColor(colors[2]);
            rectangleButton4.setFillColor(colors[3]);
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
        window.draw(imageSprite);   // 메인 로고
        window.draw(machineBodyBack); // 슬롯머신 뒷면 (그림자)
        window.draw(machineBodyFront); // 슬롯머신 본체 (앞면)
        window.draw(frontRectangle); // 앞쪽 사각형 (입체감 추가)
        window.draw(frontRectangleShadow1); // 앞쪽 사각형 그림자1
        window.draw(bigCircle1);  // 큰 동그라미  1
        window.draw(bigCircle2);  // 큰 동그라미  2
        window.draw(bigCircle3);  // 큰 동그라미  3
        window.draw(bigCircle4);  // 큰 동그라미  4
        window.draw(smallPentagon1);  // 작은 육각형  1
        window.draw(smallPentagon2);  // 작은 육각형  2
        window.draw(smallPentagon3);  // 작은 육각형  3
        window.draw(smallPentagon4);  // 작은 육각형  4
        window.draw(rectangleButton1);  // 직사각형 버튼 1
        window.draw(rectangleButton2);  // 직사각형 버튼 2
        window.draw(rectangleButton3);  // 직사각형 버튼 3
        window.draw(rectangleButton4);  // 직사각형 버튼 4
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
        window.display();             // 화면에 그린 내용을 출력
    }

    return 0;
}
