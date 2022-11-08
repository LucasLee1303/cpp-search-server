// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
            "Stop words must be excluded from documents"s);
    }
}

/*
1. тест на добавление документов можно было бы организовать проще - используя метод, который возвращает количество документов___Исправила
*/
void TestDocumentsAdding() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };

    //Было
   /* {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("dog near red town"s);
        ASSERT(found_docs.empty());
    }
    */
    //Стало
    {
        SearchServer server;
        ASSERT_EQUAL(server.GetDocumentCount(), 0);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        server.AddDocument(doc_id + 1, "dog near red town", DocumentStatus::ACTUAL, ratings);
        ASSERT_EQUAL(server.GetDocumentCount(), 2);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }

}

/*2. у вас же есть уже реализация на исключение стоп слов___Убрала тест__Исправила*/
/*
void TestStopWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const string stop_words = "in the"s;
    const vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in the town"s).empty());
    }
    {
        SearchServer server;
        server.SetStopWords(stop_words);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }
}
*/
void TestMinusWordsSupporting() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in the -city"s).empty());
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in the city"s);
        ASSERT(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        ASSERT(doc0.id == doc_id);
    }

}


void TestMatching() {
  const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };


    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [matched_words, doc_status] = server.MatchDocument("dog in the big city"s, doc_id);
        //3. v, ds - стоит делать более информативные наименования переменных, не ленитесь с этим. через пару недель будет трудно вспомнить, 
        //что подразумевали под данными переменными___Исправила
        ASSERT(matched_words.size() == 3);
        //4. проверка на матчинг - это не только количество слов, стоит сравнить и наборы слов___Исправила
        
       
    }
    //сделала так
   {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto [matched_words, doc_status] = server.MatchDocument("dog in the big city"s, doc_id);
        const vector<string> matched_result = {"city"s};
        ASSERT_EQUAL(matched_result, matched_words);
    }



   {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        auto [matched_words, doc_status] = server.MatchDocument("dog in the big -city"s, doc_id);
        ASSERT(matched_words.empty());
    }

}

void TestRelevanceSorting() {

    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_3);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(found_docs[2].id == doc_id_1);

    }
    /*5. достаточно одного блока для проверки___Убрала блок___Исправила*/
    /* {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };
        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };
        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };
        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_1);
        //6. к тому же у вас в этом блоке в каком порядке добавлены документы в сервер, в таком и выдаются - не видна сортировка
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(found_docs[2].id == doc_id_3);
    }*/


}
/*7. не хватает глагола в наименовании теста. Что за действие вы проверяете?___Исправила*/
//Было void TestRating() 
//Стало
void TestRatingCalculation()
{

    {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].rating == round ((1 + 2 + 3 + 0.0) / 3)); /* 8. отлично, но по стилю совет - всегда ставьте вокруг бинарных операций по одному пробелу!___Добавила пробелы*/
    }
}
/*9. не хватает глагола. Лучше "тест поиска документов по предикату"___Исправила*/
// Было void TestPredicate() 
// Стало
void TestSearchDocumentsByPredicate ()
{

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = { 1, 2, 3 };
    const DocumentStatus document_status = DocumentStatus::ACTUAL;

   /* {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(found_docs.empty());
    } убрала 2 блока*/
    /*10. первый два блока относятся скорее к поиску по статусу___Исправила*/
    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, []([[maybe_unused]] int document_id, DocumentStatus status, int rating)
            { 
            document_id += 1; /*11. [[maybe_unused]]___Исправила*/
            if (status == DocumentStatus::ACTUAL) { document_id += 1; };
            return rating > 1;
            });
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, []([[maybe_unused]] int document_id, DocumentStatus status, int rating) {
            document_id += 1; /*12.[[maybe_unused]]___Исправила */
            if (status == DocumentStatus::ACTUAL) { document_id += 1; };
            return rating > 3;
            });
        ASSERT(found_docs.empty());
    }

}
/*13. "тест поиска документов по статусу"___Исправила*/
// Было void TestStatus() 
// Стало
void TestSearchDocumentsByStatus ()
{

    const int doc_id = 42;
    const string content = "cat in the city city city"s;
    const vector<int> ratings = { 1, 2, 3 };
    const DocumentStatus document_status = DocumentStatus::BANNED;

    {
        SearchServer server;
        server.AddDocument(doc_id, content, document_status, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::ACTUAL);
        ASSERT(found_docs.empty());
    }

    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s, DocumentStatus::BANNED);
        ASSERT(!found_docs.empty());
        ASSERT(found_docs[0].id == doc_id);
    }


}

void TestRelevanceAccuracy() {

  {
        const int doc_id_1 = 42;
        const string content_1 = "cat in the city city city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 43;
        const string content_2 = "fox fox the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 44;
        const string content_3 = "dog dog dog in the city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);
        ASSERT(found_docs[0].id == doc_id_3);
        ASSERT(abs(found_docs[0].relevance - log(3) / 2) < 1e-6);
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - log(3) / 3) < 1e-6);
        ASSERT(found_docs[2].id == doc_id_1);
        ASSERT(abs(found_docs[2].relevance - log(3) / 6) < 1e-6);

    }

    {
        const int doc_id_1 = 52;
        const string content_1 = "cat fox dog in the city"s;
        const vector<int> ratings_1 = { 1, 2, 3 };

        const int doc_id_2 = 53;
        const string content_2 = "fox dog the city city city"s;
        const vector<int> ratings_2 = { 4, 5, 6 };

        const int doc_id_3 = 54;
        const string content_3 = "dog in the city city city"s;
        const vector<int> ratings_3 = { 2, 4, 2 };

        SearchServer server;
        server.AddDocument(doc_id_1, content_1, DocumentStatus::ACTUAL, ratings_1);
        server.AddDocument(doc_id_2, content_2, DocumentStatus::ACTUAL, ratings_2);
        server.AddDocument(doc_id_3, content_3, DocumentStatus::ACTUAL, ratings_3);
        const auto found_docs = server.FindTopDocuments("cat fox dog"s);
        ASSERT(found_docs.size() == 3);

        ASSERT(found_docs[0].id == doc_id_1);
        ///14. в первом блоке формулу использовали. а здесь также стоит сделать с формулой___ Исправила
        ASSERT(abs(found_docs[0].relevance - (log((3 * 1.0) / 1) + log((3 * 1.0) / 2)) / 6) < 1e-6);     // работает
        ASSERT(found_docs[1].id == doc_id_2);
        ASSERT(abs(found_docs[1].relevance - log((3 * 1.0) / 2) * (1.0 / 4.0) / 1.5) < 1e-6); //работает
        ASSERT(found_docs[2].id == doc_id_3);
        ASSERT(abs(found_docs[2].relevance - log (1) / 6) < 1e-6);//работает

    }

}


// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestDocumentsAdding);
    //RUN_TEST(TestStopWordsSupporting);
    RUN_TEST(TestMinusWordsSupporting);
    RUN_TEST(TestMatching);
    RUN_TEST(TestRelevanceSorting);
    RUN_TEST(TestRatingCalculation);
    RUN_TEST(TestSearchDocumentsByPredicate);
    RUN_TEST(TestSearchDocumentsByStatus);
    RUN_TEST(TestRelevanceAccuracy);
    // Не забудьте вызывать остальные тесты здесь
}

// --------- Окончание модульных тестов поисковой системы -----------
