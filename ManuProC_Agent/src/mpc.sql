BEGIN TRANSACTION;
create table auftrag (aufid int not null, kdnr int not null, vknr int not null,datum timestamp not null, primary key (aufid));
create table auftragentry (aufid int not null, vknr int not null,artikelid int not null,stueck int not null,preis numeric(6,2));
create table kunden (kundennr numeric(5) primary key,name text not null,ort text);
COMMIT;
