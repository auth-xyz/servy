#ifndef systemd
#define systemd

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

class Systemd {
public:
  int createService(const std::string& name, const std::string& description, const std::string& script, const std::string& command) {
    std::string templatePath = downloadTemplate();
    if (templatePath.empty()) {
      std::cerr << "Failed to download the template." << std::endl;
      return EXIT_FAILURE;
    }

    std::string serviceFilePath = (checkRoot()) 
      ? (std::string(home) + "/.config/systemd/user/s-" + name + ".service") 
      : "/etc/systemd/system/s-" + name + ".service";

    editTemplate(templatePath, description, script, command, serviceFilePath);
    startService(name);
    return EXIT_SUCCESS;
  }



  void displayServices() {
    for (const auto& entry : std::filesystem::directory_iterator("/etc/systemd/system/")) {
      if (entry.is_regular_file() && entry.path().filename().string().starts_with("servy-")) {
        std::cout << entry.path().filename() << std::endl;
      }
    }
  }

  void cleanUp() {
    for (const auto& entry : std::filesystem::directory_iterator("/etc/systemd/system/")) {
      if (entry.is_regular_file() && entry.path().filename().string().starts_with("servy-")) {
        std::filesystem::remove(entry.path());
        std::cout << "Removed: " << entry.path() << std::endl;
      }
    }

    system("systemctl daemon-reload");
  }

private:
  const char* home = std::getenv("HOME");
  bool ensureFile(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
  }

  int startService(const std::string& name) {
    try {
      const bool isUserMode = checkRoot();
      const std::string mode = isUserMode ? "--user " : "";

      auto buildCmd = [&](const std::string& action) {
        return "systemctl " + mode + action + " " + "s-" + name + ".service";
      };

      const std::string reloadCmd = "systemctl " + mode + "daemon-reload";
      const std::string enableCmd = buildCmd("enable");
      const std::string startCmd = buildCmd("start");

      if (system(reloadCmd.c_str()) != 0 ||
        system(enableCmd.c_str()) != 0 ||
        system(startCmd.c_str()) != 0) {
        std::cerr << "[servy] : Failed to start " << name << " service\n";
        return EXIT_FAILURE;
      }

      std::cout << "[servy] : " << name << " is running\n";
    } catch (const std::exception& err) {
      std::cerr << "Exception: " << err.what() << std::endl;
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }


  bool checkRoot() {
    bool root = false;
    if (getuid()) {
      root = true;
      return root;
    }
    else {
      return root;
    }
  }

  std::string downloadTemplate() {
    std::string url = "https://raw.githubusercontent.com/auth-xyz/assets/refs/heads/main/templates/template.service";
    std::string tempPath = "/tmp/template.service";
    std::string command = "wget -O " + tempPath + " " + url;

    if (!ensureFile(tempPath)) {
      if (system(command.c_str()) != 0) {
        std::cerr << "Failed to download template from: " << url << std::endl;
        return "";
      }
    }

    return tempPath;
  }

  void editTemplate(const std::string& templatePath, const std::string& description, const std::string& script, const std::string& command, const std::string& outputPath) {
    // Open the template file for reading
    std::ifstream inFile(templatePath);
    if (!inFile) {
      std::cerr << "Failed to open template file: " << templatePath << std::endl;
      return;
    }

    // Read the entire content of the template file
    std::ostringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    inFile.close();

    // Replace placeholders in the template
    size_t pos;
    while ((pos = content.find("{description}")) != std::string::npos) {
      content.replace(pos, 13, description);
    }
    while ((pos = content.find("{script}")) != std::string::npos) {
      content.replace(pos, 9, script + "\n");
    }
    while ((pos = content.find("{command}")) != std::string::npos) {
      content.replace(pos, 10, command);
    }

    pos = content.find("ExecStart=");
    if (pos != std::string::npos) {
      size_t endOfLine = content.find("\n", pos);
      if (endOfLine != std::string::npos) {
        content.insert(endOfLine, "\n");
      }
    }

    // Attempt to create and write to the output file
    std::cout << outputPath << std::endl;
    std::ofstream outFile(outputPath, std::ios::out | std::ios::trunc);
    if (!outFile) {
      std::cerr << "Failed to create or write to output file: " << outputPath << std::endl;
      return;
    }

    outFile << content;

    if (outFile.fail()) {
      std::cerr << "Failed to write content to output file: " << outputPath << std::endl;
    }

    outFile.close();

    // Uncomment if file permissions need to be set explicitly
    // system(("chmod 644 " + outputPath).c_str());
  }
};

#endif

