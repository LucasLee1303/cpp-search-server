# SearchServer
SearchServer - система поиска документов по ключевым словам с поддержкой минус-слов, рейтинга и статуса документов.

Основные функции:

- ранжирование результатов поиска по статистической мере TF-IDF;
- обработка стоп-слов (не учитываются поисковой системой и не влияют на результаты поиска);
- обработка минус-слов (документы, содержащие минус-слова, не будут включены в результаты поиска);
- создание и обработка очереди запросов;
- удаление дубликатов документов;
- постраничное разделение результатов поиска;
- возможность работы в многопоточном режиме.

Стандарт С++17.
