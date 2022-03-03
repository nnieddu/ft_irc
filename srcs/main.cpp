
#include "../incs/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "[Error] : need 2 args <PORT> and <PASSWORD>" << std::endl;
		return(-1);
	}
	try
	{
		server	serv(atoi(av[1]), std::string(av[2]));
		serv.run();
	}
	catch (server::quitexcept er)
	{
		std::cout << er.what() << std::endl;
		return(0);
	}
	catch (std::runtime_error &er)
	{
		std::cerr << er.what() << ": " << std::strerror(errno) << std::endl;
	}
	catch (std::invalid_argument &er)
	{
		std::cerr << "[error]: invalid argument : " << er.what() << std::endl;
	}
	catch (std::exception &er)
	{
		std::cerr << er.what() << std::endl;
	}
	return(0);
}
