#include <SFML/Graphics.hpp>
#include <iostream> // 에러 메시지를 출력하기 위해 필요

using namespace std;

int main()
{
    // 1000x600 크기의 창 생성
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Screen");

    // mascote 이미지 로드
    sf::Texture mascoteTexture;
    if (!mascoteTexture.loadFromFile("mascote.png")) {
        cerr << "mascote 이미지 출력 안됨" << endl;
        return -1;
    }

    // uplever 이미지 로드
    sf::Texture upleverTexture;
    if (!upleverTexture.loadFromFile("uplever.png")) {
        cerr << "uplever 이미지 출력 안됨" << endl;
        return -1;
    }

    // downlever 이미지 로드
    sf::Texture downleverTexture;
    if (!downleverTexture.loadFromFile("downlever.png")) {
        cerr << "downlever 이미지 출력 안됨" << endl;
        return -1;
    }

    // mascote 스프라이트(이미지나 애니메이션을 나타내는 객체) 생성
    sf::Sprite mascote;
    mascote.setTexture(mascoteTexture);

    // uplever 스프라이트 생성
    sf::Sprite uplever;
    uplever.setTexture(upleverTexture);

    // downlever 스프라이트 생성
    sf::Sprite downlever;
    downlever.setTexture(downleverTexture);

    // 이미지 크기 조정
    mascote.setScale(0.5f, 0.5f);  // mascote 이미지 50% 크기로 축소
    uplever.setScale(0.5f, 0.5f);  // uplever 이미지 50% 크기로 축소
    downlever.setScale(0.5f, 0.5f); // downlever 이미지 50% 크기로 축소

    // 중앙 위치 계산
    sf::FloatRect mascoteBounds = mascote.getGlobalBounds();
    float centerX = (window.getSize().x - mascoteBounds.width) / 2.0f;
    float centerY = (window.getSize().y - mascoteBounds.height) / 2.0f;

    // mascote 이미지 중앙 위치
    mascote.setPosition(centerX, centerY);

    // 여백을 줄임
    float spacing = -56.0f;  // uplever 간격을 조절하기 위한 변수
    uplever.setPosition(centerX + mascoteBounds.width + spacing, centerY);
    downlever.setPosition(uplever.getPosition());

    bool upleverVisible = true; // uplever가 보이는지 안보이는지 확인

    // 게임 루프
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // 창을 닫을 때
            if (event.type == sf::Event::Closed)
                window.close();

            // 마우스 클릭 시 uplever를 클릭했는지 확인
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // 클릭 위치 가져오기
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                // uplever 클릭 시 uplever 숨기고 downlever 보이기
                if (upleverVisible && uplever.getGlobalBounds().contains(mousePos)) {
                    upleverVisible = false; // uplever 숨기기
                }
            }
        }
        window.clear(sf::Color::White); // 배경을 흰색으로 설정
        window.draw(mascote); // mascote 이미지 그리기
        if (upleverVisible)
            window.draw(uplever); // uplever가 보일 때만 그리기
        else
            window.draw(downlever); // uplever가 사라지면 downlever 그리기
        window.display(); // 화면 업데이트
    }

    return 0;
}
