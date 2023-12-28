-- Выдать различные фамилии людей и число людей с каждой из этих фамилий, ограничив список фамилиями, встречающимися менее 10 раз на кафедре вычислительной техники.
-- Для реализации использовать подзапрос.

SELECT 
    Н_ЛЮДИ.ФАМИЛИЯ, 
    COUNT(*)
FROM
    Н_ЛЮДИ
WHERE 
    Н_ЛЮДИ.ФАМИЛИЯ NOT IN (
        SELECT 
            Н_ЛЮДИ.ФАМИЛИЯ
        FROM
            Н_ЛЮДИ
            LEFT JOIN Н_ВЕДОМОСТИ ON Н_ЛЮДИ.ИД = Н_ВЕДОМОСТИ.ЧЛВК_ИД
            LEFT JOIN Н_ОТДЕЛЫ ON Н_ВЕДОМОСТИ.ОТД_ИД = Н_ОТДЕЛЫ.ИД
        WHERE 
            Н_ОТДЕЛЫ.ИМЯ_В_ИМИН_ПАДЕЖЕ = 'кафедра вычислительной техники'
        GROUP BY 
            Н_ЛЮДИ.ФАМИЛИЯ
        HAVING 
            COUNT(*) >= 10
    )
GROUP BY 
    Н_ЛЮДИ.ФАМИЛИЯ;