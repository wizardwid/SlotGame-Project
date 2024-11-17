#include <SFML/Graphics.hpp>
#include <iostream> // ���� �޽����� ����ϱ� ���� �ʿ�

using namespace std;

int main()
{
    // 1000x600 ũ���� â ����
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Screen");

    // mascote �̹��� �ε�
    sf::Texture mascoteTexture;
    if (!mascoteTexture.loadFromFile("mascote.png")) {
        cerr << "mascote �̹��� ��� �ȵ�" << endl;
        return -1;
    }

    // uplever �̹��� �ε�
    sf::Texture upleverTexture;
    if (!upleverTexture.loadFromFile("uplever.png")) {
        cerr << "uplever �̹��� ��� �ȵ�" << endl;
        return -1;
    }

    // downlever �̹��� �ε�
    sf::Texture downleverTexture;
    if (!downleverTexture.loadFromFile("downlever.png")) {
        cerr << "downlever �̹��� ��� �ȵ�" << endl;
        return -1;
    }

    // mascote ��������Ʈ(�̹����� �ִϸ��̼��� ��Ÿ���� ��ü) ����
    sf::Sprite mascote;
    mascote.setTexture(mascoteTexture);

    // uplever ��������Ʈ ����
    sf::Sprite uplever;
    uplever.setTexture(upleverTexture);

    // downlever ��������Ʈ ����
    sf::Sprite downlever;
    downlever.setTexture(downleverTexture);

    // �̹��� ũ�� ����
    mascote.setScale(0.5f, 0.5f);  // mascote �̹��� 50% ũ��� ���
    uplever.setScale(0.5f, 0.5f);  // uplever �̹��� 50% ũ��� ���
    downlever.setScale(0.5f, 0.5f); // downlever �̹��� 50% ũ��� ���

    // �߾� ��ġ ���
    sf::FloatRect mascoteBounds = mascote.getGlobalBounds();
    float centerX = (window.getSize().x - mascoteBounds.width) / 2.0f;
    float centerY = (window.getSize().y - mascoteBounds.height) / 2.0f;

    // mascote �̹��� �߾� ��ġ
    mascote.setPosition(centerX, centerY);

    // ������ ����
    float spacing = -56.0f;  // uplever ������ �����ϱ� ���� ����
    uplever.setPosition(centerX + mascoteBounds.width + spacing, centerY);
    downlever.setPosition(uplever.getPosition());

    bool upleverVisible = true; // uplever�� ���̴��� �Ⱥ��̴��� Ȯ��

    // ���� ����
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // â�� ���� ��
            if (event.type == sf::Event::Closed)
                window.close();

            // ���콺 Ŭ�� �� uplever�� Ŭ���ߴ��� Ȯ��
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                // Ŭ�� ��ġ ��������
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                // uplever Ŭ�� �� uplever ����� downlever ���̱�
                if (upleverVisible && uplever.getGlobalBounds().contains(mousePos)) {
                    upleverVisible = false; // uplever �����
                }
            }
        }
        window.clear(sf::Color::White); // ����� ������� ����
        window.draw(mascote); // mascote �̹��� �׸���
        if (upleverVisible)
            window.draw(uplever); // uplever�� ���� ���� �׸���
        else
            window.draw(downlever); // uplever�� ������� downlever �׸���
        window.display(); // ȭ�� ������Ʈ
    }

    return 0;
}
