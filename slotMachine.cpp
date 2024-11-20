#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    // â ����
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Slot Machine", sf::Style::Close); // â �ִ�ȭ ��Ȱ��ȭ

    // �̹��� �ε�
    sf::Texture texture;
    if (!texture.loadFromFile("logo2.png")) {
        return -1;  // �̹��� �ε带 ������ ��� ����
    }

    sf::Sprite imageSprite;
    imageSprite.setTexture(texture);
    imageSprite.setPosition(-440, -460);  // �̹����� ȭ�� �ֻ�ܿ� ��ġ�ϵ��� ����

    // �޸� (�׸��� ȿ��)
    sf::RectangleShape machineBodyBack(sf::Vector2f(820, 320));
    machineBodyBack.setFillColor(sf::Color(217, 217, 217));
    machineBodyBack.setPosition(75, 195);

    // ���Ըӽ� ��ü
    sf::RectangleShape machineBody(sf::Vector2f(800, 300));
    machineBody.setFillColor(sf::Color(191, 236, 255));
    machineBody.setOutlineColor(sf::Color(205, 193, 255));
    machineBody.setOutlineThickness(5);
    machineBody.setPosition(80, 210);

    // ���Ըӽ� �� �ڽ�
    sf::RectangleShape machineBox(sf::Vector2f(780, 280));
    machineBox.setFillColor(sf::Color(255, 251, 243));
    machineBox.setOutlineColor(sf::Color(205, 193, 255));
    machineBox.setOutlineThickness(3);
    machineBox.setPosition(90, 220);

    // �� ����
    const int lineCount = 4;
    const float lineWidth = 3;  // �� �β�
    const float spacing = machineBox.getSize().x / (lineCount + 1);  // �� ����
    sf::RectangleShape lines[lineCount];

    for (int i = 0; i < lineCount; ++i) {
        lines[i].setSize(sf::Vector2f(lineWidth, machineBox.getSize().y)); // �ڽ� ���̿� ���߱�, ���Ʒ� ���� �߰�
        lines[i].setFillColor(sf::Color(205, 193, 255));
        lines[i].setPosition(
            machineBox.getPosition().x + spacing * (i + 1) - lineWidth / 2,
            machineBox.getPosition().y// ���ʿ��� �ణ �������� ����
        );
    }

    // ���簢��1
    sf::RectangleShape rectangle1(sf::Vector2f(750, 30)); // ���簢��
    rectangle1.setFillColor(sf::Color(227, 246, 255));
    rectangle1.setPosition(104, 515);

    // ���簢��2
    sf::RectangleShape rectangle2(sf::Vector2f(30, 50)); // ���簢��
    rectangle2.setFillColor(sf::Color(205, 193, 255));
    rectangle2.setPosition(474, 515);

    // ���׶�� (���� ���� ������)
    sf::CircleShape circle(100);
    circle.setFillColor(sf::Color(250, 185, 255));
    circle.setPosition(390, 560);

    // ���� ��ü
    sf::RectangleShape lever(sf::Vector2f(20, 260));
    lever.setFillColor(sf::Color(240, 193, 225));
    lever.setOrigin(7.5f, 100); // ȸ�� �߽� ����
    lever.setPosition(930, 315);

    // ���� ������
    sf::CircleShape leverHandle(23);
    leverHandle.setFillColor(sf::Color(203, 157, 240));
    leverHandle.setOrigin(20, 20); // �߽� ����
    leverHandle.setPosition(lever.getPosition().x, lever.getPosition().y - 100);

    // ���� ���� ���簢��
    sf::RectangleShape leverConnection(sf::Vector2f(50, 20)); // ���簢�� ���� ����
    leverConnection.setFillColor(sf::Color(240, 193, 225));
    leverConnection.setPosition(880, 455);

    // �ִϸ��̼� ���� ����
    bool isMouseOverLever = false;
    float leverAngle = 0.0f;    // ���� ȸ�� ���� ����
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
            if (event.type == sf::Event::MouseMoved) {  // ���콺 ���� ��ġ Ȯ��
                sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);    // mousePos : ���콺�� �̵��� ���� ���� x, y ��ǥ
                isMouseOverLever = leverHandle.getGlobalBounds().contains(mousePos);    // contains(mousePos) : mousePos�� leverHandle�� ���� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
            }  // true or false

            // ���콺 ��ư�� ���� �� ���� ���·� ��ȯ
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) { //  ���콺 ��ư�� �������� �� & ���� ���콺 ��ư Ŭ��
                isMouseOverLever = false;
            }
        }

        // ���� �ִϸ��̼�
        if (isMouseOverLever) { // ���콺�� ���� ���� ���� ��
            // ������ ��� ��
            float elapsedTime = animationClock.restart().asSeconds();   // ���� �ִϸ��̼��� ����Ǵ� ���� ����� �ð��� ������. (������ �󸶳� ȸ������ ���)
            leverAngle += 100.0f * elapsedTime; // ������ ȸ�� ����
            if (leverAngle > 10.0f) // ȸ�� ������ 10�� �̻����� Ŀ���� �ʵ��� ����
                leverAngle = 10.0f;
        }
        else {  // ���콺�� ���� ���� ���� ��
            // ���� ����
            float elapsedTime = animationClock.restart().asSeconds();
            leverAngle -= 100.0f * elapsedTime;
            if (leverAngle < 0.0f)
                leverAngle = 0.0f;
        }

        // ���� ȸ�� �� ������ ��ġ ����
        lever.setRotation(leverAngle);
        leverHandle.setPosition(
            lever.getPosition().x + 100 * sin(leverAngle * 3.14159 / 180),  // lever.getPosition()�� ������ �߽� ��ġ ������
            lever.getPosition().y - 100 * cos(leverAngle * 3.14159 / 180)   // 3.14159 / 180 : ������ �������� ��ȯ�ϴ� �� (������ ����)

        );

        // ���� ������ ���� ���� ���簢�� �ʺ� ����
        connectionWidth = 50.0f - (leverAngle / 20.0f) * 30.0f; //  ���� ������ ����Ͽ� �ʺ� �ٿ���
        leverConnection.setSize(sf::Vector2f(connectionWidth, 20)); // ����

        // ȭ�� �׸���
        window.clear(sf::Color::White); 
        window.draw(imageSprite);       // �ΰ�2
        window.draw(machineBodyBack);   // ���Ըӽ� ��ü �׸���
        window.draw(machineBody);       // ���Ըӽ� ��ü
        window.draw(machineBox);        // ���Ըӽ� �� �ڽ�
        for (int i = 0; i < lineCount; ++i) {
            window.draw(lines[i]);      // �� �� �׸���
        }
        window.draw(rectangle1);     // ���簢��1
        window.draw(rectangle2);     // ���簢��2
        window.draw(circle);     // ���簢��2
        window.draw(lever);          // ����
        window.draw(leverHandle);    // ���� ������
        window.draw(leverConnection); // ������ �ӽź�ü�� �����ϴ� ���簢�� ����
        window.display(); 
    }

    return 0;
}
