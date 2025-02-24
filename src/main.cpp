#include "../include/systemd.hpp"
#include <clicky/clicky.hpp>

int main(int argc, char* argv[]) {
  Systemd sysd;
  clicky cli;

  std::string serviceName = "servy-service";
  std::string serviceDescription = "b";
  std::string serviceScript;
  std::string serviceCommand;

  /* cli.add_get_argument("name", "n", false, "The service's name"); */
  /* cli.add_get_argument("script", "s", true, "script to be executed"); */
  /* cli.add_get_argument("command", "c", true, "command to be used for script execution"); */
  /* cli.add_get_argument("description", "d", false, "the description for your systemd service"); */
  cli.bulk_add_arguments({
    {"name", "n", "service filename", false},
    {"description", "", "description for service", false},
    {"script", "s", "script to run when service is ran", true},
    {"command", "c", "command to run script (eg:/usr/bin/bash)", true},
  });
  cli.parse(argc,argv);

  try {
    serviceScript = cli.get_argument("script");
    serviceCommand = cli.get_argument("command");

    if (cli.has_argument("name"))
    {
      sysd.createService(serviceName, serviceDescription, serviceScript, serviceCommand);
    } else 
    {
      serviceName = cli.get_argument("name");
      sysd.createService(serviceName, serviceDescription, serviceScript, serviceCommand);

    }

  }
  catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
