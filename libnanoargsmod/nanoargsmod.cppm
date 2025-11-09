module;

#include <charconv>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module nanoargsmod;

export namespace nanoargsmod {

/// Minimal command line parser with zero dependencies.
/// Parses all arguments in constructor for immutable access to results.
/// Supports --long, -short, --flag, positional args, and -- separator.
/// Limitations: No bundled short flags (-xvf), no option aliases,
/// no help generation, no multi-value options, no subcommands,
/// no pre-declaration of option schemas.
class CmdLineTool {
public:
    /// Parses command line arguments into named options and positional arguments.
    CmdLineTool(int argc, char** argv) : program_name_{argc > 0 ? argv[0] : ""} {
        bool after_separator = false;

        for (int i = 1; i < argc; ++i) {
            std::string_view arg{argv[i]};

            // Everything after -- is positional
            if (!after_separator && arg == "--") {
                after_separator = true;
                continue;
            }

            if (after_separator) {
                positional_args_.emplace_back(argv[i]);
                continue;
            }

            // Long-form option: --name or --name=value
            if (arg.starts_with("--")) {
                auto name_part = arg.substr(2);
                auto eq_pos = name_part.find('=');

                if (eq_pos != std::string_view::npos) {
                    // --name=value format
                    auto name = std::string{name_part.substr(0, eq_pos)};
                    auto value = std::string{name_part.substr(eq_pos + 1)};
                    options_[std::move(name)] = std::move(value);
                } else {
                    // --name format (might have following value)
                    auto name = std::string{name_part};
                    if (i + 1 < argc && argv[i + 1][0] != '-') {
                        options_[std::move(name)] = argv[++i];
                    } else {
                        options_[std::move(name)] = ""; // Boolean flag
                    }
                }
                continue;
            }

            // Short-form option: -n
            if (arg.starts_with("-") && arg.size() > 1) {
                auto name = std::string{arg.substr(1)};
                if (i + 1 < argc && argv[i + 1][0] != '-') {
                    options_[std::move(name)] = argv[++i];
                } else {
                    options_[std::move(name)] = ""; // Boolean flag
                }
                continue;
            }

            // Positional argument
            positional_args_.emplace_back(argv[i]);
        }
    }

    /// Returns parsed value of type T for named option, or nullopt if absent.
    template <typename T>
    [[nodiscard]] std::optional<T> get(std::string_view name) const;

    /// Returns true if named option or flag was present.
    [[nodiscard]] bool flag(std::string_view name) const noexcept {
        return options_.find(std::string{name}) != options_.end();
    }

    /// Returns span of all positional arguments.
    [[nodiscard]] std::span<const std::string> positional() const noexcept {
        return positional_args_;
    }

    /// Returns view of program name (argv[0]).
    [[nodiscard]] std::string_view program_name() const noexcept { return program_name_; }

private:
    std::string_view program_name_;
    std::unordered_map<std::string, std::string> options_;
    std::vector<std::string> positional_args_;
};

// Template specialization for std::string
template <>
[[nodiscard]] std::optional<std::string>
CmdLineTool::get<std::string>(std::string_view name) const {
    auto it = options_.find(std::string{name});
    if (it == options_.end()) {
        return std::nullopt;
    }
    return it->second;
}

// Template specialization for int
template <>
[[nodiscard]] std::optional<int> CmdLineTool::get<int>(std::string_view name) const {
    auto it = options_.find(std::string{name});
    if (it == options_.end()) {
        return std::nullopt;
    }

    int value{};
    auto [ptr, ec] =
        std::from_chars(it->second.data(), it->second.data() + it->second.size(), value);

    if (ec != std::errc{} || ptr != it->second.data() + it->second.size()) {
        throw std::runtime_error("Option '" + std::string{name} + "' with value '" + it->second +
                                 "' cannot be parsed as int");
    }

    return value;
}

// Template specialization for double
template <>
[[nodiscard]] std::optional<double> CmdLineTool::get<double>(std::string_view name) const {
    auto it = options_.find(std::string{name});
    if (it == options_.end()) {
        return std::nullopt;
    }

    double value{};
    auto [ptr, ec] =
        std::from_chars(it->second.data(), it->second.data() + it->second.size(), value);

    if (ec != std::errc{} || ptr != it->second.data() + it->second.size()) {
        throw std::runtime_error("Option '" + std::string{name} + "' with value '" + it->second +
                                 "' cannot be parsed as double");
    }

    return value;
}

} // namespace nanoargsmod
