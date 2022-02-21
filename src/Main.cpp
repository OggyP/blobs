#include "Platform/Platform.hpp"

#define maxIterations 1000
#define POINTSAMOUNT 50

const int screenSize[2] = { 1500, 1000 };

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Hello World!" << std::endl;
#endif

	sf::RenderTexture background;
	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	UNUSED(screenScalingFactor);
	// Use the screenScalingFactor
#if defined(__linux__)
	window.create(sf::VideoMode(screenSize[0] * screenScalingFactor, screenSize[1] * screenScalingFactor), "Blob Thingo!");
	window.create(sf::VideoMode(1680, 1050), "Blob Thingo!", sf::Style::Resize | sf::Style::Fullscreen | sf::Style::Close);
#else
	window.create(sf::VideoMode(0, 0), "Blob Thingo!", sf::Style::Resize | sf::Style::Close);
	platform.toggleFullscreen(window.getSystemHandle(), sf::Style::Fullscreen, false, sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
#endif
	window.create(sf::VideoMode(window.getSize().x, window.getSize().y), "Blob Thingo!");
	window.setFramerateLimit(60);
	platform.setIcon(window.getSystemHandle());

	background.create(window.getSize().x, window.getSize().y);

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(1, 1));

	sf::RectangleShape backgroundRect;
	backgroundRect.setSize(sf::Vector2f(window.getSize()));
	backgroundRect.setPosition(sf::Vector2f(0, 0));

	sf::CircleShape circle(2);
	circle.setFillColor(sf::Color::Red);
	circle.setOrigin(1, 1);

	sf::Font font;
	if (!font.loadFromFile("./content/BAHNSCHRIFT.ttf"))
		std::cout << "Error Loading Font!\n";

	sf::Text text;
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(200);
	text.setFont(font);
	text.setCharacterSize(40);

	sf::Shader shader;
	if (!shader.loadFromFile("content/blobShader.glsl", sf::Shader::Fragment))
		std::cout << "Error Loading Shader!\n";

	if (!shader.isAvailable())
	{
		std::cout << "Shaders are not available\n";
	}

	shader.setUniform("resolution", sf::Vector2f(window.getSize()));

	sf::Glsl::Vec2 points[POINTSAMOUNT];
	sf::Glsl::Vec2 pointsMoveVec[POINTSAMOUNT];
	float pointsMag[POINTSAMOUNT];

	srand(time(0));

	for (size_t i = 0; i < POINTSAMOUNT; i++)
	{
		points[i] = sf::Glsl::Vec2(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (window.getSize().x))), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (window.getSize().y))));
		pointsMoveVec[i] = sf::Glsl::Vec2(-1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f))), -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f))));
		pointsMag[i] = 20.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (50.0f)));
	}

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for (size_t i = 0; i < POINTSAMOUNT; i++)
		{
			const auto currentPoint = &points[i];
			const auto moveVec = &pointsMoveVec[i];
			*currentPoint += *moveVec / 0.06f;
			if (currentPoint->x > window.getSize().x && moveVec->x > 0)
				moveVec->x = -moveVec->x;
			else if (currentPoint->x < 0 && moveVec->x < 0)
				moveVec->x = -moveVec->x;
			else if (currentPoint->y > window.getSize().y && moveVec->y > 0)
				moveVec->y = -moveVec->y;
			else if (currentPoint->y < 0 && moveVec->y < 0)
				moveVec->y = -moveVec->y;
		}

		background.clear();
		shader.setUniformArray("points", points, POINTSAMOUNT);
		shader.setUniformArray("pointsMag", pointsMag, POINTSAMOUNT);
		background.draw(backgroundRect, &shader);

		// for (size_t i = 0; i < POINTSAMOUNT; i++)
		// {
		// 	circle.setPosition(points[i]);
		// 	background.draw(circle);
		// }

		const auto texture = background.getTexture();
		backgroundRect.setTexture(&texture);

		window.clear();
		window.draw(backgroundRect);

		window.display();
	}

	return 0;
}
