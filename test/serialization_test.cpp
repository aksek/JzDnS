//
// Created by aniela on 6/10/21.
//

#define BOOST_TEST_MODULE serialization_test
#include <boost/test/included/unit_test.hpp>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include "../src/message.h"

#include "../src/cryptography.hpp"


using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(serialization_encryption_test) {
    SerializeContent serializer;
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey key;
    Cryptography::load_public_key(key, "admin_public_key.pem");

    auto originalContent = serializer.serializeInt(2);

    int original_content_size = originalContent.first.size();

    std::string encrypted = Cryptography::asymmetric_encrypt(key, originalContent.first, rng);

    int encrypted_content_size = encrypted.size();

    Message originalMessage(MessageType::OK, "Admin", encrypted, originalContent.second);

    std::string serialized_message = originalMessage.serialize();

    int serialized_content_size = serialized_message.size();

    Message resultMessage(serialized_message);

    BOOST_CHECK_EQUAL(encrypted, resultMessage.getContentText());
}

BOOST_AUTO_TEST_CASE(serialization_long_test) {
    SerializeContent serializer;
    auto originalContent = serializer.serializeString("\\\"\\\\027\\\\350(\\\\341\\\\304!s\\\\350ӭ3\\\\334(a=f\\\\331\\\\345\\\\302V\\\\255\\\\270\\\\325\\\\352\\\\004\\\\063q\\\\217\\\\204\\\\376#\\\\005\\\\367\\\\367(C<\\\\277\\\\315x\\\\240\\\\342\\\\t-w\\\\256\\\\221Y\\\\230d\\\\372\\\\061\\\\231\\\\224lP\\\\331\\\\314\\\\330\\\\366\\\\212\\\\276լ6\\\\\\\\(F\\\\345\\\\003\\\\236\\\\270\\\\214t_/\\\\224A\\\\353\\\\312 \\\\274\\\\026\\\\341\\\\317Ώg*\\\\256&\\\\213\\\\033\\\\327{\\\\200\\\\065\\\\021\\\\017\\\\264\\\\325\\\\016\\\\243\\\\262\\\\262|U\\\\001\\\\246)n\\\\027KU\\\\302_\\\\373\\\\220a\\\\271\\\\a9\\\\234ٱg{\\\\372\\\\071\\\\n\\\\264.\\\\360\\\\006\\\\246\\\\021\\\\301\\\\264\\\\\\\"F\\\\240\\\\\\\"\\\\022\\\\221ݺ\\\\246\\\\264\\\\275\\\\322{\\\\r\\\\260\\\\214\\\\371\\\\211\\\\220\\\\017\\\\212\\\\000\\\\265%̝\\\\346\\\\t\\\\003hW\\\\337\\\\031\\\\325\\\\022}\\\\246\\\\343y\\\\372\\\\027\\\\064\\\\016\\\\375g\\\\261[\\\\302r\\\\002\\\\030\\\\036\\\\350[G!\\\\225\\\\362\\\\275\\\\026\\\\351\\\\n͘_\\\\351\\\\213\\\\001\\\\243\\\\377\\\\254(\\\\210w\\\\aȒ[\\\\276\\\\004\\\\252\\\\261\\\\347\\\\353\\\\002\\\\240\\\\242>݉\\\\262i\\\\264s3n\\\\032\\\\203\\\\240T\\\\277\\\\\\\\\\\\356v\\\\212\\\\221:j\\\\233E튵\\\\231b(\\\\242\\\\006;:ﳿ\\\\200\\\\360X\\\\206-\\\\236\\\\245\\\\065\\\\035-\\\\203L\\\\350\\\\252\\\\022w{ʹ\\\\251Ui\\\\275\\\\034\\\\037!\\\\334>\\\\273k\\\\f\\\\005:v\\\\372\\\\263\\\\334=D:ƭ\\\\b`en\\\\265Q\\\\030e\\\\325O{|\\\\356\\\\030\\\\037^\\\\335f&\\\\332}\\\\216\\\\250BC\\\\347\\\\357g\\\\320Y\\\\202\\\\vu\\\\024?\\\\r0[\\\\366\\\\270u6k\\\\334\\\\376-ɒ\\\\207y)\\\\367+%m\\\\\\\"~[\\\\371~\\\\034N\\\\210I\\\\353,v[\\\\333\\\\340{ߩ\\\\003\\\\226#\\\\370v=\\\\206_\\\"\"");

    Message originalMessage(MessageType::OK, "Admin", originalContent);

    std::string serialized_message = originalMessage.serialize();

    Message resultMessage(serialized_message);

    BOOST_CHECK_EQUAL(originalContent.first, resultMessage.getContentText());
}