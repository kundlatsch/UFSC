CREATE INDEX municipios_2010_geom_gist
ON public.municipios_2010
USING gist(geom);

UPDATE municipios_2010 SET geom =
st_transform(st_setsrid(geom, 4326), 900913);

-- 1)
SELECT * FROM municipios_2010;

-- 2)
SELECT * FROM municipios_2010 WHERE nome='Mafra';

-- 3)
SELECT *, st_area(geom) FROM municipios_2010 WHERE nome='Mafra';

-- 4)
SELECT nome, max(st_area(geom)) FROM municipios_2010 
WHERE uf='SC' GROUP BY nome ORDER BY max DESC limit 1;

-- CIDADES VIZINHAS DE BLUMENAU
SELECT m2.* FROM municipios_2010 m1, municipios_2010 m2
WHERE m1.nome = 'Blumenau' AND ST_Touches(m1.geom, m2.geom);
