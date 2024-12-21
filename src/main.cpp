#include "../include/systemd.hpp"
#include "clicky.hpp"

int main(int argc, char* argv[]) {
  Systemd sysd;
  clicky cli;

  std::string serviceName = "servy-service";
  std::string serviceDescription = "b";
  std::string serviceScript;
  std::string serviceCommand;

  cli.add_argument("name", "n", false, "The service's name");
  cli.add_argument("script", "s", true, "script to be executed");
  cli.add_argument("command", "c", true, "command to be used for script execution");
  cli.add_argument("description", "d", false, "the description for your systemd service");
  cli.parse(argc,argv);

  try {
    serviceScript = cli.argument("script");
    serviceCommand = cli.argument("command");

    if (cli.argument("name").empty())
    {
      sysd.createService(serviceName, serviceDescription, serviceScript, serviceCommand);
    } else 
    {
      serviceName = cli.argument("name");
      sysd.createService(serviceName, serviceDescription, serviceScript, serviceCommand);

    }

  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
