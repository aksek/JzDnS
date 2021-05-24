#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessagePairStringString)

BOOST_AUTO_TEST_CASE(TestMessagePairStringStringSerializeBezZmiany){
	MessagePairStringString komunikat(MessageType::Round_over);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Round_over\",\"size\":269,\"control\":0},\"text\":{\"winner_nick\":\"\",\"solution\":\"\"}}");
	
	komunikat = MessagePairStringString(MessageType::New_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"New_problem\",\"size\":269,\"control\":0},\"text\":{\"problem_content\":\"\",\"problem_answer\":\"\"}}");
}

BOOST_AUTO_TEST_CASE(TestMessagePairStringStringDeserializeBezZmiany){
	size_t size = 269;
	DynamicJsonDocument docRoundOver(size);
	docRoundOver["header"]["type"] = "Round_over";
	docRoundOver["header"]["size"] = size;
	docRoundOver["header"]["control"] = 0;
	docRoundOver["text"]["winner_nick"] = "";
	docRoundOver["text"]["solution"] = "";
	MessagePairStringString komunikat(MessageType::Round_over, docRoundOver);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Round_over\",\"size\":269,\"control\":0},\"text\":{\"winner_nick\":\"\",\"solution\":\"\"}}");
	
	DynamicJsonDocument docNewProblem(size);
	docNewProblem["header"]["type"] = "New_problem";
	docNewProblem["header"]["size"] = size;
	docNewProblem["header"]["control"] = 0;
	docNewProblem["text"]["problem_content"] = "";
	docNewProblem["text"]["problem_answer"] = "";
	komunikat = MessagePairStringString(MessageType::New_problem, docNewProblem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"New_problem\",\"size\":269,\"control\":0},\"text\":{\"problem_content\":\"\",\"problem_answer\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()
