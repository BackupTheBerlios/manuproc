drop function msw_verbose(int2);
create function msw_verbose(int2) returns text as
	'/usr/local/pgsql/lib/hex.so' language 'c';
update pg_proc set proiscachable=true where proname='msw_verbose';
update pg_proc set provolatile='i' where proname='msw_verbose';

drop function msw(int2);
create function msw(int2) returns text as
	'/usr/local/pgsql/lib/hex.so' language 'c';
update pg_proc set proiscachable=true where proname='msw';
update pg_proc set provolatile='i' where proname='msw';

drop function hex(int2);
create function hex(int2) returns text as
	'/usr/local/pgsql/lib/hex.so','hex2' language 'c';
drop function hex(int4);
create function hex(int4) returns text as
	'/usr/local/pgsql/lib/hex.so','hex4' language 'c';
update pg_proc set proiscachable=true where proname='hex';
update pg_proc set provolatile='i' where proname='hex';

drop function aufstat_2id(text);
create function aufstat_2id(text) returns int2 as
	'/usr/local/pgsql/lib/auftrag_status.so' language 'c';
drop function aufstat_2lab(int2);
create function aufstat_2lab(int2) returns text as
	'/usr/local/pgsql/lib/auftrag_status.so' language 'c';
update pg_proc set proiscachable=true where proname like 'aufstat_2%';
update pg_proc set provolatile='i' where proname like 'aufstat_2%';

drop function artikelbezeichnung(int4,int4,int4);
create function artikelbezeichnung(int4,int4,int4) returns text as 
	'/usr/local/pgsql/lib/artikelbezeichnung.so', 
	'artikelbezeichnung3' language 'c';
drop function artikelbezeichnung(int4,int4,int4,int4);
create function artikelbezeichnung(int4,int4,int4,int4) returns text as
	'/usr/local/pgsql/lib/artikelbezeichnung.so',
	'artikelbezeichnung4' language 'c';
drop function artikelbezeichnung(int4);
create function artikelbezeichnung(int4) returns text as
	'/usr/local/pgsql/lib/artikelbezeichnung.so',
	'artikelbezeichnung1' language 'c';
update pg_proc set proiscachable=true where proname='artikelbezeichnung';
update pg_proc set provolatile='i' where proname='artikelbezeichnung';

drop function artikelbezeichnung_schema(int4,int4);
create function artikelbezeichnung_schema(int4,int4) returns text as
	'/usr/local/pgsql/lib/artikelbezeichnung.so',
	'artikelbezeichnung_schema' language 'c';
update pg_proc set proiscachable=true where
	proname='artikelbezeichnung_schema';
update pg_proc set provolatile='i' where proname='artikelbezeichnung_schema';

drop function kw2date(int4);
create function kw2date(int4) returns text as
	'/usr/local/pgsql/lib/kw2date.so' language 'c';
update pg_proc set proiscachable=true where proname='kw2date';
update pg_proc set provolatile='i' where proname='kw2date';

drop function atoi(text);
create function atoi(text) returns int4 as
	'/usr/local/pgsql/lib/atoi.so', 'myatoi' language 'c';
update pg_proc set proiscachable=true where proname='atoi';
update pg_proc set provolatile='i' where proname='atoi';

drop function nextval_tnf(text);
create function nextval_tnf(text) returns int4 as
	'/usr/local/pgsql/lib/nextval_tnf.so' language 'c';

drop function artikelid(text);
create function artikelid(text) returns int4 as
	'/usr/local/pgsql/lib/artikelid.so' language 'c';
update pg_proc set proiscachable=true where proname='artikelid';
update pg_proc set provolatile='i' where proname='artikelid';
