#! /bin/sh

echo "begin;"

psql mabelladb -t  -c "\
select 'insert into artikel \
values('||artikel||','||breite||','||farbe||','||aufmachung \
||','||ean||',\''||bezeichnung||'\');' \
from artbez_3_1 b join artikelstamm s on \
(b.id=s.id and s.warengruppe=3 and \
s.aktiv=true) join artikelpreise p on \
(p.artikelid=s.id and p.kundennr=1)"

echo "commit;"
