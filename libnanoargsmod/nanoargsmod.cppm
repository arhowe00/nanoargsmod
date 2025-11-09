module;

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

export module nanoargsmod;

export namespace nanoargsmod {

class Args {
public:
    Args(int argc, char** argv) {
        program_name_ = (argc > 0) ? argv[0] : "";

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            // Handle -- (everything after is positional)
            if (arg == "--") {
                for (int j = i + 1; j < argc; ++j) {
                    positional_.push_back(argv[j]);
                }
                break;
            }

            // Handle --flag=value
            if (arg.starts_with("--")) {
                auto eq_pos = arg.find('=');
                if (eq_pos != std::string::npos) {
                    std::string key = arg.substr(2, eq_pos - 2);
                    std::string value = arg.substr(eq_pos + 1);
                    named_[key] = value;
                    continue;
                }

                // Handle --flag value
                std::string key = arg.substr(2);
                if (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
                    named_[key] = argv[++i];
                } else {
                    named_[key] = ""; // Flag without value
                }
                continue;
            }

            // Handle -f value
            if (arg.starts_with("-") && arg.size() > 1) {
                std::string key = arg.substr(1);
                if (i + 1 < argc && !std::string(argv[i + 1]).starts_with("-")) {
                    named_[key] = argv[++i];
                } else {
                    named_[key] = ""; // Flag without value
                }
                continue;
            }

            // Positional argument
            positional_.push_back(arg);
        }
    }

    std::string get(const std::string& name) const {
        auto it = named_.find(name);
        if (it == named_.end()) {
            throw std::runtime_error("Required argument '" + name + "' not found");
        }
        return it->second;
    }

    std::string get_or(const std::string& name, const std::string& default_val) const {
        auto it = named_.find(name);
        return (it != named_.end()) ? it->second : default_val;
    }

    int get_int(const std::string& name, int default_val = 0) const {
        auto it = named_.find(name);
        if (it == named_.end()) {
            return default_val;
        }
        try {
            return std::stoi(it->second);
        } catch (...) {
            throw std::runtime_error("Argument '" + name + "' is not a valid integer");
        }
    }

    double get_double(const std::string& name, double default_val = 0.0) const {
        auto it = named_.find(name);
        if (it == named_.end()) {
            return default_val;
        }
        try {
            return std::stod(it->second);
        } catch (...) {
            throw std::runtime_error("Argument '" + name + "' is not a valid double");
        }
    }

    bool has(const std::string& name) const { return named_.contains(name); }

    const std::vector<std::string>& positional() const { return positional_; }

    const std::string& program_name() const { return program_name_; }

private:
    std::string program_name_;
    std::map<std::string, std::string> named_;
    std::vector<std::string> positional_;
};

} // namespace nanoargsmod
