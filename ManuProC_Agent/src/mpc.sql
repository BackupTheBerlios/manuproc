BEGIN TRANSACTION;
create table artikel (artnr text not null,breite text not null, farbe text not null, aufmachung text not null, ean text, bezeichnung text, unique (ean), unique (artnr,breite,farbe,aufmachung));
INSERT INTO artikel VALUES(2021,25,360,25,4031197041435,'Scarlett');
INSERT INTO artikel VALUES(2021,25,430,25,4031197041763,'Scarlett');
INSERT INTO artikel VALUES(2021,40,360,25,4031197041428,'Scarlett');
INSERT INTO artikel VALUES(2522,25,505,20,4031197041916,'Corona');
INSERT INTO artikel VALUES(2522,40,505,20,4031197041923,'Corona');
create table auftrag (aufid int not null, kdnr int not null, vknr int not null,datum timestamp not null, primary key (aufid));
INSERT INTO auftrag VALUES(1,1,0,'2004-5-11');
create table auftragentry (aufid int not null, 
vknr int not null,
artnr text not null,
breite text not null,
farbe text not null,
aufmachung text not null,
ean text unique,
stueck int not null,preis numeric(6,2),
unique (artnr,breite,farbe,aufmachung));
INSERT INTO auftragentry VALUES(1,0,2020,10,210,50,4031197001309,10,NULL);
INSERT INTO auftragentry VALUES(1,0,2015,100,100,20,4031197019489,12,NULL);
create table kunden (kundennr numeric(5) primary key,name text not null,ort text);
INSERT INTO kunden VALUES(1,'Test','Testort');
INSERT INTO kunden VALUES(2,'Test2','Ort');
COMMIT;
