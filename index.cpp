#include <SFML/Graphics.hpp>
#include <cmath>

int main()
{
    // â ����
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Simple Slot Machine");

    // ���Ըӽ� ��ü
    sf::RectangleShape machineBody(sf::Vector2f(300, 400));
    machineBody.setFillColor(sf::Color(255, 200, 221)); // ���� ��ũ
    machineBody.setOutlineColor(sf::Color(180, 150, 200)); // ���� �����
    machineBody.setOutlineThickness(5);
    machineBody.setPosition(362, 184);

    // ���Ըӽ� �� �ڽ�
    sf::RectangleShape faceBox(sf::Vector2f(230, 100));
    faceBox.setFillColor(sf::Color(245, 230, 240)); // ���� ��ȫ
    faceBox.setOutlineColor(sf::Color(200, 180, 210)); // ���� ����
    faceBox.setOutlineThickness(3);
    faceBox.setPosition(397, 214);

    // �� �߰�
    sf::CircleShape leftEye(12);
    leftEye.setFillColor(sf::Color::White);
    leftEye.setPosition(430, 234);  // ���� �� ��ġ ����

    sf::CircleShape leftPupil(5);
    leftPupil.setFillColor(sf::Color::Black);
    leftPupil.setPosition(438, 240); // ���� ���� ��ġ ����

    sf::CircleShape rightEye(12);
    rightEye.setFillColor(sf::Color::White);
    rightEye.setPosition(560, 234);  // ������ �� ��ġ ����

    sf::CircleShape rightPupil(5);
    rightPupil.setFillColor(sf::Color::Black);
    rightPupil.setPosition(568, 240); // ������ ���� ��ġ ����

    // �� �߰�
    sf::CircleShape nose(4);
    nose.setFillColor(sf::Color(200, 150, 200)); // ������
    nose.setPosition(505, 260);

    // ���� ��ü
    sf::RectangleShape lever(sf::Vector2f(20, 180));
    lever.setFillColor(sf::Color(180, 150, 200)); // ���� �����
    lever.setOrigin(7.5f, 100); // ȸ�� �߽� ����
    lever.setPosition(703, 288);

    // ���� ������
    sf::CircleShape leverHandle(18);
    leverHandle.setFillColor(sf::Color(205, 115, 213)); // ���� ������
    leverHandle.setOrigin(16, 16); // �߽� ����
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // ���� ���� ���簢��
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // ���簢�� ���� ����
    leverConnection.setFillColor(sf::Color(180, 150, 200)); // ���� �����
    leverConnection.setPosition(665, 349);

    // �ִϸ��̼� ���� ����
    bool isMouseOverLever = false;
    float leverAngle = 0.0f;
    float connectionWidth = 50.0f; // �ʱ� �ʺ�
    sf::Clock animationClock;

    // ���� ����
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // ���콺 �̵� �� ���� ���� �ִ��� Ȯ��
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                isMouseOverLever = leverHandle.getGlobalBounds().contains(mousePos);
            }

            // ���콺 ��ư�� ���� �� ���� ���·� ��ȯ
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isMouseOverLever = false;
            }
        }

        // ���� �ִϸ��̼�
        if (isMouseOverLever) {
            // ������ ��� ��
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle += 100.0f * elapsedTime; // ������ ����
            if (leverAngle > 10.0f)
                leverAngle = 10.0f;
        }
        else {
            // ���� ����
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle -= 100.0f * elapsedTime; // ������ ����
            if (leverAngle < 0.0f)
                leverAngle = 0.0f;
        }

        // ���� ȸ�� �� ������ ��ġ ����
        lever.setRotation(leverAngle);
        leverHandle.setPosition(
            lever.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180),
            lever.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180)
        );

        // ���� ������ ���� ���� ���簢�� �ʺ� ����
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; // �ʺ� ����
        leverConnection.setSize(sf::Vector2f(connectionWidth, 20));

        // ȭ�� �׸���
        window.clear(sf::Color::White);
        window.draw(machineBody);    // ���Ըӽ� ��ü
        window.draw(faceBox);        // �� �ڽ�
        window.draw(leftEye);        // ���� ��
        window.draw(leftPupil);      // ���� ����
        window.draw(rightEye);       // ������ ��
        window.draw(rightPupil);     // ������ ����
        window.draw(nose);           // ��
        window.draw(lever);          // ����
        window.draw(leverHandle);    // ���� ������
        window.draw(leverConnection); // ������ �ӽź�ü�� �����ϴ� ���簢�� ����
        window.display();
    }

    return 0;
}
