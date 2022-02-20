#include "Platform/Platform.hpp"

#define maxIterations 1000
#define POINTSAMOUNT 100

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
	const sf::Vector2f scaledScreenSize(screenSize[0] * screenScalingFactor, screenSize[1] * screenScalingFactor);
	// Use the screenScalingFactor
	window.create(sf::VideoMode(scaledScreenSize.x, scaledScreenSize.y), "Blob Thingo!");
	window.setFramerateLimit(60);
	platform.setIcon(window.getSystemHandle());

	background.create(screenSize[0] * screenScalingFactor, screenSize[1] * screenScalingFactor);

	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(1, 1));

	sf::RectangleShape backgroundRect;
	backgroundRect.setSize(scaledScreenSize);
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

	shader.setUniform("resolution", scaledScreenSize);

	sf::Glsl::Vec2 points[POINTSAMOUNT];
	sf::Glsl::Vec2 pointsMoveVec[POINTSAMOUNT];
	float pointsMag[POINTSAMOUNT];

	for (size_t i = 0; i < POINTSAMOUNT; i++)
	{
		points[i] = sf::Glsl::Vec2(static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (screenSize[0] * screenScalingFactor))), static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (screenSize[1] * screenScalingFactor))));
		pointsMoveVec[i] = sf::Glsl::Vec2(-1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f))), -1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (2.0f))));
		pointsMag[i] = 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (29.0f)));
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
			*currentPoint += *moveVec / 10.0f;
			if (currentPoint->x > scaledScreenSize.x && moveVec->x > 0)
				moveVec->x = -moveVec->x;
			else if (currentPoint->x < 0 && moveVec->x < 0)
				moveVec->x = -moveVec->x;
			else if (currentPoint->y > scaledScreenSize.y && moveVec->y > 0)
				moveVec->y = -moveVec->y;
			else if (currentPoint->y < 0 && moveVec->y < 0)
				moveVec->y = -moveVec->y;
		}

		background.clear();
		shader.setUniformArray("points", points, POINTSAMOUNT);
		shader.setUniformArray("pointsMag", pointsMag, POINTSAMOUNT);
		background.draw(backgroundRect, &shader);

		for (size_t i = 0; i < POINTSAMOUNT; i++)
		{
			circle.setPosition(points[i]);
			background.draw(circle);
		}

		const auto texture = background.getTexture();
		backgroundRect.setTexture(&texture);

		window.clear();
		window.draw(backgroundRect);

		window.display();
	}

	return 0;
}
