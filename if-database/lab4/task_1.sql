-- Tables: Н_ОЦЕНКИ, Н_ВЕДОМОСТИ.
-- Output attributes: Н_ОЦЕНКИ.ПРИМЕЧАНИЕ, Н_ВЕДОМОСТИ.ЧЛВК_ИД.
-- Filters (AND):
-- a) Н_ОЦЕНКИ.КОД < осв.
-- b) Н_ВЕДОМОСТИ.ДАТА > 2010-06-18.
-- Type of join: LEFT JOIN.

SELECT Н_ОЦЕНКИ.ПРИМЕЧАНИЕ, Н_ВЕДОМОСТИ.ЧЛВК_ИД
FROM
    Н_ОЦЕНКИ LEFT JOIN Н_ВЕДОМОСТИ ON Н_ОЦЕНКИ.КОД = Н_ВЕДОМОСТИ.ОЦЕНКА
WHERE 
    Н_ОЦЕНКИ.КОД < 'осв' 
    AND Н_ВЕДОМОСТИ.ДАТА > '2010-06-18';


                                                             QUERY PLAN                                                              
-------------------------------------------------------------------------------------------------------------------------------------
 Hash Join  (cost=300.56..4795.22 rows=23289 width=26) (actual time=0.935..13.509 rows=25298 loops=1)
   Hash Cond: (("Н_ВЕДОМОСТИ"."ОЦЕНКА")::text = ("Н_ОЦЕНКИ"."КОД")::text)
   ->  Bitmap Heap Scan on "Н_ВЕДОМОСТИ"  (cost=299.35..4692.85 rows=26200 width=10) (actual time=0.903..5.938 rows=25731 loops=1)
         Recheck Cond: ("ДАТА" > '2010-06-18 00:00:00'::timestamp without time zone)
         Heap Blocks: exact=651
         ->  Bitmap Index Scan on "ВЕД_ДАТА_I"  (cost=0.00..292.80 rows=26200 width=0) (actual time=0.812..0.812 rows=25731 loops=1)
               Index Cond: ("ДАТА" > '2010-06-18 00:00:00'::timestamp without time zone)
   ->  Hash  (cost=1.11..1.11 rows=8 width=27) (actual time=0.023..0.024 rows=8 loops=1)
         Buckets: 1024  Batches: 1  Memory Usage: 9kB
         ->  Seq Scan on "Н_ОЦЕНКИ"  (cost=0.00..1.11 rows=8 width=27) (actual time=0.009..0.013 rows=8 loops=1)
               Filter: (("КОД")::text < 'осв'::text)
               Rows Removed by Filter: 1
 Planning Time: 25.484 ms
 Execution Time: 14.772 ms 


                                       QUERY PLAN                                        
-----------------------------------------------------------------------------------------
 Hash Join  (cost=300.56..4795.22 rows=23289 width=26)
   Hash Cond: (("Н_ВЕДОМОСТИ"."ОЦЕНКА")::text = ("Н_ОЦЕНКИ"."КОД")::text)
   ->  Bitmap Heap Scan on "Н_ВЕДОМОСТИ"  (cost=299.35..4692.85 rows=26200 width=10)
         Recheck Cond: ("ДАТА" > '2010-06-18 00:00:00'::timestamp without time zone)
         ->  Bitmap Index Scan on "ВЕД_ДАТА_I"  (cost=0.00..292.80 rows=26200 width=0)
               Index Cond: ("ДАТА" > '2010-06-18 00:00:00'::timestamp without time zone)
   ->  Hash  (cost=1.11..1.11 rows=8 width=27)
         ->  Seq Scan on "Н_ОЦЕНКИ"  (cost=0.00..1.11 rows=8 width=27)
               Filter: (("КОД")::text < 'осв'::text)