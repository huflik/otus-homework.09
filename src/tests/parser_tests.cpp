#include <gtest/gtest.h>
#include "parser.h"

struct TestLogger : ILogger {
    std::vector<Bulk> bulks;

    void Log (const Bulk& bulk) override {
        bulks.push_back(bulk);
    }
};

TEST(ParserTest, StatcBlock) {
    auto processor = std::make_shared<Processor>();
    auto logger = std::make_shared<TestLogger>();

    processor->Subscribe(logger);
    Parser parser(3, processor);

    parser.Process("a");
    parser.Process("a");
    parser.Process("a");

    ASSERT_EQ(logger->bulks.size(),1);
    EXPECT_EQ(logger->bulks[0].commands.size(), 3);
}

TEST(ParserTest, DynamicBlock) {
    auto processor = std::make_shared<Processor>();
    auto logger = std::make_shared<TestLogger>();

    processor->Subscribe(logger);
    Parser parser(3, processor);

    parser.Process("{");
    parser.Process("a");
    parser.Process("a");
    parser.Process("}");

    ASSERT_EQ(logger->bulks.size(),1);
    EXPECT_EQ(logger->bulks[0].commands.size(), 2);
}

TEST(ParserTest, IgnoreWrongClosigBrace) {
    auto processor = std::make_shared<Processor>();
    auto logger = std::make_shared<TestLogger>();

    processor->Subscribe(logger);
    Parser parser(3, processor);


    parser.Process("a");
    parser.Process("}");
    parser.Process("a");
    parser.Eof();

    ASSERT_EQ(logger->bulks.size(),1);
    EXPECT_EQ(logger->bulks[0].commands.size(), 2);
}

TEST(ParserTest, MixedBlocks) {
    auto processor = std::make_shared<Processor>();
    auto logger = std::make_shared<TestLogger>();

    processor->Subscribe(logger);
    Parser parser(3, processor);


    parser.Process("cmd1");
    parser.Process("cmd2");
    parser.Process("{");
    parser.Process("cmd3");
    parser.Process("cmd4");
    parser.Process("}");
    parser.Process("{");
    parser.Process("cmd5");
    parser.Process("cmd6");
    parser.Process("{");
    parser.Process("cmd7");
    parser.Process("cmd8");
    parser.Process("}");
    parser.Process("cmd9");
    parser.Process("}");  
    parser.Process("{");
    parser.Process("cmd10");
    parser.Process("cmd11");
    parser.Eof();

    ASSERT_EQ(logger->bulks.size(),3);
    EXPECT_EQ(logger->bulks[0].commands.size(), 2);
    EXPECT_EQ(logger->bulks[1].commands.size(), 2);
    EXPECT_EQ(logger->bulks[2].commands.size(), 5);
}