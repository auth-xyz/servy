#ifndef CLICKY_HPP
#define CLICKY_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <iostream>
#include <algorithm>

class clicky {
public:
  clicky() {
    add_flag("help", "h", "Display this help message", false);
  }

  // Method to add named arguments
  void add_argument(const std::string& name, const std::string& alias, bool required, const std::string& description) {
    arguments_[name] = {alias, required, description, ""};
    if (!alias.empty()) alias_map_[alias] = name;
  }

  // Method to add flags (boolean options)
  void add_flag(const std::string& name, const std::string& alias = "", const std::string& description = "", bool default_value = false) {
    flags_[name] = {alias, default_value, description, false};
    if (!alias.empty()) alias_map_[alias] = name;
  }

  // Method to parse command-line arguments
  void parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];

      if (arg.starts_with("--")) arg = arg.substr(2);
      else if (arg.starts_with("-")) arg = alias_map_.count(arg.substr(1)) ? alias_map_[arg.substr(1)] : arg.substr(1);

      if (flags_.count(arg)) {
        flags_[arg].value = true;
      } else if (arguments_.count(arg)) {
        std::vector<std::string> values;
        while (i + 1 < argc && argv[i + 1][0] != '-') values.push_back(argv[++i]);
        if (!values.empty()) arguments_[arg].value = join_values(values);
        else if (arguments_[arg].required) missing_args_.push_back(arg);
      } else {
        // Treat unknown arguments as positional
        positional_args_.push_back(argv[i]);
      }
    }

    if (flag("help")) {
      print_help();
      exit(0);
    }

    for (const auto& [name, arg] : arguments_) {
      if (arg.required && arg.value.empty()) {
        std::cerr << "Missing required argument: " << name << '\n';
        missing_args_.push_back(name);
      }
    }

    if (!missing_args_.empty()) {
      print_help();
      exit(1);
    }
  }

  // Method to check if a flag is present
  bool flag(const std::string& name) const {
    return flags_.at(name).value;
  }

  // Method to get the value of a named argument
  std::string argument(const std::string& name) const {
    return arguments_.at(name).value;
  }

  // Method to retrieve positional arguments (unnamed)
  std::vector<std::string> positional_arguments() const {
    return positional_args_;
  }

  // Method to join multiple values for a single argument
  std::string join_values(const std::vector<std::string>& values) const {
    return std::accumulate(values.begin(), values.end(), std::string(),
                           [](const std::string& a, const std::string& b) {
                               return a.empty() ? b : a + "\n" + b;
                           });
  }

  // Print help message
  void print_help() const {
    size_t max_length = 0;

    // Lambda to calculate maximum length of arguments/flags for formatting
    auto calculate_max_length = [&](const auto& map) {
      for (const auto& [name, item] : map) {
        size_t length = name.length() + (item.alias.empty() ? 0 : item.alias.length() + 4);
        max_length = std::max(max_length, length);
      }
    };

    calculate_max_length(flags_);
    calculate_max_length(arguments_);

    std::cout << "Flags:\n";
    for (const auto& [name, flag] : flags_) {
      std::cout << "  --" << name;
      if (!flag.alias.empty()) std::cout << ", -" << flag.alias;

      size_t current_length = name.length() + (flag.alias.empty() ? 0 : flag.alias.length() + 4);
      size_t padding = max_length - current_length + 4;
      std::cout << std::string(padding, ' ') << ": " << flag.description
                << " (default: " << (flag.default_value ? "true" : "false") << ")\n";
    }

    std::cout << "\nArguments:\n";
    for (const auto& [name, arg] : arguments_) {
      std::cout << "  --" << name;
      if (!arg.alias.empty()) std::cout << ", -" << arg.alias;

      size_t current_length = name.length() + (arg.alias.empty() ? 0 : arg.alias.length() + 4);
      size_t padding = max_length - current_length + 4;
      std::cout << std::string(padding, ' ') << ": " << arg.description
                << (arg.required ? " (required)" : " (optional)") << '\n';
    }
  }

private:
  struct Flag {
    std::string alias;
    bool default_value;
    std::string description;
    bool value;
  };

  struct Argument {
    std::string alias;
    bool required;
    std::string description;
    std::string value;
  };

  // Maps to store flags, arguments, and alias references
  std::unordered_map<std::string, Flag> flags_;
  std::unordered_map<std::string, Argument> arguments_;
  std::unordered_map<std::string, std::string> alias_map_;

  // Vector to store arguments that are missing (required but not provided)
  std::vector<std::string> missing_args_;

  // Vector to store positional arguments (file, etc.)
  std::vector<std::string> positional_args_;
};

#endif // CLICKY_HPP

