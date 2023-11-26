#include "../devgame.h"
#include "../forces.h"

class MessageService
{
public:
	static std::shared_ptr<MessageService> create()
	{
		return std::make_shared<MessageService>();
	}

};

class DevClient
{
public:
	explicit DevClient(std::shared_ptr<MessageService> broker, const Game& game)
	{
	}
};


int main()
{
	auto broker = MessageService::create();

	DevGame game;
	DevClient client(broker, game);

	return 0;
}
