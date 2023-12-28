-- Сделать запрос для получения атрибутов из указанных таблиц, применив фильтры по указанным условиям:
-- Таблицы: Н_ОЦЕНКИ, Н_ВЕДОМОСТИ.
-- Вывести атрибуты: Н_ОЦЕНКИ.КОД, Н_ВЕДОМОСТИ.ДАТА.
-- Фильтры (AND):
-- a) Н_ОЦЕНКИ.ПРИМЕЧАНИЕ = освобождение.
-- b) Н_ВЕДОМОСТИ.ИД < 1426978.
-- Вид соединения: LEFT JOIN.

SELECT 
    Н_ОЦЕНКИ.КОД, 
    Н_ВЕДОМОСТИ.ДАТА
FROM 
    Н_ОЦЕНКИ
    LEFT JOIN Н_ВЕДОМОСТИ ON Н_ОЦЕНКИ.КОД = Н_ВЕДОМОСТИ.ОЦЕНКА
WHERE 
    Н_ОЦЕНКИ.ПРИМЕЧАНИЕ = 'освобождение'
    AND Н_ВЕДОМОСТИ.ИД < 1426978;

