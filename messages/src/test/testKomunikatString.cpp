#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageString)

BOOST_AUTO_TEST_CASE(TestMessageStringSerializeBezZmiany){
	MessageString komunikat(MessageType::Problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problem\",\"size\":222,\"control\":0},\"text\":{\"problem\":\"\"}}");

	komunikat = MessageString(MessageType::Login);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Login\",\"size\":222,\"control\":0},\"text\":{\"nick\":\"\"}}");

	komunikat = MessageString(MessageType::Solution);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Solution\",\"size\":222,\"control\":0},\"text\":{\"solution\":\"\"}}");
}

BOOST_AUTO_TEST_CASE(TestMessageStringDeserializeBezZmiany){
	size_t size = 222;
	DynamicJsonDocument docProblem(size);
	docProblem["header"]["type"] = "Problem";
	docProblem["header"]["size"] = size;
	docProblem["header"]["control"] = 0;
	docProblem["text"]["problem"] = "";
	MessageString komunikat(MessageType::Problem, docProblem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problem\",\"size\":222,\"control\":0},\"text\":{\"problem\":\"\"}}");

	DynamicJsonDocument docLogin(size);
	docLogin["header"]["type"] = "Login";
	docLogin["header"]["size"] = size;
	docLogin["header"]["control"] = 0;
	docLogin["text"]["nick"] = "";
	komunikat = MessageString(MessageType::Login, docLogin);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Login\",\"size\":222,\"control\":0},\"text\":{\"nick\":\"\"}}");

	DynamicJsonDocument docSolution(size);
	docSolution["header"]["type"] = "Solution";
	docSolution["header"]["size"] = size;
	docSolution["header"]["control"] = 0;
	docSolution["text"]["solution"] = "";
	komunikat = MessageString(MessageType::Solution, docSolution);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Solution\",\"size\":222,\"control\":0},\"text\":{\"solution\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()
