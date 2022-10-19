#include "test_flow.h"



/**
 * @brief Testfall für Überprüfung allowNegatives
 * 
 */
void testNegative() {
        // Ohne decimal_shift
        std::vector<float> digits = { 1.2, 6.7};
        std::vector<float> analogs = { 9.5, 8.4};
        double preValue_extended = 16.985;
        double preValue = 16.98;
        
        const char* expected = "16.98";
        
        // extendResolution=false
        // da kein negativ, sollte kein Error auftreten
        UnderTestPost* underTestPost = init_do_flow(analogs, digits, Digital100, false, false, 0);
        setAllowNegatives(underTestPost, false);
        setPreValue(underTestPost, preValue);
        std::string result = process_doFlow(underTestPost);
        TEST_ASSERT_EQUAL_STRING("no error", underTestPost->getReadoutError().c_str());
        TEST_ASSERT_EQUAL_STRING(expected, result.c_str());
        delete underTestPost;

        // extendResolution=true
        // da negativ im Rahmen (letzte Stelle -0.2 > ergebnis), kein Error
        // Aber der PreValue wird gesetzt
        underTestPost = init_do_flow(analogs, digits, Digital100, false, true, 0);
        setAllowNegatives(underTestPost, false);
        setPreValue(underTestPost, preValue_extended);
        result = process_doFlow(underTestPost);
        TEST_ASSERT_EQUAL_STRING("no error", underTestPost->getReadoutError().c_str());
        TEST_ASSERT_EQUAL_STRING(underTestPost->RundeOutput(preValue_extended, analogs.size()+1).c_str(), result.c_str());
        delete underTestPost;

        // extendResolution=true
        // Tolleranz überschritten, Error wird gesetzt, kein ReturnValue
        preValue_extended = 16.988; // zu groß
        underTestPost = init_do_flow(analogs, digits, Digital100, false, true, 0);
        setAllowNegatives(underTestPost, false);
        setPreValue(underTestPost, preValue_extended);
        result = process_doFlow(underTestPost);
        TEST_ASSERT_EQUAL_STRING("Neg. Rate - Read:  - Raw: 16.984 - Pre: 16.988 ", underTestPost->getReadoutError().c_str());
        TEST_ASSERT_EQUAL_STRING("", result.c_str());
        delete underTestPost;

        // extendResolution=false
        // value < preValue
        preValue = 16.99; // zu groß
        underTestPost = init_do_flow(analogs, digits, Digital100, false, false, 0);
        setAllowNegatives(underTestPost, false);
        setPreValue(underTestPost, preValue_extended);
        result = process_doFlow(underTestPost);
        TEST_ASSERT_EQUAL_STRING("Neg. Rate - Read:  - Raw: 16.98 - Pre: 16.99 ", underTestPost->getReadoutError().c_str());
        TEST_ASSERT_EQUAL_STRING("", result.c_str());
        delete underTestPost;


        // extendResolution=false
        // value < preValue
        // Aber Prüfung abgeschaltet => kein Fehler
        preValue = 16.99; // zu groß
        underTestPost = init_do_flow(analogs, digits, Digital100, false, false, 0);
        setAllowNegatives(underTestPost, true);
        setPreValue(underTestPost, preValue_extended);
        result = process_doFlow(underTestPost);
        TEST_ASSERT_EQUAL_STRING("no error", underTestPost->getReadoutError().c_str());
        TEST_ASSERT_EQUAL_STRING(expected, result.c_str());
        delete underTestPost;

}
