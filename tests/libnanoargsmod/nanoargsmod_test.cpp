#include <catch2/catch_test_macros.hpp>

import nanoargsmod;

TEST_CASE("CmdLineTool handles mixed positional and flags") {
    const char* argv[] = {"prog", "file1.txt", "--verbose", "--output", "out.txt", "file2.txt"};
    nanoargsmod::CmdLineTool cmd(6, const_cast<char**>(argv));

    auto pos = cmd.positional();
    REQUIRE(pos.size() == 2);
    REQUIRE(pos[0] == "file1.txt");
    REQUIRE(pos[1] == "file2.txt");
    REQUIRE(cmd.flag("--verbose"));
    REQUIRE(cmd.get<std::string>("--output").value() == "out.txt");
}

TEST_CASE("CmdLineTool handles double dash separator") {
    const char* argv[] = {"prog", "--input", "file.txt", "--", "--not-a-flag", "-x"};
    nanoargsmod::CmdLineTool cmd(6, const_cast<char**>(argv));

    REQUIRE(cmd.get<std::string>("--input").value() == "file.txt");
    auto pos = cmd.positional();
    REQUIRE(pos.size() == 2);
    REQUIRE(pos[0] == "--not-a-flag");
    REQUIRE(pos[1] == "-x");
    REQUIRE_FALSE(cmd.flag("--not-a-flag"));
    REQUIRE_FALSE(cmd.flag("-x"));
}

TEST_CASE("CmdLineTool type conversions") {
    const char* argv[] = {"prog", "--count", "42", "--ratio", "3.14"};
    nanoargsmod::CmdLineTool cmd(5, const_cast<char**>(argv));

    REQUIRE(cmd.get<int>("--count").value() == 42);
    REQUIRE(cmd.get<double>("--ratio").value() == 3.14);
    REQUIRE(cmd.get<int>("--missing").value_or(99) == 99);
    REQUIRE_THROWS_AS(cmd.get<int>("--ratio"), std::runtime_error);
}

TEST_CASE("CmdLineTool boolean flags") {
    const char* argv[] = {"prog", "--verbose", "-d"};
    nanoargsmod::CmdLineTool cmd(3, const_cast<char**>(argv));

    REQUIRE(cmd.flag("--verbose"));
    REQUIRE(cmd.flag("-d"));
    REQUIRE(cmd.get<std::string>("--verbose").value() == "");
    REQUIRE_FALSE(cmd.flag("--quiet"));
}

TEST_CASE("CmdLineTool handles zero argc") {
    nanoargsmod::CmdLineTool cmd(0, nullptr);

    REQUIRE(cmd.program_name() == "");
    REQUIRE(cmd.positional().empty());
    REQUIRE_FALSE(cmd.flag("anything"));
}
