drop function aufstat_2id(text);
create function aufstat_2id(text) returns int2 as
	'/usr/lib/postgresql/lib/auftrag_status.so' language 'c';
drop function aufstat_2lab(int2);
create function aufstat_2lab(int2) returns text as
	'/usr/lib/postgresql/lib/auftrag_status.so' language 'c';
update pg_proc set provolatile='i' where proname like 'aufstat_2%';

drop function artikelbezeichnung(int4,int4,int4);
create function artikelbezeichnung(int4,int4,int4) returns text as 
	'/usr/lib/postgresql/lib/artikelbezeichnung.so', 
	'artikelbezeichnung3' language 'c';
drop function artikelbezeichnung(int4,int4,int4,int4);
create function artikelbezeichnung(int4,int4,int4,int4) returns text as
	'/usr/lib/postgresql/lib/artikelbezeichnung.so',
	'artikelbezeichnung4' language 'c';
drop function artikelbezeichnung(int4);
create function artikelbezeichnung(int4) returns text as
	'/usr/lib/postgresql/lib/artikelbezeichnung.so',
	'artikelbezeichnung1' language 'c';
update pg_proc set provolatile='i' where proname='artikelbezeichnung';

drop function artikelbezeichnung_schema(int4,int4);
create function artikelbezeichnung_schema(int4,int4) returns text as
	'/usr/lib/postgresql/lib/artikelbezeichnung.so',
	'artikelbezeichnung_schema' language 'c';
update pg_proc set provolatile='i' where proname='artikelbezeichnung_schema';

drop function kw2date(int4);
create function kw2date(int4) returns text as
	'/usr/lib/postgresql/lib/kw2date.so' language 'c';
update pg_proc set provolatile='i' where proname='kw2date';

drop function atoi(text);
create function atoi(text) returns int4 as
	'/usr/lib/postgresql/lib/atoi.so', 'myatoi' language 'c';
update pg_proc set provolatile='i' where proname='atoi';

drop function artikelid(text);
create function artikelid(text) returns int4 as
	'/usr/lib/postgresql/lib/artikelid.so' language 'c';
update pg_proc set provolatile='i' where proname='artikelid';

drop function ean_check(text);
create function ean_check(text) returns int4 as
	'/usr/lib/postgresql/lib/ean_check.so','ean_check' language 'c';
update pg_proc set provolatile='i' where proname='ean_check';

drop function artikelkomponente(int4,int4,int4,int4);
create function artikelkomponente(int4,int4,int4,int4) returns text as
        '/usr/lib/postgresql/lib/artikelkomponente.so',
        'artikelkomponente' language 'c';
update pg_proc set provolatile='i' where proname='artikelkomponente';
