#include <catch2/catch_test_macros.hpp>
import nanoargsmod;

TEST_CASE("Args parses long flags with values") {
    const char* argv[] = {"prog", "--input", "file.txt", "--threads", "4"};
    nanoargsmod::Args args(5, const_cast<char**>(argv));

    REQUIRE(args.get("input") == "file.txt");
    REQUIRE(args.get("threads") == "4");
    REQUIRE(args.get_int("threads") == 4);
}

TEST_CASE("Args parses long flags with equals") {
    const char* argv[] = {"prog", "--input=file.txt", "--threads=4"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.get("input") == "file.txt");
    REQUIRE(args.get_int("threads") == 4);
}

TEST_CASE("Args parses short flags") {
    const char* argv[] = {"prog", "-i", "file.txt", "-t", "4"};
    nanoargsmod::Args args(5, const_cast<char**>(argv));

    REQUIRE(args.get("i") == "file.txt");
    REQUIRE(args.get_int("t") == 4);
}

TEST_CASE("Args handles mixed flag styles") {
    const char* argv[] = {"prog", "--input", "file.txt", "-t", "4", "--output=out.txt"};
    nanoargsmod::Args args(6, const_cast<char**>(argv));

    REQUIRE(args.get("input") == "file.txt");
    REQUIRE(args.get("t") == "4");
    REQUIRE(args.get("output") == "out.txt");
}

TEST_CASE("Args handles positional arguments") {
    const char* argv[] = {"prog", "file1.txt", "file2.txt"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.positional().size() == 2);
    REQUIRE(args.positional()[0] == "file1.txt");
    REQUIRE(args.positional()[1] == "file2.txt");
}

TEST_CASE("Args handles double dash separator") {
    const char* argv[] = {"prog", "--input", "file.txt", "--", "--not-a-flag", "file2.txt"};
    nanoargsmod::Args args(6, const_cast<char**>(argv));

    REQUIRE(args.get("input") == "file.txt");
    REQUIRE(args.positional().size() == 2);
    REQUIRE(args.positional()[0] == "--not-a-flag");
    REQUIRE(args.positional()[1] == "file2.txt");
}

TEST_CASE("Args get_or returns default when key not found") {
    const char* argv[] = {"prog", "--input", "file.txt"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.get_or("output", "default.txt") == "default.txt");
    REQUIRE(args.get_or("input", "default.txt") == "file.txt");
}

TEST_CASE("Args get_int returns default when key not found") {
    const char* argv[] = {"prog", "--threads", "4"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.get_int("threads") == 4);
    REQUIRE(args.get_int("workers", 8) == 8);
}

TEST_CASE("Args get_double parses floating point") {
    const char* argv[] = {"prog", "--threshold", "0.05"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.get_double("threshold") == 0.05);
    REQUIRE(args.get_double("other", 1.5) == 1.5);
}

TEST_CASE("Args has checks for key existence") {
    const char* argv[] = {"prog", "--input", "file.txt", "--verbose"};
    nanoargsmod::Args args(4, const_cast<char**>(argv));

    REQUIRE(args.has("input"));
    REQUIRE(args.has("verbose"));
    REQUIRE_FALSE(args.has("output"));
}

TEST_CASE("Args handles flags without values") {
    const char* argv[] = {"prog", "--verbose", "--debug"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.has("verbose"));
    REQUIRE(args.has("debug"));
    REQUIRE(args.get("verbose") == "");
}

TEST_CASE("Args get throws on missing required argument") {
    const char* argv[] = {"prog"};
    nanoargsmod::Args args(1, const_cast<char**>(argv));

    REQUIRE_THROWS_AS(args.get("input"), std::runtime_error);
}

TEST_CASE("Args get_int throws on invalid integer") {
    const char* argv[] = {"prog", "--threads", "not-a-number"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE_THROWS_AS(args.get_int("threads"), std::runtime_error);
}

TEST_CASE("Args get_double throws on invalid double") {
    const char* argv[] = {"prog", "--threshold", "not-a-number"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE_THROWS_AS(args.get_double("threshold"), std::runtime_error);
}

TEST_CASE("Args stores program name") {
    const char* argv[] = {"myprogram", "--input", "file.txt"};
    nanoargsmod::Args args(3, const_cast<char**>(argv));

    REQUIRE(args.program_name() == "myprogram");
}

TEST_CASE("Args handles empty arguments") {
    const char* argv[] = {"prog"};
    nanoargsmod::Args args(1, const_cast<char**>(argv));

    REQUIRE(args.positional().empty());
    REQUIRE_FALSE(args.has("anything"));
}
